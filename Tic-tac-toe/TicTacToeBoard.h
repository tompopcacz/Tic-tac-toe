#ifndef TicTacToeBoard
#define TicTacToeBoard
#include <stdio.h>

#define EMPTY_FIELD 0
#define PLAYER_1_FIELD 1
#define PLAYER_2_FIELD 2

#define MAX_NAME_LEN 50

#define PLAYER_1_TURN 1
#define PLAYER_2_TURN 2

#define EMPTY_FIELD_CHAR ' '
#define LEADER_BOARD_MAX_LINE_COUNT 10

#define LEADEARBOARD_FILE_NAME "./leaderboard"
#define NAME_WINS_SEP '~'
#define NAME_TABLE_HEADER "Player"
#define WIN_TABLE_HEADER "Number of wins"

/// <summary>
/// Inicializuje hraci pole. Pokud se nejedna o prvni hru, uvolni pamet hraciho pole, nastavi novou 
/// sirku a vysku pole, alokuje pro pole pamet, nastavi vsechna policka na prazdna, vybere zacinajicicho hrace
/// a urci nahodne zacinajiciho hrace.
/// </summary>
/// <param name="width">Sirka hraciho pole</param>
/// <param name="height">Vyska hraciho pole</param>
/// <param name="firstTime">Rika, zda se jedna o prvni hru od spusteni programu</param>
/// <returns>Vraci 1 pokud se povede inicializace, jinak 0</returns>
int initBoard(int width, int height, int firstTime);

/// <summary>
/// Vykresli hraci pole piskvorek nahore a po stranach jsou cisla urcujici souradnice x a y - ale jde 
/// pouze o jednotky, aby byla policka ve ctvrecove matici.
/// </summary>
void printBoard();

/// <summary>
/// Pokusi se provest jedno kolo tahu hry.
/// </summary>
/// <returns>
/// 1 = tah v poradku, aktualni hrac na tahu vyhral 
/// 0 = tah v poradku
/// -1 = x a y jsou mimo hraci desku
/// -2 = pokus o obsazeni jiz obsazeneho policka
/// -3 = vsechna policka jsou plna ale neni vyherce
/// </returns>
int makeTurn(int x, int y);

/// <summary>
/// Nacte tabulku nejlepsich hracu.
/// </summary>
void loadLeaderboard();

/// <summary>
/// Ulozi tabulku nejlepsich hracu.
/// </summary>
/// <returns>Vraci 1 pokud se ulozeni povede, jinak 0</returns>
int saveLeaderBoard();

/// <summary>
/// Vypise tabulku nejlepsich hracu.
/// </summary>
/// <param name="fd">File descriptor, kam se ma vytisknout tabulka nejlepsich hracu.</param>
/// <param name="separator">char, ktery oddeluje jmeno a pocet vitezstci</param>
/// <param name="pad">1 = zarovnat, 0 = nezarovnat</param>
/// <param name="printHeader">1 = vypise hlavicku, 0 = nevypise</param>
void printLeaderBoard(FILE* fd, char separator, int pad, int printHeader);

struct Board_t
{
	int width;
	int height;
	int fieldsToWin;

	int* boardFields;
};

struct Player_t
{
	char name[MAX_NAME_LEN];
	long wins;
	char fieldMark;
};

struct leaderboardItem_t
{
	char name[MAX_NAME_LEN];
	int wins;
};

struct leaderboard_t
{
	leaderboardItem_t items[LEADER_BOARD_MAX_LINE_COUNT];
	int loaded;
};

extern leaderboard_t leaderboard;
extern int playerOnTurn;
extern int turnCnt;
extern struct Player_t playerOne; 
extern struct Player_t playerTwo;
extern struct Board_t board;

#endif