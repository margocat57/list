#ifndef LIST_STRUCT_CLASSIC_H
#define LIST_STRUCT_CLASSIC_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "list_struct.h"

const int POISON_CL = -6666;

struct list_classic{
    list_elem_t data_cl;
    list_classic* next_cl; 
    list_classic* prev_cl;
};

struct list_classic_head{
    list_classic* root;
    size_t capacity;
};

enum node_mistakes{
    NO_MISTAKE_CL             = 0,
    NULL_NODE_PTR_CL          = 1 << 1,
    NULL_NEXT_PTR_CL          = 1 << 2,
    NULL_PRED_PTR_CL          = 1 << 3,
    INCORR_DATA_SIZE_CL       = 1 << 4,
    INCORR_HEAD_CL            = 1 << 5,
    INCORR_NEXT_CL            = 1 << 6,
    INCORR_PREV_CL            = 1 << 7,
    NULL_ELEM_CORRUPTED_CL    = 1 << 8
};

list_classic_head* classic_list_ctor();

list_classic* node_ctor(list_elem_t data, list_classic* prev, list_classic* next);

void node_dtor(list_classic* node, list_classic_head* head);

list_err_t list_dtor(list_classic_head* head);

list_err_t list_add_elem_before_node(list_elem_t data, list_classic* node, list_classic_head* head);

list_err_t list_add_elem_after_node(list_elem_t data, list_classic* node, list_classic_head* head);

list_err_t list_verify_cl(const list_classic* node, list_classic_head* head);

#endif // LIST_STRUCT_CLASSIC_H