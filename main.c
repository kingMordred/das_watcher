/* To Do
    - Add support to monitor multiple directories
    - Write logs to a file
    - Add timestamp to logs
    - Trigger shell commands on specific events
    - Run as a daemon process (background service)
    - Maybe make a graphical interface
    - Add some sort of notification system
*/

#include "watcher.h"

int main(int argc, char *argv[]){
    char *path;
    
    path = setup();
    
    
    
    
    free(path);
    return 0;
}