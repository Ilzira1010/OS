#include <stdlib.h>
#include <stdio.h>
#include <string.h>
int main(int argc, char *argv[]){
	FILE *file;
	file = fopen(argv[1], "r");
	int N =strtol(argv[2], NULL, 10);
	if (file == NULL) {
		printf("Can't open the file_first");
		return -1;
	}
	
	char key = ' ';
	char count =0;
	int i=0;
	while ((count = fgetc(file)) != EOF) {
		if ((i == N) && (N !=0))  {
		scanf("%c", &key);
		i = 0;
		}
		printf("%c", count);
		i++;
	}

	if (fclose(file) == EOF) {
		printf("Can't close the file_first");
		return -1;
	} 
	return 0;
}
