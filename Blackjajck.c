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
    { 0 , 11, "�X�y�[�h" },
    { 1 , 2,  "�X�y�[�h" },
    { 2 , 3,  "�X�y�[�h" },
    { 3 , 4,  "�X�y�[�h" },
    { 4 , 5,  "�X�y�[�h" },
    { 5 , 6,  "�X�y�[�h" },
    { 6 , 7,  "�X�y�[�h" },
    { 7 , 8,  "�X�y�[�h" },
    { 8 , 9,  "�X�y�[�h" },
    { 9 , 10, "�X�y�[�h" },
    { 10, 10, "�X�y�[�h" },
    { 11, 10, "�X�y�[�h" },
    { 12, 10, "�X�y�[�h" },
    { 13, 11, "�n�[�g"   },
    { 14, 2,  "�n�[�g"   },
    { 15, 3,  "�n�[�g"   },
    { 16, 4,  "�n�[�g"   },
    { 17, 5,  "�n�[�g"   },
    { 18, 6,  "�n�[�g"   },
    { 19, 7,  "�n�[�g"   },
    { 20, 8,  "�n�[�g"   },
    { 21, 9,  "�n�[�g"   },
    { 22, 10, "�n�[�g"   },
    { 23, 10, "�n�[�g"   },
    { 24, 10, "�n�[�g"   },
    { 25, 10, "�n�[�g"   },
    { 26, 11, "�_�C��"   },
    { 27, 2,  "�_�C��"   },
    { 28, 3,  "�_�C��"   },
    { 29, 4,  "�_�C��"   },
    { 30, 5,  "�_�C��"   },
    { 31, 6,  "�_�C��"   },
    { 32, 7,  "�_�C��"   },
    { 33, 8,  "�_�C��"   },
    { 34, 9,  "�_�C��"   },
    { 35, 10, "�_�C��"   },
    { 36, 10, "�_�C��"   },
    { 37, 10, "�_�C��"   },
    { 38, 10, "�_�C��"   },
    { 39, 11, "�N���u"   },
    { 40, 2,  "�N���u"   },
    { 41, 3,  "�N���u"   },
    { 42, 4,  "�N���u"   },
    { 43, 5,  "�N���u"   },
    { 44, 6,  "�N���u"   },
    { 45, 7,  "�N���u"   },
    { 46, 8,  "�N���u"   },
    { 47, 9,  "�N���u"   },
    { 48, 10, "�N���u"   },
    { 49, 10, "�N���u"   },
    { 50, 10, "�N���u"   },
    { 51, 10, "�N���u"   }
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
            printf("�v���C���[���v : %d\n", s_playerPoint);
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
    for(i = 0; i < CARD_INDEX_MAX; i++){ //����
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
    printf("�v���C���[���v : %d\n", s_playerPoint);
    printf("�f�B�[���[���v : %d\n", s_dealerPoint);
    switch(result){
    case E_RESULT_WIN:
        printf("�v���C���[�̏����ł��B\n");
        break;
    case E_RESULT_EVEN:
        printf("���������ł��B\n");
        break;
    case E_RESULT_LOSE:
        printf("�v���C���[�̕����ł��B\n");
        break;
   default:
        printf("�G���[���������܂����B�v���O�������I�����܂��B\n");
        break;
    }
}

BOOL input(void)
{
    char inputChar = 0;
    printf("�J�[�h��ǉ����܂����H : y/N\n");
    while(TRUE){
        
        inputChar = getchar();
        if (inputChar == 'y'){
            return TRUE;
        }
        if (inputChar == 'N'){
            return FALSE;
        }
        printf("'y'��'N'����͂��Ă��������B\n");
    }
}

void printA(BOOL isPlayer, ST_Card* phand, unsigned int drawCount)
{
    char playerOrDealer[16];
    char player[16] = "�v���C���[";
    char dealer[16] = "�f�B�[���[";

    if (isPlayer){
        strcpy(playerOrDealer,player);
    } else {
        strcpy(playerOrDealer,dealer);
    }
    printf("%s: %s��%d\n", playerOrDealer , phand[(drawCount - 1)].mark ,(phand[(drawCount - 1)].number % CARD_NUMBER_MAX) + 1);
}

void printB(ST_Card* dealerHand)
{
    printf("�f�B�[���[%d���� : %s��%d\n", s_dealerDrawCount, dealerHand[(s_dealerDrawCount - 1)].mark, (dealerHand[(s_dealerDrawCount - 1)].number % CARD_NUMBER_MAX) + 1);
}