// Tic-tac-toe.cpp : Defines the entry point for the application.
//

#include "Tic-tac-toe.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "TicTacToeBoard.h"

#define MAX_PLAYERS 2

/// <summary>
/// Nacte cely radek, a uchova maxLen znaku.
/// </summary>
/// <param name="output">Ukazatel na char o velikosti maxLen - pamet musi byt alokovana.</param>
/// <param name="maxLen">Maximalni delka nacteneho radku -1 (kvuli \0)</param>
/// <param name="forbiden">Znak, ktery bude ignorovan.</param>
void readLine(char* output, int maxLen, char forbiden)
{
	char inputChar;
	int cnt = 0;
	int reachedMaxLen = 0;
	while ((inputChar = getchar()) != '\n')
	{
		if (!reachedMaxLen)
		{
			if (cnt >= maxLen - 1)
			{
				printf("Maximum length is %d. Other characters are ignored.\n", maxLen - 1);
				reachedMaxLen = 1;
				continue;
			}

			if (inputChar != forbiden)
			{
				output[cnt] = inputChar;
				++cnt;
			}
		}
	}
	output[cnt] = '\0';
}

/// <summary>
/// Nacte jmeno pro hrace o velikosti MAX_NAME_LEN
/// </summary>
/// <param name="playerName">Ukazatel na char* o minimalni velikost MAX_NAME_LEN.</param>
void getPlayerName(char* playerName)
{
	static int playerCnt = 0;
	playerCnt = playerCnt + 1 % (MAX_PLAYERS + 1);

	printf("Name of player %d: ", playerCnt);
	readLine(playerName, MAX_NAME_LEN, NAME_WINS_SEP);
}

/// <summary>
/// Nacte intovou hodnotu ze vstupu na jednom radku. Vstup musi obsahovat v prefixu nejake cislice, znaky za cislicemi jsou ignorovany. Nacita cely radek.
/// </summary>
/// <param name="text">Text s vyzvnou k zadani.</param>
/// <param name="min">Minimalni nactena hodnota.</param>
/// <returns>Vraci zpracovany vstup.</returns>
int getSize(const char* text, int min)
{
	printf("%s", text);
	int result = 0;
	const int maxNumLen = 7;
	char input[maxNumLen];

	while (true)
	{
		readLine(input, maxNumLen, '\0');
		result = strtol(input, NULL, 10);

		if (result < min)
			printf("Bad number entered. Minimum is: %d\n%s", min, text);
		else
			break;
	}

	return result;
}

/// <summary>
/// Ziska souradnice x a y.
/// </summary>
/// <param name="x">Ukazatel na int, do ktereho se ulozi hodnota x.</param>
/// <param name="y">Ukazatel na int, do ktereho se ulozi hodnota y.</param>
void getCoords(int *x, int *y)
{
	*x = getSize("Enter x coord: ", 1);
	*y = getSize("Enter y coord: ", 1);
}

/// <summary>
/// Udela jedno kolo hry - ukaze hraci desku, nacte x a y, zkontroluje vyhru. Pokud neni mozne policko oznacit, je vyzadaovano nove nacteni souradnic.
/// </summary>
/// <returns>1 pokud doslo k ukonceni hry (nekdo vyhral/nikdo nevyhral ale jsou plna policka), jinak 0.</returns>
int DoOneTurn()
{
	printBoard();
	printf("\n");

	while (true)
	{	
		char fieldMark;
		char *playerName;
		if (playerOnTurn == PLAYER_1_TURN)
		{
			playerName = playerOne.name;
			fieldMark = playerOne.fieldMark;
		}
		else
		{
			playerName = playerTwo.name;
			fieldMark = playerTwo.fieldMark;
		}

		printf("%s's (%c) turn\n", playerName, fieldMark);

		int x, y;
		getCoords(&x, &y);

		int turnResult = makeTurn(x, y);
		if (turnResult == -1)
			printf("Coords are out of range!\n\n");
		else if (turnResult == -2)
			printf("Field is already taken\n\n");
		else if (turnResult == -3)
		{
			printBoard();
			printf("\n\nNo player wins it is a TIE!\n");
			return 1;
		}
		else if (turnResult == 0)
			break;
		else if (turnResult == 1)
		{
			printBoard();
			printf("\n\nPlayer %s wins!\nYou needed %d turns for win!", playerName, turnCnt);
			return 1;
		}
	}

	printf("\n");
	return 0;
}

/// <summary>
/// Zkontroluje, zda chce hrat uzivatel znovu. Pokud ano, znovu inicializuje desku.
/// </summary>
/// <returns>Vraci 1 pokud chce uzivatel hrat znovu, jinak 0.</returns>
int wantToPlayAgain()
{
	const char yes = 'y';
	const char no = 'n';
	const char showLd = 'l';
	char c = '0';
	while (1)
	{
		printf("\nDo you want to continue? (type '%c' for YES, '%c' for NO, '%c' to show leaderboard ): ", yes, no, showLd);
		c = getchar();
		while(getchar() != '\n'); // pro enter

		if (c == 'y')
			return 1;
		else if (c == 'n')
			return 0;
		else if (c == 'l')
			printLeaderBoard(stdout, ' ', 1, 1);
	}
}

/// <summary>
/// Nastavi hru - inicializuje hraci pole, nastavi se znovu poradi tahu.
/// </summary>
/// <param name="firstTime">1, pokud je hrana 1. od spusteni programu, jinak false.</param>
void setGame(int firstTime)
{
	int widht = getSize("Board width: ", 3);
	int height = getSize("Board height: ", 3);

	initBoard(widht, height, firstTime);
}

int main()
{
	getPlayerName(playerOne.name);
	getPlayerName(playerTwo.name);

	setGame(1);
	
	int winner = 0;
	while (1)
	{
		if (DoOneTurn())
		{
			if (wantToPlayAgain())
				setGame(0);
			else
				break;
		}
	}

	saveLeaderBoard();
	return 0;
}
