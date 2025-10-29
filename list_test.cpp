#include "list_func.h"
#include "list_dump.h"
#include "list_test.h"
#include <stdio.h>

void test_add_after_and_delete(list* lt1){
    add_elem_after_idx(lt1, 10, 0);
    list_dump_func(lt1, "After adding 10 after 0 pos", __FILE__, __PRETTY_FUNCTION__, __LINE__);
    add_elem_after_idx(lt1, 20, 1);
    list_dump_func(lt1, "After adding 20 after 1 pos", __FILE__, __PRETTY_FUNCTION__, __LINE__);
    add_elem_after_idx(lt1, 30, 2);
    list_dump_func(lt1, "After adding 30 after 2 pos", __FILE__, __PRETTY_FUNCTION__, __LINE__);
    add_elem_after_idx(lt1, 40, 3);
    list_dump_func(lt1, "After adding 40 after 3 pos", __FILE__, __PRETTY_FUNCTION__, __LINE__);
    add_elem_after_idx(lt1, 50, 4);
    list_dump_func(lt1, "After adding 50 after 4 pos", __FILE__, __PRETTY_FUNCTION__, __LINE__);
    add_elem_after_idx(lt1, 60, 5);
    list_dump_func(lt1, "After adding 60 after 5 pos", __FILE__, __PRETTY_FUNCTION__, __LINE__);
    add_elem_after_idx(lt1, 35, 3);
    list_dump_func(lt1, "After adding 35 after 3 pos", __FILE__, __PRETTY_FUNCTION__, __LINE__);
    add_elem_after_idx(lt1, 36, 7);
    list_dump_func(lt1, "After adding 36 after 7 pos", __FILE__, __PRETTY_FUNCTION__, __LINE__);
    add_elem_after_idx(lt1, 32, 3);
    list_dump_func(lt1, "After adding 32 after 3 pos", __FILE__, __PRETTY_FUNCTION__, __LINE__);
    del_elem(lt1, 6);
    list_dump_func(lt1, "After deleting 6st elem", __FILE__, __PRETTY_FUNCTION__, __LINE__);
    del_elem(lt1, 1);
    list_dump_func(lt1, "After deleting 1st elem", __FILE__, __PRETTY_FUNCTION__, __LINE__);
    add_elem_after_idx(lt1, 11, 0);
    list_dump_func(lt1, "After adding 11 after 0 pos", __FILE__, __PRETTY_FUNCTION__, __LINE__);
    add_elem_after_idx(lt1, 66, 5);
    list_dump_func(lt1, "After adding 66 after 5 pos", __FILE__, __PRETTY_FUNCTION__, __LINE__);
    add_elem_after_idx(lt1, 70, 6);
    list_dump_func(lt1, "After adding 70 after 6 pos", __FILE__, __PRETTY_FUNCTION__, __LINE__);
    del_elem(lt1, 4);
    list_dump_func(lt1, "After deleting 4th elem", __FILE__, __PRETTY_FUNCTION__, __LINE__);
    del_elem(lt1, 7);
    list_dump_func(lt1, "After deleting 7th elem", __FILE__, __PRETTY_FUNCTION__, __LINE__);
}

void test_add_before_and_delete(list* lt1){
    add_elem_before_idx(lt1, 11, 0);
    list_dump_func(lt1, "After adding 11 before 0 pos(end of list)", __FILE__, __PRETTY_FUNCTION__, __LINE__);
    add_elem_before_idx(lt1, 21, 0);
    list_dump_func(lt1, "After adding 21 before 0 pos(end of list)", __FILE__, __PRETTY_FUNCTION__, __LINE__);
    add_elem_before_idx(lt1, 31, 0);
    list_dump_func(lt1, "After adding 31 before 0 pos(end of list)", __FILE__, __PRETTY_FUNCTION__, __LINE__);
    add_elem_before_idx(lt1, 41, 0);
    list_dump_func(lt1, "After adding 41 before 0 pos(end of list)", __FILE__, __PRETTY_FUNCTION__, __LINE__);
    add_elem_before_idx(lt1, 51, 0);
    list_dump_func(lt1, "After adding 51 before 0 pos(end of list)", __FILE__, __PRETTY_FUNCTION__, __LINE__);
    add_elem_before_idx(lt1, 61, 0);
    list_dump_func(lt1, "After adding 61 before 0 pos(end of list)", __FILE__, __PRETTY_FUNCTION__, __LINE__);
    add_elem_before_idx(lt1, 35, 4);
    list_dump_func(lt1, "After adding 35 before 4 pos", __FILE__, __PRETTY_FUNCTION__, __LINE__);
    add_elem_before_idx(lt1, 36, 4);
    list_dump_func(lt1, "After adding 36 before 4 pos", __FILE__, __PRETTY_FUNCTION__, __LINE__);
    add_elem_before_idx(lt1, 32, 7);
    list_dump_func(lt1, "After adding 32 before 7 pos", __FILE__, __PRETTY_FUNCTION__, __LINE__);
    del_elem(lt1, 6);
    list_dump_func(lt1, "After deleting 6st elem", __FILE__, __PRETTY_FUNCTION__, __LINE__);
    del_elem(lt1, 1);
    list_dump_func(lt1, "After deleting 1st elem", __FILE__, __PRETTY_FUNCTION__, __LINE__);
    del_elem(lt1, 2);
    list_dump_func(lt1, "After deleting 2st elem", __FILE__, __PRETTY_FUNCTION__, __LINE__);
    del_elem(lt1, 3);
    list_dump_func(lt1, "After deleting 3st elem", __FILE__, __PRETTY_FUNCTION__, __LINE__);
    add_elem_before_idx(lt1, 31, 9);
    list_dump_func(lt1, "After adding 31 before 9 pos", __FILE__, __PRETTY_FUNCTION__, __LINE__);
    add_elem_before_idx(lt1, 21, 3);
    list_dump_func(lt1, "After adding 21 before 3 pos", __FILE__, __PRETTY_FUNCTION__, __LINE__);
    add_elem_before_idx(lt1, 11, 2);
    list_dump_func(lt1, "After adding 11 before 2 pos", __FILE__, __PRETTY_FUNCTION__, __LINE__);
    add_elem_before_idx(lt1, 66, 7);
    list_dump_func(lt1, "After adding 66 before 7 pos", __FILE__, __PRETTY_FUNCTION__, __LINE__);
    add_elem_before_idx(lt1, 70, 0);
    list_dump_func(lt1, "After adding 70 before 0 pos(end of list)", __FILE__, __PRETTY_FUNCTION__, __LINE__);
    del_elem(lt1, 4);
    list_dump_func(lt1, "After deleting 4th elem", __FILE__, __PRETTY_FUNCTION__, __LINE__);
    del_elem(lt1, 7);
    list_dump_func(lt1, "After deleting 7th elem", __FILE__, __PRETTY_FUNCTION__, __LINE__);
}