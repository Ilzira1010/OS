#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>

int main(int argc, char *argv[]){
	DIR *dir_first;
	DIR *dir_second;
	char *firstDirPath = ".";

	if ((dir_first = opendir(firstDirPath)) == NULL)  {
		printf("Can't open the %s", firstDirPath);
		return 1;
	}
	struct dirent *ent;
	while (ent = readdir(dir_first)) {
		printf("%s\n", ent->d_name);
	}
	
	if (errno == -1) {
		printf("Can not read the katalog %s", firstDirPath);
		return 1;
	}
	
	if (closedir(dir_first) !=0)  {
		printf("Can not close the katalog %s", firstDirPath);
		return 1;
	}
	
	if ((dir_second = opendir(argv[1])) == NULL) {
		printf("Can't open the %s", argv[1]);
		return 1;
	}
	
	
	struct dirent *ent2;
	while (ent2 = readdir(dir_second)) {
		printf("%s\n", ent2->d_name);
	}
	
	if (errno == -1) {
		printf("Can not read the katalog %s", argv[1]);
		return 1;
	}
	
	if (closedir(dir_second) != 0) {
		printf ("Can not close the katalog %s", argv[1]);
		return 1;
	}
	return 0;
}
	
