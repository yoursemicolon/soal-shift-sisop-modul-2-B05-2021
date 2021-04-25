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
Dan berikut adalah perintan yang harus dijalankan. Command ```zip -m``` akan men-zip file dan menghapus folder original.
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
# Soal 2
Loba bekerja di sebuah petshop terkenal, suatu saat dia mendapatkan zip yang berisi banyak sekali foto peliharaan dan Ia diperintahkan untuk mengkategorikan foto-foto peliharaan tersebut. Loba merasa kesusahan melakukan pekerjaanya secara manual, apalagi ada kemungkinan ia akan diperintahkan untuk melakukan hal yang sama. Kamu adalah teman baik Loba dan Ia meminta bantuanmu untuk membantu pekerjaannya.

**Note:**
- Setiap data peliharaan disimpan sebagai nama foto dengan format `[jenis peliharaan]:[nama peliharaan]:[umur peliharaan dalam tahun]`. Jika terdapat lebih dari satu peliharaan, data dipisahkan menggunakan _underscore(_)_.
- Tidak boleh menggunakan fungsi system(), mkdir(), dan rename().
- Menggunakan fork dan exec.

### Jawab 2a
Pertama-tama program perlu mengextract zip yang diberikan ke dalam folder `“/home/[user]/modul2/petshop”`. Karena bos Loba teledor, dalam zip tersebut bisa berisi folder-folder yang tidak penting, maka program harus bisa membedakan file dan folder sehingga dapat memproses file yang seharusnya dikerjakan dan menghapus folder-folder yang tidak dibutuhkan.

_Source Code_
```C
//2a
void BukaZIP()
{
    pid_t c_id;
    int status;
    c_id = fork();
    
    char S[100] = "/home/nadiatiara/pets.zip";
    char T[100] = "/home/nadiatiara/revisi/petshop";
 
    if (c_id < 0){
        exit(1);
    }
    if(c_id == 0){
        char *Buat_Folder[] = {"mkdir","-p", T, NULL};
        eks("/bin/mkdir", Buat_Folder);
        char *Buka_ZIP[] = {"unzip",S, "-d",T, "*.jpg", NULL};
        eks("/usr/bin/unzip", Buka_ZIP);
    }else {
        while ((wait(&status)) > 0);
    }
}
```

**PENJELASAN**

Berdasarkan source code diatas, fungsi BukaZIP digunakan untuk meng-Unzip folder pets.zip yang berformat **.jpg** saja dan menghapus folder-folder yang tidak penting, kemudian akan dipindah ke dalam folder baru bernama ```petshop``` . Dikarenakan tidak boleh menggunakan ``mkdir`` maka proses dilakukan menggunakan ``fork`` dan ``exec``.

### Jawab 2b
Foto peliharaan perlu dikategorikan sesuai jenis peliharaan, maka kamu harus membuat folder untuk setiap jenis peliharaan yang ada dalam zip. Karena kamu tidak mungkin memeriksa satu-persatu, maka program harus membuatkan folder-folder yang dibutuhkan sesuai dengan isi zip.
Contoh: Jenis peliharaan kucing akan disimpan dalam `“/petshop/cat”`, jenis peliharaan kura-kura akan disimpan dalam `“/petshop/turtle”`.

_Source Code_
```C
//2b
                        strcat(T, tipe);
                        char *Buat_Folder[] = {"mkdir", "-p", T, NULL};
                        eks("/bin/mkdir", Buat_Folder);
 
                        strcpy(n_hewan, nama);
```

**PENJELASAN**


### Jawab 2c
Setelah folder kategori berhasil dibuat, programmu akan memindahkan foto ke folder dengan kategori yang sesuai dan di rename dengan nama peliharaan.
Contoh: `“/petshop/cat/joni.jpg”`. 

### Jawab 2d
Karena dalam satu foto bisa terdapat lebih dari satu peliharaan maka foto harus di pindah ke masing-masing kategori yang sesuai. 
Contoh: foto dengan nama `“dog;baro;1_cat;joni;2.jpg”` dipindah ke folder `“/petshop/cat/joni.jpg”` dan `“/petshop/dog/baro.jpg”`.

_Source Code_
```C
//2c
//2d
                        strcat(nama, ".jpg");
                        strcat(vp2, res2);
 
                        char *salinfolder[] = {"cp", vp2, T, NULL};
                        eks("/bin/cp", salinfolder);
 
                        strcpy(txt_p, T);
                        strcpy(vp3, T);
                        strcat(vp3, "/");
                        strcat(vp3, res2);
                        strcat(T, "/");
                        strcat(T, nama);
 
                        char *rename_file[] = {"rename_file", vp3, T, NULL};
                        eks("/bin/mv", rename_file);
```

**PENJELASAN**

### Jawab 2e
Di setiap folder buatlah sebuah file `"keterangan.txt"` yang berisi nama dan umur semua peliharaan dalam folder tersebut. **Format harus sesuai contoh**.

```
nama : joni
umur  : 3 tahun

nama : miko
umur  : 2 tahun
```
_Source Code_
```C
//2e
                        strcat(txt_p, "/keterangan.txt");
                        strcpy(ket_file, "nama : ");
                        strcat(ket_file, n_hewan);
                        strcat(ket_file, "\numur : ");
                        strcat(ket_file, umur);
                        strcat(ket_file, "tahun\n\n");
 
                        FILE *ket = fopen(txt_p, "a");
                        fputs(ket_file, ket);
                        fclose(ket);
```

**PENJELASAN**

**Output**
- folder petshop berisi file foto-foro peliharaan yang sudah di unzip.
<img width="1311" alt="folder petshop" src="https://user-images.githubusercontent.com/72669398/115989566-36c86d80-a5e9-11eb-9438-cbb6e6a18d40.png">

- folder petshop berisi folder-folder nama masing-masing peliharaan dan keterangan berisi nama dan umur peliharaan.
![Isi petshop](https://user-images.githubusercontent.com/72669398/115989764-25339580-a5ea-11eb-8918-fc541d2065d5.jpg)

- isi folder salah satu peliharaan. misal: cat.
![isi folder cat](https://user-images.githubusercontent.com/72669398/115989806-58762480-a5ea-11eb-83a8-e776f227dd55.jpg)

- isi keterangan.txt dari folder cat.
![keterangan cat](https://user-images.githubusercontent.com/72669398/115989807-5a3fe800-a5ea-11eb-9990-4c45dcee26fb.jpg)

### Test Keterangan
cek test keterangan menggunakan link yang diberikan asuji : ```https://gist.github.com/segi3/0f90621bb2d118b0b6fadc98d5835b02```, gunanya untuk menghitung jumlah isi keterangan pada 2e yang ada pada folder masing-masing hewan.
menghasilkan output sebagai berikut:
![hasil ceket](https://user-images.githubusercontent.com/72669398/115988941-fd423300-a5e5-11eb-8c85-8a9ae2690dd1.jpg)

### Kendala
- Bingung dengan penggunaan fork dan exec, awalnya saya kira dengan mendeklasrasikan fork sekali bisa digunakan pada proses lain ternyata tidak bisa. Jadi, saya membuat fungsi func yang gunanya supaya tidak menggunakan fork berulang kali.
- agak sulit untuk mengerjakan dikarenakan macos error, tau errornya karena seharusnya file foto langsung ter-unzip di folder petshop tetapi ini malah terbentuk 2 folder baru yang muncul entah dari mana seperti gambar dibawah ini:
<img width="120" alt="Screen Shot 2021-04-25 at 09 16 04" src="https://user-images.githubusercontent.com/72669398/115978029-1aa3dc80-a5a7-11eb-8e30-5a22f7da5665.png">

- ketika mencoba test nomer 2e menggunakan link ```https://gist.github.com/segi3/0f90621bb2d118b0b6fadc98d5835b02``` hasil tidak sama dengan jumlah isi keterangan 2e, hasilnya ada 2 keterangan, seperti dibawah ini:
<img width="278" alt="Screen Shot 2021-04-25 at 02 28 43" src="https://user-images.githubusercontent.com/72669398/115978056-4f179880-a5a7-11eb-98c0-bf5e7b89e499.png">


<a name="soal3"></a>
## Soal 3

### Jawab 3a

### Jawab 3b

### Jawab 3c

### Jawab 3d

### Jawab 3e

### Kendala

### Referensi

