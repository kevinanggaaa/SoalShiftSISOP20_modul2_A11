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
#include <limits.h>

int main()
{
  pid_t anak_id;
  anak_id = fork();
  // If there's error for child creation
  if (anak_id < 0) {
    // exit_failure constant is equal to 8
    exit(8);
  }

  if (anak_id == 0)
  {
    // Exec pkill soal2.exe
    char *pkillArgv[] = {"pkill", "soal2", NULL};
    execv("/usr/bin/pkill", pkillArgv);
  } else {
    // wait for child program
    wait(NULL);
    printf("running rm on file");
    execl("/bin/rm", "rm", "killer2.exe", NULL);
    printf("test, still shows after execl");
  }
  // Child program
  // delete exe self
}
