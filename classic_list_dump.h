#ifndef CL_LIST_DUMP_H
#define CL_LIST_DUMP_H
#include <string.h>
#include <stdio.h>
#include "list_struct.h"
#include "classic_list.h"

struct filenames_for_dump_cl{
    char* dot_filename_cl;
    char* svg_filename_cl;
};

void list_dump_func_cl(list_classic_head* head, const char* debug_msg, const char *file, const char *func, int line, ...) __attribute__ ((format (printf, 2, 6)));

#endif // CL_LIST_DUMP_H