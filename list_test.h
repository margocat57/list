#ifndef LIST_TEST_H
#define LIST_TEST_H
#include "list_struct.h"
#include "classic_list.h"

void test_add_after_and_delete(list* lt1);

void test_add_before_and_delete(list* lt1);

void test_classic_list_add_before_idx(list_classic_head* head, list_classic* ltc);

#endif // LIST_TEST_H