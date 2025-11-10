#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "classic_list.h"

#define CHECK_AND_RET_MISTAKE_CL(bad_condition, err_code, msg)\
    if(bad_condition){ \
        err = err_code; \
        fprintf(stderr, msg); \
        return err; \
    } \


list_classic_head* classic_list_ctor(){
    list_classic_head* head = (list_classic_head*)calloc(1, sizeof(list_classic_head));
    head->root = node_ctor(POISON_CL, NULL, NULL);
    head->capacity = 1;

    return head;
}

list_classic* node_ctor(list_elem_t data, list_classic* prev, list_classic* next){
    list_classic* node = (list_classic*)calloc(1, sizeof(list_classic));
    if(!node){
        fprintf(stderr, "Can't alloc data for node");
        return NULL;
    }

    node->data_cl = data;
    node->prev_cl = prev;
    node->next_cl = next;

    return node;
}

void node_dtor(list_classic* node, list_classic_head* head){
    if(node){
        if(node->prev_cl && node->next_cl){
            node->prev_cl->next_cl = node->next_cl;
            node->next_cl->prev_cl = node->prev_cl;
        }
        memset(node, 0, sizeof(list_classic));
        free(node);
        head->capacity--;
    }
}

list_err_t list_dtor(list_classic_head* head){
    list_err_t err = NO_MISTAKE_CL;
    err = list_verify_cl(head->root, head);
    if(err) return err;

    list_classic* node = head->root;
    list_classic* node_next = head->root;
    while(head->capacity > 0){
        if(node->next_cl){
            node_next = node->next_cl;
        }
        node_dtor(node, head);
        if(node_next){
            node = node_next;
        }
    }
    memset(head, 0, sizeof(list_classic_head));
    free(head);

    return err;
}

list_err_t list_add_elem_before_node(list_elem_t data, list_classic* node, list_classic_head* head){
    list_err_t err = NO_MISTAKE_CL;
    err = list_verify_cl(node, head);
    if(err) return err;

    if(head->capacity == 1){
        list_classic* new_node = node_ctor(data, node, node);
        node->prev_cl = new_node;
        node->next_cl = new_node;
        head->capacity++;
        err = list_verify_cl(node, head);
        return err;
    }

    list_classic* new_node = node_ctor(data, node->prev_cl, node);
    node->prev_cl->next_cl = new_node;
    node->prev_cl = new_node;

    head->capacity++;

    err = list_verify_cl(node, head);
    return err;
}

list_err_t list_add_elem_after_node(list_elem_t data, list_classic* node, list_classic_head* head){
    list_err_t err = NO_MISTAKE_CL;
    err = list_verify_cl(node, head);
    if(err) return err;

    if(head->capacity == 1){
        list_classic* new_node = node_ctor(data, node, node);
        node->prev_cl = new_node;
        node->next_cl = new_node;
        head->capacity++;
        err = list_verify_cl(node, head);
        return err;
    }

    list_classic* new_node = node_ctor(data, node, node->next_cl);
    node->next_cl->prev_cl = new_node;
    node->next_cl = new_node;

    head->capacity++;

    err = list_verify_cl(node, head);
    return err;
}

list_err_t list_verify_cl(const list_classic* node, list_classic_head* head){
    list_err_t err = NO_MISTAKE_CL;

    CHECK_AND_RET_MISTAKE_CL(head->root->data_cl != POISON_CL, 
                        NULL_ELEM_CORRUPTED_CL, "DATA[0] ELEMENT CORRUPTED\n");

    for(size_t idx = 0; idx < head->capacity; idx++){
        CHECK_AND_RET_MISTAKE_CL(!node, NULL_NODE_PTR_CL, "NODE PTR IS NULL\n");

        CHECK_AND_RET_MISTAKE_CL(head->capacity <= 0, INCORR_DATA_SIZE_CL, "INCORRECT DATA SIZE\n");

        if(head->capacity == 1){
            continue;
        }

        CHECK_AND_RET_MISTAKE_CL(!node->next_cl, NULL_NEXT_PTR_CL, "LIST NEXT PTR IS NULL\n");

        CHECK_AND_RET_MISTAKE_CL(!node->prev_cl, NULL_PRED_PTR_CL, "LIST PRED PTR IS NULL\n");

        CHECK_AND_RET_MISTAKE_CL(node != node->prev_cl->next_cl, INCORR_PREV_CL, "INCORRECT CONNECTION BETWEEN NODE AND PREV\n");

        CHECK_AND_RET_MISTAKE_CL(node != node->next_cl->prev_cl, INCORR_NEXT_CL, "INCORRECT CONNECTION BETWEEN NODE AND NEXT\n");

        node = node->next_cl;
    }

    return err;

}