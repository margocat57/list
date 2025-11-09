#include "list_func.h"
#include "list_test.h"
#include "list_dump.h"
#include "classic_list.h"
#include <stdio.h>

int main(){
    list l1 = list_ctor(5); 
    // имя логфайла - глобальная характеристика программы
    test_add_after_and_delete(&l1);

    list l2 = list_ctor(5); 
    test_add_before_and_delete(&l2);

    list_classic_head* head = classic_list_ctor();
    test_classic_list_add_before_idx(head, head->root);

    list_dtor(&l1);
    list_dtor(&l2);
    list_dtor(head);
    return 0;
}