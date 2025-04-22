/* To Do
    - Add support to monitor multiple directories
    - Write logs to a file
    - Add timestamp to logs
    - Trigger shell commands on specific events
    - Run as a daemon process (background service)
    - Maybe make a graphical interface
    - Add some sort of notification system
    - Add sig to terminate program
*/

#include "watcher.h"

int main(int argc, char *argv[]){
    char *path;
    
    path = setup();
    monitor(path);
    
    
    
    
    free(path);
    return 0;
}