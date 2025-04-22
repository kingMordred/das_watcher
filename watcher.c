#include "watcher.h"

char*  setup(){
    char answer='\0';
    int retry = 0;
    char *path;
    int path_len = 0;

    while(retry < 3)
    {
        printf("Create a new path for monitoring ? (y/n)\n");
        scanf(" %c", &answer);
        //printf("%c", answer);
        retry++;

        if(answer == 'y' || answer == 'n') break;
        if(retry == 3)
        {
            printf("Max number of retries, bye\n");
            exit(1);
        }
    }

    switch (answer)
    {
    case 'y':
        printf("Specify new file or dir to watch (full path):\n");
        path = readline("Enter full path: ");
        //should add error handling with mkdir
        char *p = strdup(path);
        char *sep = strchr(p+1, '/');
        while(sep != NULL)
        {
            *sep = '\0';
            if (mkdir(p, 0755) && errno != EEXIST)
            {
                fprintf(stderr, "error while trying to create %s\n", p);
            }
            *sep = '/';
            sep = strchr(sep+1, '/');
        }
        //create final dir in path if not ending in /
        if (mkdir(p, 0755) && errno != EEXIST)
        {
            fprintf(stderr, "error while trying to create %s\n", p);
        }
        free(p);
        return path;
        break;
    case 'n':
        printf("Specify path to watch (full path):\n");
        path = readline("Enter full path: ");
        return path;
        break;
    default:
        break;
    }

}

int monitor(const char* path){
    int fd;
    int wd;
    char buf[BUF_LEN] __attribute__((aligned(4)));
    ssize_t len, i = 0;
    
    /*Love - Linux System Programming (O'Reilly, 2007) - Page 238*/
    fd = inotify_init ( );
    if (fd == -1){
        perror ("inotify_init");
        exit (EXIT_FAILURE);
    }

    wd = inotify_add_watch (fd, path, IN_ACCESS | IN_MODIFY);
    if (wd == -1) 
    {
        perror ("inotify_add_watch");
        exit (EXIT_FAILURE);
    }

    while (1)
    {
        /* read BUF_LEN bytes' worth of events */
    len = read (fd, buf, BUF_LEN);
    /* loop over every read event until none remain */
    while (i < len) 
    {
        struct inotify_event *event = (struct inotify_event *) &buf[i];
        printf ("wd=%d mask=%d cookie=%d len=%d dir=%s\n", event->wd, event->mask, event->cookie, event->len, (event->mask & IN_ISDIR) ? "yes" : "no");
        /* if there is a name, print it */
        if (event->len) printf ("name=%s\n", event->name);
        /* update the index to the start of the next event */
        i += sizeof(struct inotify_event) + event->len;
    }
    len = 0;
    i = 0;
    }
    
}