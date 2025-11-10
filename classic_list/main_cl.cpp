#include "classic_list.h"
#include "cl_list_test.h"
#include <stdio.h>

int main(){
    list_classic_head* head = classic_list_ctor();
    test_classic_list_add_before_idx(head, head->root);

    list_dtor(head);
    return 0;
}