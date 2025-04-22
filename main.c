/* To Do
    - Add support to monitor multiple directories ?
    - Trigger shell commands on specific events ?
    - Run as a daemon process (background service)
    - Maybe make a graphical interface ?
    - Add sig to terminate program
    - Can we log which process did what ?
*/

#include "watcher.h"

int main(int argc, char *argv[]){
    Preferences choices;
    
    choices = setup();
    monitor(choices);
    
    
    
    
    free(choices.path);
    return 0;
}