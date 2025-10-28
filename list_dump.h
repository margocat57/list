#ifndef LIST_DUMP_H
#define LIST_DUMP_H
#include <string.h>
#include "list_struct.h"

void list_dump_func(const list* list, const char* output_file, const char* debug_msg, const char *file, const char *func, int line);

#endif // LIST_DUMP_H