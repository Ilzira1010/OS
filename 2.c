#include <stdio.h>
#include <string.h>
#define PATH_MAX 4096
int main(int argc, char * argv[]){
	FILE *file;
	char path[PATH_MAX];
	snprintf(path, sizeof path, "%s%s", "./", argv[1]);
	file = fopen(path, "w");
	if (file == NULL) {
		printf("Can't open the file");
		return -1;
	}
	
	char key = ' ';
	int a;
	while ((a=getc(stdin)) != EOF) {
		if (a == '!') {
			break;
		}
		if ((fputc((int) key, file)) == EOF) {
			printf("Can't record to file");
		return -1;
		}
		fputc((int)a, file);
	}

	if ((fclose(file)) != 0) {
		printf("Can't close the file");
		return -1;
	} 
	return 0;
}
