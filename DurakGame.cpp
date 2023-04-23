#include <stdio.h>
#include <iostream>
#include <stdlib.h>

struct card {
    int mast;
    int value;
    bool kozyr;
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

void setprint(card* set, int n, bool numeration);
bool askyn();
int askint(int intmin, int intmax);


int main() {
    card set[6];
    set[0] = { chervi,6 };
    set[1] = { bubi, valet };
    set[2] = { piki,10 };
    set[3] = { kresti,tuz };
    set[4] = { chervi,8 };
    set[5] = { bubi,dama };
    setprint(&set[0], 6, true);
    int choise = askint(1,6);
    system("CLS");
    setprint(&set[choise - 1], 1, false);
    bool c = false;
    while (c != true) {
       c = askyn();
    } 
    return 0;
}

void setprint(card* set, int n, bool numeration) {
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
        for (int i = 0; i < n; i++) {
            printf("  %i  \t", i + 1);
        }
    }
    printf("\n\n");
}

int askint(int intmin, int intmax) { // protected input of only int values in a range, int range sets as arguments in function
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