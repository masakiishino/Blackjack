#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

typedef enum{
    E_FALSE,
    E_TRUE
} E_BOOL;

typedef enum{
    E_RESULT_WIN,
    E_RESULT_EVEN,
    E_RESULT_LOSE,
    E_RESULT_INVALID
} E_RESULT;

typedef struct{
    unsigned int number;
    unsigned int point;
    char mark[13]; /* 日本語の文字コードが一文字3Byteなので、四文字+null文字の1Byteで13Byte */
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

#define DEALER (0)
#define PLAYER (1)
#define FIRST_DRAW (1)
#define SECOND_DRAW (2)
#define TARGET_NUMBER (21)
#define CARD_INDEX_MAX (52)
#define CARD_NUMBER_MAX (13)
#define SHUFFLE_COUNT (500)
#define HAND_MAX (11)
#define DIFFERENCE_ACE_POINT (10)
#define ACE_POINT (11)
#define DEALER_STAY_SCORE (16)

//#define DEBUG
//#define LOGIC_TEST_1
//#define LOGIC_TEST_2
//#define LOGIC_TEST_3
//#define LOGIC_TEST_4

#ifdef LOGIC_TEST_1
#define DBG_PRINT1(...) printf("LOGIC_TEST_1 [%d/10] OK!!!\n",__VA_ARGS__);
#else
#define DBG_PRINT1(...)
#endif

#ifdef LOGIC_TEST_2
#define DBG_PRINT2(...) printf("LOGIC_TEST_2 [%d/5] OK!!!\n",__VA_ARGS__);
#else
#define DBG_PRINT2(...)
#endif

#ifdef LOGIC_TEST_3
#define DBG_PRINT3(...) printf("LOGIC_TEST_3 [%d/2] OK!!!\n",__VA_ARGS__);
#else
#define DBG_PRINT3(...)
#endif

#ifdef LOGIC_TEST_4
#define DBG_PRINT4(...) printf("LOGIC_TEST_4 [%d/3] OK!!!\n",__VA_ARGS__);
#else
#define DBG_PRINT4(...)
#endif

void shuffleDeck(unsigned int* pDeck);
void drawCard(unsigned int* pDeck, ST_Card* pHand, unsigned int* pDrawCount, unsigned int* pPoint,unsigned int* pAceCount);
E_RESULT resultJudgment(void);
void printResult(E_RESULT result);
void printA(E_BOOL isPlayer, ST_Card* pHand, unsigned int drawCount);
void printB(ST_Card* pDealerHand);
E_BOOL isInput(void);

int main(void)
{
    unsigned int deck[CARD_INDEX_MAX];
    ST_Card dealerHand[HAND_MAX];
    ST_Card playerHand[HAND_MAX];
    E_RESULT result = E_RESULT_INVALID;

    #ifdef DEBUG
    unsigned int i,j;
    unsigned temp = 0;
    unsigned debugAceCount = 0;
    E_BOOL errorFlag = E_FALSE;
    #endif

    shuffleDeck(deck);

    #ifdef DEBUG
    printf("----- shuffleDeck test start -----\n");
    for (i = 0; i < CARD_INDEX_MAX; i++) {
        printf("deck = %d\n",deck[i]);
    }
    /* バブルソートでdeckの配列を順番に並び替える */
    for (i = 0; i < CARD_INDEX_MAX; i++) {
      for (j = CARD_INDEX_MAX; j > i; j--) {
        if (deck[j-1] > deck[j]) {
          temp = deck[j-1];
          deck[j-1] = deck[j];
          deck[j] = temp;
        }
      }
    }
    /* 並び替えた値とiを比較し、数字に抜けがない事を確認 */
    for (i = 0; i < CARD_INDEX_MAX; i++) {
        if (deck[i] != i) {
            printf("[[ERROR] deck = %d    i = %d\n",deck[i], i);
            errorFlag = E_TRUE;
        }
    }
    if (errorFlag) {
        printf("[ERROR]shuffleDeck failed");
    }
    printf("----- shuffleDeck test end -----\n");

    for (i = 0; i < 6; i++) {
        printf("deck[%d] = ",i);
        scanf("%d",&deck[i]);
    }
    #endif

    drawCard(deck, dealerHand, &s_dealerDrawCount, &s_dealerPoint, &s_dealerAceCount);
    drawCard(deck, dealerHand, &s_dealerDrawCount, &s_dealerPoint, &s_dealerAceCount);
    drawCard(deck, playerHand, &s_playerDrawCount, &s_playerPoint, &s_playerAceCount);
    drawCard(deck, playerHand, &s_playerDrawCount, &s_playerPoint, &s_playerAceCount);

    #ifdef DEBUG
    printf("----- drawCard test start -----\n");
    printf("deck[0] = %d\n",deck[0]);
    /* drawCardで取得したデータとテーブルの値を比較し、整合性を確認 */
    if ((dealerHand[0].number != s_cardsTbl[(deck[0])].number) ||
        (dealerHand[0].point != s_cardsTbl[(deck[0])].point) ||
        (strcmp(dealerHand[0].mark, s_cardsTbl[(deck[0])].mark) != 0)) {
        printf("[ERROR]drawCard failed : deck[1]\n");
        printf("[ERROR]drawCard : number = %d    point = %d    mark = %s\n", dealerHand[0].number, dealerHand[0].point, dealerHand[0].mark);
        printf("[ERROR]s_cardTbl: number = %d    point = %d    mark = %s\n", s_cardsTbl[(deck[0])].number, s_cardsTbl[(deck[0])].point, s_cardsTbl[(deck[0])].mark);
    } else {
        printf("OK!!!\n");
    }
    printf("deck[1] = %d\n",deck[1]);
    /* drawCardで取得したデータとテーブルの値を比較し、整合性を確認 */
    if ((dealerHand[1].number != s_cardsTbl[(deck[1])].number) ||
        (dealerHand[1].point != s_cardsTbl[(deck[1])].point) ||
        (strcmp(dealerHand[1].mark, s_cardsTbl[(deck[1])].mark) != 0)) {
        printf("[ERROR]drawCard failed : deck[1]\n");
        printf("[ERROR]drawCard : number = %d    point = %d    mark = %s\n", dealerHand[1].number, dealerHand[1].point, dealerHand[1].mark);
        printf("[ERROR]s_cardTbl: number = %d    point = %d    mark = %s\n", s_cardsTbl[(deck[1])].number, s_cardsTbl[(deck[1])].point, s_cardsTbl[(deck[1])].mark);
    } else {
        printf("OK!!!\n");
    }
    printf("deck[2] = %d\n",deck[2]);
    /* drawCardで取得したデータとテーブルの値を比較し、整合性を確認 */
    if ((playerHand[0].number != s_cardsTbl[(deck[2])].number) ||
        (playerHand[0].point != s_cardsTbl[(deck[2])].point) ||
        (strcmp(playerHand[0].mark, s_cardsTbl[(deck[2])].mark) != 0)) {
        printf("[ERROR]drawCard failed : deck[2]\n");
        printf("[ERROR]drawCard : number = %d    point = %d    mark = %s\n", playerHand[0].number, playerHand[0].point, playerHand[0].mark);
        printf("[ERROR]s_cardTbl: number = %d    point = %d    mark = %s\n", s_cardsTbl[(deck[2])].number, s_cardsTbl[(deck[2])].point, s_cardsTbl[(deck[2])].mark);
    } else {
        printf("OK!!!\n");
    }
    printf("deck[3] = %d\n",deck[3]);
    /* drawCardで取得したデータとテーブルの値を比較し、整合性を確認 */
    if ((playerHand[1].number != s_cardsTbl[(deck[3])].number) ||
        (playerHand[1].point != s_cardsTbl[(deck[3])].point) ||
        (strcmp(playerHand[1].mark, s_cardsTbl[(deck[3])].mark) != 0)) {
        printf("[ERROR]drawCard failed : deck[3]\n");
        printf("[ERROR]drawCard : number = %d    point = %d    mark = %s\n", playerHand[1].number, playerHand[1].point, playerHand[1].mark);
        printf("[ERROR]s_cardTbl: number = %d    point = %d    mark = %s\n", s_cardsTbl[(deck[3])].number, s_cardsTbl[(deck[3])].point, s_cardsTbl[(deck[3])].mark);
    } else {
        printf("OK!!!\n");
    }
    /* s_dealerDrawCountが正しいか確認 */
    if (s_dealerDrawCount != 2) {
        printf("[ERROR]drawCard failed : s_dealerDrawCountが正しく設定されていること\n");
        printf("[ERROR]s_dealerDrawCount = %d\n",s_dealerDrawCount);
        printf("[ERROR]s_dealerDrawCount = 2\n");
    } else {
        printf("s_dealerDrawCount OK!!!\n");
    }
    /* s_dealerPointが正しいか確認 */
    if (s_dealerPoint != (s_cardsTbl[(deck[0])].point + s_cardsTbl[(deck[1])].point)) {
        printf("[ERROR]drawCard failed : s_dealerPoint\n");
        printf("[ERROR]drawCard s_dealerPoint = %d\n",s_dealerPoint);
        printf("[ERROR]s_dealerPoint = %d\n",s_cardsTbl[(deck[0])].point + s_cardsTbl[(deck[1])].point);
    } else {
        printf("s_dealerPoint OK!!!\n");
    }
    /* s_dealerAceCountが正しいか確認 */
    if (s_cardsTbl[(deck[0])].point == ACE_POINT) {
        debugAceCount++;
    }
    if (s_cardsTbl[(deck[1])].point == ACE_POINT) {
        debugAceCount++;
    }
    if (s_dealerAceCount != debugAceCount) {
        printf("[ERROR]drawCard failed : s_dealerAceCount\n");
        printf("[ERROR]drawCard s_dealerAceCount = %d\n",s_dealerPoint);
        printf("[ERROR]s_dealerAceCount = %d\n",debugAceCount);
    } else {
        printf("s_dealerAceCount OK!!!\n");
    }
    /* s_playerDrawCountが正しいか確認 */
    if (s_playerDrawCount != 2) {
        printf("[ERROR]drawCard failed : s_playerDrawCount\n");
        printf("[ERROR]s_playerDrawCount = %d\n",s_playerDrawCount);
        printf("[ERROR]s_playerDrawCount = 2\n");
    } else {
        printf("s_playerDrawCount OK!!!\n");
    }
    /* s_playerPointが正しいか確認 */
    if (s_playerPoint != (s_cardsTbl[(deck[2])].point + s_cardsTbl[(deck[3])].point)) {
        printf("[ERROR]drawCard failed : s_playerPoint\n");
        printf("[ERROR]drawCard s_playerPoint = %d\n",s_playerPoint);
        printf("[ERROR]s_playerPoint = %d\n",s_cardsTbl[(deck[2])].point + s_cardsTbl[(deck[3])].point);
    } else {
        printf("s_playerPoint OK!!!\n");
    }
    /* s_playerAceCountが正しいか確認 */
    debugAceCount = 0;
    if (s_cardsTbl[(deck[2])].point == ACE_POINT) {
        debugAceCount++;
    }
    if (s_cardsTbl[(deck[3])].point == ACE_POINT) {
        debugAceCount++;
    }
    if (s_playerAceCount != debugAceCount) {
        printf("[ERROR]drawCard failed : s_playerAceCount\n");
        printf("[ERROR]drawCard s_playerAceCount = %d\n",s_playerAceCount);
        printf("[ERROR]s_playerAceCount = %d\n",debugAceCount);
    } else {
        printf("s_playerAceCount OK!!!\n");
    }
    printf("----- drawCard test end -----\n");
    #endif

    printA(DEALER, dealerHand, FIRST_DRAW);
    printA(PLAYER, playerHand, FIRST_DRAW);
    printA(PLAYER, playerHand, SECOND_DRAW);

    if (s_playerPoint == TARGET_NUMBER) {
        /* do nothing */
        DBG_PRINT3(1);
    } else {
        DBG_PRINT1(1);
        if (s_playerAceCount == 2) {
            s_playerAceCount --;
            s_playerPoint = s_playerPoint - DIFFERENCE_ACE_POINT;
            DBG_PRINT1(2);
        }
        DBG_PRINT2(1);

        while(1){
            printf("プレイヤー合計 : %d\n", s_playerPoint);
            if (isInput()) {
                drawCard(deck, playerHand, &s_playerDrawCount, &s_playerPoint, &s_playerAceCount);
                printA(PLAYER, playerHand, s_playerDrawCount);
                DBG_PRINT1(3);
                if (s_playerPoint == TARGET_NUMBER) {
                    DBG_PRINT2(4);
                    break;
                } else {
                    DBG_PRINT1(4);
                    if (s_playerPoint > TARGET_NUMBER) {
                        DBG_PRINT2(2);
                        if (s_playerAceCount == 0) {
                            DBG_PRINT4(1);
                            break;
                        } else {
                            s_playerAceCount --;
                            s_playerPoint = s_playerPoint - DIFFERENCE_ACE_POINT;
                            DBG_PRINT2(3);
                        }
                    }
                    DBG_PRINT1(5);
                }
            } else {
                DBG_PRINT1(6);
                break;
            }
        }
    }
    printB(dealerHand);
    if (s_dealerAceCount == 2) {
        s_dealerAceCount --;
        s_dealerPoint = s_dealerPoint - DIFFERENCE_ACE_POINT;
        DBG_PRINT1(7);
    }
    DBG_PRINT2(5);

    while(1){
        if (s_dealerPoint <= DEALER_STAY_SCORE) {
            drawCard(deck, dealerHand, &s_dealerDrawCount, &s_dealerPoint, &s_dealerAceCount);
            printB(dealerHand);
            DBG_PRINT1(8);
            if (s_dealerPoint > TARGET_NUMBER) {
                DBG_PRINT4(2);
                if (s_dealerAceCount == 0) {
                    DBG_PRINT4(3);
                    break;
                } else {
                    s_dealerAceCount --;
                    s_dealerPoint = s_dealerPoint - DIFFERENCE_ACE_POINT;
                    DBG_PRINT3(2);
                }
            }
            DBG_PRINT1(9);
        } else {
            DBG_PRINT1(10);
            break;
        }
    }
    result = resultJudgment();
    #ifdef DEBUG
    printf("----- resultJudgment test start -----\n");
    if (result == E_RESULT_INVALID) {
        printf("[ERROR]resultJudgment failed : result = %d\n",result);
    } else {
        printf("result = %d OK!!!\n",result);
    }
    printf("----- resultJudgment test end -----\n");
    #endif
    printResult(result);
    return 0;
}

void shuffleDeck(unsigned int* pDeck)
{
    unsigned int randomNumber;
    unsigned int temp;
    unsigned int i;
    srand((unsigned int) time(NULL));
    for (i = 0; i < CARD_INDEX_MAX; i++) {
        pDeck[i] = i;
    }
    for (i = 0; i < SHUFFLE_COUNT; i++) {
        randomNumber = rand() % CARD_INDEX_MAX;
        temp = pDeck[(i % CARD_INDEX_MAX)];
        pDeck[(i % CARD_INDEX_MAX)] = pDeck[randomNumber];
        pDeck[randomNumber] = temp;
    }
}

void drawCard(unsigned int* pDeck, ST_Card* pHand, unsigned int* pDrawCount, unsigned int* pPoint,unsigned int* pAceCount)
{
    unsigned int i;

    for (i = 0; i < CARD_INDEX_MAX; i++) {
        if (pDeck[(s_playerDrawCount + s_dealerDrawCount)] == s_cardsTbl[i].number) {
            break;
        }
    }
    pHand[*pDrawCount] = s_cardsTbl[i];
    *pPoint += s_cardsTbl[i].point;
    if (pHand[*pDrawCount].point == ACE_POINT) {
        *pAceCount += 1;
    }
    *pDrawCount += 1;
}

E_RESULT resultJudgment(void)
{
    E_RESULT result;
    if ((s_playerPoint > TARGET_NUMBER) || ((s_dealerPoint <= TARGET_NUMBER) && (s_playerPoint < s_dealerPoint))) {
        result = E_RESULT_LOSE;
    } else {
        if (s_playerPoint == s_dealerPoint) {
            result = E_RESULT_EVEN;
        } else {
            result = E_RESULT_WIN;
        }
    }
    return result;
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

E_BOOL isInput(void)
{
    char inputChar = 0;
    printf("カードを追加しますか？ : y/N\n");
    while(E_TRUE){
        inputChar = getchar();
        if (inputChar == 'y') {
            return E_TRUE;
        }
        if (inputChar == 'N') {
            return E_FALSE;
        }
        printf("'y'か'N'を入力してください。\n");
    }
}

void printA(E_BOOL isPlayer, ST_Card* pHand, unsigned int drawCount)
{
    char playerOrDealer[16]; /* 日本語の文字コードが一文字3Byteなので、五文字+null文字の1Byteで16Byte */
    char player[] = "プレイヤー";
    char dealer[] = "ディーラー";

    #ifdef DEBUG
    printf("isPlayer = %d\n",isPlayer);
    #endif

    if (isPlayer) {
        strcpy(playerOrDealer,player);
    } else {
        strcpy(playerOrDealer,dealer);
    }

    printf("%s: %sの%d\n", playerOrDealer , pHand[(drawCount - 1)].mark ,(pHand[(drawCount - 1)].number % CARD_NUMBER_MAX) + 1);
}

void printB(ST_Card* pDealerHand)
{
    printf("ディーラー%d枚目 : %sの%d\n", s_dealerDrawCount, pDealerHand[(s_dealerDrawCount - 1)].mark, (pDealerHand[(s_dealerDrawCount - 1)].number % CARD_NUMBER_MAX) + 1);
}
