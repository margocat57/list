#include "classic_list.h"
#include "cl_list_test.h"
#include "classic_list_dump.h"
#include <stdio.h>

void test_classic_list_add_before_idx(list_classic_head* head, list_classic* ltc){
    list_add_elem_before_node(11, head->root, head);
    list_dump_func_cl(head, "After adding %d before %d pos(end of list)", __FILE__, __PRETTY_FUNCTION__, __LINE__, 11, 0);
    list_add_elem_before_node(21, head->root, head);
    list_dump_func_cl(head, "After adding %d before %d pos(end of list)", __FILE__, __PRETTY_FUNCTION__, __LINE__, 21, 0);
    list_add_elem_before_node(31, head->root, head);
    list_dump_func_cl(head, "After adding %d before %d pos(end of list)", __FILE__, __PRETTY_FUNCTION__, __LINE__, 31, 0);
    list_add_elem_before_node(41, head->root, head);
    list_dump_func_cl(head, "After adding %d before %d pos(end of list)", __FILE__, __PRETTY_FUNCTION__, __LINE__, 41, 0);
    list_add_elem_before_node(51, head->root, head);
    list_dump_func_cl(head, "After adding %d before %d pos(end of list)", __FILE__, __PRETTY_FUNCTION__, __LINE__, 51, 0);
    list_add_elem_before_node(61, head->root, head);
    list_dump_func_cl(head, "After adding %d before %d pos(end of list)", __FILE__, __PRETTY_FUNCTION__, __LINE__, 61, 0);
    list_add_elem_before_node(35, head->root->next_cl->next_cl->next_cl->next_cl, head);
    list_dump_func_cl(head, "After adding %d before %d pos", __FILE__, __PRETTY_FUNCTION__, __LINE__, 35, 4);
    list_add_elem_after_node(36, head->root->next_cl->next_cl->next_cl->next_cl, head);
    list_dump_func_cl(head, "After adding %d after %d pos", __FILE__, __PRETTY_FUNCTION__, __LINE__, 36, 4);
    node_dtor(head->root->next_cl->next_cl, head);
    list_dump_func_cl(head, "After del", __FILE__, __PRETTY_FUNCTION__, __LINE__);
}