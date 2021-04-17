#include <sys/types.h>
#include <wait.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#define TOTALFOLDER 3
#define MAX_PATH 260

char current_dir[] = "/home/yoursemicolon/sisop-modul2/";
char *stev_foldername[] = {"Pyoto", "Musyik", "Fylm"};
char *zip_filename[] = {"Foto_for_Stefany.zip", "Musik_for_Stevany.zip", "Film_for_Stevany.zip"};
char *extract_foldername[] = {"FOTO", "MUSIK", "FILM"};
char *download_link[] = {"https://drive.google.com/uc?id=1FsrAzb9B5ixooGUs0dGiBr-rC7TS9wTD&export=download",
                        "https://drive.google.com/uc?id=1ZG8nRBRPquhYXq_sISdsVcXx5VdEgi-J&export=download",
                        "https://drive.google.com/uc?id=1ktjGgDkL0nNpY-vT7rT7O6ZI47Ke9xcp&export=download"};

 // 1f remove eall folders remained (empty extract folders here)
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

bool isNowTheBirthday(int mon, int day, int min) {
    int bday_mon = 4;
    int bday_day = 9;
    int bday_min = 22;

    if(bday_mon == mon && bday_day == day
        && bday_min == min) {
            return true;
        }
    return false;
}

void daemonStart() {
    pid_t pid, sid;
    pid = fork();

    if (pid < 0) {
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    umask(0);

    sid = setsid();
    if (sid < 0) {
        exit(EXIT_FAILURE);
    }

    if ((chdir(current_dir)) < 0) {
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}

int main() {
    // run daemon
    daemonStart();

    while(1) {
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

        // ini bday_hour dan bday_day-nya diubah dulu
        int bday_hour = 22;
        if(isNowTheBirthday(month, day, minute) && bday_hour == hour) {
            // 1f zip stev folders and delete all folders remained
            zipStevanyFolders();
            removeExtractFolders();
        } else if(isNowTheBirthday(month, day, minute) && bday_hour-6 == hour) { // 1e run automatically 6 hours before the birthday

                // 1a make folders
                makeFolders();

                // 1b download zip files
                for(int i=0; i<TOTALFOLDER; i++) {
                    downloadFiles(i);
                }

                // 1c extract zip files
                for(int i=0; i<TOTALFOLDER; i++) {
                    extractFiles(i);
                }

                // 1d move file from extract folders to stev folders
                for(int i=0; i<TOTALFOLDER; i++) {
                    moveFiles(i);
                }
            }

        sleep(30);
    }
}
