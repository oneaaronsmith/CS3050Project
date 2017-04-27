#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NUMSAVEFILES 3
#define MAXNAMESIZE 20
//#define STARTCASH 200
//#define MAXFILELENGTH 100
//Necessary structs
typedef enum locator {
	EXIST,
	NOTEXIST
} Locator;

typedef struct player {
	char* name;
	int money;
} Player;

Locator findSaveFile();
void loadSavedGameHandler();
void DeleteSavedGameHandler();
void loadGameStateData(char*** names, int** states, int* fileCount);
char* choiceToFile(int num);
void freeMemory(char** names, int* states);
void startNewGame();
int chooseSaveFile();
void deleteFile(int choice/*, char* filename*/);
void saveFile(int choice, char* filename, char* playerName, int money);
void play(char* playerName, int progress, int money, int choice);
void saveFileData(int choice/*, char* filename*/, char* playerName, int progress, int money);

char* getPlayerName();
Player* loadSave(char* filename);
