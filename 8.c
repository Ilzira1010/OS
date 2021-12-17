#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <locale.h>
#include <time.h>
 
#define BUF_SIZE 1024
 
char dir1[BUF_SIZE];
char dir2[BUF_SIZE];
char program_name[BUF_SIZE];
char file[BUF_SIZE];
char **listdir1;
char **listdir2;
FILE *outfile;
 
 
void full_path(char *path, char *dir_name, char *full_path) {
  strcpy(full_path, path);
  strcat(full_path, "/");
  strcat(full_path, dir_name);
}
 
char *process_time(char *timestr){
  char* buf = (char *)malloc(sizeof(char) * 5);
  char* result = (char *)malloc(sizeof(char) * 12);
  strncpy(result,&timestr[4],7);
  strncpy(buf,&timestr[20],4);
  strcat(result,buf);
  result[11] = '\0';
  free(buf);
  return result;
  
 
}
 
char *display_permission ( int st_mode )
{
  char *str = (char *)malloc(sizeof(char) * 11);
  if (S_ISDIR(st_mode)) str[0] = 'd'; else str[0] = '-';
  if (st_mode & S_IRUSR) str[1] = 'r'; else str[1] = '-';
  if (st_mode & S_IWUSR) str[2] = 'w'; else str[2] = '-';
  if (st_mode & S_IXUSR) str[3] = 'x'; else str[3] = '-';
  if (st_mode & S_IRGRP) str[4] = 'r'; else str[4] = '-';
  if (st_mode & S_IWGRP) str[5] = 'w'; else str[5] = '-';
  if (st_mode & S_IXGRP) str[6] = 'x'; else str[6] = '-';
  if (st_mode & S_IROTH) str[7] = 'r'; else str[7] = '-';
  if (st_mode & S_IWOTH) str[8] = 'w'; else str[8] = '-';
  if (st_mode & S_IXOTH) str[9] = 'x'; else str[9] = '-';
  str[10] = '\0';
  return str;
 
}
 
int process_dir(char *dir_name, char** listdir[],int *count) {
  DIR *dir = opendir(dir_name);
  
  if (dir == NULL) {
    fprintf(stderr, "%s : %s : %s\n", program_name, strerror(errno), dir_name);
     // printf("%s: Permission to directory %s denied\n", program_name, dir_name);
      return 0;
    }
 
  struct dirent *dir_read;
  while ((dir_read = readdir(dir)) != NULL)  {
    if (dir_read->d_type == DT_DIR) {
        if (dir_read->d_name[0] != '.') {
          char *dir_path = (char*) calloc(BUF_SIZE,sizeof(char));
          strcpy(dir_path,dir_name);
          strcat(dir_path,"/");
          strcat(dir_path,dir_read->d_name);
          full_path(dir_name, dir_read->d_name, dir_path);
          process_dir(dir_path,listdir,count);
          free(dir_path);
        }
    } else  if (dir_read->d_type == DT_LNK) {
      continue;
    } else if (dir_read->d_type == DT_REG) {
        char *file_path = (char*) calloc(BUF_SIZE,sizeof(char));
        strcpy(file_path,dir_name);
        strcat(file_path,"/");
        strcat(file_path,dir_read->d_name);
      FILE *f;
           if ((f =fopen(file_path,"r")) == NULL){
          fprintf(stderr, "%s : %s : %s\n", program_name, strerror(errno), file_path);
          //printf("%s : Permission to %s denied \n", program_name, file_path);
          //fprintf(outfile, "%s : %s : %s\n",program_name, stderror(errno),file_path);
        }
        else{
        (*count)++;
        (*listdir) = (char**)realloc((*listdir),(*count)*sizeof(char*));
        (*listdir)[(*count) - 1] = (char *)calloc(strlen(file_path)+1,sizeof(char));
        strcpy((*listdir)[(*count) - 1], file_path);
        fclose(f);
       }
         free(file_path);
        }
    }
 
  if (dir)
    closedir(dir);
  return (*count);
}
void process_files(char *file1, char *file2) {
  if (0 != strcmp(file1,file2)) {
  
  FILE *F1;
  FILE *F2;
  F1 = fopen(file1,"r");
  F2 = fopen(file2,"r");
 if (F1 == NULL)
  {
    //printf("%s : Permission to %s is denied \n", program_name, file1);
   // fprintf(outfile, "%s : Permission to %s is denied \n",program_name, file1);
    return;
  }
 
  if (F2 == NULL)
  {
   // printf("%s : Permission to %s is denied \n",program_name, file2);
   // fprintf(outfile, "%s : Permission to %s is denied \n",program_name, file2);
    return;
  }
  char *buffer1 = (char*)calloc(127,sizeof(char));
  char *buffer2 = (char*)calloc(127,sizeof(char));
  struct stat info;
  fseek(F1,0,SEEK_END);
  fseek(F2,0,SEEK_END);
  if  ( (ftell(F1) != 0) && (ftell(F2) != 0))
  {
 
  fseek(F1,0,SEEK_SET);
  fseek(F2,0,SEEK_SET);
  while ((NULL != fgets(buffer1,127,F1)) && (NULL !=fgets(buffer2,127,F2)))
  {
      if (0 != strcmp(buffer1,buffer2))
      {
        free(buffer1);
        free(buffer2);
        fclose(F1);
        fclose(F2);
        return;
      }
  }
 
 stat(file1,&info);
 printf("%s %ld %s %s %ld \n ",file1, info.st_size, process_time(ctime(&info.st_ctim.tv_sec)), display_permission(info.st_mode), info.st_ino);
fprintf(outfile, "%s %ld %s %s %ld \n ",file1, info.st_size, process_time(ctime(&info.st_ctim.tv_sec)), display_permission(info.st_mode), info.st_ino);
 stat(file2,&info);
 printf("%s %ld %s %s %ld \n",file2, info.st_size,process_time(ctime(&info.st_ctim.tv_sec)),  display_permission(info.st_mode), info.st_ino);
fprintf(outfile, "%s %ld %s %s %ld \n\n",file2, info.st_size,process_time(ctime(&info.st_ctim.tv_sec)),  display_permission(info.st_mode), info.st_ino);
 printf("\n");
  }
  free(buffer1);
  free(buffer2);
  fclose(F1);
  fclose(F2);
    }
  return;
}
int args(int argc, char** argv){
 
   if (argc < 4) {
    printf("%s: Need more args\n", argv[0]);
    exit(-1);
  }
  strncpy(program_name, argv[0]+2,strlen(argv[0])-2);
  outfile = fopen(argv[3], "w");
 
  if(realpath(argv[1], dir1) == NULL){
    fprintf(stderr, "%s : %s : %s\n", program_name, strerror(errno), argv[1]);
   // printf("%s: Permission to %s denied\n", program_name,argv[1]);
    exit(-1);
  }
   if(realpath(argv[2], dir2) == NULL){
    fprintf(stderr, "%s : %s : %s\n", program_name, strerror(errno), argv[2]);
     //printf("%s: Permission to %s denied\n", program_name,argv[2]);
     exit(-1);
   }
 
 
}
 
 
int main(int argc, char* argv[])
{
  args(argc,argv);
 //processing directories & making list of filenames
  int count1 = 0;
 count1 = process_dir(dir1,&listdir1,&count1);
 int count2 = 0;
 count2 = process_dir(dir2,&listdir2,&count2);
 //file comparing
 
 for (int i = 0; i < count1; i++) {
  for (int j = 0; j < count2; j++) {
     process_files(listdir1[i], listdir2[j]);
    }
  }
  free(listdir1);
  free(listdir2);
  fclose(outfile);
return 0;
}
