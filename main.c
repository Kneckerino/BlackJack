#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int cardsInDeck = 52;
const int dealer = 1, player = 2;
int playerCardAmount = 0;
int dealerCardAmount = 0;
int playerHand[52];
int dealerHand[52];
int deck[4][13]; //Playing deck
char cards[4][13][6][10]; //All cards

void ResetDeck();
void InitCards();
void ShowCards();
void GiveCardsToDealer();
void DealCard(int person);
int IsBlackJack(int hand[], int size);
int GreatestLegalValue(int hand[], int size);

int main() {
    srand(time(NULL)); //Initialize Random
    ResetDeck();
    InitCards();

    int turn = 1;
    int money = 500;
    int bet;

    printf("Welcome to BlackJack!\n");
    printf("your total is %d\nHow much do you wager?>", money);
    do {
        scanf(" %d", &bet);
        if (bet > money) printf("You don't have that kind of money, try again>");
    } while (bet > money);
    money -= bet;

    //Deal cards to player then dealer then player then dealer again
    DealCard(player);
    DealCard(dealer);
    DealCard(player);
    if (IsBlackJack(playerHand, playerCardAmount) == 1) {

        ShowCards();
        printf("BlackJack! You won!");
        return 0;
    }
    DealCard(dealer);
    if (IsBlackJack(dealerHand, dealerCardAmount) == 1) {

        ShowCards();
        printf("Dealer had BlackJack, you lost...");
        return 0;
    }

    ShowCards();

    char answer;
    do {
        printf("\nDo you wish to receive another card? y/n >");
        do {
            scanf(" %c", &answer);
            if (answer != 'y' && answer != 'n') printf("Could not determine answer, please enter again >");

        } while (answer != 'y' && answer != 'n');

        if (answer == 'y') {
            DealCard(player);
            ShowCards();

            if (GreatestLegalValue(playerHand, playerCardAmount) == 0) {
                printf("You have more than 21 \n You lost...\n");
                bet = 0;
                return 0;
            }
        }
    } while (answer != 'n');

    GiveCardsToDealer();
    ShowCards();

    int dealerCardVal = GreatestLegalValue(dealerHand, dealerCardAmount);
    int playerCardVal = GreatestLegalValue(playerHand, playerCardAmount);
    if (dealerCardVal >= playerCardVal) {
        printf("Dealer (%d) had more points than you (%d) and won", dealerCardVal, playerCardVal);
        bet = 0;
    }
    else {
        printf("You (%d) had more points than dealer (%d) and won", playerCardVal, dealerCardVal);
        money += bet * 2;
    }
}

int IsBlackJack(int hand[], int size) {

    for (int i = 0; i < size; i++) {
        if (hand[i] % 13 == 0) {
            for (int j = 0; j < size; j++) {
                if (hand[j] % 13 >= 10) return 1;
            }
            return 0;
        }
    }
    return 0;
}

void ShowCards() { //Show dealers cards and players cards all face-up
    printf("Dealer Hand (%d):\n", GreatestLegalValue(dealerHand, dealerCardAmount));
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < dealerCardAmount; j++) {

            int suit = dealerHand[j] / 13;
            int value = dealerHand[j] % 13;

            printf("   ");
            for (int k = 0; k < 10; ++k) {
                printf("%c", cards[suit] [value] [i] [k]);
            }
        }
        printf("\n");
    }

    printf("Your Hand (%d):\n", GreatestLegalValue(playerHand, playerCardAmount));
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < playerCardAmount; j++) {

            int suit = playerHand[j] / 13;
            int value = playerHand[j] % 13;

            printf("   ");
            for (int k = 0; k < 10; ++k) {
                printf("%c", cards[suit] [value] [i] [k]);
            }
        }
        printf("\n");
    }
}

void InitCards() { //Make cards from the array

    //Make skeleton
    for (int i = 0; i < 4; ++i) { //Choose suit
        for (int j = 0; j < 13; ++j) { //Choose which card type
            int k = 0;

            //Make top row
            for (int l = 0; l < 10; ++l) {
                cards[i][j][k][l] = '-';
            }

            //fill 'er up with spaces
            for (k = 1; k < 5; ++k) {
                int l = 0;
                cards[i][j][k][l] = '|'; //Make left edge

                for (l = 1; l < 9; ++l) {
                    cards[i][j][k][l] = ' ';
                }

                cards[i][j][k][l] = '|'; //Make right edge
            }

            //Make bottom Row
            for (int l = 0; l < 10; ++l) {
                cards[i][j][k][l] = '-';
            }
        }
    }

    //Give a visible value to the card
    char cardValues[13] = {'A','2','3','4','5','6','7','8','9',' ','J','Q','K'};
    for (int i = 0; i < 4; ++i) { //For each suit

        for (int j = 0; j < 13; ++j) { //for each value

            //Write the value in the corners
            if (j == 9) { //Since '10' would be a string we have to resolve it by other means

                //Top left corner
                cards[i][j][1][1] = '1';
                cards[i][j][1][2] = '0';

                //Bottom right corner
                cards[i][j][4][7] = '1';
                cards[i][j][4][8] = '0';
            }
            else {

                //Top left corner
                cards[i][j][1][1] = cardValues[j];

                //Bottom right corner
                cards[i][j][4][8] = cardValues[j];
            }

            //Make a sign in the card for the suit
            /*  /\         /\       */
            /* /  \ /\/\  /  \  ()  */
            /* \  / \  /  \/\/ (  ) */
            /*  \/   \/    ||   /\  */
            if (i == 0) { //Diamonds
                /*  /\  */ cards[i][j][1][4] = '/'; cards[i][j][1][5] = '\\';
                /* /  \ */ cards[i][j][2][3] = '/'; cards[i][j][2][6] = '\\';
                /* \  / */ cards[i][j][3][3] = '\\'; cards[i][j][3][6] = '/';
                /*  \/  */ cards[i][j][4][4] = '\\'; cards[i][j][4][5] = '/';
            }
            else if (i == 1) { //Hearts
                /*      */
                /* /\/\ */ cards[i][j][2][3] = '/'; cards[i][j][2][4] = '\\'; cards[i][j][2][5] = '/'; cards[i][j][2][6] = '\\';
                /* \  / */ cards[i][j][3][3] = '\\'; cards[i][j][3][6] = '/';
                /*  \/  */ cards[i][j][4][4] = '\\'; cards[i][j][4][5] = '/';
            }
            else if (i == 2) { //Spades
                /*  /\  */ cards[i][j][1][4] = '/'; cards[i][j][1][5] = '\\';
                /* /  \ */ cards[i][j][2][3] = '/'; cards[i][j][2][6] = '\\';
                /* \/\/ */ cards[i][j][3][3] = '\\'; cards[i][j][3][4] = '/'; cards[i][j][3][5] = '\\'; cards[i][j][3][6] = '/';
                /*  ||  */ cards[i][j][4][4] = '|'; cards[i][j][4][5] = '|';
            }
            else { //Clubs
                /*      */
                /*  ()  */ cards[i][j][2][4] = '('; cards[i][j][2][5] = ')';
                /* (  ) */ cards[i][j][3][3] = '('; cards[i][j][3][6] = ')';
                /*  /\  */ cards[i][j][4][4] = '/'; cards[i][j][4][5] = '\\';
            }
        }


    }
}

void ResetDeck() {
    for (int i = 0; i < 4; i++) { //I REALLY dont wanna declare it manually
        for (int j = 0; j < 13; j++) {
            deck[i][j] = 1;
        }
    }
    for (int i = 0; i < cardsInDeck; i++) {
        playerHand[i] = 0;
        dealerHand[i] = 0;
    }
}

void DealCard(int person) {
    int dealtCard;
    do {
        dealtCard = rand() % 52;
    } while (deck[dealtCard / 13][dealtCard % 13] == 0);
    deck[dealtCard / 13][dealtCard % 13] = 0;

    if (person == 1) dealerHand[dealerCardAmount++] = dealtCard;
    else if (person == 2) playerHand[playerCardAmount++] = dealtCard;

    else exit(EXIT_FAILURE);
}

void GiveCardsToDealer() {

    while (GreatestLegalValue(dealerHand, dealerCardAmount) < 17) {
        DealCard(dealer);
    }
    if (GreatestLegalValue(dealerHand, dealerCardAmount) > 21) {
        printf("Dealer had more than 21, You win!\n");
    }
}

int GreatestLegalValue(int hand[], int size) {

    int totalValue = 0;
    int aces = 0;
    for (int i = 0; i < size; ++i) { //Find total value of all cards combined

        int card = hand[i] % 13;

        if (card > 0 && card < 9) totalValue += card + 1; //If it's a number 2-9

        else if (card >= 9) totalValue += 10; //If it is a 10 or a face card

        else if (card == 0) {totalValue += 11; aces++;}; //If it's an ace
    }

    if (totalValue > 21) { //If above limit then convert aces to 1 instead of 11
        if (aces) {
            for (int i = 0; i < aces; ++i) {
                if (totalValue > 21) totalValue -= 10;
            }
            if (totalValue > 21) return 0; //You still lost
            else return totalValue;
        }
        else return 0; //Lost game
    }
    else return totalValue;
}