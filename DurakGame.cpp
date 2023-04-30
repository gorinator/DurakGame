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
bool askvzyat();
int askint(int intmin, int intmax);
void swapcards(card* set, int a, int b);
void copycard(card* set, int a, int b);
void printgame(card* set, int compcardmin, int compcardmax, int konmin, int konmax, int usercardmin, int usercardmax);
void zeroswap(card* set, int a, int b);
void sortcards(card* set, int firstpos, int lastpos);
int searchcardforgo(card* set, int firstpos, int lastpos, bool gamestart);
int searchcardforbid(card* set, int firstpos, int lastpos, int cardtobid);
int searchcardforadd(card* set, int firstpos, int lastpos, int konmin, int konmax);
bool checkcardanswer(card* set, int answercardpos, int konmaxprev);
bool askbito();
bool sayberu();

int main() {
    srand((unsigned int)time(NULL)); // reset randomization base by current time(time.h should be included)
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
    int posfinder = 0; // storing temp positions

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
    konmax = konmin - 1;

    /* Main game process starts from here */
    bool comphod = false;
    int firstcard = searchcardforgo(&set[0], usercardmin, compcardmax, true); // gamestart = true, finding minimal kozyr or another minimum card for game start
    if ((firstcard >= compcardmin) && (firstcard <= compcardmax)) { // if computer going first
        comphod = true;
    }
    else {
        comphod = false;
    }

    while (((compcardmax - compcardmin) >= 0) && ((usercardmax - usercardmin) >= 0)) {

        if (comphod == false) {
            if ((konmax - konmin) < 0) {
                system("CLS");
                printcompcards(compcardmin, compcardmax);
                printf("\n\n\n\n\n\n\n");
                sortcards(&set[0], usercardmin, usercardmax);
                printusercards(&set[0], usercardmin, usercardmax);
                int choise = (askint(usercardmin - usercardmin + 1, usercardmax - usercardmin + 1)) - 1;
                zeroswap(&set[0], usercardmin + choise, ++konmax);
                zeroswap(&set[0], usercardmax, usercardmin + choise);
                usercardmax--;
            }
            else {
                bool check = false;
                while (!check) {
                    printgame(&set[0], compcardmin, compcardmax, konmin, konmax, usercardmin, usercardmax);
                    int choise = (askint(usercardmin - usercardmin + 1, usercardmax - usercardmin + 1)) - 1;
                    for (int i = konmin; i <= konmax; i++) {
                        if (set[usercardmin + choise].value == set[i].value) {
                            check = true;
                        }
                    }
                    if (check) {
                        zeroswap(&set[0], usercardmin + choise, ++konmax);
                        zeroswap(&set[0], usercardmax, usercardmin + choise);
                        usercardmax--;
                        break;
                    }
                    else {
                        bool bito = askbito();
                        if (bito) {
                            int d = konmax;
                            for (int i = konmin; i <= d; i++) {
                                zeroswap(&set[0], konmax--, bitomin++);
                            }
                            while (((usercardmax - usercardmin) < 5) && (kolodamin <= kolodamax)) {
                                zeroswap(&set[0], kolodamin++, ++usercardmax);
                            }
                            while (((compcardmax - compcardmin) < 5) && (kolodamin <= kolodamax)) {
                                zeroswap(&set[0], kolodamin++, ++compcardmax);
                            }
                            comphod = true;
                            break;
                        }
                    }
                }
            }
            int f = searchcardforbid(&set[0], compcardmin, compcardmax, konmax);
            if (f != -1) {
                zeroswap(&set[0], f, ++konmax);
                zeroswap(&set[0], compcardmax--, f);
            }
            else {
                while (1) {
                    bool say = sayberu();
                    if (say) {
                        printgame(&set[0], compcardmin, compcardmax, konmin, konmax, usercardmin, usercardmax);
                        int choise = (askint(usercardmin - usercardmin + 1, usercardmax - usercardmin + 1)) - 1;
                        bool check = false;
                        for (int i = konmin; i <= konmax; i++) {
                            if (set[usercardmin + choise].value == set[i].value) {
                                check = true;
                            }
                        }
                        if (check) {
                            zeroswap(&set[0], usercardmin + choise, ++konmax);
                            zeroswap(&set[0], usercardmax, usercardmin + choise);
                            usercardmax--;
                        }
                        printgame(&set[0], compcardmin, compcardmax, konmin, konmax, usercardmin, usercardmax);
                    }
                    else {
                        int h = konmax;
                        for (int i = konmin; i <= h; i++) {
                            zeroswap(&set[0], konmax--, ++compcardmax);
                        }
                        while (((usercardmax - usercardmin) < 5) && (kolodamin <= kolodamax)) {
                            zeroswap(&set[0], kolodamin++, ++usercardmax);
                        }
                        while (((compcardmax - compcardmin) < 5) && (kolodamin <= kolodamax)) {
                            zeroswap(&set[0], kolodamin++, ++compcardmax);
                        }
                        comphod = false;
                        break;
                    }
                }
            }
        }

        if (comphod == true) {
            while (1) {
                if ((compcardmax - compcardmin) >= 0) {
                    if ((konmax - konmin) < 0) {
                        posfinder = searchcardforgo(&set[0], compcardmin, compcardmax, false); //gamestart = false, finding minimum card for optimal computer going
                        zeroswap(&set[0], posfinder, ++konmax);
                        zeroswap(&set[0], compcardmax, posfinder);
                        compcardmax--;
                    }
                    printgame(&set[0], compcardmin, compcardmax, konmin, konmax, usercardmin, usercardmax);
                    int choise = (askint(usercardmin - usercardmin + 1, usercardmax - usercardmin + 1)) - 1;
                    if (checkcardanswer(&set[0], usercardmin + choise, konmax)) {
                        konmax++;
                        zeroswap(&set[0], usercardmin + choise, konmax);
                        zeroswap(&set[0], usercardmax, usercardmin + choise);
                        usercardmax--;
                        while (1) {
                            int s2 = searchcardforadd(&set[0], compcardmin, compcardmax, konmin, konmax);
                            if (s2 != -1) {
                                zeroswap(&set[0], s2, ++konmax);
                                zeroswap(&set[0], compcardmax--, s2);
                                comphod = true;
                                break;
                            }
                            else {
                                int e = konmax;
                                for (int i = konmin; i <= e; i++) {
                                    zeroswap(&set[0], konmax--, bitomin++);
                                }
                                while (((compcardmax - compcardmin) < 5) && (kolodamin <= kolodamax)) {
                                    zeroswap(&set[0], kolodamin++, ++compcardmax);
                                }
                                while (((usercardmax - usercardmin) < 5) && (kolodamin <= kolodamax)) {
                                    zeroswap(&set[0], kolodamin++, ++usercardmax);
                                }
                                comphod = false;
                                break;
                            }
                        }
                    }
                    else {
                        if (askvzyat()) {
                            while (searchcardforadd(&set[0], compcardmin, compcardmax, konmin, konmax) != -1) {
                                zeroswap(&set[0], (searchcardforadd(&set[0], compcardmin, compcardmax, konmin, konmax)), ++konmax);
                                zeroswap(&set[0], compcardmax--, (searchcardforadd(&set[0], compcardmin, compcardmax, konmin, konmax)));
                            }
                            int c = konmax;
                            for (int i = konmin; i <= c; i++) {
                                zeroswap(&set[0], konmax--, ++usercardmax);
                            }
                            while (((compcardmax - compcardmin) < 5) && (kolodamax <= kolodamax)) {
                                zeroswap(&set[0], kolodamin++, ++compcardmax);
                            }
                            comphod = true;
                            break;
                        }
                    }
                }
                break;
            }
        }
    }

    if (((compcardmax - compcardmin) >= 0) && ((usercardmax - usercardmin) < 0)) {
        printf("You are a Winner !!!");
    }
    else if (((compcardmax - compcardmin) < 0) && ((usercardmax - usercardmin) < 0)) {
        printf("Nichya !!!");
    }
    else {
        printf("You lose this game (((");
    }
    return 0;
}

void printgame(card* set, int compcardmin, int compcardmax, int konmin, int konmax, int usercardmin, int usercardmax) {
    system("CLS");
    printcompcards(compcardmin, compcardmax);
    printkon(&set[0], konmin, konmax);
    sortcards(&set[0], usercardmin, usercardmax);
    printusercards(&set[0], usercardmin, usercardmax);
}

void zeroswap(card* set, int a, int b) { // from a to b with erazing a 
    set[b].value = set[a].value;
    set[b].mast = set[a].mast;
    set[b].kozyr = set[a].kozyr;
    set[a].value = 0;
    set[a].mast = 0;
    set[a].kozyr = 0;
}

void swapcards(card* set, int a, int b) { // from a to b
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
        printf("/ %c%i/\t", '+', n - 6);
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

bool askvzyat() { // protected bool input of Yes or No, only 'y','Y','n','N' allowed or words with this chars as first
    char charanswerb = 'x'; // char for initialisation
    do {
        printf("Vzyat? y/n: ");
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

bool sayberu() { // protected bool input of Yes or No, only 'y','Y','n','N' allowed or words with this chars as first
    char charanswerb = 'x'; // char for initialisation
    do {
        printf("Beru, whant to add cards? y/n: ");
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

bool askbito() { // protected bool input of Yes or No, only 'y','Y','n','N' allowed or words with this chars as first
    char charanswerb = 'x'; // char for initialisation
    do {
        printf("Bito? y/n: ");
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

void sortcards(card* set, int firstpos, int lastpos) {
    int max = set[firstpos].value;
    int maxpos = firstpos;
    for (int i = 0; i < lastpos - firstpos; i++) {
        for (int j = firstpos; j <= lastpos - i; j++) { //searching maximum and writing it to the end, next step not search in that end
            if (set[j].value > max) {
                max = set[j].value;
                maxpos = j;
            }
        }
        swapcards(&set[0], maxpos, lastpos - i);
        max = set[firstpos].value;
        maxpos = firstpos;
    }
}

int searchcardforgo(card* set, int firstpos, int lastpos, bool gamestart) { //function of searching card for finding smallest one to start the game. gamestart = 1 for finding smallest card for game start in kozyrs, gamestart = 0 for finding smallest card for bidding current card. Bool bid for bidding ore for going with minimals not kozyr
    int min = 15;
    int minmast = 7;
    int minpos = firstpos;

    if (gamestart) {
        for (int i = firstpos; i <= lastpos; i++) {
            if ((set[i].value < min) && (set[i].mast == set[0].mast)) { //serching minimum kozyr card position
                min = set[i].value;
                minpos = i;
            }
        }
        if (min < 15) {
            return minpos; // returns minimum kozyr posotoin if finded at least one kozyr in a range
        }
        else {
            for (int i = firstpos; i <= lastpos; i++) {
                if ((set[i].value <= min) && (set[i].mast < minmast) && (set[i].mast != 0)) { //serching card with minimum value and mast
                    min = set[i].value;
                    minpos = i;
                    minmast = set[i].mast;
                }
            }
            return minpos;
        }
    }

    if (!(gamestart)) {
        for (int i = firstpos; i <= lastpos; i++) {
            if ((set[i].value < min) && (set[i].mast != set[0].mast) && (set[i].mast != 0)) { //serching minimum not kozyr card position
                min = set[i].value;
                minpos = i;
            }
        }
        if (min < 15) {
            return minpos;
        }
        else {
            for (int i = firstpos; i <= lastpos; i++) {
                if ((set[i].mast == set[0].mast) && (set[i].value < min)) { //serching minimumu kozyr if no usual cards
                    min = set[i].value;
                    minpos = i;
                }
            }
            if (min < 15) {
                return minpos;
            }
            else {
                return -1;
            }
        }
    }
    return -1;
}

int searchcardforbid(card* set, int firstpos, int lastpos, int cardtobid) { //function of searching card for bid some card
    int min = 15;
    int minmast = 7;
    int minpos = firstpos;
    for (int i = firstpos; i <= lastpos; i++) {
        if ((set[i].value > set[cardtobid].value) && (set[i].mast == set[cardtobid].mast) && (set[i].value < min)) { //serching minimum kozyr card position
            min = set[i].value;
            minpos = i;
        }
    }
    if (min < 15) {
        return minpos; // returns minimum kozyr posotoin if finded at least one kozyr in a range
    }
    else {
        for (int i = firstpos; i <= lastpos; i++) {
            if ((set[cardtobid].mast != set[0].mast) && (set[i].mast == set[0].mast) && (set[i].value < min)) { //serching kozyr minimum card for bidding
                min = set[i].value;
                minpos = i;
            }
        }
    }
    if (min < 15) {
        return minpos; // returns minimum kozyr posotoin if finded at least one kozyr in a range
    }
    else {
        return -1;
    }
}

int searchcardforadd(card* set, int firstpos, int lastpos, int konmin, int konmax) { //function of searching card for adding to kon
    int min = 15;
    int minmast = 7;
    int minpos = firstpos;
    for (int j = konmin; j <= konmax; j++) {
        for (int i = firstpos; i <= lastpos; i++) {
            if ((set[i].value == set[j].value) && (set[i].mast != set[0].mast) && (set[i].value < min) && (set[i].value != 0)) { //serching minimum kozyr card position
                min = set[i].value;
                minpos = i;
            }
        }
    }
    if (min < 15) {
        return minpos;
    }
    else {
        for (int j = konmin; j <= konmax; j++) {
            for (int i = firstpos; i <= lastpos; i++) {
                if ((set[i].value == set[j].value) && (set[i].mast == set[0].mast) && (set[i].value < min) && (set[i].value != 0)) { //serching minimum kozyr card position to add to kon //dificulty of game can set here
                    min = set[i].value;
                    minpos = i;
                }
            }
        }
    }
    if (min < 15) {
        return minpos;
    }
    else {
        return -1;
    }
}

bool checkcardanswer(card* set, int answercardpos, int konmaxprev) {
    if (((set[answercardpos].mast == set[konmaxprev].mast)) && ((set[konmaxprev].mast != set[0].mast)) && (set[answercardpos].value > set[konmaxprev].value)) {
        return true;
    }
    if (((set[answercardpos].mast == set[0].mast)) && ((set[konmaxprev].mast != set[0].mast))) {
        return true;
    }
    if (((set[answercardpos].mast == set[konmaxprev].mast)) && ((set[konmaxprev].mast == set[0].mast)) && (set[answercardpos].value > set[konmaxprev].value)) {
        return true;
    }
    else {
        return false;
    }
}
