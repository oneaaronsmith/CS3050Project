/*
  CS3050 Final Project
  This file contains functions for handling the saving, deleting, and loading of blackjack game states.
*/
#include "blackjack.h"

int chooseSaveFile()
{
	printf("\nchoose saves\n");
	//Initial load for data management.
	char** names;
	int* money;
	int numFiles;
	int choice;
	int confirm = 1;

	loadGameStateData(&names, &money, &numFiles);

	//Get a chosen file slot to save to. The file slot must be valid, and
	//old files can be overriden.
	while(1)
	{
		confirm = 1;
		printf("\nChoose a file you would like to save to: \n");
		printf(">");
		scanf("%d", &choice);
		while((choice < 1 || choice > NUMSAVEFILES))
		{
			printf("Invalid choice. Please choose a different option:\n ");
			printf(">");
			scanf("%d", &choice);
		}

		if(money[choice-1] != 0)
		{
			printf("There is a file here. Would you like to override it? (1 = yes, 0 = no)\n");
			printf(">");
			scanf("%d", &confirm);
			while(confirm != 1 && confirm != 0)
			{
				printf("Invalid choice. Please choose a different option:\n");
				printf(">");
				scanf("%d", &confirm);
			}
		}

		if(confirm == 1)
		{
			//Go ahead and delete the old hero file immediately if there is one.
			if(money[choice-1] == 2)
			{
				deleteFile(choice);
			}
			freeMemory(names, money);
			//I haven't found a bug yet with the state = 1, but there might be one somewhere.
			printf("end choose save file");
			return choice;
		}
	}
}

void saveFileData(int choice/*, char* filename*/, char* playerName, int progress, int money)
{
	char** n;
	int* s;

	n = (char**)malloc(sizeof(char*)*NUMSAVEFILES);
	int i;
	for(i = 0; i < NUMSAVEFILES; i++)
	{
		n[i] = (char*)malloc(sizeof(char)*MAXNAMESIZE);
	}

	s = (int*)malloc(sizeof(int)*NUMSAVEFILES);

	//Print out options
	FILE* fp = fopen("Players.txt", "r");

	int numFiles;
	fscanf(fp, "%d", &numFiles);

	for(i = 0; i < NUMSAVEFILES; i++)
	{
		fscanf(fp, "%s | %d", n[i], &s[i]);
	}

	fclose(fp);

	fp = fopen("Players.txt", "w");
	//FILE* fp2;

	switch(progress)
	{
		case 1:
			fprintf(fp, "%d\n", numFiles+1);
			for(i = 0; i < NUMSAVEFILES; i++)
			{
				if(i == choice - 1)
				{
					printf("The playerName is %s and their money is %d",playerName, money);
					fprintf(fp, "%s | %d\n", playerName, money);
				}
				else
				{
					fprintf(fp, "%s | %d\n", n[i], s[i]);
				}
			}
			break;
		}
		/*case 2:
			fprintf(fp, "%d\n", numFiles);
			for(i = 0; i < MAXNUMSAVEFILES; i++)
			{
				if(strcmp(n[i], playerName) == 0)
				{
					s[i] = 2;
				}
				fprintf(fp, "%s | %d\n", n[i], s[i]);
			}
			fp2 = fopen(filename, "w");
			for(i = 0; i < NUMCHOSENHEROES; i++)
			{
				fprintf(fp2, "%s | %d | %d | %d | %d\n", heroes[i].name,
					(heroes[i].health), (heroes[i].baseDefense), (heroes[i].baseAttack),
					(heroes[i].baseSpeed));
			}
			fclose(fp2);
			break;
		case 3:
			fprintf(fp, "%d\n", numFiles);
			for(i = 0; i < MAXNUMSAVEFILES; i++)
			{
				if(strcmp(n[i], playerName) == 0)
				{
					s[i] = 3;
				}
				fprintf(fp, "%s | %d\n", n[i], s[i]);
			}
			fp2 = fopen(filename, "a");
			for(i = 0; i < NUMCHOSENHEROES; i++)
			{
				if(heroes[i].heroSword != NULL)
				{
					fprintf(fp2, "%s | %d | %d | %s \r\n",
						heroes[i].heroSword->name, heroes[i].heroSword->cost,
						heroes[i].heroSword->attack, heroes[i].heroSword->description);
				}
				else
				{
					fprintf(fp2, "NULL | 0 | 0 | NULL \r\n");
				}
			}

			for(i = 0; i < NUMCHOSENHEROES; i++)
			{
				if(heroes[i].heroShield != NULL)
				{
					fprintf(fp2, "%s | %d | %d | %s \r\n",
						heroes[i].heroShield->name, heroes[i].heroShield->cost,
						heroes[i].heroShield->defense, heroes[i].heroShield->description);
				}
				else
				{
					fprintf(fp2, "NULL | 0 | 0 | NULL \r\n");
				}

			}*/
			//fclose(fp2);
			fclose(fp);
}

Locator findSaveFile()
{
	//The findSaveFile() looks at the Players.txt file to determine whether there are any games available to load.

	//Open the file for reading.
	FILE* fp = fopen("Players.txt", "r");

	//If the file does not open, send an error message.
	if(!fp)
	{
		printf("Error: Unable to read Players.txt");
		exit(1);
	}
	int numFiles;

	//read the number of save file slots that are taken up. Print an error if it happens.
	if(fscanf(fp, "%d", &numFiles) < 1)
	{
		printf("Players.txt: failed to read number of save files. Check input file.\n");
		exit(1);
	}
	printf("The number of files is %d",numFiles);

	//Set whether save files exist or not.
	Locator l;
	if (numFiles == 0)
	{
		l = NOTEXIST;
	}
	else
	{
		l = EXIST;
	}

	//Close file, print finish message, return value.
	fclose(fp);
	printf("Check complete.\n\n");
	return l;
}

void loadGameStateData(char*** names, int** money, int* fileCount)
{
	printf("\n\n");
	char** n = malloc(sizeof(char*)*NUMSAVEFILES);
	int i;
	for(i = 0; i < NUMSAVEFILES; i++)
	{
		n[i] = malloc(sizeof(char)*MAXNAMESIZE);
	}
	int* s = malloc(sizeof(int)*NUMSAVEFILES);

	//again, should not fail.
	FILE* fp = fopen("Players.txt", "r");
	if(!fp)
	{
		printf("Error: Could not read Players.txt");
		freeMemory(n, s);
		exit(1);
	}

	int numFiles;
	if(fscanf(fp, "%d", &numFiles) < 1)
	{
		printf("Error: Could not read the number of save files. Please check the input file.\n");
		freeMemory(n,s);
		exit(1);
	}

	printf("\nThe number of save file is %d\n", NUMSAVEFILES);
	for(i = 0; i < NUMSAVEFILES; i++)
	{
		if(fscanf(fp, "%s | %d", n[i], &s[i]) < 2)
		{
			printf("Error: Could not read save file data #%d. Check input file.\n", i+1);
			freeMemory(n,s);
			exit(1);
		}
		if(s[i] <= 0)
		{
			printf("%d. [No file present]\n",i+1);
		}
		else {
			printf("%d. %s : $%d\n",i+1,n[i],s[i]);
		}
		/*switch(s[i])
		{
			case 0:
				printf("%d: [No File Present]\n", i+1);
				break;
			case 1:
				printf("%d: %s - ", i+1, n[i]);
				printf("Heroes have not been selected.\n");
				break;
			case 2:
				printf("%d: %s - ", i+1, n[i]);
				printf("Heroes have been selected.\n");
				break;
			case 3:
				printf("%d: %s - ", i+1, n[i]);
				printf("Items have been selected.\n");
				break;
		}*/
	}

	fclose(fp);

	//complete the links after everything is loaded.
	*names = n;
	*money = s;
	*fileCount = numFiles;
}

void loadSavedGameHandler()
{
	//initial load of all data that might be needed.
	char** names;
	int* money;
	int numFiles;
	int choice;

	loadGameStateData(&names, &money, &numFiles);

	while(1)
	{
		printf("\nChoose a file you would like to load: \n");
		printf("(-1 to return to main menu)\n");
		printf(">");
		scanf("%d", &choice);
		while((choice < 1 || choice > NUMSAVEFILES) && choice != -1)
		{
			printf("Invalid choice. Please choose a different option:\n ");
			printf(">");
			scanf("%d", &choice);
		}

		if(choice == -1)
		{
			freeMemory(names, money);
			return;
		}

		if(money[choice-1] <= 0)
		{
			printf("There is no saved file present in this slot.\n");
		}
		else
		{
			//From here, a valid saved file has been selected, so we need to interpret where
			//the save file is in terms of game progression. This way we know what data to load
			//the state value will never be anything other than 0, 1, or 2.
			play(names[choice-1], 1, money[choice-1], choice);
			/*if(money[choice-1] == 1)
			{
				//Heroes have not been selected yet.
				printf("Cool story bro");
			}
			else
			{
				//Heroes have already been selected.
				Hero* heroes = loadSavedData(choiceToFile(choice-1), states[choice-1] == 3 ? 1 : 0);
				if(!heroes)
				{
					printf("Failed to load heroes. Exiting...\n");
					freeMemory(names, states);
					exit(1);
				}
				playGame(heroes, names[choice-1], NULL, states[choice-1], choice);*/
			}
			freeMemory(names, money);
			exit(1);
			return;
		}
	}
//}

void DeleteSavedGameHandler()
{

	//Everything in this file is almost identical to the function above, just different ending actions.
	//Could move a lot of this to its own function, but eh
	char** names;
	int* money;
	int numFiles;
	//int flag = 0;
	int choice = -1;

	while (1)
	{
		//Small optimization. If we didn't delete a file, we don't have to reload the data
		//since nothing changed.
		//if(flag == 0)
		//{
			//to cover the first time the loop goes through.
			//Can't free if nothing's been loaded yet.
			if(choice != -1)
			{
				freeMemory(names, money);
			}
			loadGameStateData(&names, &money, &numFiles);
		//}

		printf("\nChoose a file you would like to delete: \n");
		printf("(-1 to return to main menu)\n");
		printf(">");
		scanf("%d", &choice);
		while((choice < 1 || choice > NUMSAVEFILES) && choice != -1)
		{
			printf("Invalid choice. Please choose a different option:\n ");
			printf(">");
			scanf("%d", &choice);
		}

		if(choice == -1)
		{
			//This is the only way to get out of this function.
			freeMemory(names, money);
			return;
		}

		if(money[choice-1] == 0)
		{
			//shouldn't get here, but just in case.
			//can't get here because the numFiles controls what's
			//a valid value for chioce.
			printf("There is no saved file present in this slot.\n");
			//flag = 1;
		}
		else
		{
			deleteFile(choice);
			//flag = 0;
		}
	}
}

void deleteFile(int choice)
{
	char** n;
	int* s;

	n = malloc(sizeof(char*)*NUMSAVEFILES);
	int i;
	for(i = 0; i < NUMSAVEFILES; i++)
	{
		n[i] = malloc(sizeof(char)*MAXNAMESIZE);
	}

	s = malloc(sizeof(int)*NUMSAVEFILES);

	FILE* fp = fopen("Players.txt", "r");

	int numFiles;
	fscanf(fp, "%d", &numFiles);

	for(i = 0; i < NUMSAVEFILES; i++)
	{
		fscanf(fp, "%s | %d", n[i], &s[i]);
	}

	fclose(fp);

	fp = fopen("Players.txt", "w");

	fprintf(fp, "%d\n", numFiles-1);
	for(i = 0; i < NUMSAVEFILES; i++)
	{
		if(i != choice - 1)
		{
			fprintf(fp, "%s | %d\n", n[i], s[i]);
		}
		else
		{
			fprintf(fp, "000 | 0\n");
		}
	}
	fclose(fp);

	fp = fopen("Players.txt","r");
	int fileCounter = 0;
	fscanf(fp, "%d", &numFiles);
	for(i = 0; i < NUMSAVEFILES; i++)
	{
		fscanf(fp, "%s | %d", n[i], &s[i]);
		if(s[i] == 0) {
			fileCounter++;
		}
	}
	fclose(fp);

	if(fileCounter == NUMSAVEFILES) {
		fp = fopen("Players.txt","w");
		fprintf(fp,"%d\n",0);
		for(i = 0; i < NUMSAVEFILES; i++)
		{
				fprintf(fp, "000 | 0\n");
		}
		fclose(fp);
	}

}

void freeMemory(char** names, int* money)
{
	int i;
	//seg faults? Check input file/gdb.
	for(i = 0; i < NUMSAVEFILES; i++)
	{
		free(names[i]);
	}
	free(names);
	free(money);
}
