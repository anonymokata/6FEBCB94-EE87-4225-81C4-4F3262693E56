#include <stddef.h>
#include <string.h>
#include <check_stdint.h>
#include <sys/types.h>
#include <check.h>
#include "rpn.h"

START_TEST(infix_op_check)
{
    ck_assert_ptr_eq(infix_to_rpn("a@b", 0, NULL, 0), NULL);
}
END_TEST

START_TEST(infix_max_len_check)
{
    ck_assert_ptr_eq(infix_to_rpn("a+b", 0, NULL, 0), NULL);
}
END_TEST

START_TEST(buff_len_check)
{
    ck_assert_ptr_eq(infix_to_rpn("a+b", 0, NULL, 0), NULL);
}
END_TEST

START_TEST(a_plus_b)
{
    char buff[128];
    memset(buff, 0, sizeof(buff));
    ck_assert_str_eq(infix_to_rpn("a+b", 3, buff, sizeof(buff)), "ab+");
}
END_TEST

START_TEST(a_plus_b_minus_c)
{
    char buff[128];
    memset(buff, 0, sizeof(buff));
    ck_assert_str_eq(infix_to_rpn("a+b-c", 5, buff, sizeof(buff)), "abc-+");
}
END_TEST

START_TEST(a_plus_b_minus_c_minus_d)
{
    char buff[128];
    memset(buff, 0, sizeof(buff));
    ck_assert_str_eq(infix_to_rpn("a+b-c-d", 7, buff, sizeof(buff)), "abcd--+");
}
END_TEST

Suite *rpn_suite()
{
    Suite *s = suite_create("rpn");
    TCase *tc_core;
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, infix_op_check);
    tcase_add_test(tc_core, infix_max_len_check);
    tcase_add_test(tc_core, buff_len_check);
    tcase_add_test(tc_core, a_plus_b);
    tcase_add_test(tc_core, a_plus_b_minus_c);
    tcase_add_test(tc_core, a_plus_b_minus_c_minus_d);
    suite_add_tcase(s, tc_core);
    return s;
}

int main()
{
    Suite *s = rpn_suite();
    SRunner *sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    int number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return number_failed;
}
