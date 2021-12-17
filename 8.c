#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>

FILE *resultFile;

void recursiveComprasionFiles(char directoryPath[], char filePath[], char resultFilePath[]) {
    DIR *dir;
    DIR *subDir;
    FILE *file;
    FILE *file2;  

    if ( (dir = opendir(directoryPath)) != NULL ) { // если dirPath1 - папка

        struct dirent *de1;
        while ((de1 = readdir(dir)) != NULL) { // если подпапка в директории 1 - папка

            if (strcmp(de1->d_name, ".") == 0 || strcmp(de1->d_name, "..") == 0) {
                continue;
            }

            char newDirPath1[512];
            snprintf(newDirPath1, sizeof newDirPath1, "%s%s%s", directoryPath, "/", de1->d_name);
            
            if ( (file = fopen(filePath, "r")) != NULL && (file2 = fopen(newDirPath1, "r")) != NULL) {
                struct stat s1;
                struct stat s2;
                stat(filePath, &s1);
                stat(newDirPath1, &s2);

                char f1 = ' ';
                char f2 = ' ';
                if (s1.st_size == s2.st_size) {
                    while (1) {
                        f1 = fgetc(file);
                        f2 = fgetc(file2);
                        if ((f1 == EOF && f2 != EOF) || (f2 == EOF && f1 != EOF) || (f1 != f2)) {
                            break;
                        }

                        if (f1 == EOF && f2 == EOF) {
                            char resultForF1[1024];
                            char resultForF2[1024];
                            snprintf(resultForF1, sizeof resultForF1, "%s size: %li creationData: %s mode: %d ino desc %ld", filePath, s1.st_size, asctime(localtime(&s1.st_ctime)), s1.st_mode, s1.st_ino);
                            snprintf(resultForF2, sizeof resultForF2, "\t EQUALS TO \t%s size: %li creationData: %s mode: %d ino desc %ld\n\n", newDirPath1, s2.st_size, asctime(localtime(&s2.st_ctime)), s2.st_mode, s2.st_ino);


                            fputs(resultForF1, resultFile);
                            fputs(resultForF2, resultFile);
                            printf("%s", resultForF1);
                            printf("%s", resultForF2);
                            
                            break;
                        }
                    }
                }
            }

            if ((fclose(file)) == EOF) {
                printf("Не удалось закрыть файл");
            }

            if ((fclose(file2)) == EOF) {
                printf("Не удалось закрыть файл");
            }


            if ( (subDir = opendir(newDirPath1)) != NULL ) {
                recursiveComprasionFiles(newDirPath1, filePath, resultFilePath);
            }
        }
        
        if (closedir(dir) != 0) {
            printf("Не удалось закрыть каталог %s", directoryPath);
        }
    }
    return;
}

void recursiveGetAllFilesInDirectory(char currentDirectory[], char anotherDirectory[], char resultFilePath[]) {
    DIR *dir;
    DIR *subDir;
    FILE *file;

    if ( (dir = opendir(currentDirectory)) != NULL ) {

        struct dirent *de;
        while ( (de = readdir(dir)) != NULL ) {

            if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0) {
                continue;
            }

            char newDirPath[512];
            snprintf(newDirPath, sizeof newDirPath, "%s%s%s", currentDirectory, "/", de->d_name);
            if ( (subDir = opendir(newDirPath)) != NULL ) {
                recursiveGetAllFilesInDirectory(newDirPath, anotherDirectory, resultFilePath);
            }

            if ( (file = fopen(newDirPath, "r")) != NULL ) {
                recursiveComprasionFiles(anotherDirectory, newDirPath, resultFilePath);
            }

            if ((fclose(file)) == EOF) {
                printf("Не удалось закрыть файл");
            }
        }
    }
}

int main(int argc, char *argv[]) {
    char *currentDirPath = argv[1];
    char *anotherDirPath = argv[2];
    char *resultFilePath = argv[3];

    if ( (resultFile = fopen(resultFilePath, "w")) == NULL ) {
        return 1;
    }

    recursiveGetAllFilesInDirectory(currentDirPath, anotherDirPath, resultFilePath);

    if (fclose(resultFile) == EOF) {
        printf("Не удалось закрыть итоговый файл");
    }
}
