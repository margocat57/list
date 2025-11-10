#ifndef LIST_FUNC_H
#define LIST_FUNC_H
#include <string.h>
#include "list_struct.h"

#ifdef _DEBUG
    #define DEBUG(...) __VA_ARGS__
#else 
    #define DEBUG(...)
#endif //_DEBUG

list list_ctor(ssize_t num_of_elem);

list_err_t list_verify(const list* list);

list_err_t add_elem_before_idx(list* list, list_elem_t elem, ssize_t idx);

list_err_t add_elem_after_idx(list* list, list_elem_t elem, ssize_t idx);

list_elem_t return_by_phys_idx(const list* list1, size_t idx);

list_err_t list_linearize(list* list1);

list_err_t del_elem(list* list, ssize_t idx);

void list_dtor(list* list);

#endif // LIST_FUNC_H