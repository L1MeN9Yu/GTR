//
// Created L1MeN9Yu on 2017/11/24.
//

#include <sys/errno.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "gtr_file_utilty.h"

off_t gtr_get_file_size(const char *file_path) {
    struct stat st;

    if (stat(file_path, &st) == 0) {
        return st.st_size;
    }

    printf("Cannot determine size of %s: %s\n", file_path, strerror(errno));

    return 0;
}

void gtr_create_directory_if_not_exist(const char *directory_path) {
    struct stat st = {0};

    if (stat(directory_path, &st) == -1) {
        mkdir(directory_path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    }
}

char *get_app_temp_directory(void) {
    char *env_temp_dir = getenv("TMPDIR");
    if (!env_temp_dir) {
        env_temp_dir = P_tmpdir;
    }
    const char *temp_dir_name = "/gtr_temp";
    char *temp_dir = malloc(sizeof(char) * (strlen(env_temp_dir) + strlen(temp_dir_name) + 1));
    strcpy(temp_dir, env_temp_dir);
    strcat(temp_dir, temp_dir_name);
    return temp_dir;
}

