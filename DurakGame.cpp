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
void setprintmulti(card* set, int n, bool numeration);
bool askyn();
int askint(int intmin, int intmax);

int main() {
    srand(time(NULL)); // reset randomization base by current time(time.h should be included)
    card set[36];
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
        int c = i - (rand() % (i+1)) ;
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

    setprintmulti(&set[0], 36, true);


    int choise = askint(1, 6);
    system("CLS");
    return 0;
}

void setprintmulti(card* set, int n, bool numeration) {
    if (n > 0 && n <= 6) {
        setprint(&set[0], n, numeration, 0);
    }
    
    if (n > 6 && n <= 12) {
        setprint(&set[0], 6, numeration, 0);
        setprint(&set[6], n - 6, numeration, 6);
    }
    
    if (n > 12 && n <= 18) {
        setprint(&set[0], 6, numeration, 0);
        setprint(&set[6], 6, numeration, 6);
        setprint(&set[12], n - 12, numeration, 12);
    } 

    if (n > 18 && n <= 24) {
        setprint(&set[0], 6, numeration, 0);
        setprint(&set[6], 6, numeration, 6);
        setprint(&set[12], 6, numeration, 12);
        setprint(&set[18], n - 18, numeration, 18);
    }

    if (n > 24 && n <= 30) {
        setprint(&set[0], 6, numeration, 0);
        setprint(&set[6], 6, numeration, 6);
        setprint(&set[12], 6, numeration, 12);
        setprint(&set[18], 6, numeration, 18);
        setprint(&set[24], n - 24, numeration, 24);
    }

    if (n > 30 && n <= 36) {
        setprint(&set[0], 6, numeration, 0);
        setprint(&set[6], 6, numeration, 6);
        setprint(&set[12], 6, numeration, 12);
        setprint(&set[18], 6, numeration, 18);
        setprint(&set[24], 6, numeration, 24);
        setprint(&set[30], n - 30, numeration, 30);
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
            printf("  %i  \t", i);
        }
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
        printf("Exit? y or n: ");
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
