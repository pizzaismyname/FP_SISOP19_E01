# FP_SISOP19_E01

## Penggunaan daemon
Menggunakan `daemon` supaya proses dapat berjalan di balik layar.
```c
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

while(1){
    //loop utama
}

exit(EXIT_SUCCESS);
```

## Mendapatkan waktu saat ini
```c
time_t rawtime;
struct tm *timeinfo;

time(&rawtime);
timeinfo = localtime(&rawtime);
```

## Mengeksekusi perintah setiap detik ke-0
```c
if (timeinfo->tm_sec == 0)
{
    //kode perintah
}
```

## Membaca file `crontab.data`
```c
char tabfile[PATH_MAX];
sprintf(tabfile,"%s/crontab.data",cwd);
FILE *tab = fopen(tabfile, "r");
if (tab != NULL)
{
    //pembacaan file
}
```

## Mengeksekusi perintah pada file `crontab.data` per baris
Jika setiap kolom waktu adalah `*`, maka perintah dapat langsung dijalankan. Jika isi kolom waktu adalah angka `0-9`, maka perintah dijalankan ketika waktu saat ini sesuai dengan angka pada kolom tersebut. Untuk menggantikan `system()`, dibuat _child process_ dengan `fork()`, kemudian digunakan `execl("/bin/sh", "sh", "-c", cmd, NULL);` untuk mengeksekusi _command_.
```c
char i[3], h[3], d[3], m[3], dw[3], cmd[101];
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
        //Pengganti system()
        pid_t f = fork();
        if (f == 0)
        {
            fclose(tab);
            execl("/bin/sh", "sh", "-c", cmd, NULL);
        }
    }
}
fclose(tab);

```
