# Laporan Resmi Soal Shift 2
Kelompok B05
* Amanda Rozi Kurnia 05111940000094
* Melanchthon Bonifacio Butarbutar 05111940000097
* Nadia Tiara Febriana 05111940000217

[Soal Shift 2](https://docs.google.com/document/d/121ZqwL7KXiKy3YpQgP6INuYlb4969WOj1GlZ9umO65w/edit)

## Table of Contents
1. [Soal 1](#soal1)
2. [Soal 2](#soal2)
3. [Soal 3](#soal3)

<a name="soal1"></a>
## Soal 1
Pada soal ini, kita diminta untuk membuat program C dengan ketentuan sebagai berikut.
1. (a) Buat folder dengan nama ```Musyik, Fylm dan Pyoto```.
2. (b) Download ```zip``` file musik, film dan foto dari link yang disediakan.
3. (c) Extract ```zip``` file yang telah didownload.
4. (d) Pindahkan hasil extract ke folder ```Musyik, Fylm dan Pyoto``` (hanya file saja).
5. (e) Jalankan semua hal di atas secara otomatis pada 09 April 16.22 WIB.
6. (f) ```zip``` semua folder dengan nama ```Lopyu_Stevany.zip``` (semua folder di delete dan hanya menyisakan ```zip```) dan jalankan otomatis pada 09 April 22.22 WIB.

**Note**
* Semua poin dijalankan oleh 1 script di latar belakang
* Tidak boleh menggunakan fungsi system(), mkdir(), dan rename()
* Tidak boleh pake cron 
* Menggunakan fork dan exec


### Jawab 1a
Karena tidak boleh menggunakan ```mkdir()```, maka digunakan ```fork``` dan ```exec``` agar dapat menjalankan perintah secara bersamaan yaitu ```fork``` untuk create new process dan ```exec``` untuk memanggil command ```mkdir```. Disini kami menggunakan ```wait``` untuk delay sebelum ke proses selanjutnya.
```c
// 1a make folders
void makeFolders() {
    pid_t child_id;
    int status;

    child_id = fork();
    if(child_id == 0) {
        char *argv[] = {"mkdir", stev_foldername[0], stev_foldername[1], stev_foldername[2], NULL};
        execv("/usr/bin/mkdir", argv);
    }
    while(wait(&status) > 0);
}
```
Dari hasil eksekusi, terbuat folder ```Musyik, Fylm dan Pyoto```. <br>
<img src="https://github.com/yoursemicolon/soal-shift-sisop-modul-2-B05-2021/blob/main/screenshoots/soal1-a.png" width="600"></img>

### Jawab 1b
Mendownload zip file dari link yang telah disediakan. Kami memanggil fungsi ```downloadFiles()``` dari fungsi ```main``` dan menggunakan ```exec``` untuk memanggil command ```wget```.
```c
// 1b download files
void downloadFiles(int indeks) {
    pid_t child_id;
    int status;

    child_id = fork();
    if(child_id == 0) {
        char *argv[] = {"wget", "--no-check-certificate", "-q", download_link[indeks], "-O", zip_filename[indeks], NULL};
        execv("/usr/bin/wget", argv);
    }
    while(wait(&status) > 0);
}
```
Dari hasil eksekusi di atas, terdownload file zip yang bernama ```Musik_for_Stevany```, ```Foto_for_Stevany``` dan ```Film_for_Stevany```. <br>
<img src="https://github.com/yoursemicolon/soal-shift-sisop-modul-2-B05-2021/blob/main/screenshoots/soal1-b.png" width="600"></img>

### Jawab 1c
Extract ```zip``` file yang didownload dengan command ```unzip```.
```c
// 1c extract files from downloaded zip files
void extractFiles(int indeks) {
    pid_t child_id;
    int status;

    child_id = fork();
    if(child_id == 0) {
        char *argv[] = {"unzip", "-q", zip_filename[indeks], NULL};
        execv("/usr/bin/unzip", argv);
    }
    while(wait(&status) > 0);
}
```

### Jawab 1d
Kemudian, pindahkan file yang ada di dalam folder hasil extract ```MUSIK```, ```FOTO```, dan ```FILM``` masing-masing ke dalam folder ```Musyik```, ```Pyoto```, dan ```Fylm``` (hanya filenya saja). Untuk memindahkan isi dari masing-masing folder, kami menggunakan perintah berikut.
```shell script
find source_path -type f -iname  "*.*" -exec mv {} destination_path ;
```
Berikut penjelasannya.
* ```-type f``` untuk mendefinisikan bahwa yang dicari adalah file. Jika menggunakan ```-d``` maka yang dicari directory.
* ```-iname``` untuk mencari file berdasarkan namanya dan mengabaikan case sensitive.
* ```mv {}``` untuk menentukan target folder dan memindahkan file dari directory awal. 
Untuk mendapatkan source_path dan destination_path kami menggunakan fungsi ```strcpy()``` dan ```strcat()```.
```c
// 1d move file from extract folder to stev folder
void moveFiles(int indeks) {
    pid_t child_id;
    int status;

    char extract_path[MAX_PATH];
    char stev_path[MAX_PATH];

    strcpy(extract_path, current_dir);
    strcpy(stev_path, current_dir);

    strcat(extract_path, extract_foldername[indeks]);
    strcat(stev_path, stev_foldername[indeks]);

    child_id = fork();
    if(child_id == 0) {
        char *argv[] = {"find", extract_path, "-type", "f", "-iname", "*.*", "-exec", "mv", "{}", stev_path, ";", NULL};
        execv("/usr/bin/find", argv);
    }
    while(wait(&status) > 0);
}
```
Berikut hasil eksekusinya. <br>
<img src="https://github.com/yoursemicolon/soal-shift-sisop-modul-2-B05-2021/blob/main/screenshoots/soal1-d.png" width="600"></img>

### Jawab 1e
Untuk menjalankannya secara otomatis pada 09 April 16.22 WIB, digunakan process ```daemon```. Untuk menyesuaikan dengan setting waktu dengan local time, kami menggunakan library ```time.h``` dan ```tm structure``` pada C.
```c
    //obtain the current time
    time_t now = time(NULL);
    time(&now);

    // return a pointer to a tm structure with its members
    struct tm *local = localtime(&now);

    int month, day, hour, minute, second;
    month = local -> tm_mon + 1;
    day = local -> tm_mday;
    hour = local -> tm_hour;
    minute = local -> tm_min;
```
Untuk memeriksanya, digunakan perintah
```c
if(isNowTheBirthday(month, day, minute) && bday_hour-6 == hour) {
    // do all instructions before
}
```
Hasil eksekusi: <br>
<img src="https://github.com/yoursemicolon/soal-shift-sisop-modul-2-B05-2021/blob/main/screenshoots/soal1-e.png" width="600"></img>

### Jawab 1f
Tepat pada 09 April 22.22 WIB, ```zip``` semua folder dengan nama ```Lopyu_Stevany.zip```. Masih menggunakan proses ```daemon``` dan dengan setting waktu yang sudah disesuaikan. Berikut perintah untuk memeriksa setting waktunya.
```c
if(isNowTheBirthday(month, day, minute) && bday_hour == hour)
```
Dan berikut adalah perintan yang harus dijalankan. Command ```zip -r``` akan men-zip file dan menghapus folder original.
```c
// 1f zip all stev folders and delete original folders
void zipStevanyFolders() {
    pid_t child_id;
    int status;

    child_id = fork();
    if(child_id == 0) {
        char *argv[] = {"zip", "-q", "-r", "-m", "Lopyu_Stevanny.zip", stev_foldername[0], stev_foldername[1], stev_foldername[2], NULL};
        execv("/usr/bin/zip", argv);
    }
    while(wait(&status) > 0);
}
```
Untuk menghapus folder lainnya, yaitu folder hasil extract yang sudah kosong digunakan command ```rm -d```.
```c
void removeExtractFolders() {
    pid_t child_id;
    int status;

    child_id = fork();
    if(child_id == 0) {
        char *argv[] = {"rm", "-d", extract_foldername[0], extract_foldername[1], extract_foldername[2], NULL};
        execv("/usr/bin/rm", argv);
    }
    while(wait(&status) > 0);
}
```
Hasil eksekusi program: <br>
<img src="https://github.com/yoursemicolon/soal-shift-sisop-modul-2-B05-2021/blob/main/screenshoots/soal1-f.png" width="600"></img>

### Kendala
* Awalnya program tidak berjalan ketika di execute, setelah kodingan dimasukkan ke dalam fungsi terpisah baru bisa tereksekusi.
* Bingung dengan penggunaan fork dan exec
* Belum tau cara mendapatkan setting waktu di local time

### Referensi
* http://www.csc.villanova.edu/~mdamian/Past/csc2405sp13/notes/Exec.pdf
* https://askubuntu.com/questions/172629/how-do-i-move-all-files-from-one-folder-to-another-using-the-command-line
* https://www.techiedelight.com/print-current-date-and-time-in-c/
* http://www.cplusplus.com/reference/ctime/localtime/

<a name="soal2"></a>
## Soal 2

### Jawab 2a

### Jawab 2b

### Jawab 2c

### Jawab 2d

### Jawab 2e

### Kendala

### Referensi

<a name="soal3"></a>
## Soal 3

### Jawab 3a

### Jawab 3b

### Jawab 3c

### Jawab 3d

### Jawab 3e

### Kendala

### Referensi

