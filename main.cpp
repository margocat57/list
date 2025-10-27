#include "list_func.h"
#include "list_dump.h"
#include <stdio.h>

int main(){
    list l1 = list_ctor(5); // а вот здесь как вариант можно указывать где был создан
    add_elem(&l1, 10, 0);
    add_elem(&l1, 20, 1);
    add_elem(&l1, 30, 2);
    add_elem(&l1, 40, 3);
    add_elem(&l1, 50, 4);
    add_elem(&l1, 60, 5);
    add_elem(&l1, 35, 3);
    list_dump_func(&l1, "l1_dump_1", "log.htm", "After adding35");
    add_elem(&l1, 36, 7);
    list_dump_func(&l1, "l1_dump_2", "log.htm", "After adding36");
    add_elem(&l1, 32, 3);
    list_dump_func(&l1, "l1_dump_3", "log.htm", "After adding32");
    del_elem(&l1, 6);
    list_dump_func(&l1, "l1_dump_4", "log.htm", "After deleting 6st elem");
    del_elem(&l1, 1);
    list_dump_func(&l1, "l1_dump_5", "log.htm", "After deleting 1st elem");
    add_elem(&l1, 11, 0);
    list_dump_func(&l1, "l1_dump_6", "log.htm", "After adding 11");
    add_elem(&l1, 66, 5);
    list_dump_func(&l1, "l1_dump_7", "log.htm", "After adding66");
    add_elem(&l1, 70, 6);
    list_dump_func(&l1, "l1_dump_8", "log.htm", "After adding70");
    //del_elem(&l1, 10);

    // del_elem(&l1, 1);
    // add_elem(&l1, 36, 5); 
    // add_elem(&l1, 45, 5);
    // вставка по логическому индексу в нелинеаризованном массиве перестает работать
    // и как мне сказал ментор - там уже только циклы
    // add_elem(&l1, 37, 6);
    // add_elem(&l1, 70, 9); //  но его нужно на 7 или 10 место вставлять
    // add_elem(&l1, 37, 6);
    // cлучай 36 5 спросить
    // add_elem(&l1, 45, 5);
    list_dtor(&l1);
    // list_dump_html(&l1, "log.htm", "First try");
    return 0;
}