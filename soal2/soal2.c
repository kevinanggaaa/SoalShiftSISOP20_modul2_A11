#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/syslog.h>
#include <sys/types.h>
#include <sys/prctl.h>
#include <limits.h>

/* void handlerFunc(int signum) 
{ 
  wait(NULL); 
} */

int presistChild = 0;

void deleteFolder(char timeString[])
{
  // Download pics
  pid_t anak_id;

  anak_id = fork();
  // If there's error for child creation
  if (anak_id < 0) {
    // exit_failure constant is equal to 8
    exit(8);
  }

  if (anak_id == 0)
  {
    char *rmArgv[] = {"rm", "-r", timeString, NULL};
    execv("/bin/rm", rmArgv);
  } else {
    wait(NULL);
  }
}

void zipDirectory(char timeString[])
{
  // Download pics
  pid_t anak_id;

  anak_id = fork();
  // If there's error for child creation
  if (anak_id < 0) {
    // exit_failure constant is equal to 8
    exit(8);
  }

  if (anak_id == 0)
  {
    // If folder is not included, append
    // NOTE: if folder is included use this
    // char *zipArgv[] = {"zip", "-r", timeString, timeString, NULL};
    char dest[100];
    memset(dest, 0, sizeof(dest));
    char *relativeAdd = "../";
    strcpy(dest, relativeAdd);
    strcat(dest, timeString);
    printf("dest is: %s\n", dest);
    char *zipArgv[] = {"zip", "-r", dest, ".", NULL};
    execv("/usr/bin/zip", zipArgv);
  } else {
    wait(NULL);
  }
}

void downloadPicsumNow()
{
  // Download pics
  pid_t anak_id;

  anak_id = fork();
  // If there's error for child creation
  if (anak_id < 0) {
    // exit_failure constant is equal to 8
    exit(8);
  }
  
  if (anak_id == 0)
  {
    // Download with (t%1000)+100 width (square) where t = time(NULL);
    char time_string_file[80];
    int time_int_file = time(NULL);
    // get time string for folder name
    time_t t_time_t_file = time_int_file;
    struct tm time_struct_file = *localtime(&t_time_t_file);
    snprintf(
      time_string_file, 
      sizeof(time_string_file), 
      "%d-%02d-%02d_%02d:%02d:%02d", 
      time_struct_file.tm_year + 1900, 
      time_struct_file.tm_mon + 1, 
      time_struct_file.tm_mday, 
      time_struct_file.tm_hour, 
      time_struct_file.tm_min, 
      time_struct_file.tm_sec
    );
    time_int_file = time_int_file%1000 + 100;
    // download from https://picsum.photos/width to time_string_file file
    // Gambar tersebut diberi nama dengan format timestamp [YYYYmm-dd_HH:ii:ss]
    char download_src[80];
    snprintf(download_src ,sizeof(download_src), "https://picsum.photos/%d", time_int_file);
    char *wgetArgv[] = {"wget", "-q", "-O",  time_string_file, download_src, NULL};
    // char *wgetArgv[] = {"wget", download_src, NULL};
    execv("/usr/bin/wget", wgetArgv);
  } 
}

int main(int argc, char *argv[]){
  if (argc == 2) {
    if (strcmp (argv[1], "-a") == 0) {
      presistChild = 0;
    } else if (strcmp (argv[1], "-b") == 0) {
      presistChild = 1;
    }
  }
  pid_t anak_id;
  char *workingDir = "/home/kaw/Documents/Pertemuan2/shift";
  // Create child process
  anak_id = fork();
  // If there's error for child creation
  if (anak_id < 0) {
    // exit_failure constant is equal to 8
    exit(8);
  }

  // Kill parent if we're done creating child process
  if (anak_id > 0) {
    // exit_sucess constant is equal to 0
    exit(0);
  }
  // from now on it's child process

  // Random placement of closing file flags STDIN, STDOUT, STDERR
  close(STDOUT_FILENO);

  int sid = setsid();
  // If there's an error while assigning new sid
  // exit_failure constant is equal to 8
  if (sid < 0) exit(8);

  // Random placement of closing file flags STDIN, STDOUT, STDERR
  close(STDIN_FILENO);

  int chdirStat = chdir(workingDir);
  // if there's an error while changind directory
  if (chdirStat < 0) 
  // exit_failure constant is equal to 8
    exit(8);

  // Random placement of closing file flags STDIN, STDOUT, STDERR
  close(STDERR_FILENO);

  // change file permissions created by daemon process
  umask(0);

  // Create killer program
  pid_t killer_pid = fork();
  if (killer_pid < 0) { exit(EXIT_FAILURE); } // There's an error, cancel
  if (killer_pid == 0) {
    char *killerArg[] = {"gcc", "killer2.c", "-o", "killer2.exe", NULL};
    execv("/usr/bin/gcc", killerArg);
  }

  // Start of 1st child program
  while (1) {
    printf("Got into child daemon\n");
    // Get string of current time format
    char timeString[80];
    time_t t = time(NULL);
    struct tm timeStruct = *localtime(&t);
    snprintf(
      timeString, 
      sizeof(timeString), 
      "%d-%02d-%02d_%02d:%02d:%02d", 
      timeStruct.tm_year + 1900, 
      timeStruct.tm_mon + 1, 
      timeStruct.tm_mday, 
      timeStruct.tm_hour, 
      timeStruct.tm_min, 
      timeStruct.tm_sec
    );

    // Make new directory based on current time string
    mkdir(timeString, 0777);
    
    // Start of child child process
    pid_t anak_anak_id = fork();
    // If there's error for child creation
    if (anak_anak_id < 0) {
      // exit_failureconstant is equal to 8
      exit(8);
    }
    signal(SIGCHLD,SIG_IGN);
    // if is child child process
    if (anak_anak_id == 0) {
      signal(SIGCHLD,SIG_DFL);
      if (presistChild) {
        prctl(PR_SET_NAME,"dirChld\0",NULL,NULL,NULL);
      }

      printf("got into child process\n");
      // Change dir to new dir
      chdir(timeString);
      
      pid_t down_child_pid = fork();
      // If there's error for child creation
      if (down_child_pid < 0) {
        // exit_failureconstant is equal to 8
        exit(8);
      }
      // if is child child process
      if (down_child_pid == 0) {
        // handle all SIGCHLD requests
        // signal(SIGCHLD, handlerFunc); 
        signal(SIGCHLD,SIG_IGN);
        // Download every 5 seconds for 20 iterations
        for (int j = 0; j < 20; j++) {
        // for (int j = 0; j < 2; j++) {
          downloadPicsumNow();
          printf("Child child process waiting 5 sec\n");
          sleep(5);
        }
      } else {
        wait(NULL);
      }
      // Return signal handling to the default way
      // Wait until program is done downloading all files
      // Move to parent directory
      // Zip directory
      printf("zipping \n");
      zipDirectory(timeString);
      printf("zipped\n");
      chdir("..");
      // Delete the current folder
      printf("deleteing timestring folder\n");
      deleteFolder(timeString);
      printf("deleted timestring folder\n");
      exit(EXIT_SUCCESS);
    }
    printf("child process waiting 30 sec\n");
    sleep(30);
  }
}
