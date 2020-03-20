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
#include <dirent.h> 

int is_regular_file(const char *path)
{
  struct stat path_stat;
  stat(path, &path_stat);
  return S_ISREG(path_stat.st_mode);
}


void mkdirMieSedap()
{
  pid_t mkdir_id;

  mkdir_id = fork();
  // If there's error for child creation
  if (mkdir_id < 0) 
    // exit_failure constant is equal to 8
    exit(8);

  // child process
  if (mkdir_id == 0)
  {
    pid_t indomie_id;

    indomie_id = fork();
    // If there's error for child creation
    if (indomie_id < 0) 
      // exit_failure constant is equal to 8
      exit(8);

    // child process
    if (indomie_id == 0)
    {
      char *mkdirArgs[] = {"mkdir", "-p", "indomie", NULL};
      execv("/bin/mkdir", mkdirArgs);
    }
    // Get sig child of indomie
    wait(NULL);
    sleep(5);

    pid_t sedap_id;
    sedap_id = fork();
    // If there's error for child creation
    if (sedap_id < 0) 
      // exit_failure constant is equal to 8
      exit(8);

    // child process
    if (sedap_id == 0)
    {
      char *mkdirArgs[] = {"mkdir", "-p", "sedaap", NULL};
      execv("/bin/mkdir", mkdirArgs);
    }
    // Get sig child of sedap
    wait(NULL);
    exit(EXIT_SUCCESS);
  }
}

void unzip()
{
  pid_t anak_id;

  anak_id = fork();
  // If there's error for child creation
  if (anak_id < 0) 
    // exit_failure constant is equal to 8
    exit(8);

  // child process
  if (anak_id == 0)
  {
    char *argUnzip[] = {"unzip", "jpg.zip", NULL};
    execv("/usr/bin/unzip", argUnzip);
  }
}

int main() {
  // change current directory
  chdir("/home/zenados/modul2");
  // make directory
  mkdirMieSedap();
  // unzip stuffs
  unzip();
  // wait for mkdirMieSedap and unzip
  wait(NULL);
  wait(NULL);
  // waitpid example waitpid(pid, &status, 0);

  // read files and directory in directory
  struct dirent *de;  // Pointer for directory entry 

  // opendir() returns a pointer of DIR type.  
  DIR *dr = opendir("/home/zenados/modul2/jpg"); 

  if (dr == NULL)  // opendir returns NULL if couldn't open directory 
  { 
      printf("Could not open current directory" ); 
      return 0; 
  } 

  // Refer http://pubs.opengroup.org/onlinepubs/7990989775/xsh/readdir.html 
  // for readdir() 
  // How many childs are there?
  int nChild = 0;
  while ((de = readdir(dr)) != NULL) 
  {
    if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0 ) {
      continue;
    }
    printf("%s\n", de->d_name); 
    char path[100];
    strcpy(path, "/home/zenados/modul2/jpg/");
    strcat(path, de->d_name);
    // If path is file
    if (is_regular_file(path))
    {
      // Move to sedaap
      // Add 1 more to the amount of children need to be waited
      nChild++;
      pid_t mover_pid = fork();
      if (mover_pid < 0) { exit(EXIT_FAILURE); } // There's an error, cancel
      if (mover_pid == 0) {
        char *mvArg[] = {"mv", path, "/home/zenados/modul2/sedaap", NULL};
        execv("/bin/mv", mvArg);
      }
    // If path is directory
    } else {
      // Move directory to indomie
      // Add 1 more to the amount of children need to be waited
      nChild++;
      // Dispatch dirChild
      pid_t dir_pid = fork();
      if (dir_pid < 0) { exit(EXIT_FAILURE); } // There's an error, cancel
      if (dir_pid == 0) {
        // Dispatch child
        pid_t mover_pid = fork();
        if (mover_pid < 0) { exit(EXIT_FAILURE); } // There's an error, cancel
        if (mover_pid == 0) {
          char *mvArgv[] = {"mv", path, "/home/zenados/modul2/indomie", NULL};
          execv("/bin/mv", mvArgv);
        } // End of mover child
        // get mover child sig
        wait(NULL);
        
        // Prepare touch path
        char touchPath[100];
        memset(touchPath, 0, sizeof(touchPath));
        strcpy(touchPath, "/home/zenados/modul2/indomie/");
        strcat(touchPath, de->d_name);
        // touch file coba1.txt
        pid_t touch1_pid = fork();
        if (touch1_pid < 0) { exit(EXIT_FAILURE); } // There's an error, cancel
        if (touch1_pid == 0) {
          // TODO: char new file
          strcat(touchPath, "/coba1.txt");
          char *touchArgv[] = {"touch", touchPath, NULL};
          execv("/bin/touch", touchArgv);
        } // End of touch1 child
        // wait 3 seconds
        sleep(3);
        // get touch1 child sig
        wait(NULL);

        // touch2 file coba2.txt
        pid_t touch2_pid = fork();
        if (touch2_pid < 0) { exit(EXIT_FAILURE); } // There's an error, cancel
        if (touch2_pid == 0) {
          // TODO: char new file
          strcat(touchPath, "/coba2.txt");
          char *touchArgv[] = {"touch", touchPath, NULL};
          execv("/bin/touch", touchArgv);
        } // End of touch2 child
        // get touch2 child sig
        wait(NULL);

        exit(EXIT_SUCCESS);
      }
    }
  }

  // Wait for all children
  while (nChild-- > 0) {
    wait(NULL);
  }

  closedir(dr);     
  return 0; 
}
