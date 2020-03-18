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

int main(int argc,char *argv[]) {
  int detik, menit, jam, detik_sk, menit_sk, jam_sk;
  char *stringToLongPtr;

  if (argc != 5) {
    printf ("jumlah input salah\n");
    return -1;
  }

  if(strcmp(argv[1],"*")==0) {
    detik = -1;
  } else {
    detik = strtol(argv[1], &stringToLongPtr, 10);// && argv[1] >= 0 && argv[1] < 23 && argv[1] == 60 )
    if (stringToLongPtr == argv[1]) {
      printf("input detik harus integer atau '*'\n");
      return -1;
    } else if (!(detik <= 59 && detik >= 0)) {
      printf("Detik tidak sesuai dengan ketentuan, lebih dari waktu dalam hari\n");
      return -1;
    }
  } 


  if(strcmp(argv[2],"*")==0) {
    menit = -1;
  } else {
    menit = strtol(argv[2], &stringToLongPtr, 10);
    if (stringToLongPtr == argv[2]) {
      printf("input menit harus integer atau '*'\n");
      return -1;
    } else if (!(menit <= 59 && menit >= 0)) {
      printf("Menit tidak sesuai dengan ketentuan, lebih dari waktu dalam hari\n");
      return -1;
    }
  }
  if (strcmp(argv[3],"*")==0){
    jam = -1;
  } else {
    jam = strtol(argv[3], &stringToLongPtr, 10);
    if (stringToLongPtr == argv[3]) {
      printf("input jam harus integer atau '*'\n");
      return -1;
    } else if (!(jam <= 23 && jam >= 0)){
      printf("Jam tidak sesuai dengan ketentuan, lebih dari waktu dalam hari\n");
      return -1;
    }
  }


  pid_t pid, sid;        // Variabel untuk menyimpan PID
  pid = fork();     // Menyimpan PID dari Child Process
  /* Keluar saat fork gagal
  * (nilai variabel pid < 0) */
  if (pid < 0) {
    exit(EXIT_FAILURE);
  }

  /* Keluar saat fork berhasil
  * (nilai variabel pid adalah PID dari child process) */
  if (pid > 0) {
    exit(EXIT_SUCCESS);
  }

  umask(0);

  sid = setsid();
  if (sid < 0) {
    exit(EXIT_FAILURE);
  }

  if ((chdir("/home/zenados/Documents/2shift")) < 0) {
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  signal(SIGCHLD, SIG_IGN);
  while (1) {
    time_t sk = time(NULL);
    struct tm *sk_tm = localtime(&sk);

    detik_sk = sk_tm->tm_sec;
    menit_sk = sk_tm->tm_min;
    jam_sk = sk_tm->tm_hour;

    printf("jam_sk: %d; menit: %d; detik: %d\n", jam_sk, menit_sk, detik_sk);
    if((jam_sk == jam || jam == -1) && (menit_sk == menit || menit == -1) && (detik_sk == detik || detik == -1)){
      pid_t child;
      child = fork();

      int status;

      if (child == 0){
        char *tempString = malloc(sizeof(char)*200);
        char *bashArgv[] = {"bash", tempString, NULL};
        strcpy(tempString, argv[4]);
        execv("/bin/bash", bashArgv);
      }
    }
    sleep(1);
  }
}
