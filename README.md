# SoalShiftSISOP20_modul2_A11

## Soal 1
Buatlah program C yang menyerupai crontab untuk menjalankan script bash dengan ketentuan sebagai berikut:
  * Program menerima 4 argumen berupa:
    * Detik: 0-59 atau * (any value)
    * Menit: 0-59 atau * (any value)
    * Jam: 0-23 atau * (any value)
    * Path file .sh
  * Program akan mengeluarkan pesan error jika argumen yang diberikan tidak sesuai
  * Program hanya menerima 1 config cron
  * Program berjalan di background (daemon)
  * Tidak boleh menggunakan fungsi system()

**Contoh: ./program \* 34 7 /home/somi/test.sh**

Program dengan argumen seperti contoh di atas akan menjalankan script test.sh setiap detik pada jam 07:34.

```
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
```

## Soal 2
Shisoppu mantappu! itulah yang selalu dikatakan Kiwa setiap hari karena sekarang dia merasa sudah jago materi sisop. Karena merasa jago, suatu hari Kiwa iseng membuat sebuah program.
  * Pertama-tama, Kiwa membuat sebuah folder khusus, di dalamnya dia membuat sebuah program C yang per 30 detik membuat sebuah folder dengan nama timestamp [YYYY-mm-dd_HH:ii:ss].
  * Tiap-tiap folder lalu diisi dengan 20 gambar yang di download dari https://picsum.photos/, dimana tiap gambar di download setiap 5 detik. Tiap gambar berbentuk persegi dengan ukuran (t%1000)+100 piksel dimana t adalah detik Epoch Unix. Gambar tersebut diberi nama dengan format timestamp [YYYY-mm-dd_HH:ii:ss].
  * Agar rapi, setelah sebuah folder telah terisi oleh 20 gambar, folder akan di zip dan folder akan di delete(sehingga hanya menyisakan .zip).
  * Karena takut program tersebut lepas kendali, Kiwa ingin program tersebut men- generate sebuah program "killer" **yang siap di run(executable)** untuk menterminasi semua operasi program tersebut. Setelah di run, program yang menterminasi ini lalu akan mendelete dirinya sendiri.
  * Kiwa menambahkan bahwa program **utama** bisa dirun dalam dua mode, yaitu MODE_A dan MODE_B. untuk mengaktifkan MODE_A, program harus dijalankan dengan argumen -a. Untuk MODE_B, program harus dijalankan dengan argumen -b. Ketika dijalankan dalam MODE_A, program utama akan langsung menghentikan semua operasinya ketika program killer dijalankan. Untuk MODE_B, ketika program killer dijalankan, program utama akan berhenti tapi membiarkan proses di setiap folder yang masih berjalan sampai selesai(semua
folder terisi gambar, terzip lalu di delete). 

Kiwa lalu terbangun dan sedih karena menyadari programnya hanya sebuah mimpi.
Buatlah program dalam mimpi Kiwa jadi nyata!

**Catatan:**
* Tidak boleh memakai system().
* Program utama harus ter-detach dari terminal

**Hint:**
* Gunakan fitur picsum.photos untuk mendapatkan gambar dengan ukuran tertentu
* Epoch Unix bisa didapatkan dari time()

```
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
```

## Soal 3
Jaya adalah seorang programmer handal mahasiswa informatika. Suatu hari dia memperoleh tugas yang banyak dan berbeda tetapi harus dikerjakan secara bersamaan (multiprocessing).
  * Program buatan jaya harus bisa membuat dua direktori di **“/home/[USER]/modul2/”**. Direktori yang pertama diberi nama **“indomie”**, lalu lima detik kemudian membuat direktori yang kedua bernama **“sedaap”**.
  * Kemudian program tersebut harus meng-ekstrak file **jpg.zip** di direktori **“/home/[USER]/modul2/”**. Setelah tugas sebelumnya selesai, ternyata tidak hanya itu tugasnya.
  * Diberilah tugas baru yaitu setelah di ekstrak, hasil dari ekstrakan tersebut (di dalam direktori **“home/[USER]/modul2/jpg/”**) harus dipindahkan sesuai dengan pengelompokan, semua file harus dipindahkan ke **“/home/[USER]/modul2/sedaap/”** dan semua direktori harus dipindahkan ke **“/home/[USER]/modul2/indomie/”**.
  * Untuk setiap direktori yang dipindahkan ke **“/home/[USER]/modul2/indomie/”** harus membuat dua file kosong. File yang pertama diberi nama **“coba1.txt”**, lalu 3 detik kemudian membuat file bernama **“coba2.txt”**.
(contoh : **“/home/[USER]/modul2/indomie/{nama_folder}/coba1.txt”**).

Karena Jaya terlalu banyak tugas dia jadi stress, jadi bantulah Jaya agar bisa membuat
program tersebut.

**Catatan :**
* Tidak boleh memakai system().
* Tidak boleh memakai function C mkdir() ataupun rename().
* Gunakan exec dan fork
* Direktori “.” dan “..” tidak termasuk

```
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
    char *argUnzip[] = {"unzip", "jpg.zip", "-d", "jpg", NULL};
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
```
