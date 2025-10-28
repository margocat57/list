#ifndef LIST_DUMP_H
#define LIST_DUMP_H
#include <string.h>
#include "list_struct.h"

#define CHECK_AND_RET_DUMP(bad_condition, msg)\
    if(bad_condition){ \
        fprintf(stderr, msg); \
        return; \
    } \

struct filenames_for_dump{
    char* dot_filename;
    char* svg_filename;
};

void list_dump_func(const list* list, const char* output_file, const char* debug_msg, const char *file, const char *func, int line);

#endif // LIST_DUMP_H