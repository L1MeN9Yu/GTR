//
//  mcf_ios_utility.c
//
//  Created L1MeN9Yu on 2017/12/13.

#include "mcf_ios_utility.h"
#include <stdlib.h>
#include <string.h>

char *get_app_temp_directory(void) {
    char *env_temp_dir = getenv("TMPDIR");
    const char *temp_dir_name = "/mcf_temp";
    char * temp_dir = malloc(sizeof(char) * (strlen(env_temp_dir) + strlen(temp_dir_name) + 1));
    strcpy(temp_dir, env_temp_dir);
    strcat(temp_dir, temp_dir_name);
    return temp_dir;
}