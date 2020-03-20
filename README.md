

## Kevin Angga Wijaya - 05111840000024
## Julius - 05111840000082
![Run as admin](https://user-images.githubusercontent.com/60419316/76970734-e51dd000-695e-11ea-9e0f-e837779ca615.png)
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
## Penjelasan Soal 1
```
int main(int argc,char *argv[]) {
  int detik, menit, jam, detik_sk, menit_sk, jam_sk;
  char *stringToLongPtr;
```  
Pertama - tama, kita harus menginput data yang diperlukan dan mendeklarasikan variabel variabel yang dibutuhkan.

```
  if (argc != 5) {
    printf ("jumlah input salah\n");
    return -1;
  }
```
Jika input tidak sama dengan 5, maka print "jumlah input salah\n"

```
  if(strcmp(argv[1],"*")==0) {
    detik = -1;
  } else {
    detik = strtol(argv[1], &stringToLongPtr, 10);
    if (stringToLongPtr == argv[1]) {
      printf("input detik harus integer atau '*'\n");
      return -1;
    } else if (!(detik <= 59 && detik >= 0)) {
      printf("Detik tidak sesuai dengan ketentuan, lebih dari waktu dalam hari\n");
      return -1;
    }
  } 
```
  Jika `argv[1]` sama dengan `"*"`, maka `detik = -1`. Selain itu, `detik` diinput dengan nilai integer basis 10 pada `argv[1]`. Jika isi dari `stringToLongPtr` sama dengan `argv[1]`, yang berarti isi `argv[1]` adalah huruf atapun simbol (selain `"*"`), maka print `"input detik harus integer atau ' * ' \n"`, lalu proses berhenti. selain itu, jika `detik` tidak kurang dari sama dengan 59 dan tidak lebih sama dengan 0, maka print `"Detik tidak sesuai dengan ketentuan, lebih dari waktu dalam hari\n"`, lalu proses berhenti.

```
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
```
Jika `argv[2]` sama dengan `"*"`, maka `menit = -1`. Selain itu, `menit` diinput dengan nilai integer basis 10 pada `argv[2]`. Jika isi dari `stringToLongPtr` sama dengan `argv[2]`, yang berarti isi `argv[2]` adalah huruf atapun simbol (selain `"*"`), maka print `"input menit harus integer atau ' * ' \n"`, lalu proses berhenti. selain itu, jika `menit` tidak kurang dari sama dengan 59 dan tidak lebih sama dengan 0, maka print `"menit tidak sesuai dengan ketentuan, lebih dari waktu dalam hari\n"`, lalu proses berhenti.
 
```
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
```
Jika `argv[3]` sama dengan `"*"`, maka `jam = -1`. Selain itu, `jam` diinput dengan nilai integer basis 10 pada `argv[3]`. Jika isi dari `stringToLongPtr` sama dengan `argv[3]`, yang berarti isi `argv[3]` adalah huruf atapun simbol (selain `"*"`), maka print `"input jam harus integer atau ' * ' \n"`, lalu proses berhenti. selain itu, jika `jam` tidak kurang dari sama dengan 23 dan tidak lebih sama dengan 0, maka print `"jam tidak sesuai dengan ketentuan, lebih dari waktu dalam hari\n"`, lalu proses berhenti.

```
  pid_t pid, sid;     
  pid = fork();     
```
Mendeklarasikan variabel untuk menyimpan PID dan menyimpan PID dari Child Process

```
  if (pid < 0) {
    exit(EXIT_FAILURE);
  }

  if (pid > 0) {
    exit(EXIT_SUCCESS);
  }
```
Membunuh parent process agar sistem operasi mengira bahwa proses telah selesai.

```
  umask(0);
```
Mengubah mode file agar mendapatkan akses penuh terhadap file yang dibuat oleh daemon.

```
  sid = setsid();
  if (sid < 0) {
    exit(EXIT_FAILURE);
  }
```
Membuat Unique Session ID

```
  if ((chdir("/home/kaw/Documents/Pertemuan2/shift")) < 0) {
    exit(EXIT_FAILURE);
  }
```
Mengubah working directory

```
  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);
```
Menutup File Descriptor Standar

```
  signal(SIGCHLD, SIG_IGN);
```
Pengganti fungsi `wait`

```
    time_t sk = time(NULL);
    struct tm *sk_tm = localtime(&sk);
```
Mendeklarasikan variabel `sk` dengan tipe data `time_t` dan diisi dengan nilai waktu sistem saat program berjalan. Lalu, isi `sk` diinput ke sebuah struct `sk_tm` dengan format struct `tm`.

```
    detik_sk = sk_tm->tm_sec;
    menit_sk = sk_tm->tm_min;
    jam_sk = sk_tm->tm_hour;
    printf("jam_sk: %d; menit: %d; detik: %d\n", jam_sk, menit_sk, detik_sk);
```
Mendeklarasikan nilai dari `detik_sk`, `menit_sk`, dan `jam_sk` dengan waktu sistem pada struct `sk_tm`. Lalu diprint yang berguna untuk debugging.

```
    if((jam_sk == jam || jam == -1) && (menit_sk == menit || menit == -1) && (detik_sk == detik || detik == -1)){
      pid_t child;
      child = fork();
```
Jika (nilai `jam` yang diinput sama dengan jam sistem atau nilai `jam` sama dengan -1) dan (nilai `menit` yang diinput sama dengan menit sistem atau nilai `menit` sama dengan -1) dan (nilai `detik` yang diinput sama dengan detik sistem atau nilai `detik` sama dengan -1), dibuat process child.

```
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
```
Jika nilai childnya 0, maka dideklarasikan char `tempString`, lalu deklarasi char `bashArgv[]` yang isinya `tempString`, lalu dicopy nilai dari `argv[4]` ke `tempString`, lalu diexec sehingga ngebash `bashArgv`. Lalu sistem dijalankan setiap detik.

## Testing
Kami mengetes menggunakan file .sh sebagai berikut.  
![image](https://user-images.githubusercontent.com/60419316/77056590-354f6d80-6a05-11ea-99a9-e5520bf7ba87.png)
### Detik -1
![image](https://user-images.githubusercontent.com/60419316/77044596-4b076780-69f2-11ea-905c-a39902ed4b1f.png)
### Menit -1
![image](https://user-images.githubusercontent.com/60419316/77044885-ccf79080-69f2-11ea-822f-4c434d494df2.png)
### Jam -1
![image](https://user-images.githubusercontent.com/60419316/77045157-3c6d8000-69f3-11ea-9bb6-f2b8351fffa1.png)
### Jalan setiap menit pada detik ke 5
![image](https://user-images.githubusercontent.com/60419316/77056456-020cde80-6a05-11ea-928d-18ac925cb729.png)


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
```
## Testing
### Compile program
![image](https://user-images.githubusercontent.com/60419316/77169794-a9187580-6aec-11ea-938c-5814ff6b55a6.png)
