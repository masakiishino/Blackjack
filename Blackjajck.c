#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

typedef enum{
    FALSE,
    TRUE
} BOOL;

typedef enum{
    E_RESULT_WIN,
    E_RESULT_EVEN,
    E_RESULT_LOSE,
    E_RESULT_INVALID
} E_RESULT;

typedef struct{
    unsigned int number;
    unsigned int point;
    char mark[12];
} ST_Card;

static unsigned int s_dealerPoint     = 0;
static unsigned int s_playerPoint     = 0;
static unsigned int s_dealerAceCount  = 0;
static unsigned int s_playerAceCount  = 0;
static unsigned int s_dealerDrawCount = 0;
static unsigned int s_playerDrawCount = 0;


const static ST_Card s_cardsTbl[] =
{
    { 0 , 11, "スペード" },
    { 1 , 2,  "スペード" },
    { 2 , 3,  "スペード" },
    { 3 , 4,  "スペード" },
    { 4 , 5,  "スペード" },
    { 5 , 6,  "スペード" },
    { 6 , 7,  "スペード" },
    { 7 , 8,  "スペード" },
    { 8 , 9,  "スペード" },
    { 9 , 10, "スペード" },
    { 10, 10, "スペード" },
    { 11, 10, "スペード" },
    { 12, 10, "スペード" },
    { 13, 11, "ハート"   },
    { 14, 2,  "ハート"   },
    { 15, 3,  "ハート"   },
    { 16, 4,  "ハート"   },
    { 17, 5,  "ハート"   },
    { 18, 6,  "ハート"   },
    { 19, 7,  "ハート"   },
    { 20, 8,  "ハート"   },
    { 21, 9,  "ハート"   },
    { 22, 10, "ハート"   },
    { 23, 10, "ハート"   },
    { 24, 10, "ハート"   },
    { 25, 10, "ハート"   },
    { 26, 11, "ダイヤ"   },
    { 27, 2,  "ダイヤ"   },
    { 28, 3,  "ダイヤ"   },
    { 29, 4,  "ダイヤ"   },
    { 30, 5,  "ダイヤ"   },
    { 31, 6,  "ダイヤ"   },
    { 32, 7,  "ダイヤ"   },
    { 33, 8,  "ダイヤ"   },
    { 34, 9,  "ダイヤ"   },
    { 35, 10, "ダイヤ"   },
    { 36, 10, "ダイヤ"   },
    { 37, 10, "ダイヤ"   },
    { 38, 10, "ダイヤ"   },
    { 39, 11, "クラブ"   },
    { 40, 2,  "クラブ"   },
    { 41, 3,  "クラブ"   },
    { 42, 4,  "クラブ"   },
    { 43, 5,  "クラブ"   },
    { 44, 6,  "クラブ"   },
    { 45, 7,  "クラブ"   },
    { 46, 8,  "クラブ"   },
    { 47, 9,  "クラブ"   },
    { 48, 10, "クラブ"   },
    { 49, 10, "クラブ"   },
    { 50, 10, "クラブ"   },
    { 51, 10, "クラブ"   }
};

#define DEALER 0
#define PLAYER 1
#define FIRST_DRAW 1
#define SECOND_DRAW 2
#define TARGET_NUMBER 21
#define CARD_INDEX_MAX 52
#define CARD_NUMBER_MAX 13
#define SHUFFLE_COUNT 500
#define HAND_MAX 11
#define DIFFERENCE_ACE_POINT 10
#define ACE_POINT 11
#define DEALER_STAY_SCORE 16

void shuffle(unsigned int* pdeck);
void draw(unsigned int* pdeck, ST_Card* phand, unsigned int* pdrawCount, unsigned int* ppoint,unsigned int* paceCount);
void judgment(E_RESULT* result);
void printResult(E_RESULT result);
void printA(BOOL isPlayer, ST_Card* phand, unsigned int drawCount);
void printB(ST_Card* dealerHand);
BOOL input(void);

int main(void)
{
    unsigned int deck[CARD_INDEX_MAX];
    ST_Card dealerHand[HAND_MAX];
    ST_Card playerHand[HAND_MAX];
    E_RESULT result = E_RESULT_INVALID;
    BOOL drawFlag = TRUE;

    shuffle(deck);
    draw(deck, dealerHand, &s_dealerDrawCount, &s_dealerPoint, &s_dealerAceCount);
    draw(deck, dealerHand, &s_dealerDrawCount, &s_dealerPoint, &s_dealerAceCount);
    draw(deck, playerHand, &s_playerDrawCount, &s_playerPoint, &s_playerAceCount);
    draw(deck, playerHand, &s_playerDrawCount, &s_playerPoint, &s_playerAceCount);

    printA(DEALER, dealerHand, FIRST_DRAW);
    printA(PLAYER, playerHand, FIRST_DRAW);
    printA(PLAYER, playerHand, SECOND_DRAW);

    if (s_playerPoint == TARGET_NUMBER){
    } else {
        if (s_playerAceCount == 2){
            s_playerAceCount --;
            s_playerPoint = s_playerPoint - DIFFERENCE_ACE_POINT;
        }
        while(drawFlag){
            printf("プレイヤー合計 : %d\n", s_playerPoint);
            if (input()){
                draw(deck, playerHand, &s_playerDrawCount, &s_playerPoint, &s_playerAceCount);
                printA(PLAYER, playerHand, s_playerDrawCount);
                if (s_playerPoint == TARGET_NUMBER){
                	printB(dealerHand);
                    judgment(&result);
                    printResult(result);
                    return 0;
                } else {
                    if (s_playerPoint > TARGET_NUMBER){
                        if (s_playerAceCount == 0){
                        	printB(dealerHand);
                            judgment(&result);
                            printResult(result);
                            return 0;
                        } else {
                            s_playerAceCount --;
                            s_playerPoint = s_playerPoint - DIFFERENCE_ACE_POINT;
                        }
                    }
                }
            } else {
                drawFlag = FALSE;
            }
        }
    }
    printB(dealerHand);
    if (s_dealerAceCount == 2){
        s_dealerAceCount --;
        s_dealerPoint = s_dealerPoint - DIFFERENCE_ACE_POINT;
    }
    drawFlag = TRUE;
    
    while(drawFlag){
        if(s_dealerPoint  <= DEALER_STAY_SCORE){
            draw(deck, dealerHand, &s_dealerDrawCount, &s_dealerPoint, &s_dealerAceCount);
            printB(dealerHand);
            if (s_dealerPoint > TARGET_NUMBER){
                if (s_dealerAceCount == 0){
                    drawFlag = FALSE;
                } else {
                    s_dealerAceCount --;
                    s_dealerPoint  = s_dealerPoint - DIFFERENCE_ACE_POINT;
                }
            }
        } else {
            drawFlag = FALSE;
        }
    }
    judgment(&result);
    printResult(result);
    return 0;
}

void shuffle(unsigned int* pdeck)
{
    unsigned int randomNumber;
    unsigned int temp;
    unsigned int i;
    srand((unsigned int) time(NULL));
    for(i = 0; i < CARD_INDEX_MAX; i++){ //条件
        pdeck[i] = i;
    }
    for(i = 0; i < SHUFFLE_COUNT; i++){
        randomNumber =  rand() % CARD_INDEX_MAX;
        temp = pdeck[(i % CARD_INDEX_MAX)];
        pdeck[(i % CARD_INDEX_MAX)] = pdeck[randomNumber];
        pdeck[randomNumber] = temp;
    }
}

void draw(unsigned int* pdeck, ST_Card* phand, unsigned int* pdrawCount, unsigned int* ppoint,unsigned int* paceCount)
{
    unsigned int i;

    for(i = 0; i < CARD_INDEX_MAX; i++){
        if (pdeck[(s_playerDrawCount + s_dealerDrawCount)] == s_cardsTbl[i].number){
            break;
        }
    }
    phand[*pdrawCount] =  s_cardsTbl[i];
    *ppoint +=  s_cardsTbl[i].point;
    if (phand[*pdrawCount].point  == ACE_POINT){
        *paceCount += 1;
    }
    *pdrawCount += 1;
}

void judgment(E_RESULT* result)
{
    if ((s_playerPoint > TARGET_NUMBER) || (s_dealerPoint  <= TARGET_NUMBER && s_playerPoint < s_dealerPoint)){
        *result = E_RESULT_LOSE;
    } else {
        if (s_playerPoint == s_dealerPoint){
            *result = E_RESULT_EVEN;
        } else {
            *result = E_RESULT_WIN;
        }
    }
}

void printResult(E_RESULT result)
{
    printf("プレイヤー合計 : %d\n", s_playerPoint);
    printf("ディーラー合計 : %d\n", s_dealerPoint);
    switch(result){
    case E_RESULT_WIN:
        printf("プレイヤーの勝ちです。\n");
        break;
    case E_RESULT_EVEN:
        printf("引き分けです。\n");
        break;
    case E_RESULT_LOSE:
        printf("プレイヤーの負けです。\n");
        break;
   default:
        printf("エラーが発生しました。プログラムを終了します。\n");
        break;
    }
}

BOOL input(void)
{
    char inputChar = 0;
    printf("カードを追加しますか？ : y/N\n");
    while(TRUE){
        
        inputChar = getchar();
        if (inputChar == 'y'){
            return TRUE;
        }
        if (inputChar == 'N'){
            return FALSE;
        }
        printf("'y'か'N'を入力してください。\n");
    }
}

void printA(BOOL isPlayer, ST_Card* phand, unsigned int drawCount)
{
    char playerOrDealer[16];
    char player[16] = "プレイヤー";
    char dealer[16] = "ディーラー";

    if (isPlayer){
        strcpy(playerOrDealer,player);
    } else {
        strcpy(playerOrDealer,dealer);
    }
    printf("%s: %sの%d\n", playerOrDealer , phand[(drawCount - 1)].mark ,(phand[(drawCount - 1)].number % CARD_NUMBER_MAX) + 1);
}

void printB(ST_Card* dealerHand)
{
    printf("ディーラー%d枚目 : %sの%d\n", s_dealerDrawCount, dealerHand[(s_dealerDrawCount - 1)].mark, (dealerHand[(s_dealerDrawCount - 1)].number % CARD_NUMBER_MAX) + 1);
}