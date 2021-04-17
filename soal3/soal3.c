#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <syslog.h>

int main(int argc, char *argv[])
{
    pid_t pid, sid; // Variabel untuk menyimpan PID

    pid = fork(); // Menyimpan PID dari Child Process

    /* Keluar saat fork gagal
  * (nilai variabel pid < 0) */
    if (pid < 0)
    {
        exit(EXIT_FAILURE);
    }

    /* Keluar saat fork berhasil
  * (nilai variabel pid adalah PID dari child process) */
    if (pid > 0)
    {
        exit(EXIT_SUCCESS);
    }
    umask(0);
    sid = setsid();
    if (sid < 0)
    {
        exit(EXIT_FAILURE);
    }

    //SOAL 3D DAN 3E
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

    while (1)
    {

        //SOAL 3A
        //melakukan inisialisasi timer untuk folder atau direktori yang akan dicreate setiap 40 detik
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

        //SOAL 3B
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

        // while (wait(&status) > 0)
        //     ;

        //menggunakan command zip dengan argumen -rm agar meremove semua file setelah zip berhasil dibuat
        if (fork() == 0)
        {
            chdir("..");
            char name[50];
            strcpy(name, stamp);
            strcat(name, ".zip");
            char *argv[] = {"zip", "-rm", name, stamp, NULL};
            execv("/bin/zip", argv);
        }
        forcheck = 0;
        // if (fork() == 0)
        // {
        //     char *argv[] = {"rm", "-r", stamp, NULL};
        //     execv("/bin/rm", argv);
        // }

        // if (fork() == 0)
        // {
        //     char *argv[] = {"rm", "-r", "status.txt", NULL};
        //     execv("/bin/rm", argv);
        // }

        //sleep selama 39 detik karena sebelumnya telah melakukan sleep untuk menunggu keberhasilan pembuatan folder
        sleep(39);
    }
}
