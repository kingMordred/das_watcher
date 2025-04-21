#include "watcher.h"

char*  setup(){
    char answer='\0';
    int retry = 0;
    char *path;
    int path_len = 0;

    while(retry < 3){
        printf("Create a new path for monitoring ? (y/n)\n");
        scanf(" %c", &answer);
        //printf("%c", answer);
        retry++;

        if(answer == 'y' || answer == 'n') break;
        if(retry == 3){
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