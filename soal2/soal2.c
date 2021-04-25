#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/wait.h>
#include<unistd.h>
#include<dirent.h>
#include<string.h>

char* cutJPG (char*s)
{
    int m,n;
    char* new;
 
    for (n = 0; s[n] != '\0'; n++);
    m = n - 4 + 1;
    if (m < 1){
        return NULL;}
    new = (char*) malloc (m * sizeof(char));
    for (n = 0; n < m - 1; n++)
        new[n] = s[n];
        new[n] = '\0';
        return new;
}

void eks (char execute[], char *p[])
{
    int status;
    pid_t c_id;
    c_id = fork();
    if(c_id == 0){
        execv(execute, p);
    }else{
        ((wait(&status))>0);
        return;
    }
}
 
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
 
void renamenewfolder(){
    struct dirent *dp;
    int status;
    pid_t c_id;
    c_id = fork();
    char T[100]="/home/nadiatiara/revisi/petshop";
 
    if(c_id < 0){
        exit(1);
    }
 
    if(c_id == 0){
        DIR *dir;
        dir=opendir(T);
 
        if(dir!= NULL){
            while((dp = readdir(dir)) != NULL)
            {
                if(dp -> d_type == DT_REG) //DT_REG utk cek file berdasarkan folder
                {
                    char *T1, *T2,*T3, *T4;
                    char *nf = dp -> d_name;
                    char *nf_new = cutJPG(nf);
                    //char T[100]="/home/nadiatiara/revisi/petshop/";
                    char res1[100], res2[100], vp2[100], vp3[100];
                    char tipe[50], nama[50], umur[50];
 
                    for(T1 = strtok_r(nf_new, "_", &T3);T1!=NULL;T1=strtok_r(NULL, "_", &T3))
                    {
                        char txt_p[100], ket_file[100], n_hewan[100];
                        char T[100]="/home/nadiatiara/revisi/petshop/";
 
                        strcpy(res1, nf);
                        strcpy(res2, nf);
                        strcpy(vp2, T);
                        strcpy(vp3, T);
                        int j = 0;
                        for(T2 = strtok_r(T1, ";", &T4);T2 != NULL;T2 = strtok_r(NULL, ";", &T4))
                        {
                            if(j==0){strcpy(tipe, T2);}
                            if(j==1){strcpy(nama, T2);}
                            if(j==2){strcpy(umur, T2);}
                            j++;
                        }
 
                        //2b
                        strcat(T, tipe);
                        char *Buat_Folder[] = {"mkdir", "-p", T, NULL};
                        eks("/bin/mkdir", Buat_Folder);
 
                        strcpy(n_hewan, nama);
 
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
                    }
                }
            }
        }
    }
    else{
        ((wait(&status))>0);
    }
}

void hapus()
{
    int status;
    pid_t c_id;
    c_id = fork();
    char T[100] = "/home/nadiatiara/revisi/petshop";
    if(c_id < 0){
        exit(EXIT_FAILURE);
    }
    if(c_id == 0){
        DIR *dir;
        dir = opendir(T);
        if(dir != NULL) {
            struct dirent *infolder;
            while((infolder = readdir(dir))!=NULL){
                if(infolder -> d_type == DT_REG){
                    char S[100] = "/home/nadiatiara/revisi/petshop/";
                    strcat(S, infolder -> d_name);
                    char *hapusgambar[] = {"remove","-rf", S, NULL};
                    eks("/bin/rm",hapusgambar);
                }
            }
        }
    }
    else
    {
        ((wait(&status)) > 0);
    }
}
 
int main(){
    int status;
    pid_t c_id;
    c_id= fork();
    if(c_id < 0){
        exit(1);
    }
    if(c_id == 0){
        BukaZIP();renamenewfolder();hapus();
    }
    else {
        ((wait(&status)) > 0);
    }
}
