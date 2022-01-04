#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
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
	int rval;
	int wval;
	rval = access(first, R_OK);
	if (rval == 0) {
		printf("%s is readable ", first);
	} else { 
		printf("%s is not readable (access denied)", first);
	}
	
	wval =access(second, W_OK);
	
	if (wval ==0) {
		printf("%s is writable", second);
	} else if (errno ==EACCES) {
		printf("%s is not writable (access denied)", second);
	} else if ( errno == EROFS) {
		printf("%s is not writable (read-only filesystem)", second);
	} 	

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

