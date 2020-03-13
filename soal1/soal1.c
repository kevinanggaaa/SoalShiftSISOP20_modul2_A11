#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
  int detik, menit, jam, detik_sk, menit_sk, jam_sk;
  char *stringToLongPtr;

  if (argc != 5) {
    printf ("jumlah input salah\n");
    return 0;
  }


  if(strcmp(argv[1],"*")==0) {
    detik = -1;
  } else {
    detik = strtol(argv[1], &stringToLongPtr, 10);// && argv[1] >= 0 && argv[1] < 23 && argv[1] == 60 )
  } if (stringToLongPtr == argv[1]) {
    printf("input detik harus integer atau '*'\n");
  }

  if(strcmp(argv[2],"*")==0)
    menit = -1;
  else// && argv[2] >= 0 && argv[2] < 23 && argv[2] == 60 )
    menit = strtol(argv[2], &stringToLongPtr, 10);
  if (stringToLongPtr == argv[2])
    printf("input menit harus integer atau '*'\n");

  if (strcmp(argv[3],"*")==0)
    jam = -1;
  else// && argv[3] >= 0 && argv[3] < 23 && argv[3] == 60 ){
    jam = strtol(argv[3], &stringToLongPtr, 10);
  if (stringToLongPtr == argv[3])
    printf("input jam harus integer atau '*'\n");
  
  if(!(
    ((detik <= 59 && detik >= 0) || detik == -1) && 
    ((menit <= 59 && menit >= 0) || menit == -1) && 
    ((jam <= 23 && jam >= 0) || jam == -1)
    )) {
    printf("input tidak sesuai dengan ketentuan, lebih dari waktu dalam hari\n");
    return 0;
  }

  pid_t anak_id, sid;        // Variabel untuk menyimpan PID
  char *workingDir = "/home/zenados/Documents/2shift";

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
  // close(STDOUT_FILENO);

  sid = setsid();
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
  // close(STDERR_FILENO);

  // change file permissions created by daemon process
  umask(0);

  signal(SIGCHLD, SIG_IGN);
  printf("before while works\n");
  while (1) {
    // Initial run, the the appropriate
    // hitung sleep time untuk call pertama 
    struct tm *skrg_tm;
    time_t skrg;
    printf("is in while\n");

    int jedaHour, jedaMin, jedaSec;
    int resHour = 0, resMin = 0, resSec = 0;
    printf("start of calculation\n");
    skrg = time(NULL);
    skrg_tm = localtime(&skrg);
    int jam_skrg = skrg_tm->tm_hour;
    printf("jam: %d; jam_skrg: %d\n", jam, jam_skrg);
    if (jam != -1) {
      jedaHour = (jam - jam_skrg)%24;
      if (jedaHour > 0) {
        sleep(jedaHour*3600);
        resHour = 23 - jedaHour;
      }
    }
    printf("jam: %d; jam_skrg: %d; jedaHour: %d\n", jam, jam_skrg, jedaHour);

    skrg = time(NULL);
    skrg_tm = localtime(&skrg);
    int menit_skrg = skrg_tm->tm_min;
    printf("menit: %d; menit_skrg: %d\n", menit, menit_skrg);
    if (menit != -1) {
      jedaMin = (menit - menit_skrg)%60;
      if (jedaMin > 0) {
        sleep(jedaMin*60);
        resMin = 59 - jedaMin;
      }
    }
    printf("menit: %d; menit_skrg: %d; jedaMin: %d\n", menit, menit_skrg, jedaMin);
    
    skrg = time(NULL);
    skrg_tm = localtime(&skrg);
    int detik_skrg = skrg_tm->tm_sec;
    printf("detik: %d; detik_skrg: %d\n", detik, detik_skrg);
    if (detik != -1){
      jedaSec = (detik - detik_skrg)%60;
      if (jedaSec) {
        sleep(jedaSec);
        resSec = 60 - jedaSec;
      }
    } else {
      sleep(1);
    }
    printf("detik: %d; detik_skrg: %d; jedaSec: %d\n", detik, detik_skrg, jedaSec);

    pid_t child;
    child = fork();

    int status;
    if (child == 0) {
      char *tempString = malloc(sizeof(char)*200);
      char *bashArgv[] = {"bash", tempString, NULL};
      strcpy(tempString, argv[4]);
      execv("/bin/bash", bashArgv);
    }
    printf("Sleeping for: hour: %d; min: %d; sec: %d\n", resHour, resMin, resSec);
    sleep(resHour*3600 + resMin*60 + resSec);
  }
}
