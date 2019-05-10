# FP_SISOP19_E01

## Daemon

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

## Thread
```c

```