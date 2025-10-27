#ifndef LIST_DUMP_H
#define LIST_DUMP_H
#include <string.h>
#include "list_struct.h"

void gerenate_dot_png_file(list* list, const char* name_file);

void list_dump_html(list* list, const char* file_name, const char* debug_msg);

#endif // LIST_DUMP_H