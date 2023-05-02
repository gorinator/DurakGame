#include <iostream>
#include <stdlib.h>
#include <time.h>

struct card {
    int mast; // 3 - 6, will be converted to ascii char at output
    int value; // 6 - 14, 6 - 10 for common cards, and 11 - 14 for valet, dama, korol, tuz
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

void setprint(card* set, int n, bool numeration, int numstart); //basic function of printing N cards symbol by symbol, line by line
void printcompcards(int firstpos, int lastpos); // function for printing computer cards without showing info, only count, maximum 6 cards printed, if more in set char + and additional number of cards appeares
void printkon(card* set, int firstpos, int lastpos); //function of printing current playing set without numeration, printing by 6 cards in line
void printusercards(card* set, int firstpos, int lastpos); // function for printing user cards by 6 in line, with numeration
void printgame(card* set, int compcardmin, int compcardmax, int konmin, int konmax, int usercardmin, int usercardmax); // function for printing full current game field, combo function consist of other functions
int searchcardforgo(card* set, int firstpos, int lastpos, bool gamestart); //function of searching card for finding smallest card in set of comp to start the game. gamestart = 1 for finding smallest card for game start in kozyrs, gamestart = 0 for finding smallest card for bidding current card. 
int searchcardforbid(card* set, int firstpos, int lastpos, int cardtobid); //function of searching optimal minimum card for bid some card
int searchcardforadd(card* set, int firstpos, int lastpos, int konmin, int konmax); //function of searching card for adding to kon
int askint(int intmin, int intmax); // protected input of only positiv int values in a range, int range sets as arguments in function
bool sayberu(); // protected bool input of Yes or No, only 'y','Y','n','N' allowed on question "Beru, whant to add cards? y/n: "
bool askvzyat(); // protected bool input of Yes or No, only 'y','Y','n','N' allowed on question "Vzyat? y/n:" 
bool askbito(); // protected bool input of Yes or No, only 'y','Y','n','N' allowed on question "Bito? y/n: "
bool checkcardanswer(card* set, int answercardpos, int konmaxprev); // check function if enetered by user card really allowed for bidding
void swapcards(card* set, int a, int b); // swap card between a and b
void zeroswap(card* set, int a, int b); // copy card from a to b with erazing a 
void sortcards(card* set, int firstpos, int lastpos); // function of ranging user cards by values


int main() {
    srand((unsigned int)time(NULL)); // reset randomization base by current time(time.h should be included)
    card set[168]; // (0 - 167 range) = 168 !!!
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

    card erasecard = { erasecard.mast = 0, erasecard.value = 0 }; //safety erasecard))) Also need for easy debugging, example printing aftergame area and see where cards are located

    for (int i = 36; i <= kolodamax; i++) { // erasing empty range with erasecard
        set[i] = erasecard;
    }

    for (int i = 0; i < 6; i++) { //swaping first 6 card after randomization to area of user cards 36 - 71
        zeroswap(&set[0], i, usercardmin + i);
    }
    usercardmax = usercardmin + 5;

    set[0].value = set[35].value; //make copy of kozyr befor moving, copy set[35] to set[0] position
    set[0].mast = set[35].mast;

    for (int i = 0; i < 6; i++) { //swaping next 6 card after randomization to area of comp cards 72 - 107
        zeroswap(&set[0], i + 6, compcardmin + i);
    }
    compcardmax = compcardmin + 5;

    for (int i = 0; i < 24; i++) { //swaping next 24 card after randomization to area of koloda cards 144 - 167
        zeroswap(&set[0], i + 12, kolodamin + i);
    }
    kolodamin--; //need as game use prefix ++option
    konmax = konmin - 1; //need as game use prefix ++option


    /* Main game process starts from here */

    bool comphod = false; // storing who is player now
    int firstcard = searchcardforgo(&set[0], usercardmin, compcardmax, true); // gamestart = true, finding minimal kozyr or another minimum card for game start
    if ((firstcard >= compcardmin) && (firstcard <= compcardmax)) { // if computer going first
        comphod = true;
    }
    else {
        comphod = false; // if user have minimum kozyr in his set he is going first
    }

    do {

        if (((compcardmax - compcardmin) < 0) && (kolodamin == kolodamax)) { //checking if we should stop game if result of game can be find just now
            break;
        }
        if (((usercardmax - usercardmin) < 0) && (kolodamin == kolodamax)) { //checking if we should stop game if result of game can be find just now
            break;
        }

        while ((comphod == false) && ((usercardmax - usercardmin) >= 0)) { //computer is bidding, user going
            if ((konmax - konmin) < 0) { // ned for correct printing full game set if no cards in kon
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
            else { // if there are cards in kon use that way
                bool check = false;
                while (!check) {
                    printgame(&set[0], compcardmin, compcardmax, konmin, konmax, usercardmin, usercardmax);
                    int choise = (askint(usercardmin - usercardmin + 1, usercardmax - usercardmin + 1)) - 1;
                    for (int i = konmin; i <= konmax; i++) {
                        if (set[usercardmin + choise].value == set[i].value) {
                            check = true;
                        }
                    }
                    if (check) { // checking if user can put card to kon
                        zeroswap(&set[0], usercardmin + choise, ++konmax);
                        zeroswap(&set[0], usercardmax, usercardmin + choise);
                        usercardmax--;
                        break;
                    }
                    else { // if user enter card that cant be put on kon, say bito
                        bool bito = askbito();
                        if (bito) {
                            int d = konmax;
                            for (int i = konmin; i <= d; i++) {
                                zeroswap(&set[0], konmax--, bitomin++);
                            }
                            while (((usercardmax - usercardmin) < 5) && (kolodamin < kolodamax)) { //adding cards up to 6 after bito, but only when there is cards in koloda
                                zeroswap(&set[0], ++kolodamin, ++usercardmax);
                            }
                            while (((compcardmax - compcardmin) < 5) && (kolodamin < kolodamax)) { //adding cards up to 6 after bito, but only when there is cards in koloda
                                zeroswap(&set[0], ++kolodamin, ++compcardmax);
                            }
                            comphod = true; // say that now computer is going
                            break;
                        }
                    }
                }
            }
            if (comphod == false) { // computer answer for user placed card
                int f = searchcardforbid(&set[0], compcardmin, compcardmax, konmax); // searching card for bid by computer
                if (f != -1) { // placing if finding, if no such card we will recive -1 from function
                    zeroswap(&set[0], f, ++konmax);
                    zeroswap(&set[0], compcardmax--, f);
                }
                else { // if computer have no card for bid and function send us -1 say beru tu user
                    while (1) {
                        bool say = sayberu();
                        if (say) { // adding cards if we answered yes add cards more
                            printgame(&set[0], compcardmin, compcardmax, konmin, konmax, usercardmin, usercardmax);
                            int choise = (askint(usercardmin - usercardmin + 1, usercardmax - usercardmin + 1)) - 1;
                            bool check = false;
                            for (int i = konmin; i <= konmax; i++) {
                                if (set[usercardmin + choise].value == set[i].value) {
                                    check = true;
                                }
                            }
                            if (check) { // adding only if checking value true
                                zeroswap(&set[0], usercardmin + choise, ++konmax);
                                zeroswap(&set[0], usercardmax, usercardmin + choise);
                                usercardmax--;
                            }
                            printgame(&set[0], compcardmin, compcardmax, konmin, konmax, usercardmin, usercardmax);
                        }
                        else { // after all addings when we say dont whant to add, relocate cards to compcards from kon with added cards from user
                            int h = konmax;
                            for (int i = konmin; i <= h; i++) { // relocating cards here
                                zeroswap(&set[0], konmax--, ++compcardmax);
                            }
                            while (((usercardmax - usercardmin) < 5) && (kolodamin < kolodamax)) { //adding cards up to 6 after bito, but only when there is cards in koloda
                                zeroswap(&set[0], ++kolodamin, ++usercardmax);
                            }
                            while (((compcardmax - compcardmin) < 5) && (kolodamin < kolodamax)) { //adding cards up to 6 after bito, but only when there is cards in koloda
                                zeroswap(&set[0], ++kolodamin, ++compcardmax);
                            }
                            comphod = false; // say that still we are going
                            break;
                        }
                    }
                }
            }
        }

        while ((comphod == true) && (compcardmax - compcardmin) >= 0) { // if computer is going and user is bidding
            if ((konmax - konmin) < 0) { // finding minimum valued card if previos was bito or firs hod computer going
                posfinder = searchcardforgo(&set[0], compcardmin, compcardmax, false); //gamestart = false, finding minimum card for optimal computer going
                if (posfinder != -1) { // check if we can find some card 
                    zeroswap(&set[0], posfinder, ++konmax);
                    zeroswap(&set[0], compcardmax, posfinder);
                    compcardmax--;
                }
                else { // if cant find card for going breaking comphod and we will be catched by next process of finding game end
                    break;
                }
            }
            printgame(&set[0], compcardmin, compcardmax, konmin, konmax, usercardmin, usercardmax); // whaiting answer from user
            int choise = (askint(usercardmin - usercardmin + 1, usercardmax - usercardmin + 1)) - 1;
            if (checkcardanswer(&set[0], usercardmin + choise, konmax)) { // validating user hod
                konmax++;
                zeroswap(&set[0], usercardmin + choise, konmax); //putting user card to kon if validation is ok 
                zeroswap(&set[0], usercardmax, usercardmin + choise); // swap last card of user to finded one
                usercardmax--; // make user set shotter by -1 card that we put to kon
                int s2 = searchcardforadd(&set[0], compcardmin, compcardmax, konmin, konmax); // searching if computer have some cards to add to new kon
                if (s2 != -1) { //if computer have card to add do it
                    zeroswap(&set[0], s2, ++konmax);
                    zeroswap(&set[0], compcardmax--, s2);
                    comphod = true;
                    break; // break for adding only one card in time
                }
                int e = konmax;
                for (int i = konmin; i <= e; i++) { //if computer have no cards to add so comp say bito and putting cards from kon to bito
                    zeroswap(&set[0], konmax--, bitomin++);
                }
                while (((compcardmax - compcardmin) < 5) && (kolodamin < kolodamax)) { //adding cards up to 6 after bito, but only when there is cards in koloda
                    zeroswap(&set[0], ++kolodamin, ++compcardmax);
                }
                while (((usercardmax - usercardmin) < 5) && (kolodamin < kolodamax)) { //adding cards up to 6 after bito, but only when there is cards in koloda
                    zeroswap(&set[0], ++kolodamin, ++usercardmax);
                }
                comphod = false; // say that next player is user
                break;
            }
            else {
                if (askvzyat()) { // if user cant bid card or put not correct card for bidding 
                    while ((searchcardforadd(&set[0], compcardmin, compcardmax, konmin, konmax)) != -1) { // searching cars in computers set that an be add to kon before moving to user
                        int e = searchcardforadd(&set[0], compcardmin, compcardmax, konmin, konmax);
                        zeroswap(&set[0], e, ++konmax);
                        zeroswap(&set[0], compcardmax--, e);
                    }
                    int c = konmax;
                    for (int i = konmin; i <= c; i++) {
                        zeroswap(&set[0], konmax--, ++usercardmax); // moving kon to user after all addings from computer
                    }
                    while (((compcardmax - compcardmin) < 5) && (kolodamin < kolodamax)) { //adding cards up to 6 after bito, but only when there is cards in koloda
                        zeroswap(&set[0], ++kolodamin, ++compcardmax);
                    }
                    comphod = true; // saying that computer still going
                    break;
                }
            }
            break;
        }

    } while (((compcardmax - compcardmin) >= 0) && ((usercardmax - usercardmin) >= 0)); // searching if winner can be founded on that point

    system("CLS"); // clear screen before printing winner

    if (((compcardmax - compcardmin) >= 0) && ((usercardmax - usercardmin) < 0)) { //if user has no cards and computer hase some - user is winner
        printf(" You are a Winner !!! \n\n");
    }
    else if (((compcardmax - compcardmin) < 0) && ((usercardmax - usercardmin) < 0)) { //if user has no cards and computer has no cards - there is no  winers, nichya
        printf(" Nichya !!! \n\n");
    }
    else {
        printf(" You lose this game ((( \n\n"); //else situation if computer has no cards and user has some - computer is winner
    }

    system("PAUSE"); // pusing system to show who win the game
    return 0;
}

void setprint(card* set, int n, bool numeration, int numstart) { //basic function of printing N cards symbol by symbol, line by line
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

void printcompcards(int firstpos, int lastpos) { // function for printing computer cards without showing info, only count, maximum 6 cards printed, if more in set char + and additional number of cards appeares
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

void printkon(card* set, int firstpos, int lastpos) { //function of printing current playing set without numeration, printing by 6 cards in line
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

void printusercards(card* set, int firstpos, int lastpos) { // function for printing user cards by 6 in line, with numeration
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

void printgame(card* set, int compcardmin, int compcardmax, int konmin, int konmax, int usercardmin, int usercardmax) { // function for printing full current game field, combo function consist of other functions
    system("CLS");
    printcompcards(compcardmin, compcardmax);
    printkon(&set[0], konmin, konmax);
    sortcards(&set[0], usercardmin, usercardmax);
    printusercards(&set[0], usercardmin, usercardmax);
}

int searchcardforgo(card* set, int firstpos, int lastpos, bool gamestart) { //function of searching card for finding smallest card in set of comp to start the game. gamestart = 1 for finding smallest card for game start in kozyrs, gamestart = 0 for finding smallest card for bidding current card. 
    int min = 15;
    int minmast = 7;
    int minpos = firstpos;

    if (gamestart) { // for finding who is first playing
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

    if (!(gamestart)) { // for all other game process for finding smallest card to go, priority non cozyr
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

int searchcardforbid(card* set, int firstpos, int lastpos, int cardtobid) { //function of searching optimal minimum card for bid some card
    int min = 15;
    int minmast = 7;
    int minpos = firstpos;
    for (int i = firstpos; i <= lastpos; i++) {
        if ((set[i].value > set[cardtobid].value) && (set[i].mast == set[cardtobid].mast) && (set[i].value < min)) { //serching minimum card for bidding in same mast. ozyrs can bid other kozyrs as they in same mast
            min = set[i].value;
            minpos = i;
        }
    }
    if (min < 15) {
        return minpos; // returns card posotoin if finded 
    }
    else {
        for (int i = firstpos; i <= lastpos; i++) {
            if ((set[cardtobid].mast != set[0].mast) && (set[i].mast == set[0].mast) && (set[i].value < min)) { //serching kozyr minimum card for bidding if another one is non kozyr
                min = set[i].value;
                minpos = i;
            }
        }
    }
    if (min < 15) {
        return minpos; // returns minimum kozyr posotoin if finded
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
            if ((set[i].value == set[j].value) && (set[i].mast != set[0].mast) && (set[i].value < min) && (set[i].value != 0)) { //serching non kozyr first for add to kon
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
                if ((set[i].value == set[j].value) && (set[i].mast == set[0].mast) && (set[i].value < min) && (set[i].value != 0)) { //serching kozyr card position to add to kon //dificulty of game can set here
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

bool sayberu() { // protected bool input of Yes or No, only 'y','Y','n','N' allowed on question "Beru, whant to add cards? y/n: "
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

bool askvzyat() { // protected bool input of Yes or No, only 'y','Y','n','N' allowed on question "Vzyat? y/n:" 
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

bool askbito() { // protected bool input of Yes or No, only 'y','Y','n','N' allowed on question "Bito? y/n: "
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

bool checkcardanswer(card* set, int answercardpos, int konmaxprev) { // check function if enetered by user card really allowed for bidding
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

void swapcards(card* set, int a, int b) { // swap card between a and b
    card c;
    c.value = set[a].value;
    c.mast = set[a].mast;
    set[a].value = set[b].value;
    set[a].mast = set[b].mast;
    set[b].value = c.value;
    set[b].mast = c.mast;
}

void zeroswap(card* set, int a, int b) { // copy card from a to b with erazing a 
    set[b].value = set[a].value;
    set[b].mast = set[a].mast;
    set[a].value = 0;
    set[a].mast = 0;
}

void sortcards(card* set, int firstpos, int lastpos) { // function of ranging user cards by values
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
