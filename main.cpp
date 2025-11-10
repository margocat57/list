#include "list_func.h"
#include "list_test.h"
#include "list_dump.h"
#include <stdio.h>

int main(){
    list l1 = list_ctor(5); 
    // имя логфайла - глобальная характеристика программы
    test_add_after_and_delete(&l1);

    list l2 = list_ctor(5); 
    test_add_before_and_delete(&l2);

    list_dtor(&l1);
    list_dtor(&l2);
    return 0;
}