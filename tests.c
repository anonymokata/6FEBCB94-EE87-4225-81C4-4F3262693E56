#include <stddef.h>
#include <string.h>
#include <check_stdint.h>
#include <sys/types.h>
#include <check.h>
#include "rpn.h"

START_TEST (test_name)
{
    ck_assert_int_eq(1, 2);
}
END_TEST

Suite* rpn_suite()
{
    Suite* s = suite_create("rpn");
    TCase* tc_core;
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_name);
    suite_add_tcase(s, tc_core);
    return s;
}

int main()
{
    Suite* s = rpn_suite();
    SRunner* sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    int number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return number_failed;
}
