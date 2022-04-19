//Sagiv Antebi
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define SIZE 100
#define EXIT "exit"
#define HISTORY "history"
#define CD "cd"
#define SPACE " "
#define COLON ":"
#define SLASH "/"
#define PATH_STR "PATH"

void addToHistory(char historyComAndPID[SIZE][SIZE], char command[SIZE], char PID_child[SIZE], int i);

void printHistory(char historyComAndPID[SIZE][SIZE], int sizeOfArray);

int main(int argc, const char *argv[]) {
    char historyCommands[SIZE][SIZE];
    int historyPID[SIZE];
    char historyComAndPID[SIZE][SIZE];
    char buffer[SIZE];
    char bufferCopy1[SIZE];
    char bufferCopy2[SIZE];
    char pidChar[SIZE];
    int counter = 0;
    int stat;
    pid_t childID;
    pid_t pid = getpid();
    int indexToArgv;
    char *originalPath = getenv(PATH_STR);
    //the array of commands
    char *argvChild[SIZE];
    char *newPath = malloc(strlen(originalPath) + 1);
    strcpy(newPath, originalPath);
    int i;
    for (i = 0; i < argc; ++i) {
        newPath = realloc(newPath, strlen(newPath) + strlen(argv[i]) + 2);
        sprintf(newPath, "%s:%s", originalPath, argv[i]);
    }
    setenv("PATH", newPath, 1);
    //free newPath
    free(newPath);

    //main loop - until the user insert "exit"
    do {
        //free the array  - argv
        for (int j = 0; j < indexToArgv; j++) {
            free(argvChild[j]);
            argvChild[j] = 0;
        }
        indexToArgv = 0;
        printf("$ ");
        fflush(stdout);
        scanf("%[^\n]%*c", buffer);
        strcpy(historyCommands[counter], buffer);
        //For the free section
        strcpy(bufferCopy1, buffer);
        //For adding to the history
        strcpy(bufferCopy2, buffer);

        char *endStrToken;
        char *token = strtok_r(buffer, " ", &endStrToken);
        char firstToken[SIZE];
        strcpy(firstToken, token);
        //go through the SPACE of the commands inserted in the shell
        while (token != NULL) {
            argvChild[indexToArgv] = (char *) malloc(sizeof(token));
            strcpy(argvChild[indexToArgv], token);
            token = strtok_r(NULL, SPACE, &endStrToken);
            indexToArgv++;
        }

        //If the command is exit we should exit the program
        if (strcmp(firstToken, EXIT) == 0) {
            for (int j = 0; j < indexToArgv; j++) {
                free(argvChild[j]);
                argvChild[j] = 0;
            }
            break;
        }
            //If the command is exit we should exit the program
        else if (strcmp(firstToken, HISTORY) == 0) {
            sprintf(pidChar, "%d", pid);
            addToHistory(historyComAndPID, bufferCopy2, pidChar, counter);
            counter++;
            printHistory(historyComAndPID, counter);
        }
            //If the command is cd
        else if (strcmp(firstToken, CD) == 0) {
            token = strtok_r(NULL, SPACE, &endStrToken);
            //if there is just cd ignore it
            if (argvChild[1] == NULL) {
                perror("cd failed\n");
                addToHistory(historyComAndPID, bufferCopy2, pidChar, counter);
                counter++;
            }
            //if there is just ~ ignore it
            else if (strcmp(argvChild[1], "~") == 0) {
                perror("cd failed\n");
                addToHistory(historyComAndPID, bufferCopy2, pidChar, counter);
                counter++;
            }
            else {
                chdir(argvChild[1]);
            }
        }
        //The fork got an error
        else if ((childID = fork()) == -1) {
            perror("fork failed\n");
            exit(-1);
        }
        //The child
        else if (childID == 0) {
            pid = getpid();
            historyPID[counter] = pid;
            indexToArgv = 0;

            //ThatS is for the using of token in token - found it on the web and this is LEGIT!
            char *end_str;
            char *tokenPath = strtok_r(getenv(PATH_STR), COLON, &end_str);

            char pathCreate[SIZE * SIZE];
            char pathCreateWithCommand[SIZE * SIZE];
            //go through the : of the paths
            while (tokenPath != NULL) {
                char *end_token;
                char *tokenPathSlash = strtok_r(tokenPath, SLASH, &end_token);
                strcpy(pathCreate, SLASH);
                strcat(pathCreate, tokenPathSlash);
                //go through the / of the path
                while (tokenPathSlash != NULL) {
                    strcpy(pathCreateWithCommand, pathCreate);
                    strcat(pathCreateWithCommand, SLASH);
                    strcat(pathCreateWithCommand, firstToken);

                    //check if the command exists
                    if (access(pathCreateWithCommand, F_OK) != -1) {
                        execvp(pathCreateWithCommand, argvChild);
                        exit(0);
                    } else {

                    }
                    tokenPathSlash = strtok_r(NULL, SLASH, &end_token);
                    if (tokenPathSlash != NULL) {
                        strcat(pathCreate, SLASH);
                        strcat(pathCreate, tokenPathSlash);
                    }
                }
                tokenPath = strtok_r(NULL, COLON, &end_str);
            }
            exit(0);
        }
            //The father
        else {
            wait(&stat);
            sprintf(pidChar, "%d", childID);
            addToHistory(historyComAndPID, bufferCopy2, pidChar, counter);
            counter++;
        }
    } while (1);
    //free the array  - argv - after the exit
    for (int j = 0; j < indexToArgv; j++) {
        free(argvChild[j]);
        argvChild[j] = 0;
    }
    return 0;
}

void addToHistory(char historyComAndPID[SIZE][SIZE], char command[SIZE], char PID_child[SIZE], int i) {
    strcpy(historyComAndPID[i], PID_child);
    strcat(historyComAndPID[i], SPACE);
    strcat(historyComAndPID[i], command);
}


void printHistory(char historyComAndPID[SIZE][SIZE], int sizeOfArray) {
    for (int i = 0; i < sizeOfArray; ++i) {
        printf("%s\n", (char *) historyComAndPID[i]);
    }
}



