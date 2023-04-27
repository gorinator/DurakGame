#include <iostream>
#include <stdlib.h>
#include <time.h>

struct card {
    int mast; // 3 - 6
    int value; // 6 - 14
    int kozyr; //10 (10 + value min will be 16)
};

enum masti {  // ♥ - 3, ♦ - 4, ♣ - 5, ♠ - 6
    chervi = 3,
    bubi = 4,
    kresti = 5,
    piki = 6
};

enum karty {  // V - 11, D - 12, K - 13, T - 14
    valet = 11,
    dama = 12,
    karol = 13,
    tuz = 14
};

void setprint(card* set, int n, bool numeration, int numstart);
void printusercards(card* set, int firstpos, int lastpos);
void printcompcards(int firstpos, int lastpos);
void printkon(card* set, int firstpos, int lastpos);
bool askyn();
int askint(int intmin, int intmax);
void swapcards(card* set, int a, int b);
void copycard(card* set, int a, int b);
void printgame(card* set, int compcardmin, int compcardmax, int konmin, int konmax, int usercardmin, int usercardmax);

int main() {
    srand(time(NULL)); // reset randomization base by current time(time.h should be included)
    card set[168]; // 0 - 167 = 168 !!!
    int m = 3; //initial mast
    int v = 6; //initial value
    for (int i = 0; i < 36; i++) { // generating array with numbers from 0 to 35
        set[i].value = v;
        set[i].mast = m;
        v++;
        if (v > 14) {
            v = 6;
            m++;
        }
    }
    for (int i = 0; i < 36; i++) { // randomizing of array by swapping random item with i
        int c = i - (rand() % (i + 1));
        card a;
        a.value = set[i].value;
        a.mast = set[i].mast;
        set[i].value = set[c].value;
        set[i].mast = set[c].mast;
        set[c].value = a.value;
        set[c].mast = a.mast;
    }

    int k = set[35].mast; //setting kozyr, last card mast used for that
    for (int i = 0; i < 36; i++) {
        if (set[i].mast == k) {
            set[i].kozyr = 10;
        }
        else set[i].kozyr = 0;
    }

    // position 0 in array used for storing kozyr, see next in a code
    int konmin = 1;
    int konmax = 35;
    int usercardmin = 36;
    int usercardmax = 71;
    int compcardmin = 72;
    int compcardmax = 107;
    int bitomin = 108;
    int bitomax = 143;
    int kolodamin = 144;
    int kolodamax = 167;

    card erasecard = { erasecard.kozyr = 0, erasecard.mast = 0, erasecard.value = 0 }; //safety erasecard)))

    for (int i = 36; i <= kolodamax; i++) { // erasing empty range with erasecard
        set[i] = erasecard;
    }

    for (int i = 0; i < 6; i++) { //swaping first 6 card after randomization to area of user cards 36 - 71
        swapcards(&set[0], i, usercardmin + i);
    }
    usercardmax = usercardmin + 5;

    copycard(&set[0], 0, 35); //make copy of kozyr befor moving, copy set[35] to set[0] position

    for (int i = 0; i < 6; i++) { //swaping next 6 card after randomization to area of comp cards 72 - 107
        swapcards(&set[0], i + 6, compcardmin + i);
    }
    compcardmax = compcardmin + 5;

    for (int i = 0; i < 24; i++) { //swaping next 24 card after randomization to area of koloda cards 144 - 167
        swapcards(&set[0], i + 12, kolodamin + i);
    }
    kolodamax = kolodamin + 23;

    for (int i = konmin; i <= konmax; i++) { // erasing kon (prev. random set area) with erasecard
        set[i] = erasecard;
    }
    konmax = konmin;
    

    printgame(&set[0], compcardmin, compcardmax, konmin, konmax, usercardmin, usercardmax);
    int choise = (askint(usercardmin - usercardmin + 1, usercardmax - usercardmin + 1)) - 1;
    set[konmin] = set[usercardmin + choise]; //just for test
    printgame(&set[0], compcardmin, compcardmax, konmin, konmax, usercardmin, usercardmax);
    choise = (askint(usercardmin - usercardmin + 1, usercardmax - usercardmin + 1)) - 1;
    system("CLS");
    setprint(&set[usercardmin + choise], 1, false, 0);
    bool exit;
    do {
        exit = askyn();
    } while (!exit);
    return 0;
}

/*
int searchcard(card* set, int firstpos, int lastpos, int cardtobid, bool gamestart) { //function of searching card for bid sumcard or for finding smallest one to start the game. gamestart = 1 for finding smallest card for game start in kozyrs, gamestart = 0 for finding card for bidding current card.
    if (gamestart) {

    }
    return -1;
}
*/


void printgame(card* set, int compcardmin, int compcardmax, int konmin, int konmax, int usercardmin, int usercardmax) {
    system("CLS");
    printcompcards(compcardmin, compcardmax);
    printkon(&set[0], konmin, konmax);
    printusercards(&set[0], usercardmin, usercardmax);
}

void swapcards(card* set, int a, int b) {
    card c;
    c.value = set[a].value;
    c.mast = set[a].mast;
    c.kozyr = set[a].kozyr;
    set[a].value = set[b].value;
    set[a].mast = set[b].mast;
    set[a].kozyr = set[b].kozyr;
    set[b].value = c.value;
    set[b].mast = c.mast;
    set[b].kozyr = c.kozyr;
}

void copycard(card* set, int a, int b) { //copy make copy b to a
    set[a].value = set[b].value;
    set[a].mast = set[b].mast;
    set[a].kozyr = set[b].kozyr;
}

void printusercards(card* set, int firstpos, int lastpos) {
    int n = (lastpos - firstpos) + 1;
    if (n > 0 && n <= 6) {
        printf("--- Kozyr %c ---<Yours cards>-----------------\t\n", (char)set[0].mast);
        setprint(&set[firstpos], n, true, 0);
    }

    if (n > 6 && n <= 12) {
        printf("--- Kozyr %c ---<Yours cards>-----------------\t\n", (char)set[0].mast);
        setprint(&set[firstpos], 6, true, 0);
        setprint(&set[firstpos + 6], n - 6, true, 6);
    }

    if (n > 12 && n <= 18) {
        printf("--- Kozyr %c ---<Yours cards>-----------------\t\n", (char)set[0].mast);
        setprint(&set[firstpos], 6, true, 0);
        setprint(&set[firstpos + 6], 6, true, 6);
        setprint(&set[firstpos + 12], n - 12, true, 12);
    }

    if (n > 18 && n <= 24) {
        printf("--- Kozyr %c ---<Yours cards>-----------------\t\n", (char)set[0].mast);
        setprint(&set[firstpos], 6, true, 0);
        setprint(&set[firstpos + 6], 6, true, 6);
        setprint(&set[firstpos + 12], 6, true, 12);
        setprint(&set[firstpos + 18], n - 18, true, 18);
    }

    if (n > 24 && n <= 30) {
        printf("--- Kozyr %c ---<Yours cards>-----------------\t\n", (char)set[0].mast);
        setprint(&set[firstpos], 6, true, 0);
        setprint(&set[firstpos + 6], 6, true, 6);
        setprint(&set[firstpos + 12], 6, true, 12);
        setprint(&set[firstpos + 18], 6, true, 18);
        setprint(&set[firstpos + 24], n - 24, true, 24);
    }

    if (n > 30 && n <= 36) {
        printf("--- Kozyr %c ---<Yours cards>-----------------\t\n", (char)set[0].mast);
        setprint(&set[firstpos], 6, true, 0);
        setprint(&set[firstpos + 6], 6, true, 6);
        setprint(&set[firstpos + 12], 6, true, 12);
        setprint(&set[firstpos + 18], 6, true, 18);
        setprint(&set[firstpos + 24], 6, true, 24);
        setprint(&set[firstpos + 30], n - 30, true, 30);
    }
}

void setprint(card* set, int n, bool numeration, int numstart) {
    for (int i = 0; i < n; i++) {
        printf("/////\t");
    }
    printf("\n");
    for (int i = 0; i < n; i++) {
        printf("/ %c /\t", (char)set[i].mast);
    }
    printf("\n");
    for (int i = 0; i < n; i++) {
        switch (set[i].value) {
        case 10:
            printf("/ %i/\t", set[i].value);
            break;
        case 11:
            printf("/ %c /\t", 'V');
            break;
        case 12:
            printf("/ %c /\t", 'D');
            break;
        case 13:
            printf("/ %c /\t", 'K');
            break;
        case 14:
            printf("/ %c /\t", 'T');
            break;
        default:
            printf("/ %i /\t", set[i].value);
            break;
        }
    }
    printf("\n");
    for (int i = 0; i < n; i++) {
        printf("/ %c /\t", (char)set[i].mast);
    }
    printf("\n");
    for (int i = 0; i < n; i++) {
        printf("/////\t");
    }
    printf("\n");
    if (numeration) {
        for (int i = numstart; i < (n + numstart); i++) {
            printf("  %i  \t", i + 1);
        }
    }
    printf("\n\n");
}

void printcompcards(int firstpos, int lastpos) {
    int n = (lastpos - firstpos) + 1;
    if (n <= 6) {
        for (int i = 0; i < n; i++) {
            printf("/////\t");
        }
        printf("\n");
        for (int i = 0; i < n; i++) {
            printf("/////\t");
        }
        printf("\n");
        for (int i = 0; i < n; i++) {
            printf("/////\t");
        }
        printf("\n");
        for (int i = 0; i < n; i++) {
            printf("/////\t");
        }
        printf("\n");
        for (int i = 0; i < n; i++) {
            printf("/////\t");
        }
        printf("\n");
        printf("--------------<Computer cards>---------------\t\n");
    }
    if (n > 6) {
        for (int i = 0; i < 6; i++) {
            printf("/////\t");
        }
        printf("\n");
        for (int i = 0; i < 6; i++) {
            printf("/////\t");
        }
        printf("\n");
        for (int i = 0; i < 5; i++) {
            printf("/////\t");
        }
        printf("/ %c%i/\t", '+', n - 5);
        printf("\n");
        for (int i = 0; i < 6; i++) {
            printf("/////\t");
        }
        printf("\n");
        for (int i = 0; i < 6; i++) {
            printf("/////\t");
        }
        printf("\n");
        printf("--------------<Computer cards>---------------\t\n");
    }
    printf("\n\n");
}

int askint(int intmin, int intmax) { // protected input of only positiv int values in a range, int range sets as arguments in function
    int intanswer = -2147483648; // min_int value for initialisation
    do {
        printf("Enter number from %i to %i:", intmin, intmax);
        std::string intin;
        std::cin >> intin;
        int k = 0;
        int n = 0; // digitlen
        int c = 1; // multiplier
        while (intin[k] != 0 && (intin[k] >= '0' && intin[k] <= '9')) { // find digital part lenth and calculate needed multiplier
            if (k > 0) {
                c *= 10;
            }
            n++;
            k++;
        }
        if (n > 0) {
            intanswer = 0;
            for (int i = 0; i < n; i++) {
                int temp = int(intin[i]) - 48; // decode char to int ASCII position and correct to rigt result by bias
                intanswer += temp * c; //multiplier of register depended on digitlen
                c /= 10; //change multiplier for next digit
            }
        }
    } while (intanswer < intmin || intanswer > intmax);
    return intanswer;
}

bool askyn() { // protected bool input of Yes or No, only 'y','Y','n','N' allowed or words with this chars as first
    char charanswerb = 'x'; // char for initialisation
    do {
        printf("Exit? y/n: ");
        std::string in;
        std::cin >> in;
        charanswerb = in[0]; // reading of first char in string
    } while (charanswerb != 'y' && charanswerb != 'Y' && charanswerb != 'n' && charanswerb != 'N');
    if (charanswerb == 'y' || charanswerb == 'Y') {
        return true;
    }
    else {
        return false;
    }
}

void printkon(card* set, int firstpos, int lastpos) {
    int n = (lastpos - firstpos) + 1;
    if (n > 0 && n <= 6) {
        setprint(&set[firstpos], n, false, 0);
    }

    if (n > 6 && n <= 12) {
        setprint(&set[firstpos], 6, false, 0);
        setprint(&set[firstpos + 6], n - 6, false, 6);
    }

    if (n > 12 && n <= 18) {
        setprint(&set[firstpos], 6, false, 0);
        setprint(&set[firstpos + 6], 6, false, 6);
        setprint(&set[firstpos + 12], n - 12, false, 12);
    }

    if (n > 18 && n <= 24) {
        setprint(&set[firstpos], 6, false, 0);
        setprint(&set[firstpos + 6], 6, false, 6);
        setprint(&set[firstpos + 12], 6, false, 12);
        setprint(&set[firstpos + 18], n - 18, false, 18);
    }

    if (n > 24 && n <= 30) {
        setprint(&set[firstpos], 6, false, 0);
        setprint(&set[firstpos + 6], 6, false, 6);
        setprint(&set[firstpos + 12], 6, false, 12);
        setprint(&set[firstpos + 18], 6, false, 18);
        setprint(&set[firstpos + 24], n - 24, false, 24);
    }

    if (n > 30 && n <= 36) {
        setprint(&set[firstpos], 6, false, 0);
        setprint(&set[firstpos + 6], 6, false, 6);
        setprint(&set[firstpos + 12], 6, false, 12);
        setprint(&set[firstpos + 18], 6, false, 18);
        setprint(&set[firstpos + 24], 6, false, 24);
        setprint(&set[firstpos + 30], n - 30, false, 30);
    }
}
