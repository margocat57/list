#include "list_func.h"
#include "list_dump.h"
#include "list_test.h"
#include <stdio.h>

void test_add_after_and_delete(list* lt1){
    add_elem_after_idx(lt1, 10, 0);
    list_dump_func(lt1, "After adding %d after %d pos", __FILE__, __PRETTY_FUNCTION__, __LINE__, 10, 0);
    add_elem_after_idx(lt1, 20, 1);
    list_dump_func(lt1, "After adding %d after %d pos", __FILE__, __PRETTY_FUNCTION__, __LINE__, 20, 1);
    add_elem_after_idx(lt1, 30, 2);
    list_dump_func(lt1, "After adding %d after %d pos", __FILE__, __PRETTY_FUNCTION__, __LINE__, 30, 2);
    add_elem_after_idx(lt1, 40, 3);
    list_dump_func(lt1, "After adding %d after %d pos", __FILE__, __PRETTY_FUNCTION__, __LINE__, 40, 3);
    add_elem_after_idx(lt1, 50, 4);
    list_dump_func(lt1, "After adding %d after %d pos", __FILE__, __PRETTY_FUNCTION__, __LINE__, 50, 4);
    add_elem_after_idx(lt1, 60, 5);
    list_dump_func(lt1, "After adding %d after %d pos", __FILE__, __PRETTY_FUNCTION__, __LINE__, 60, 5);
    add_elem_after_idx(lt1, 35, 3);
    list_dump_func(lt1, "After adding %d after %d pos", __FILE__, __PRETTY_FUNCTION__, __LINE__, 35, 3);
    add_elem_after_idx(lt1, 36, 7);
    list_dump_func(lt1, "After adding %d after %d pos", __FILE__, __PRETTY_FUNCTION__, __LINE__, 36, 7);
    add_elem_after_idx(lt1, 32, 3);
    list_dump_func(lt1, "After adding %d after %d pos", __FILE__, __PRETTY_FUNCTION__, __LINE__, 32, 3);
    del_elem(lt1, 6);
    list_dump_func(lt1, "After deleting %dst elem", __FILE__, __PRETTY_FUNCTION__, __LINE__, 6);
    del_elem(lt1, 1);
    list_dump_func(lt1, "After deleting %dst elem", __FILE__, __PRETTY_FUNCTION__, __LINE__, 1);
    add_elem_after_idx(lt1, 11, 0);
    list_dump_func(lt1, "After adding %d after %d pos", __FILE__, __PRETTY_FUNCTION__, __LINE__, 11, 0);
    add_elem_after_idx(lt1, 66, 5);
    list_dump_func(lt1, "After adding %d after %d pos", __FILE__, __PRETTY_FUNCTION__, __LINE__, 66, 5);
    add_elem_after_idx(lt1, 70, 6);
    list_dump_func(lt1, "After adding %d after %d pos", __FILE__, __PRETTY_FUNCTION__, __LINE__, 70, 6);
    del_elem(lt1, 4);
    list_dump_func(lt1, "After deleting %dth elem", __FILE__, __PRETTY_FUNCTION__, __LINE__, 4);
    del_elem(lt1, 7);
    list_dump_func(lt1, "After deleting %dth elem", __FILE__, __PRETTY_FUNCTION__, __LINE__, 7);
    list_linearize(lt1);
    list_dump_func(lt1, "After linearizing", __FILE__, __PRETTY_FUNCTION__, __LINE__);

    // портим
    lt1->next[5] = 3;
    list_dump_func(lt1, "After damaging %zd idx next(in array but incorr index)", __FILE__, __PRETTY_FUNCTION__, __LINE__, 5);

    lt1->prev[5] = 9;
    list_dump_func(lt1, "After damaging %zd idx prev(in array but incorr index)", __FILE__, __PRETTY_FUNCTION__, __LINE__, 5);
}

void test_add_before_and_delete(list* lt1){
    add_elem_before_idx(lt1, 11, 0);
    list_dump_func(lt1, "After adding %d before %d pos(end of list)", __FILE__, __PRETTY_FUNCTION__, __LINE__, 11, 0);
    add_elem_before_idx(lt1, 21, 0);
    list_dump_func(lt1, "After adding %d before %d pos(end of list)", __FILE__, __PRETTY_FUNCTION__, __LINE__, 21, 0);
    add_elem_before_idx(lt1, 31, 0);
    list_dump_func(lt1, "After adding %d before %d pos(end of list)", __FILE__, __PRETTY_FUNCTION__, __LINE__, 31, 0);
    add_elem_before_idx(lt1, 41, 0);
    list_dump_func(lt1, "After adding %d before %d pos(end of list)", __FILE__, __PRETTY_FUNCTION__, __LINE__, 41, 0);
    add_elem_before_idx(lt1, 51, 0);
    list_dump_func(lt1, "After adding %d before %d pos(end of list)", __FILE__, __PRETTY_FUNCTION__, __LINE__, 51, 0);
    add_elem_before_idx(lt1, 61, 0);
    list_dump_func(lt1, "After adding %d before %d pos(end of list)", __FILE__, __PRETTY_FUNCTION__, __LINE__, 61, 0);
    add_elem_before_idx(lt1, 35, 4);
    list_dump_func(lt1, "After adding %d before %d pos", __FILE__, __PRETTY_FUNCTION__, __LINE__, 35, 4);
    add_elem_before_idx(lt1, 36, 4);
    list_dump_func(lt1, "After adding %d before %d pos", __FILE__, __PRETTY_FUNCTION__, __LINE__, 36, 4);
    add_elem_before_idx(lt1, 32, 7);
    list_dump_func(lt1, "After adding %d before %d pos", __FILE__, __PRETTY_FUNCTION__, __LINE__, 32, 7);
    del_elem(lt1, 6);
    list_dump_func(lt1, "After deleting %dst elem", __FILE__, __PRETTY_FUNCTION__, __LINE__, 6);
    del_elem(lt1, 1);
    list_dump_func(lt1, "After deleting %dst elem", __FILE__, __PRETTY_FUNCTION__, __LINE__, 1);
    del_elem(lt1, 2);
    list_dump_func(lt1, "After deleting %dst elem", __FILE__, __PRETTY_FUNCTION__, __LINE__, 2);
    del_elem(lt1, 3);
    list_dump_func(lt1, "After deleting %dst elem", __FILE__, __PRETTY_FUNCTION__, __LINE__, 3);
    add_elem_before_idx(lt1, 31, 9);
    list_dump_func(lt1, "After adding %d before %d pos", __FILE__, __PRETTY_FUNCTION__, __LINE__, 31, 9);
    add_elem_before_idx(lt1, 21, 3);
    list_dump_func(lt1, "After adding %d before %d pos", __FILE__, __PRETTY_FUNCTION__, __LINE__, 21, 3);
    add_elem_before_idx(lt1, 11, 2);
    list_dump_func(lt1, "After adding %d before %d pos", __FILE__, __PRETTY_FUNCTION__, __LINE__, 11, 2);
    add_elem_before_idx(lt1, 66, 0);
    list_dump_func(lt1, "After adding %d before %d pos(end of list)", __FILE__, __PRETTY_FUNCTION__, __LINE__, 66, 0);
    add_elem_before_idx(lt1, 70, 0);
    list_dump_func(lt1, "After adding %d before %d pos(end of list)", __FILE__, __PRETTY_FUNCTION__, __LINE__, 70, 0);
    del_elem(lt1, 4);
    list_dump_func(lt1, "After deleting %dth elem", __FILE__, __PRETTY_FUNCTION__, __LINE__, 4);
    del_elem(lt1, 7);
    list_dump_func(lt1, "After deleting %dth elem", __FILE__, __PRETTY_FUNCTION__, __LINE__, 7);
    // list_linearize(lt1);
    // list_dump_func(lt1, "After linearizing", __FILE__, __PRETTY_FUNCTION__, __LINE__);

    // портим
    lt1->next[5] = 57;
    list_dump_func(lt1, "After damaging %zd idx next(out of arr)", __FILE__, __PRETTY_FUNCTION__, __LINE__, 5);

    lt1->prev[5] = 75;
    list_dump_func(lt1, "After damaging %zd idx prev(out of arr)", __FILE__, __PRETTY_FUNCTION__, __LINE__, 5);
}

