#ifndef LIST_STRUCT_H
#define LIST_STRUCT_H
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

const char* const LOG_FILE = "log.htm";

typedef double list_elem_t;

typedef uint64_t list_err_t;

enum list_mistakes{
    NO_MISTAKE                = 0,
    DATA_ALLOC_ERR            = 1 << 0,
    NEXT_ALLOC_ERR            = 1 << 1,
    PRED_ALLOC_ERR            = 1 << 2,
    IDX_OUT_OF_ARR            = 1 << 3,
    NULL_LIST_PTR             = 1 << 4,
    NULL_DATA_PTR             = 1 << 5,
    NULL_NEXT_PTR             = 1 << 6,
    NULL_PRED_PTR             = 1 << 7,
    INCORR_DATA_SIZE          = 1 << 8,
    INCORR_HEAD               = 1 << 9,
    INCORR_TAIL               = 1 << 10,
    INCORR_FREE               = 1 << 11,
    NULL_ELEM_CORRUPTED       = 1 << 12,
    INCORR_FILL_NEXT_PREV_ARR = 1 << 13,
    FREE_ELEM_ISNT_POISON     = 1 << 14,
    INCORR_LENGTH_LIST        = 1 << 15,
    INCORR_NUM_OF_NPOISON_EL  = 1 << 16
};

const list_elem_t POISON = -6666;

struct list{
    list_elem_t* data;
    ssize_t* next; 
    ssize_t* prev;
    ssize_t free;
    ssize_t size;
    ssize_t num_of_elem;
};

#endif // LIST_STRUCT_H