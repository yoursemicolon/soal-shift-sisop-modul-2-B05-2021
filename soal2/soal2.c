#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<dirent.h>
#include<string.h>


//soal 2a
void BukaZIP()
{
    pid_t C_id; // Variabel untuk menyimpan PID

    int sts; //status

    C_id = fork();

    char S[1000] = "/Users/nadiatiara/Downloads/pets.zip";
    char T[1000] = "/Users/nadiatiara/praktikum_sisop/modul2/petshop";

    // Keluar saat fork gagal (nilai variabel C < 0) 
    if (C_id < 0) 
    {
        #define EXIT_FAILURE 1
        exit (1); // Jika gagal membuat proses baru, program akan berhenti

    }if (C_id == 0) 
    {// this is child
        char *buat_folder[] = {"mkdir", "-p", T, NULL};

        execv("/usr/bin/mkdir", buat_folder);
    
        char *buka_zip[] = { "unzip", S, "-d", T, "*.jpg" , T, NULL}; 

        execv("/usr/bin/unzip", buka_zip);
    }
    else{
        while 
            ((wait(&sts)) > 0);
    }
}

//nomer 2b
void no2b(char *BP) //BP=basepath
{

    struct dirent *x;
    char P[1000];//path
    char N[1000];//nama

    DIR *d = opendir(BP); //d=dir

    char *str;
    char y[1000];

    char S[1000] = "/Users/nadiatiara/Downloads/pets.zip";
    char T[1000] = "/Users/nadiatiara/modul2/petshop";

    if (!d)
    {
        return;
    }

    while ((x = readdir(d)) != NULL)
    {
        if 
            (strcmp(x->d_name, ".") != 0 && strcmp(x->d_name, "..") != 0)
        {
            //printf("%s\n", dp->d_name);
            str = x->d_name;
            snprintf(N, sizeof N, "%s", x->d_name);

            char* m = strtok(str, ";_"); 

            int j=0;
            
            while 
                (m!= NULL)
            {
                if 
                    (fork()==0)
                {
                    //char y[1000];

                    snprintf(y, sizeof y, "T/%s", m);
                    
                    char *a[]={"mkdir", "-p", "T",y, NULL};

                    execv("/usr/bin/mkdir", a);
                }
                m = strtok(NULL, ";_");
                j++;
            }
            
            // Bangun jalur baru dari jalur dasar
            strcpy(P, BP);
            strcat(P, "/");
            strcat(P, x->d_name);

            no2b(P);
        }
    }
    closedir(d);
}

//nomor 2c
//nomor 2d
//nomor 2e

int main() {
    char *P;
    void no2b(char *P);
    pid_t C_id; // Variabel untuk menyimpan PID

    int sts; //status

    //2a
    C_id = fork();

    // Keluar saat fork gagal (nilai variabel C < 0) 
    if (C_id < 0) 
    {
        #define EXIT_FAILURE 1
        exit (1); // Jika gagal membuat proses baru, program akan berhenti

    }if (C_id == 0) 
    {
        BukaZIP();
        
        no2b("T"); 
    }
    else{
         while 
            ((wait(&sts)) > 0);
        //no2b("T"); 
    }
}
