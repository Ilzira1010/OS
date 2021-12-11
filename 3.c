#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define PATH_MAX 4096
int main(int argc, char *argv[]){
	FILE *file_first;
	FILE *file_second;
	char first[PATH_MAX];
	char second[PATH_MAX];
	
	snprintf(first, sizeof first, "%s%s", "./", argv[1]);
	snprintf(second, sizeof second, "%s%s", "./", argv[2]);
	file_first = fopen(first, "r");
	file_second = fopen(second, "w"); 

	if (file_first == NULL) {
		printf("Can't open the file_first");
		return -1;
	}
	if (file_second == NULL) {
		printf("Can't open the file_first");
		return -1;
	}
	
	char key = ' ';
	while ((key = fgetc(file_first)) != EOF) {
		if ((fputc((int) key, file_second)) == EOF) {
		printf("Can't recorder to file_second");
		return -1;
		}
	}
	if ((fclose(file_first)) == EOF) {
		printf("Can't close the file_first");
		return -1;
	} 
	if ((fclose(file_second)) == EOF) {
		printf("Can't close the file_second");
		return -1;
	} 
}
