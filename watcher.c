#include "watcher.h"

Preferences  setup(){
    char answer = '\0';
    char notification = '\0';
    int retry = 0;
    char *path;
    int path_len = 0;
    Preferences choices;

    while(retry < 3)
    {
        printf("Create a new path for monitoring ? (y/n)\n");
        scanf(" %c", &answer);
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
        //return path;
        choices.path = path;
        break;
    case 'n':
        printf("Specify path to watch (full path):\n");
        path = readline("Enter full path: ");
        //return path;
        choices.path = path;
        break;
    default:
        break;
    }

    printf("Do you want to be notified every time an action occurs on the Path ? (y/n)\n");
    scanf(" %c", &notification);
    switch (notification)
    {
    case 'y':
        choices.notification = 1;
        break;
    case 'n':
        choices.notification = 0;
        break;
    default:
        choices.notification = -1;
        break;
    }

    return choices;

}

void notify(const int notification){
    int fd;

    if(notification)
    {
        system("notify-send Watcher \"A action has occured in your path. See log file.\"");
    }
}

int monitor(const Preferences choices){
    int fd;
    int wd;
    char buf[BUF_LEN] __attribute__((aligned(4)));
    ssize_t len, i = 0;
    FILE *fp;
    time_t timenow;

    fp = fopen("log.txt", "w");
    fclose(fp);
    
    /*Love - Linux System Programming (O'Reilly, 2007) - Page 238*/
    fd = inotify_init ( );
    if (fd == -1){
        perror ("inotify_init");
        exit (EXIT_FAILURE);
    }

    wd = inotify_add_watch (fd, choices.path, IN_ACCESS | IN_MODIFY | IN_CREATE | IN_DELETE);
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
            time(&timenow);
            char *curr_time = ctime(&timenow);
            curr_time[strlen(curr_time)-1] = '\0';

            fp = fopen("log.txt", "a");
            struct inotify_event *event = (struct inotify_event *) &buf[i];

            /*cases that interest us:
            - Directory is read from
            - Dir is written to ; specify if a file or dir was written
            */
            if(event->mask & IN_CREATE)
            {
                if(event->len)
                {
                    if(event->mask & IN_ISDIR)
                    {
                        printf("The following directory was created in Path: %s\n", event->name);
                        fprintf(fp, "%s - The following directory was created in Path: %s\n", curr_time, event->name);
                    }
                    else
                    {
                        printf("The following file was created in Path: %s\n", event->name);
                        fprintf(fp, "%s - The following file was created in Path: %s\n", curr_time, event->name);
                    }
                }
            }

            if(event->mask & IN_DELETE) 
            {
                printf("File : %s was deleted from Path\n", event->name);
                fprintf(fp, "%s - File : %s was deleted from Path\n", curr_time, event->name);
            }

            if(event->mask & IN_ACCESS)
            {
                printf ("The directory was read from!\n");
                fprintf (fp, "%s - The directory was read from!\n", curr_time);
            }


            /* update the index to the start of the next event */
            i += sizeof(struct inotify_event) + event->len;
            fclose(fp);
            notify(choices.notification);
        }
        len = 0;
        i = 0;
    }
    
}