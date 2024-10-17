#ifndef __BUILD_JUST_H__
#define __BUILD_JUST_H__

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

// String -> instruction -> command and (argument -> flag and value) 
typedef const char* String;



int run_command();


#endif // __BUILD_JUST_H__


int run_command() {
    return system("ls -a");
}