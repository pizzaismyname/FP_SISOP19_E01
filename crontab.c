#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <syslog.h>
#include <linux/limits.h>
#include <pthread.h>

FILE *tab;
char cmd[101];
pthread_t tid[1000];

void *thread()
{
    //Pengganti system()
    pid_t f = fork();
    if (f == 0)
    {
        fclose(tab);
        execl("/bin/sh", "sh", "-c", cmd, NULL);
    }
}

int main()
{
    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));

    pid_t pid, sid;
    pid = fork();
    if (pid < 0)
        exit(EXIT_FAILURE);
    if (pid > 0)
        exit(EXIT_SUCCESS);

    umask(0);

    sid = setsid();
    if (sid < 0)
        exit(EXIT_FAILURE);

    if ((chdir("/")) < 0)
        exit(EXIT_FAILURE);

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    while (1)
    {
        //Reading time
        time_t rawtime;
        struct tm *timeinfo;

        time(&rawtime);
        timeinfo = localtime(&rawtime);

        int id = -1;

        //Selalu eksekusi di detik ke-0
        if (timeinfo->tm_sec == 0)
        {
            char tabfile[PATH_MAX];
            sprintf(tabfile, "%s/crontab.data", cwd);
            tab = fopen(tabfile, "r");
            if (tab != NULL)
            {
                char i[3], h[3], d[3], m[3], dw[3];
                while (EOF != fscanf(tab, "%2s %2s %2s %2s %2s %100[^\r\n]", &i, &h, &d, &m, &dw, &cmd))
                {
                    //Clears out \n and \r
                    if (fgetc(tab) != '\n')
                        fgetc(tab);
                    if (
                        (strcmp(i, "*") == 0 ? 1 : atoi(i) == timeinfo->tm_min) &&
                        (strcmp(h, "*") == 0 ? 1 : atoi(h) == timeinfo->tm_hour) &&
                        (strcmp(d, "*") == 0 ? 1 : atoi(d) == timeinfo->tm_mday) &&
                        (strcmp(m, "*") == 0 ? 1 : atoi(m) == (timeinfo->tm_mon + 1)) &&
                        (strcmp(dw, "*") == 0 ? 1 : atoi(dw) == timeinfo->tm_wday))
                    {
                        //Penggunaan thread
                        pthread_create(&(tid[++id]), NULL, &thread, NULL);
                    }
                }
                fclose(tab);
            }
        }

        for (int i = 0; i < id; i++)
        {
            pthread_kill(tid[i]);
        }

        sleep(1);
    }

    exit(EXIT_SUCCESS);
}