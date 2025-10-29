#include "list_func.h"
#include "list_dump.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

static list_err_t list_realloc(list* list, size_t elem);

static list_err_t check_next(const list* list);

static list_err_t check_pred(const list* list);

list list_ctor(size_t num_of_elem){
    list list1 = {};
    if(num_of_elem >= (size_t)(0.8 * (double)SIZE_MAX)){
        fprintf(stderr, "INCORRECT list size\n");
        return {};
    }
    list1.data = (list_elem_t*)calloc(num_of_elem, sizeof(list_elem_t));
    if(!list1.data){
        fprintf(stderr, "Can't alloc memory for list.data array\n");
        return {};
    }

    list1.next =(int*)calloc(num_of_elem, sizeof(int));
    if(!list1.next){
        fprintf(stderr, "Can't alloc memory for list.next array\n");
        list_dtor(&list1);
        return {};
    }

    list1.pred =(int*)calloc(num_of_elem, sizeof(int));
    if(!list1.pred){
        fprintf(stderr, "Can't alloc memory for list.pred array\n");
        list_dtor(&list1);
        return {};
    }

    list1.data[0] = POISON;

    list1.next[0] = 0;
    list1.pred[0] = 0;
    for(int idx = 1; idx < (int)num_of_elem; idx++){
        list1.data[idx] = POISON;
        list1.next[idx] = (idx < (int)num_of_elem - 1) ? idx + 1 : 0;
        list1.pred[idx] = -1;
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

    CHECK_AND_RET_MISTAKE(!list->pred, NULL_PRED_PTR, "LIST PRED PTR IS NULL\n");

    CHECK_AND_RET_MISTAKE(list->num_of_elem == 0 || list->num_of_elem >= (size_t)(0.8 * (double)SIZE_MAX), 
                        INCORR_DATA_SIZE, "INCORRECT DATA SIZE\n");

    CHECK_AND_RET_MISTAKE(list->next[0] < 0 || list->next[0] >= (int)list->num_of_elem, 
                        INCORR_HEAD, "INCORRECT HEAD VALUE\n");

    CHECK_AND_RET_MISTAKE(list->pred[0] < 0 || list->pred[0] >= (int)list->num_of_elem, 
                        INCORR_TAIL, "INCORRECT TAIL VALUE\n");

    CHECK_AND_RET_MISTAKE(list->free < 0 || list->free >= (int)list->num_of_elem,
                        INCORR_FREE, "INCORRECT FREE VALUE\n");

    CHECK_AND_RET_MISTAKE(list->data[0] != POISON, NULL_ELEM_CORRUPTED, "DATA[0] ELEMENT CORRUPTED\n");

    DEBUG(
    if(list->next[0] != 0 && list->pred[0] != 0){
        err |= check_next(list);
        err |= check_pred(list);
    }
    )
    return err;

}

// возвращать больше ошибок
static list_err_t check_next(const list* list){
    int number_of_next = 0;
    for(int data_idx = 1; data_idx < (int)list->num_of_elem; data_idx++){
        if(list->next[data_idx] < 0 || list->next[data_idx] >= (int)list->num_of_elem){
            list_dump_func(list, "log.htm", "Next element index for data_idx element out of array", __FILE__, __PRETTY_FUNCTION__, __LINE__);
            return INCORR_FILL_NEXT_ARRAY;
        }

        if(data_idx == list->pred[0]){
            if(list->next[data_idx] == 0){
                continue;
            }
            fprintf(stderr, "NEXT ELEMENT FOR TAIL ELEMENT IS NOT ZERO\n");
            list_dump_func(list, "log.htm", "NEXT ELEMENT FOR TAIL ELEMENT IS NOT ZERO", __FILE__, __PRETTY_FUNCTION__, __LINE__);
            return INCORR_FILL_NEXT_ARRAY;
        }

        if(list->data[data_idx] == POISON) {
            if(list->data[list->next[data_idx]] == POISON){
                continue;
            }
            fprintf(stderr, "For elem [%d] that is POISON next elem is not POISON\n", data_idx);
            list_dump_func(list, "log.htm", "For POISON element next elem is not POISON", __FILE__, __PRETTY_FUNCTION__, __LINE__);
            return INCORR_FILL_NEXT_ARRAY;
        }

        number_of_next = 0;
        for(int next_idx = 1; next_idx < (int)list->num_of_elem; next_idx++) {
            if(list->next[next_idx] == data_idx){
                number_of_next++;
            }
        }

        if(data_idx == list->next[0]) {
            if(number_of_next == 0) {
                continue;
            }
            fprintf(stderr, "Head elem [%d] has %d previous elements\n", data_idx, number_of_next);
            list_dump_func(list, "log.htm", "Head element has not zero previous elements", __FILE__, __PRETTY_FUNCTION__, __LINE__);
            return INCORR_FILL_NEXT_ARRAY;
        } 

        if(number_of_next != 1) {
            fprintf(stderr, "Elem [%d] has %d next elements (expected 1)\n", data_idx, number_of_next);
            list_dump_func(list, "log.htm", "One of the elements has more than one next elements", __FILE__, __PRETTY_FUNCTION__, __LINE__);
            return INCORR_FILL_NEXT_ARRAY;
        }
    }
    return NO_MISTAKE;
}

static list_err_t check_pred(const list* list){
    int number_of_prev = 0;
    for(int data_idx = 1; data_idx < (int)list->num_of_elem; data_idx++){
        if((list->pred[data_idx] < 0 && list->pred[data_idx] != -1) || list->pred[data_idx] >= (int)list->num_of_elem){
            list_dump_func(list, "log.htm", "Prev element index for data_idx element out of array", __FILE__, __PRETTY_FUNCTION__, __LINE__);
            return INCORR_FILL_PRED_ARRAY;
        }

        if(data_idx == list -> next[0]){
            if(list->pred[data_idx] == 0){
                continue;
            }
            fprintf(stderr, "PREVIOUS ELEMENT FOR HEAD ELEMENT IS NOT ZERO\n");
            list_dump_func(list, "log.htm", "PREVIOUS ELEMENT FOR HEAD ELEMENT IS NOT ZERO", __FILE__, __PRETTY_FUNCTION__, __LINE__);
            return INCORR_FILL_PRED_ARRAY;
        }

        if(list->data[data_idx] == POISON){
            if(list->pred[data_idx] == -1){
                continue;
            }
            fprintf(stderr, "For elem [%d] that is POISON previous elem is not -1\n", data_idx);
            list_dump_func(list, "log.htm", "For POISON element previous elem is not -1", __FILE__, __PRETTY_FUNCTION__, __LINE__);
            return INCORR_FILL_PRED_ARRAY;
        }

        number_of_prev = 0;
        for(int prev_idx = 1; prev_idx < (int)list->num_of_elem; prev_idx++) {
            if(list->pred[prev_idx] == data_idx){
                number_of_prev++;
            }
        }

        if(data_idx == list->pred[0]) {
            if(number_of_prev == 0) {
                continue;
            }
            fprintf(stderr, "Tail elem [%d] has %d next elements\n", data_idx, number_of_prev);
            list_dump_func(list, "log.htm", "Tail elem has more than one next elements", __FILE__, __PRETTY_FUNCTION__, __LINE__);
            return INCORR_FILL_PRED_ARRAY;
        } 

        if(number_of_prev != 1) {
            fprintf(stderr, "Elem [%d] has %d previous elements (expected 1)\n", data_idx, number_of_prev);
            list_dump_func(list, "log.htm", "Element has more than one previous elements", __FILE__, __PRETTY_FUNCTION__, __LINE__);
            return INCORR_FILL_PRED_ARRAY;
        }
    }
    return NO_MISTAKE;
}

list_err_t add_elem_before_idx(list* list, list_elem_t elem, int idx){
    list_err_t err = NO_MISTAKE;

    err = list_verify(list);
    if(err) return err;

    if(idx <= 0 && (idx >= (int)list->num_of_elem && list->free != 0)){
        fprintf(stderr, "Incorrect index %d in add function\n", idx);
        return IDX_OUT_OF_ARR;
    }

    if(list->free == 0){
        size_t old_num_of_elem = list->num_of_elem;
        err = list_realloc(list, list->num_of_elem * 2);
        if(err) return err;
        list->free = old_num_of_elem;
    }

    int new_free = 0;
    new_free = list->next[list->free];

    list->data[list->free] = elem;
    if(list->pred[idx] == -1 || idx == list->num_of_elem){
        list->pred[list->free] = list->pred[0];
    }
    else{
        list->pred[list->free] = list->pred[idx];
    }
    list->pred[list->next[list->pred[list->free]]] = list -> free;
    list->next[list->free] = list->next[list->pred[list->free]];
    list->next[list->pred[list->free]] = list->free;

    list->free = new_free;

    err = list_verify(list);
    return err;
} 

list_err_t add_elem_after_idx(list* list, list_elem_t elem, int idx){
    list_err_t err = NO_MISTAKE;

    err = list_verify(list);
    if(err) return err;

    if(idx < 0 && (idx >= (int)list->num_of_elem && list->free != 0)){
        fprintf(stderr, "Incorrect index %d in add function\n", idx);
        return IDX_OUT_OF_ARR;
    }

    if(list->free == 0){
        err = list_realloc(list, list->num_of_elem * 2);
        if(err) return err;
        list->free = idx + 1;
    }

    int new_free = 0;
    new_free = list->next[list->free];

    list->data[list->free] = elem;
    list->next[list->free] = list->next[idx];
    list->next[idx] = list->free;
    list->pred[list->free] = list->pred[list->next[list->free]];
    list->pred[list->next[list->free]] = list->free;

    list->free = new_free;

    err = list_verify(list);
    return err;
} 

list_err_t del_elem(list* list, int idx){
    list_err_t err = NO_MISTAKE;

    err = list_verify(list);
    if(err) return err;

    if(idx <= 0 || idx >= (int)list->num_of_elem){
        fprintf(stderr, "Incorrect index %d in delete function\n", idx);
        return IDX_OUT_OF_ARR;
    }

    list->pred[list->next[idx]] = list -> pred[idx];
    list->next[list->pred[idx]] = list -> next[idx];
    list->next[idx] = list -> free;
    list->pred[idx] = -1;
    list->data[idx] = POISON;
    list -> free = idx;

    err = list_verify(list);
    return err;
}

static list_err_t list_realloc(list* list, size_t elem){
    size_t new_size = elem + 1;

    list_elem_t* list_data_new = (list_elem_t*)realloc(list->data, sizeof(list_elem_t) * new_size);
    if(!list_data_new){
        fprintf(stderr, "Can't realloc data array to edd new elem\n");
        return DATA_ALLOC_ERR; 
    }
    list->data = list_data_new;

    int* list_next_new = (int*)realloc(list->next, sizeof(int) * new_size);
    if(!list_next_new){
        fprintf(stderr, "Can't realloc next array to edd new elem\n");
        return NEXT_ALLOC_ERR; 
    }
    list->next = list_next_new;

    int* list_pred_new = (int*)realloc(list->pred, sizeof(int) * new_size);
    if(!list_pred_new){
        fprintf(stderr, "Can't realloc pred array to edd new elem\n");
        list_dtor(list);
        return PRED_ALLOC_ERR; 
    }
    list->pred = list_pred_new;

    for(int idx = (int)list->num_of_elem; idx < (int)new_size; idx++){
        list->next[idx] = (idx < (int)new_size - 1) ? idx + 1 : 0;
        list->pred[idx] = -1;
        list->data[idx] = POISON;
    }
    list->num_of_elem = new_size;

    return NO_MISTAKE;
}


void list_dtor(list* list){
    if(!list){
        fprintf(stderr, "LIST ptr is null - list cannot be free\n");
        return;
    }

    if(list->data){
        memset(list->data, 0, list->num_of_elem * sizeof(list_elem_t));
        free(list->data);
        list->data = NULL;
    }

    if(list->next){
        memset(list->next, 0, list->num_of_elem * sizeof(int));
        free(list->next);
        list->next = NULL;
    }

    if(list->pred){
        memset(list->pred, 0, list->num_of_elem * sizeof(int));
        free(list->pred);
        list->pred = NULL;
    }
}