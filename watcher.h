#ifndef WATCHER_H
#define WATCHER_H

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/inotify.h>
#include <limits.h>
#include <unistd.h>

#define BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))

char* setup();
int monitor(const char*);



#endif