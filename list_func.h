#ifndef LIST_FUNC_H
#define LIST_FUNC_H
#include <string.h>
#include "list_struct.h"

#ifdef _DEBUG
    #define DEBUG(...) __VA_ARGS__
#else 
    #define DEBUG(...)
#endif //_DEBUG

#define CHECK_AND_RET_MISTAKE(bad_condition, err_code, msg)\
    if(bad_condition){ \
        err = err_code; \
        fprintf(stderr, msg); \
        return err; \
    } \

list list_ctor(size_t num_of_elem);

list_err_t list_verify(const list* list);

list_err_t add_elem(list* list, list_elem_t elem, int idx);

list_err_t del_elem(list* list, int idx);

void list_dtor(list* list);

#endif // LIST_FUNC_H