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
1. (a) Buat folder dengan nama ```Musyik, Fylm dan Pyoto``` (tidak boleh menggunakan ```mkdir()```).
2. (b) Download ```zip``` file musik, film dan foto dari link yang disediakan.
3. (c) Extract ```zip``` file yang telah didownload.
4. (d) Pindahkan hasil extract ke folder ```Musyik, Fylm dan Pyoto``` (hanya file saja).
5. (e) Jalankan semua hal di atas secara otomatis pada 09 April 16.22 WIB.
6. (f) ```zip``` semua folder dengan nama ```Lopyu_Stevany.zip``` (semua folder di delete dan hanya menyisakan ```zip```)

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
(hasil screenshoot)

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
Dari hasil eksekusi di atas, terdownload folder yang bernama ```Musik_for_Stevany```, ```Foto_for_Stevany``` dan ```Film_for_Stevany```. <br>
(hasil screenshoot)

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
Berikut hasil dari extract. <br>
(hasil screenshoot)

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
(hasil screenshoot)








### Jawab 1d

### Jawab 1e

### Jawab 1f


<a name="soal2"></a>
## Soal 2

<a name="soal3"></a>
## Soal 3
