//
// Created L1MeN9Yu on 2017/11/24.
//

#ifdef __cplusplus
extern "C"{
#endif

off_t gtr_get_file_size(const char *file_path);

void gtr_create_directory_if_not_exist(const char *directory_path);

/**
 * 获取iOS Application沙盒的tmp目录
 * @return temp路径
 */
char *get_app_temp_directory(void);

#ifdef __cplusplus
};
#endif
