#include "list_func.h"
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
    for(int idx = 1; idx < num_of_elem; idx++){
        list1.data[idx] = POISON;
        list1.next[idx] = (idx < num_of_elem - 1) ? idx + 1 : 0;
        list1.pred[idx] = -1;
    }
    list1.head = 0;
    list1.free = 1;
    list1.tail = 0;

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

    CHECK_AND_RET_MISTAKE(list->head < 0 || list->head >= list->num_of_elem, 
                        INCORR_HEAD, "INCORRECT HEAD VALUE\n");

    CHECK_AND_RET_MISTAKE(list->tail < 0 || list->tail >= list->num_of_elem, 
                        INCORR_TAIL, "INCORRECT TAIL VALUE\n");

    CHECK_AND_RET_MISTAKE(list->free < 0 || list->free >= list->num_of_elem,
                        INCORR_FREE, "INCORRECT FREE VALUE\n");

    CHECK_AND_RET_MISTAKE(list->data[0] != POISON, NULL_ELEM_CORRUPTED, "DATA[0] ELEMENT CORRUPTED\n");

    CHECK_AND_RET_MISTAKE(list->next[0] != 0 || list->pred[0] != 0, 
                        NULL_IDX_CORRUPTED, "list->next[0] != 0 or list->pred[0] != 0\n");

    DEBUG(
    if(list->head != 0 && list->tail != 0){
        err |= check_next(list);
        err |= check_pred(list);
    }
    )
    return err;

}

// возвращать больше ошибок
static list_err_t check_next(const list* list){
    int number_of_next = 0;
    for(int data_idx = 1; data_idx < list->num_of_elem; data_idx++){
        if(list->next[data_idx] < 0 || list->next[data_idx] >= list->num_of_elem){
            // здесь дамп с картинкой
            return INCORR_FILL_NEXT_ARRAY;
        }

        if(data_idx == list -> tail){
            if(list->next[list->tail] == 0){
                continue;
            }
            fprintf(stderr, "NEXT ELEMENT FOR TAIL ELEMENT IS NOT ZERO\n");
            // здесь дамп с картинкой
            return INCORR_FILL_NEXT_ARRAY;
        }

        if(list->data[data_idx] == POISON) {
            if(list->data[list->next[data_idx]] == POISON){
                continue;
            }
            fprintf(stderr, "For elem [%d] that is POISON next elem is not POISON\n", data_idx);
            // здесь дамп с картинкой
            return INCORR_FILL_NEXT_ARRAY;
        }

        number_of_next = 0;
        for(int next_idx = 1; next_idx < list->num_of_elem; next_idx++) {
            if(list->next[next_idx] == data_idx){
                number_of_next++;
            }
        }

        if(data_idx == list->head) {
            if(number_of_next == 0) {
                continue;
            }
            fprintf(stderr, "Head elem [%d] has %d previous elements\n", data_idx, number_of_next);
            // здесь дамп с картинкой
            return INCORR_FILL_NEXT_ARRAY;
        } 

        if(number_of_next != 1) {
            fprintf(stderr, "Elem [%d] has %d next elements (expected 1)\n", data_idx, number_of_next);
            // здесь дамп с картинкой
            return INCORR_FILL_NEXT_ARRAY;
        }
    }
    return NO_MISTAKE;
}

static list_err_t check_pred(const list* list){
    int number_of_prev = 0;
    for(int data_idx = 1; data_idx < list->num_of_elem; data_idx++){
        if((list->pred[data_idx] < 0 && list->pred[data_idx] != -1) || list->pred[data_idx] >= (int)list->num_of_elem){
            // здесь дамп с картинкой
            return INCORR_FILL_PRED_ARRAY;
        }

        if(data_idx == list -> head){
            if(list->pred[list->head] == 0){
                continue;
            }
            fprintf(stderr, "PREVIOUS ELEMENT FOR HEAD ELEMENT IS NOT ZERO\n");
            // здесь дамп с картинкой
            return INCORR_FILL_PRED_ARRAY;
        }

        if(list->data[data_idx] == POISON){
            if(list->pred[data_idx] == -1){
                continue;
            }
            fprintf(stderr, "For elem [%d] that is POISON previous elem is not -1\n", data_idx);
            // здесь дамп с картинкой
            return INCORR_FILL_PRED_ARRAY;
        }

        number_of_prev = 0;
        for(int prev_idx = 1; prev_idx < list->num_of_elem; prev_idx++) {
            if(list->pred[prev_idx] == data_idx){
                number_of_prev++;
            }
        }

        if(data_idx == list->tail) {
            if(number_of_prev == 0) {
                continue;
            }
            fprintf(stderr, "Tail elem [%d] has %d next elements\n", data_idx, number_of_prev);
            // здесь дамп с картинкой
            return INCORR_FILL_PRED_ARRAY;
        } 

        if(number_of_prev != 1) {
            fprintf(stderr, "Elem [%d] has %d previous elements (expected 1)\n", data_idx, number_of_prev);
            // здесь дамп с картинкой
            return INCORR_FILL_PRED_ARRAY;
        }
    }
    return NO_MISTAKE;
}

list_err_t add_elem(list* list, list_elem_t elem, int idx){
    list_err_t err = NO_MISTAKE;

    // мы же можем вставлять после последнего когда только тогда когда список полон
    if(idx < 0 && (idx >= list->num_of_elem && list->free != 0)){
        fprintf(stderr, "Incorrect index %d in add function\n", idx);
        return IDX_OUT_OF_ARR;
    }

    err = list_verify(list);
    if(err){
        return err;
    }

    // проверка что массив пуст --- тогда вставляем в начало
    if(list->head == 0 && list->tail == 0){
        list->data[1] = elem;
        list->pred[1] = 0;
        list->next[1] = 0;
        list->free = 2;
        list->tail = 1;
        list->head = 1; 
        err = list_verify(list);
        return err;
    }

    // проверка что весь массив полон 
    if(list->free == 0){
        err = list_realloc(list, list->num_of_elem * 2);
        if(err){
            return err;
        }
        list->free = idx + 1;
    }

    int new_free = 0;

    if (idx == 0) {
        new_free = list->next[idx + 1];
        list -> next[idx + 1] = list -> pred[list -> next[list -> head]];
        list -> pred[idx + 1] = 0;
        list->head = idx + 1;
        list -> pred[list -> next[idx+1]] = list -> head;
        list -> data [idx + 1] = elem;
        list->free = new_free;

        err = list_verify(list);
        return err;
    }

    new_free = list->next[list->free];
    list->data[list->free] = elem;
    list->next[list->free] = list->next[idx];
    list->next[idx] = list->free;
    if(list->next[list->free] == 0){
        list->pred[list->free] = idx;
        list->tail = list->free;
    }
    else{
        list->pred[list->free] = list->pred[list->next[list->free]];
        list->pred[list->next[list->free]] = list->free;
    }
    list->free = new_free;

    err = list_verify(list);
    return err;
} 

list_err_t del_elem(list* list, int idx){
    list_err_t err = NO_MISTAKE;

    if(idx <= 0 || idx >= list->num_of_elem){
        fprintf(stderr, "Incorrect index %d in delete function\n", idx);
        return IDX_OUT_OF_ARR;
    }

    err = list_verify(list);
    if(err){
        return err;
    }

    if(idx == 1){
        list->head = list->next[idx];
    }
    if(list->next[idx] == 0){
        list->tail = list->pred[idx];
    }

    if(list->next[idx] != 0){
        list->pred[list->next[idx]] = list -> pred[idx];
    }
    if(idx != 1){
        list->next[list->pred[idx]] = list -> next[idx];
    }
    list->next[idx] = list -> free;
    list->pred[idx] = -1;
    list->data[idx] = POISON;
    list -> free = idx;

    err = list_verify(list);
    return err;
}

static list_err_t list_realloc(list* list, size_t elem){
    size_t new_size = elem + 1;
    list->data = (list_elem_t*)realloc(list->data, sizeof(list_elem_t) * new_size);
    if(!list->data){
        fprintf(stderr, "Can't realloc data array to edd new elem\n");
        list_dtor(list);
        return DATA_ALLOC_ERR; 
    }
    list->next = (int*)realloc(list->next, sizeof(int) * new_size);
    if(!list->next){
        fprintf(stderr, "Can't realloc next array to edd new elem\n");
        list_dtor(list);
        return NEXT_ALLOC_ERR; 
    }
    list->pred = (int*)realloc(list->pred, sizeof(int) * new_size);
    if(!list->pred){
        fprintf(stderr, "Can't realloc pred array to edd new elem\n");
        list_dtor(list);
        return PRED_ALLOC_ERR; 
    }
    for(int idx = list->num_of_elem; idx < new_size; idx++){
        list->next[idx] = (idx < new_size - 1) ? idx + 1 : 0;
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