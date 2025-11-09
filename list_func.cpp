#include "list_func.h"
#include "list_dump.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#define CHECK_AND_RET_MISTAKE(bad_condition, err_code, msg)\
    if(bad_condition){ \
        err = err_code; \
        fprintf(stderr, msg); \
        return err; \
    } \

static list_err_t list_realloc(list* list, ssize_t elem);

static list_err_t check_size_next(const list* list);

static list_err_t check_size_prev(const list* list);

static list_err_t check_correct_order_next_prev(const list* list);

void list_elem_dtor(void* ptr, size_t len_bytes);

list list_ctor(ssize_t num_of_elem){
    list list1 = {};
    if(num_of_elem < 0){
        fprintf(stderr, "INCORRECT list size\n");
        return {};
    }
    list1.data = (list_elem_t*)calloc((size_t)num_of_elem, sizeof(list_elem_t));
    if(!list1.data){
        fprintf(stderr, "Can't alloc memory for list.data array\n");
        return {};
    }

    list1.next =(ssize_t*)calloc((size_t)num_of_elem, sizeof(ssize_t));
    if(!list1.next){
        fprintf(stderr, "Can't alloc memory for list.next array\n");
        list_dtor(&list1);
        return {};
    }

    list1.prev =(ssize_t*)calloc((size_t)num_of_elem, sizeof(ssize_t));
    if(!list1.prev){
        fprintf(stderr, "Can't alloc memory for list.pred array\n");
        list_dtor(&list1);
        return {};
    }

    list1.data[0] = POISON;
    list1.size = 0;

    list1.next[0] = 0;
    list1.prev[0] = 0;
    for(ssize_t idx = 1; idx < num_of_elem; idx++){
        list1.data[idx] = POISON;
        list1.next[idx] = (idx < num_of_elem - 1) ? idx + 1 : 0;
        list1.prev[idx] = -1;
    }
    list1.free = 1;

    list1.num_of_elem = num_of_elem;

    return list1;
}

list_err_t list_verify(const list* list){
    list_err_t err = NO_MISTAKE;

    CHECK_AND_RET_MISTAKE(!list, NULL_LIST_PTR, "LIST PTR IS NULL\n");

    CHECK_AND_RET_MISTAKE(!list->data, NULL_DATA_PTR, "LIST DATA PTR IS NULL\n");

    CHECK_AND_RET_MISTAKE(!list->next, NULL_NEXT_PTR, "LIST NEXT PTR IS NULL\n");

    CHECK_AND_RET_MISTAKE(!list->prev, NULL_PRED_PTR, "LIST PRED PTR IS NULL\n");

    CHECK_AND_RET_MISTAKE(list->num_of_elem <= 0, INCORR_DATA_SIZE, "INCORRECT DATA SIZE\n");

    CHECK_AND_RET_MISTAKE(list->next[0] < 0 || list->next[0] >= list->num_of_elem, 
                        INCORR_HEAD, "INCORRECT HEAD VALUE\n");

    CHECK_AND_RET_MISTAKE(list->prev[0] < 0 || list->prev[0] >= list->num_of_elem, 
                        INCORR_TAIL, "INCORRECT TAIL VALUE\n");

    CHECK_AND_RET_MISTAKE(list->free < 0 || list->free >= list->num_of_elem,
                        INCORR_FREE, "INCORRECT FREE VALUE\n");

    CHECK_AND_RET_MISTAKE(list->data[0] != POISON, 
                        NULL_ELEM_CORRUPTED, "DATA[0] ELEMENT CORRUPTED\n");

    CHECK_AND_RET_MISTAKE(list->data[list->free] != POISON, 
                        FREE_ELEM_ISNT_POISON, "FREE ELEMENT ISN'T POISON\n");

    DEBUG(
    if(list->next[0] != 0 && list->prev[0] != 0){
        err |= check_correct_order_next_prev(list);
        err |= check_size_next(list);
        err |= check_size_prev(list);
    }
    )
    return err;

}

static list_err_t check_size_next(const list* list){
    ssize_t data_idx = 0;
    ssize_t size_dbg = 0;
    while(list->next[data_idx] != 0){
        size_dbg++;
        data_idx = list->next[data_idx];
    }
    if(size_dbg != list->size){
        fprintf(stderr, "Incorrect size counted by next array expexted %zd, got %zd\n", list->size, size_dbg);
        list_dump_func(list, "Incorrect size counted by next array", __FILE__, __PRETTY_FUNCTION__, __LINE__);
        return INCORR_NUM_OF_NPOISON_EL;
    }
    return NO_MISTAKE;
}

static list_err_t check_size_prev(const list* list){
    ssize_t data_idx = 0;
    ssize_t size_dbg = 0;
    while(list->prev[data_idx] != 0){
        size_dbg++;
        data_idx = list->prev[data_idx];
    }

    if(size_dbg != list->size){
        fprintf(stderr, "Incorrect size counted by prev array expexted %zd, got %zd\n", list->size, size_dbg);
        list_dump_func(list, "Incorrect size counted by prev array", __FILE__, __PRETTY_FUNCTION__, __LINE__);
        return INCORR_NUM_OF_NPOISON_EL;
    }
    return NO_MISTAKE;
}

static list_err_t check_correct_order_next_prev(const list* list){
    int number_of_next = 0;
    for(ssize_t data_idx = 1; data_idx < list->num_of_elem; data_idx++){
        if(list->next[data_idx] < 0 || list->next[data_idx] >= list->num_of_elem){
            fprintf(stderr, "Next element index for data_idx element out of array\n");
            list_dump_func(list, "Next element index for data_idx element out of array", __FILE__, __PRETTY_FUNCTION__, __LINE__);
            return INCORR_FILL_NEXT_PREV_ARR;
        }

        if((list->prev[data_idx] < 0 && list->prev[data_idx] != -1) || list->prev[data_idx] >= list->num_of_elem){
            fprintf(stderr, "Prev element index for data_idx element out of array\n");
            list_dump_func(list, "Prev element index for data_idx element out of array", __FILE__, __PRETTY_FUNCTION__, __LINE__);
            return INCORR_FILL_NEXT_PREV_ARR;
        }

        if(data_idx == list->prev[0]){
            if(list->next[data_idx] == 0){
                continue;
            }
            fprintf(stderr, "NEXT ELEMENT FOR TAIL ELEMENT IS NOT ZERO\n");
            list_dump_func(list, "NEXT ELEMENT FOR TAIL ELEMENT IS NOT ZERO", __FILE__, __PRETTY_FUNCTION__, __LINE__);
            return INCORR_FILL_NEXT_PREV_ARR;
        }

        if(data_idx == list -> next[0]){
            if(list->prev[data_idx] == 0){
                continue;
            }
            fprintf(stderr, "PREVIOUS ELEMENT FOR HEAD ELEMENT IS NOT ZERO\n");
            list_dump_func(list, "PREVIOUS ELEMENT FOR HEAD ELEMENT IS NOT ZERO", __FILE__, __PRETTY_FUNCTION__, __LINE__);
            return INCORR_FILL_NEXT_PREV_ARR;
        }

        if(list->data[data_idx] == POISON){
            if(list->prev[data_idx] == -1){
                continue;
            }
            fprintf(stderr, "PREVIOUS ELEMENT FOR POISON IS NOT -1\n");
            list_dump_func(list, "PREVIOUS ELEMENT FOR POISON IS NOT -1", __FILE__, __PRETTY_FUNCTION__, __LINE__);
            return INCORR_FILL_NEXT_PREV_ARR;
        }

        if(list->prev[list->next[data_idx]] != data_idx ){
            fprintf(stderr, "Previous for next element(%zd) for element(%zd) != element, but = %zd",  list->next[data_idx], data_idx, list->prev[list->next[data_idx]]);
            list_dump_func(list, "INCORRECT ORDER OF ELEMENTS", __FILE__, __PRETTY_FUNCTION__, __LINE__);
            return INCORR_FILL_NEXT_PREV_ARR;
        }

        if(list->next[list->prev[data_idx]] != data_idx ){
            fprintf(stderr, "Next for previous element(%zd) for element(%zd) != element, but = %zd)", list->prev[data_idx], data_idx, list->next[list->prev[data_idx]]);
            list_dump_func(list, "INCORRECT ORDER OF ELEMENTS", __FILE__, __PRETTY_FUNCTION__, __LINE__);
            return INCORR_FILL_NEXT_PREV_ARR;
        }

    }
    return NO_MISTAKE;
}


list_err_t add_elem_before_idx(list* list, list_elem_t elem, ssize_t idx){
    list_err_t err = NO_MISTAKE;

    err = list_verify(list);
    if(err) return err;

    if(idx < 0 || (idx >= list->num_of_elem && list->free != 0)){
        fprintf(stderr, "Incorrect index %zd in add function\n", idx);
        list_dump_func(list, "INCORRECT INDEX IN ADD FUNC", __FILE__, __PRETTY_FUNCTION__, __LINE__);
        return IDX_OUT_OF_ARR;
    }

    if(list->free == 0){
        ssize_t old_num_of_elem = list->num_of_elem;
        err = list_realloc(list, list->num_of_elem * 2);
        if(err) return err;
        list->free = old_num_of_elem;
    }

    ssize_t new_free = 0;
    new_free = list->next[list->free];

    list->data[list->free] = elem;
    // вставить в хвост не умеет делать - точнее умеет вставлять перед 0 элементом
    list->prev[list->free] = list->prev[idx];
    list->prev[list->next[list->prev[list->free]]] = list -> free;
    list->next[list->free] = list->next[list->prev[list->free]];
    list->next[list->prev[list->free]] = list->free;
    list->size++;

    list->free = new_free;

    err = list_verify(list);
    return err;
} 

list_err_t add_elem_after_idx(list* list, list_elem_t elem, ssize_t idx){
    list_err_t err = NO_MISTAKE;

    err = list_verify(list);
    if(err) return err;

    if(idx < 0 || (idx >= list->num_of_elem && list->free != 0)){
        fprintf(stderr, "Incorrect index %zd in add function\n", idx);
        list_dump_func(list, "INCORRECT INDEX IN ADD FUNC", __FILE__, __PRETTY_FUNCTION__, __LINE__);
        return IDX_OUT_OF_ARR;
    }

    if(list->free == 0){
        err = list_realloc(list, list->num_of_elem * 2);
        if(err) return err;
        list->free = idx + 1;
    }

    ssize_t new_free = 0;
    new_free = list->next[list->free];

    list->data[list->free] = elem;
    list->next[list->free] = list->next[idx];
    list->next[idx] = list->free;
    list->prev[list->free] = list->prev[list->next[list->free]];
    list->prev[list->next[list->free]] = list->free;
    list->size++;

    list->free = new_free;

    err = list_verify(list);
    return err;
} 

list_err_t del_elem(list* list, ssize_t idx){
    list_err_t err = NO_MISTAKE;

    err = list_verify(list);
    if(err) return err;

    if(idx <= 0 || idx >= list->num_of_elem){
        fprintf(stderr, "Incorrect index %zd in delete function\n", idx);
        list_dump_func(list, "INCORRECT INDEX IN DEL FUNC", __FILE__, __PRETTY_FUNCTION__, __LINE__);
        return IDX_OUT_OF_ARR;
    }

    list->prev[list->next[idx]] = list -> prev[idx];
    list->next[list->prev[idx]] = list -> next[idx];
    list->next[idx] = list -> free;
    list->prev[idx] = -1;
    list->data[idx] = POISON;
    list -> free = idx;
    list->size--;

    err = list_verify(list);
    return err;
}

static list_err_t list_realloc(list* list, ssize_t elem){
    ssize_t new_size = elem + 1;

    list_elem_t* list_data_new = (list_elem_t*)realloc(list->data, sizeof(list_elem_t) * (size_t)new_size);
    if(!list_data_new){
        fprintf(stderr, "Can't realloc data array to edd new elem\n");
        return DATA_ALLOC_ERR; 
    }
    list->data = list_data_new;

    ssize_t* list_next_new = (ssize_t*)realloc(list->next, sizeof(ssize_t) * (size_t)new_size);
    if(!list_next_new){
        fprintf(stderr, "Can't realloc next array to edd new elem\n");
        return NEXT_ALLOC_ERR; 
    }
    list->next = list_next_new;

    ssize_t* list_prev_new = (ssize_t*)realloc(list->prev, sizeof(ssize_t) * (size_t)new_size);
    if(!list_prev_new){
        fprintf(stderr, "Can't realloc pred array to edd new elem\n");
        list_dtor(list);
        return PRED_ALLOC_ERR; 
    }
    list->prev = list_prev_new;

    for(ssize_t idx = list->num_of_elem; idx < new_size; idx++){
        list->next[idx] = (idx < new_size - 1) ? idx + 1 : 0;
        list->prev[idx] = -1;
        list->data[idx] = POISON;
    }
    list->num_of_elem = new_size;

    return NO_MISTAKE;
}

list_elem_t return_by_phys_idx(const list* list1, size_t idx){
    if(idx == 0 || idx > list1->num_of_elem){
        fprintf(stderr, "Index is not correct - can't work");
        return POISON;
    }
    return list1->data[idx];
}

list_err_t list_linearize(list* list1){
    list_err_t err = NO_MISTAKE;

    err = list_verify(list1);
    if(err) return err;

    if(list1->size == 0){
        list1->data[0]=POISON;
        list1->next[0]=0;
        list1->prev[0]=0;
        return err;
    }

    list list_new = {};
    if(list1->size < list1->num_of_elem / 4){
        list list_new = list_ctor(list1->num_of_elem / 2);
    }
    else{
        list list_new = list_ctor(list1->num_of_elem);
    }
    

    ssize_t idx = list1->next[0];
    list_new.data[0] = POISON;
    for(ssize_t count = 1; count < list1->size + 1; count++){
        if(idx != 0){
            list_new.data[count] = list1->data[idx];
        }
        if(idx == 0){
            list_new.data[count] = list1->data[list1->prev[0]];
        }
        idx = list1->next[idx];
    }

    for(ssize_t count = list1->size + 1; count < list1->num_of_elem; count++){
        list_new.data[count] = POISON;
    }

    list1->free = list1->size + 1;
    list_new.prev[0] = list1->size;
    for(ssize_t idx1 = 0; idx1 < list1->num_of_elem; idx1++){
        if(idx1 == list_new.prev[0] || idx1 == list1->num_of_elem - 1){
            list_new.next[idx1] = 0;
        }
        else{
            list_new.next[idx1] = idx1 + 1;
        }
    }

    for(ssize_t idx2 = 1; idx2 < list1->num_of_elem; idx2++){
        if(idx2 > list1->size){
            list_new.prev[idx2] = -1;
        }
        else{
            list_new.prev[idx2] = idx2 - 1;
        }
    }

    list_dtor(list1);

    list1->data = list_new.data;
    list1->next = list_new.next;
    list1->prev = list_new.prev;

    err = list_verify(list1);
    return err;
}

void list_elem_dtor(void* ptr, size_t len_bytes){
    if(ptr){
        memset(ptr, 0, len_bytes);
        free(ptr);
        ptr = NULL;
    }
}


void list_dtor(list* list){
    if(!list){
        fprintf(stderr, "LIST ptr is null - list cannot be free\n");
        return;
    }

    list_elem_dtor(list->data, (size_t)list->num_of_elem * sizeof(list_elem_t));

    list_elem_dtor(list->next, (size_t)list->num_of_elem * sizeof(ssize_t));

    list_elem_dtor(list->prev, (size_t)list->num_of_elem * sizeof(ssize_t));
}