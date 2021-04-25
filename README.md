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

Berdasarkan source code diatas, fungsi BukaZIP digunakan untuk meng-Unzip folder pets.zip yang berformat **.jpg** saja dan menghapus folder-folder yang tidak penting, kemudian akan dipindah ke dalam folder baru bernama ```petshop``` . Dikarenakan tidak boleh menggunakan mkdir maka proses dilakukan menggunakan fork dan exec.

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

Berdasarkan source code diatas, digunakan untuk membuat folder baru bernamakan masing-masing peliharaan. co : ```/petshop/cat```

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

Berdasarkan source code 2c dan 2d diatas, soal 2c digunakan untuk memindahkan foto peliharaan ke dalam folder sesuai dengan kategori yang sesuai. kemudian, soal 2d digunakan untuk memindahkan foto peliharaan yang di dalam satu foto bisa terdapat lebih dari satu foto peliharaan dan di sesuaikan berdasarnakn nama kategori hewannya.

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

Berdasarkan source code pada soal 2e ini, akan membuat file ```keterangan.txt``` . kemudian akan menyalin ```"nama : "``` ke dalam ket_file. lalu, menggabungkan data nama hewan, menggabungkan umur, serta menambahkan kata tahun setelah data umur.

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
Pada soal ini, kita diminta untuk membuat program C dengan ketentuan sebagai berikut.
1. (a) Setiap 40 detik buat folder dengan nama sesuai timestamp ```[YYYY-mm-dd_HH:ii:ss]```
2. (b) Setiap direktori yang sudah dibuat diisi dengan 10 gambar yang didownload dari https://picsum.photos/, dimana setiap gambar akan didownload setiap 5 detik. Setiap gambar yang didownload akan diberi nama dengan format timestamp ```[YYYY-mm-dd_HH:ii:ss]``` dan gambar tersebut berbentuk persegi dengan ukuran (n%1000) + 50 pixel dimana n adalah detik Epoch Unix.
3. (c) Setelah direktori telah terisi dengan 10 gambar, program tersebut akan membuat sebuah file```status.txt```, dimana didalamnya berisi pesan “Download Success” yang terenkripsi dengan teknik Caesar Cipher dan dengan shift 5. Karena Ranora orangnya perfeksionis dan rapi, dia ingin setelah file tersebut dibuat, direktori akan di zip dan direktori akan didelete, sehingga menyisakan hanya file zip saja.
4. (d) Ranora harus membuat program bash dimana program "Killer" tersebut akan menterminasi semua proses program yang sedang berjalan dan akan menghapus dirinya sendiri setelah program dijalankan.
5. (e) Pembimbing magang Ranora juga ingin nantinya program utama yang dibuat Ranora dapat dijalankan di dalam dua mode. Untuk mengaktifkan mode pertama, program harus dijalankan dsdengan argumen -z, dan Ketika dijalankan dalam mode pertama, program utama akan langsung menghentikan semua operasinya Ketika program Killer dijalankan. Sedangkan untuk mengaktifkan mode kedua, program harus dijalankan dengan argumen -x, dan Ketika dijalankan dalam mode kedua, program utama akan berhenti namun membiarkan proses di setiap direktori yang masih berjalan hingga selesai 

### Jawab 3a
```
 int forcheck = 0;
        time_t timer;
        char stamp[50];
        struct tm *tm_info;
        int status = 0;
        timer = time(NULL);
        tm_info = localtime(&timer);

        strftime(stamp, 50, "%Y-%m-%d_%H:%M:%S", tm_info);

        if (fork() == 0)
        {
            char *argv[] = {"mkdir", stamp, NULL};
            execv("/bin/mkdir", argv);
        }
        sleep(1);
        ...
```
Yang pertama harus dilakukan adalah melakuan inisialisasi timer ```time_t timer``` dan meng-assign ```time(NULL)``` ke timer tersebut. <br> Kemudian menginisialisasi ```struct tm *tm_info``` yang akan membantu kita untuk mengubah ```timer``` menjadi waktu local dengan ```localtime(&timer)```. <br> Kemudian mengubahnya menjadi char dengan format yang sesuai dengan ```strftime(stamp, 50, "%Y-%m-%d_%H:%M:%S", tm_info)```.
<br>
Kemudian melakukan spawning process yang akan membuat directory dengan format ```%Y-%m-%d_%H:%M:%S``` <br>
```
if (fork() == 0)
        {
            char *argv[] = {"mkdir", stamp, NULL};
            execv("/bin/mkdir", argv);
        }
sleep(1);
```
Di sini menggunakan sleep untuk memastikan proses pembuatan directory selesai sebelum lanjut ke proses selanjutnya.
### Jawab 3b
```
if (fork() == 0)
        {
            chdir(stamp);
            for (int i = 0; i < 10; i++)
            {
                //melakukan inisialisasi timer baru untuk tiap foto yang didownload
                printf("%d\n", i);
                time_t timer2;
                char stamp2[50];
                struct tm *tm_info2;
                // di sini melakukan pengurangan waktu sebanyak 1 detik karena sebelumnya melakukan sleep selama satu detik untuk menunggu direktori dibuat
                timer2 = time(NULL);
                tm_info2 = localtime(&timer2);
                tm_info2->tm_sec -= 1;
                mktime(tm_info2);

                strftime(stamp2, 50, "%Y-%m-%d_%H:%M:%S", tm_info2);

                int size = (int)timer2 % 1000 + 50;
                char link[50];
                sprintf(link, "https://picsum.photos/%d", size);
                if (fork() == 0)
                {
                    char *argv[] = {"wget", link, "-O", stamp2, NULL};
                    execv("/bin/wget", argv);
                }

                printf("sleeping\n");
                sleep(5);
                printf("now awake/n");
                forcheck++;
            }
        }
```
Pertama masuk ke firectory yang telah dibuat dengan ```chdir(stamp)``` <br>
Kemudian dalam dalam ```for (int i = 0; i < 10; i++)```
- Melakukan inisialisasi ```time_t timer2``` yang akan disimpan sebagai string ke ```stamp2```
- Melakukan pengurangan waktu sebanyak 1 detik karena setelah membuat directory telah dilakukan ```sleep(1)```. Hal ini dilakukan dengan
  ```
  tm_info2->tm_sec -= 1;
  mktime(tm_info2);
  ```
- Kemudian membuat variable ```int size``` yang akan menyimpan ukuran gambar ```(int)timer2 % 1000 + 50```
- Menyimpan link dengan ukuran ke variable ```char link[50]``` dengan menggunakan ```sprintf(link, "https://picsum.photos/%d", size)```
- Membuat proses baru dengan execv dengan ```wget```
```
if (fork() == 0)
                {
                    char *argv[] = {"wget", link, "-O", stamp2, NULL};
                    execv("/bin/wget", argv);
                }

 printf("sleeping\n");
 sleep(5);
 printf("now awake/n");
 forcheck++;
```
- Menggunakan ```sleep(5)``` untuk melakukannya setiap 5 detik.
- ```forcheck++``` akan digunakan saat pembuatan ```status.txt``` dan menandakan loop telah selesai
### Jawab 3c
```
if (forcheck == 10) // pada saat awal menjalankan program langsung membuat file status.txt jadi menggunakan check untuk membuat di akhir loop
        {
            char success[] = {"Download Success"};
            for (int i = 0; i < strlen(success); i++)
            {
                //eksepsi agar whitespace tidak diencrypt
                if (success[i] != 32)
                {
                    //untuk setiap karakter di aatas ascii 117 balik ke awal
                    if (success[i] > 117)
                    {
                        success[i] -= 21;
                        continue;
                    }
                    success[i] += 5;
                }
            }
            // while (wait(&status) > 0)
            //     ;
            printf("creating status.txt");
            FILE *fPtr = NULL;
            fPtr = fopen("status.txt", "w");
            fputs(success, fPtr);
            fclose(fPtr);
        }
```
- Meyimpan string ```Download Success``` ke dalam ```char success[]```
- Untuk setiap karakter dalam string melakukan enkripsi Caesar Cipher dengan shift 5
- ```if (success[i] != 32)``` agar spasi tidak ikut terenkripsi
- Ketika huruf di atas ascii 117 kembali ke awal
```
if (success[i] > 117)
                    {
                        success[i] -= 21;
                        continue;
                    }
```
- Selain itu shift sebanyak 5 ```success[i] += 5;```
- Kemudian membuat file dan menuliskan pesan ```success``` ke dalam file ```status.txt```
```
printf("creating status.txt");
FILE *fPtr = NULL;
fPtr = fopen("status.txt", "w");
fputs(success, fPtr);
fclose(fPtr);
```
### Jawab 3d dan 3e
```
 if (argc == 2)
    {
        if (strcmp(argv[1], "-z") == 0)
        {
            FILE *fPtr = NULL;
            fPtr = fopen("killer.sh", "w");
            fprintf(fPtr, "#!/bin/bash\npkill soal3\nrm killer.sh\n");
            fclose(fPtr);
        }

        else if (strcmp(argv[1], "-x") == 0)
        {
            FILE *fPtr = NULL;
            fPtr = fopen("killer.sh", "w");
            fprintf(fPtr, "#!/bin/bash\nkill %d\nrm killer.sh\n", getpid());
            fclose(fPtr);
        }
    }
```
- Ketika count argumen == 2 ```if (argc == 2)```
- Jika argumen ```-z``` maka membuat file ```killer.sh``` yang berisikan ```pkill soal3``` untuk menghentikan semua proses yang berjalan dan ```rm killer.sh``` untuk menghapus dirinya sendiri setelah selssai.
- Jika argumen ```-x``` maka membuat file ```killer.sh``` yang berisikan ```kill %d``` dengan ```%d``` merupakan ```getpid()``` Daemon untuk menghentikan Daemon dan ```rm killer.sh``` untuk menghapus diri sendiri. Hal ini akan menyebabkan proses yang sedang berjalan tetap selesai hingga melakukan zip.
- 
### Kendala
- Belum tahu penggunaan fork()
- Bingung bagaimana cara menghentikan proses tetapi proses yang sedang berjalan tetap selesai.
### Referensi

