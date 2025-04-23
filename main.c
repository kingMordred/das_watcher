/* To Do
    - Add support to monitor multiple directories ?
    - Trigger shell commands on specific events ?
    - Maybe make a graphical interface ?
    - Add sig to terminate program and cleanup
    - Can we log which process did what ?
*/

#include "watcher.h"

int main(int argc, char *argv[]){
    Preferences choices;
    pid_t pid;
    struct rlimit rl;

    choices = setup();
    
    //daemonizing the process; 
    /*Love - Linux System Programming (O'Reilly, 2007) - Page 159*/
    //create new process
    pid = fork();
    if(pid == -1) return -1;
    else if(pid != 0) exit(EXIT_SUCCESS);

    //create new session & process group
    if(setsid() == -1) return -1;

    //set working dir to root
    //if(chdir("/") == -1) return -1;

    //close all open files
    getrlimit(RLIMIT_NOFILE, &rl);
    for(int i = 0; i<rl.rlim_cur; i++) close(i);

    //redirect stdin, stdout, stderr to /dev/null
    open("/dev/null", O_RDWR);
    dup(0);
    dup(0);

    //we can use int daemon (int nochdir, int noclose); instead

    monitor(choices);
    
    
    free(choices.path);
    return 0;
}