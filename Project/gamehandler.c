/*CS3050 Final Project
  This file attempts to start and save games.
*/

#include "blackjack.h"

int main(void)
{
	while (1)
	{
		Locator finder = findSaveFile();
		int choice;
		printf("Welcome to blackjack!\n");
		printf("Select your option.\n");
		printf("1: Start a new game\n");
		if (finder == EXIST)
		{
			printf("2: Load a saved game\n");
			printf("3: Delete a saved file\n");
			printf("4: Quit\n");
			printf(">");
			scanf("%d", &choice);
			while(choice < 1 || choice > 5)
			{
				printf("Invalid option. Try again: \n");
				printf(">");
				scanf("%d", &choice);
			}
			switch (choice)
			{
			case 1:
				startNewGame();
				break;
			case 2:
				loadSavedGameHandler();
				break;
			case 3:
				DeleteSavedGameHandler();
				break;
			case 4:
				return 0;
			}
		}
		else
		{
			printf("2: Quit\n");
			printf(">");
			scanf("%d", &choice);
			while(choice < 1 || choice > 2)
			{
				printf("Invalid option. Try again: \n");
				printf(">");
				scanf("%d", &choice);
			}
			switch (choice)
			{
			case 1:
				startNewGame();
				break;
			case 2:
				return 0;
			}
		}
	}
}

void startNewGame()
{
	play(NULL, 0, 100, -1);
}

char* getPlayerName()
{
	char* name = malloc(sizeof(char)*MAXNAMESIZE);
	printf("Please input your name: ");
	scanf("%s", name);
	printf("Hello %s. Test your luck with blackjack!\n", name);
	return name;
}

void play(char* playerName, int progress, int money, int choice)
{
	if(progress > 0)
	{
		printf("\nWelcome back %s!.\n\n", playerName);
	}
	//holds what the last thing done was.
	int initialprogress = progress;

	//Hero* chosenHeroes;
	//Shop* s;
	//another check for how the data is laid out. This is so we can immediately print out
	//heroes if we have already selected heroes.
	/*if(progress > 1)
	{
		//chosenHeroes = heroes;
	}*/

	switch(progress)
	{
		//initial player identification
		case 0:
			playerName = getPlayerName();
			choice = chooseSaveFile();
			saveFileData(choice/*, NULL*/, playerName, 1, money);
		//hero selection
		case 1:
			printf("\nThis is where we play\n");
			break;
		default:
			//What?
			printf("Something fatal happened with the progress value. Check gamestate.txt.\n");
			break;
	}

	//I don't actually remember why I did this, but it works.
	if(initialprogress == 0)
	{
		free(playerName);
	}
	return;
}
