#include "TicTacToeBoard.h"
#include <time.h>
#include <stdlib.h>
#include <io.h>
#include <string.h>

int playerOnTurn;
int turnCnt;
int turnPart;
struct Player_t playerOne;
struct Player_t playerTwo;
struct Board_t board;
struct leaderboard_t leaderboard { .loaded = 0 };

/// <summary>
/// Vybere zacinajiciho hrace a zacinajicimu hraci nsatvi jako znak O, druhemu X
/// </summary>
void setRandTurn()
{
	srand(time(NULL));
	playerOnTurn = rand() % 2 == 0 ? PLAYER_2_TURN : PLAYER_1_TURN;
	if (playerOnTurn == PLAYER_1_TURN)
	{
		playerOne.fieldMark = 'O';
		playerTwo.fieldMark = 'X';
		return;
	}

	playerOne.fieldMark = 'X';
	playerTwo.fieldMark = 'O';
}

/// <summary>
/// Vycistit pole od vsech tahu
/// </summary>
void clearBoard()
{
	for (int i = 0; i < board.width * board.height; i++)
		board.boardFields[i] = EMPTY_FIELD;
}

/// <summary>
/// Inicializuje hraci pole. Pokud se nejedna o prvni hru, uvolni pamet hraciho pole, nastavi novou 
/// sirku a vysku pole, alokuje pro pole pamet, nastavi vsechna policka na prazdna, vybere zacinajicicho hrace
/// a urci nahodne zacinajiciho hrace.
/// </summary>
/// <param name="width">Sirka hraciho pole</param>
/// <param name="height">Vyska hraciho pole</param>
/// <param name="firstTime">Rika, zda se jedna o prvni hru od spusteni programu</param>
/// <returns>Vraci 1 pokud se povede inicializace, jinak 0</returns>
int initBoard(int width, int height, int firstTime)
{
	if(!firstTime)
		free(board.boardFields);
	else
	{
		playerOne.wins = 0;
		playerTwo.wins = 0;
	}

	turnCnt = 1;
	turnPart = 1;
	board.width = width;
	board.height = height;

	board.boardFields = (int*)malloc(sizeof(int) * height * width);
	if (board.boardFields == NULL)
		return 0;

	clearBoard();
	setRandTurn();
	
	if (width <= 5 || height <= 5)
		board.fieldsToWin = 3;
	else
		board.fieldsToWin = 5;

	return 1;
}

/// <summary>
/// Ziska policko hrace.
/// </summary>
/// <returns>Policko hrace</returns>
char getFieldChar(int field)
{
	if (field == PLAYER_1_FIELD)
		return playerOne.fieldMark;
	else if (field == PLAYER_2_FIELD)
		return playerTwo.fieldMark;
	else
		return EMPTY_FIELD_CHAR;
}

/// <summary>
/// Vykresli hraci pole piskvorek nahore a po stranach jsou cisla urcujici souradnice x a y - ale jde 
/// pouze o jednotky, aby byla policka ve ctvrecove matici.
/// </summary>
void printBoard()
{
	// vypsat souradnice nahore
	printf("  ");
	for (int i = 0; i < board.width; ++i)
		printf("%d", (i + 1) % 10);

	// odsazeni
	printf("\n");

	// vykresleni pole
	for (int y = 0; y < board.height; ++y)
	{
		int first = 1;
		for (int x = 0; x < board.width; ++x)
		{
			// vykresleni souradnice vlevo
			if (first)
			{
				printf("%d|", (y + 1) % 10);
				first = 0;
			}

			printf("%c", getFieldChar(board.boardFields[y * board.width + x]));
		}
		printf("\n");
	}
}

/// <summary>
/// Zvysi hodnotu y, zachova hodnotu x
/// </summary>
/// <param name="x">Ukazatel na int, reprezentujici souradnici x</param>
/// <param name="y">Ukazatel na int, reprezentujici souradnici y</param>
void incYkeepX(int* x, int* y)
{
	*y = *y + 1;
}

/// <summary>
/// Zvysi hodnotu y, i x
/// </summary>
/// <param name="x">Ukazatel na int, reprezentujici souradnici x</param>
/// <param name="y">Ukazatel na int, reprezentujici souradnici y</param>
void incYincX(int* x, int* y)
{
	*y = *y + 1;
	*x = *x + 1;
}

/// <summary>
/// Zvysi hodnotu y, snizi hodnotu x
/// </summary>
/// <param name="x">Ukazatel na int, reprezentujici souradnici x</param>
/// <param name="y">Ukazatel na int, reprezentujici souradnici y</param>
void incYdecX(int* x, int* y)
{
	*y = *y + 1;
	*x = *x - 1;
}

/// <summary>
/// Zachova hodnotu y, zvysi hodnotu x
/// </summary>
/// <param name="x">Ukazatel na int, reprezentujici souradnici x</param>
/// <param name="y">Ukazatel na int, reprezentujici souradnici y</param>
void keepYincX(int* x, int* y)
{
	*x = *x + 1;
}

/// <summary>
/// Zkontroluje, jestli ma hrac na tahu spojeni potrebnych znaku k vyhre.
/// </summary>
/// <param name="x">Reprezentuje souradnici x</param>
/// <param name="y">Reprezentuje souradnici y</param>
/// <param name="prepareXY">Ukazatel na Funkci, ktera pripravi pozice x a y a tim udava smer spojovani</param>
/// <returns>Vraci 1 pokud v danem smeru byla nalezena sekvence o potrebne delce k vyhrani hry, jinak 0</returns>
int hasPlayerOnTurnWinningFields(int x, int y, void(*prepareXY)(int*, int*))
{
	int playerField = playerOnTurn == PLAYER_1_TURN ? PLAYER_1_FIELD : PLAYER_2_FIELD;
	for (int i = 0; i < board.fieldsToWin; ++i)
	{
		// doslo k prekorecni hriste
		if (x < 0 || y < 0 || x >= board.width || y >= board.height)
			return 0;

		// policko nepatri aktualnimu hraci
		if (playerField != board.boardFields[y * board.width + x])
			return 0;
		
		// prepocitani souradnic
		(*prepareXY)(&x, &y);
	}

	return 1;
}

/// <summary>
/// Zkontroluje, zda aktualni hrac vyhral hru.
/// </summary>
/// <returns>Vraci 1 pokud aktualni hrac vyhral, jinak 0</returns>
int isPlayerOnTurnWinner()
{
	for (int y = 0; y < board.height; ++y)
	{
		for (int x = 0; x < board.width; ++x)
		{
			// policko neni nutne kontrolovat (je prazdne)
			if (board.boardFields[y * board.width + x] == EMPTY_FIELD)
				continue;

			// kontrola ve smeru dolu | vprvo dolu | vlevo dolu | vpravo - pokud 
			if (hasPlayerOnTurnWinningFields(x, y, incYkeepX) || hasPlayerOnTurnWinningFields(x, y, incYincX) || 
				hasPlayerOnTurnWinningFields(x, y, incYdecX) || hasPlayerOnTurnWinningFields(x, y, keepYincX))
				return 1;
		}
	}
	return 0;
}

/// <summary>
/// Zkontroluje, zda je hraci pole uz plne.
/// </summary>
/// <returns>Vraci 1 je hraci pole uz plne, jinak 0</returns>
int isBoardFull()
{
	for (int i = 0; i < board.width * board.height; ++i)
	{
		if (board.boardFields[i] == EMPTY_FIELD)
			return 0;
	}

	return 1;
}

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
int makeTurn(int x, int y)
{
	// kotrnola, zda x a y nejsou souradnice mimo hraci pole
	if (x > board.width || y > board.height || x < 0 || y < 0)
		return -1;

	int fieldIndex = (y - 1) * board.width + (x - 1);
	// kontrola, zda neni pole jiz zabrane
	if (board.boardFields[fieldIndex] != EMPTY_FIELD)
		return -2;

	// Provedeni tahu, nastaveni potrebynuch promenych - novy hrac, ktery ma byt na tahu, nastaveni ukazatele na pocitadlo vitezstvi.
	int newPlayerOnTurn;
	long* winCount;
	if (playerOnTurn == PLAYER_1_TURN)
	{
		board.boardFields[fieldIndex] = PLAYER_1_FIELD;
		newPlayerOnTurn = PLAYER_2_TURN;
		winCount = &(playerOne.wins);
	}
	else
	{
		board.boardFields[fieldIndex] = PLAYER_2_FIELD;
		newPlayerOnTurn = PLAYER_1_TURN;
		winCount = &(playerTwo.wins);
	}

	// Kotrnola, zda aktualni hrac vyhral a pripadne zvyseni pocitadla vyher.
	if (isPlayerOnTurnWinner())
	{
		*winCount = (*winCount) + 1;
		return 1;
	}// Kontrola, zda je deska plna
	else if (isBoardFull())
		return -3;

	// Pokud hraje druhy hrac, zvysi se pocitadlo tahu
	if (++turnPart % 2 == 1)
		turnCnt++;

	playerOnTurn = newPlayerOnTurn;
	return 0;
}

/// <summary>
/// Naplni tabulku prazdnymi hodnotami
/// </summary>
void fillLeaderboard()
{
	for (int i = 0; i < LEADER_BOARD_MAX_LINE_COUNT; ++i)
	{
		leaderboard.items[i].name[0] = '\0';
		leaderboard.items[i].wins = 0;
	}
}

/// <summary>
/// Nacte tabulku nejlepsich hracu.
/// </summary>
void loadLeaderboard()
{
	fillLeaderboard(); // nastavi se do tabulky prazdne hodnoty
	leaderboard.loaded = 1;
	FILE* leaderBoardFd = fopen(LEADEARBOARD_FILE_NAME, "r");

	// soubor neexistuje/nejde otevrit => bude muset stacit prazdny leaderboard
	if (leaderBoardFd == NULL)
		return;
	
	// nacteni souboru s vysledky
	const int lineMaxLen = MAX_NAME_LEN * 2 + 1;
	char line[lineMaxLen];

	// zpracovani dat do tabulky (cteni radku)
	for (int i = 0; i < LEADER_BOARD_MAX_LINE_COUNT; ++i)
	{
		// neni co cist, konci fce
		if (fgets(line, lineMaxLen, leaderBoardFd) == NULL)
			break;

		// nacteni jmena
		int j;
		int lineLen = strlen(line);
		for (j = 0; j < lineLen && line[j] != NAME_WINS_SEP && line[j] != '\n'; ++j)
			leaderboard.items[i].name[j] = line[j];
		
		// nacteni vyher
		leaderboard.items[i].wins = j + 1 < lineLen? strtol(&(line[j + 1]), NULL, 10) : 0;
	}

	fclose(leaderBoardFd);
}

/// <summary>
/// Zarovna jmeno uzivatele zprava na MAX_NAME_LEN znaku
/// </summary>
/// <param name="nameToWrite">Ukazatel na jmeno, ktere ma byt zarovnane</param>
/// <param name="paddedName">Vystup - zarovnane jmeno</param>
/// <param name="maxLen">Delka vysledneho zarovnani</param>
void padName(char *nameToWrite, char *paddedName, int maxLen)
{
	int foundNull = 0;
	for (int i = 0; i < maxLen; ++i)
	{
		if (!foundNull && nameToWrite[i] != '\0')
			paddedName[i] = nameToWrite[i];
		else
		{
			foundNull = 1;
			paddedName[i] = ' ';
		}
	}
	paddedName[maxLen - 1] = '\0';
}

/// <summary>
/// Ziska nejdelsi mozny vypis
/// </summary>
int getMaxNameLen()
{
	int max = strlen(leaderboard.items[0].name);
	for (int i = 1; i < LEADER_BOARD_MAX_LINE_COUNT; ++i)
		max = max < strlen(leaderboard.items[i].name) ? strlen(leaderboard.items[i].name) : max;
	max = max < strlen(playerOne.name) ? strlen(playerOne.name) : max;
	max = max < strlen(playerTwo.name) ? strlen(playerTwo.name) : max;
	max = max < strlen(NAME_TABLE_HEADER) ? strlen(NAME_TABLE_HEADER) : max;

	return max;
}

/// <summary>
/// Vypise tabulku nejlepsich hracu.
/// </summary>
/// <param name="fd">File descriptor, kam se ma vytisknout tabulka nejlepsich hracu.</param>
/// <param name="separator">char, ktery oddeluje jmeno a pocet vitezstci</param>
/// <param name="pad">1 = zarovnat, 0 = nezarovnat</param>
/// <param name="printHeader">1 = vypise hlavicku, 0 = nevypise</param>
void printLeaderBoard(FILE* fd, char separator, int pad, int printHeader)
{
	if (!leaderboard.loaded)
		loadLeaderboard();

	int indexToStore = 0;
	int playerOneSaved = 0;
	int playerTwoSaved = 0;
	int padLen = getMaxNameLen() + 2; // +2 kvuli \0 a mezere mezi jmenem a poctem vitezu

	int winsToWrite;
	char* nameToWrite;

	if (printHeader)
	{
		char *header = (char*)malloc(sizeof(char) * padLen);
		if (header != NULL)
		{
			padName((char*)NAME_TABLE_HEADER, header, padLen);
			fprintf(fd, "\n%s%c%s\n", header, separator, WIN_TABLE_HEADER);
			free(header);
		}
	}

	for (int i = 0; i < LEADER_BOARD_MAX_LINE_COUNT; ++i)
	{
		if (!playerOneSaved && leaderboard.items[indexToStore].wins < playerOne.wins)
		{
			winsToWrite = playerOne.wins;
			nameToWrite = playerOne.name;
			playerOneSaved = 1;
		}
		else if (!playerTwoSaved && leaderboard.items[indexToStore].wins < playerTwo.wins)
		{
			winsToWrite = playerTwo.wins;
			nameToWrite = playerTwo.name;
			playerTwoSaved = 1;
		}
		else if (leaderboard.items[indexToStore].wins == 0 && leaderboard.items[indexToStore].name[0] == '\0')
			break; // Zbytek tabulky je prazdny
		else
		{
			winsToWrite = leaderboard.items[indexToStore].wins;
			nameToWrite = leaderboard.items[indexToStore].name;
			++indexToStore;
		}
		
		if (pad)
		{
			char paddedName[MAX_NAME_LEN];
			padName(nameToWrite, paddedName, padLen);
			nameToWrite = paddedName;
		}
		
		fprintf(fd, "%s%c%d\n", nameToWrite, separator, winsToWrite);
	}
}

/// <summary>
/// Ulozi tabulku nejlepsich hracu.
/// </summary>
/// <returns>Vraci 1 pokud se ulozeni povede, jinak 0</returns>
int saveLeaderBoard()
{
	FILE* leaderBoardFd = fopen(LEADEARBOARD_FILE_NAME, "w");
	if (leaderBoardFd == NULL)
		return 0;

	printLeaderBoard(leaderBoardFd, NAME_WINS_SEP, 0, 0);

	fclose(leaderBoardFd);
	return 1;
}
