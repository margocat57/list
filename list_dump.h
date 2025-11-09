#ifndef LIST_DUMP_H
#define LIST_DUMP_H
#include <string.h>
#include <stdio.h>
#include "list_struct.h"
#include "classic_list.h"

struct filenames_for_dump{
    char* dot_filename;
    char* svg_filename;
};

void list_dump_func(const list* list, const char* debug_msg, const char *file, const char *func, int line, ...) __attribute__ ((format (printf, 2, 6)));

// void list_cl_dump_func(const list_classic_head* head, const char* debug_msg, const char *file, const char *func, int line);

#endif // LIST_DUMP_H