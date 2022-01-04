#include <stdlib.h>
#include <stdio.h>
#include <string.h>
int main(int argc, char *argv[]){
	FILE *file;
	file = fopen(argv[1], "r");
	int N = strtol(argv[2], NULL, 10);
	if (file == NULL) {
		printf("Can't open the files");
		return -1;
	}
	
	int count = 1;
	int a;
	while ((a = fgetc(file)) != EOF) {
		if ((count == N) && (N !=0) && (a == '\n')) {
			count = 1;
			if(fgetc(stdin)) {
				printf("\n");
			}
			continue;
		}
		if (count <= N) {
			printf("%c", a);
		}
		if ( a == '\n'){
			count++;
		}
	}

	if (fclose(file) == EOF) {
		printf("Can't close the file");
		return -1;
	} 
	return 0;
}
