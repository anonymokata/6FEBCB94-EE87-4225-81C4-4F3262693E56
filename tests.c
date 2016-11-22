#include <stddef.h>
#include <string.h>
#include <check_stdint.h>
#include <sys/types.h>
#include <check.h>
#include "rpn.h"

START_TEST(infix_op_check)
{
    ck_assert_ptr_eq(infix_to_rpn("a@b", 0, NULL, 0, NULL, 0), NULL);
}
END_TEST

START_TEST(infix_max_len_check)
{
    ck_assert_ptr_eq(infix_to_rpn("a+b", 0, NULL, 0, NULL, 0), NULL);
}
END_TEST

START_TEST(buff_len_check)
{
    ck_assert_ptr_eq(infix_to_rpn("a+b", 0, NULL, 0, NULL, 0), NULL);
}
END_TEST
char buff[128], scratchbuff[128];

START_TEST(a_plus_b)
{
    memset(buff, 0, sizeof(buff));
    ck_assert_str_eq(infix_to_rpn("a+b", 3, buff, sizeof(buff), scratchbuff, sizeof(scratchbuff)), "ab+");
}
END_TEST

START_TEST(a_plus_b_minus_c)
{
    memset(buff, 0, sizeof(buff));
    ck_assert_str_eq(infix_to_rpn("a+b-c", 5, buff, sizeof(buff), scratchbuff, sizeof(scratchbuff)), "abc-+");
}
END_TEST

START_TEST(a_plus_p_b_minus_c_p)
{
    memset(buff, 0, sizeof(buff));
    ck_assert_str_eq(infix_to_rpn("a+(b-c)", 7, buff, sizeof(buff), scratchbuff, sizeof(scratchbuff)), "abc-+");
}
END_TEST

START_TEST(a_plus_b_minus_c_minus_d)
{
    memset(buff, 0, sizeof(buff));
    ck_assert_str_eq(infix_to_rpn("a+b-c-d", 7, buff, sizeof(buff), scratchbuff, sizeof(scratchbuff)), "abcd--+");
}
END_TEST

START_TEST(p_a_plus_b_p_minus_c)
{
    memset(buff, 0, sizeof(buff));
    ck_assert_str_eq(infix_to_rpn("(a+b)-c", 7, buff, sizeof(buff), scratchbuff, sizeof(scratchbuff)), "ab+c-");
}
END_TEST

START_TEST(abcdef)
{
    memset(buff, 0, sizeof(buff));
    ck_assert_str_eq(infix_to_rpn("a+b-c*d/e^f", sizeof(buff), buff, sizeof(buff), scratchbuff, sizeof(scratchbuff)), "abcdef^/*-+");
}
END_TEST

START_TEST(abcdef_short)
{
    memset(buff, 0, sizeof(buff));
    ck_assert_str_eq(infix_to_rpn("a+b-c*d/e^f", 3, buff, sizeof(buff), scratchbuff, sizeof(scratchbuff)), "ab+");
}
END_TEST

START_TEST(l_div_m_pow_n_mult_o_minus_p)
{
    memset(buff, 0, sizeof(buff));
    ck_assert_str_eq(infix_to_rpn("l/m^n*o-p", 9, buff, sizeof(buff), scratchbuff, sizeof(scratchbuff)), "lmn^/o*p-");
}
END_TEST

START_TEST(l_div_m_pow_n_mult_o_minus_p_with_parens)
{
    memset(buff, 0, sizeof(buff));
    ck_assert_str_eq(infix_to_rpn("((l/(m^n))*o)-p", sizeof(buff), buff, sizeof(buff), scratchbuff, sizeof(scratchbuff)), "lmn^/o*p-");
}
END_TEST

START_TEST(p_p_v_div_w_p_pow_x_p_mul_p_y_sub_z_p)
{
    memset(buff, 0, sizeof(buff));
    ck_assert_str_eq(infix_to_rpn("((v/w)^x)*(y-z)", sizeof(buff), buff, sizeof(buff), scratchbuff, sizeof(scratchbuff)), "vw/x^yz-*");
}
END_TEST

START_TEST(the_big_one)
{
    memset(buff, 0, sizeof(buff));
    ck_assert_str_eq(infix_to_rpn("(a+g)*(((b-a)+c)^(c+(e*(d^f))))", sizeof(buff), buff, sizeof(buff), scratchbuff, sizeof(scratchbuff)), "ag+ba-c+cedf^*+^*");
}
END_TEST

START_TEST(inf_a_plus_b)
{
    memset(buff, 0, sizeof(buff));
    ck_assert_str_eq(rpn_to_infix("ab+", 3, buff, sizeof(buff), scratchbuff, sizeof(scratchbuff)), "a+b");
}
END_TEST

START_TEST(inf_a_plus_b_plus_c)
{
    memset(buff, 0, sizeof(buff));
    ck_assert_str_eq(rpn_to_infix("abc++", 5, buff, sizeof(buff), scratchbuff, sizeof(scratchbuff)), "a+b+c");
}
END_TEST

START_TEST(inf_a_plus_b_minus_c)
{
    memset(buff, 0, sizeof(buff));
    ck_assert_str_eq(rpn_to_infix("abc-+", 5, buff, sizeof(buff), scratchbuff, sizeof(scratchbuff)), "a+b-c");
}
END_TEST

START_TEST(inf_l_div_m_pow_n_mult_o_minus_p)
{
    memset(buff, 0, sizeof(buff));
    ck_assert_str_eq(rpn_to_infix("lmn^/o*p-", 9, buff, sizeof(buff), scratchbuff, sizeof(scratchbuff)), "l/m^n*o-p");
}
END_TEST

START_TEST(inf_l_div_m_pow_n_mult_o_minus_p_with_parens)
{
    memset(buff, 0, sizeof(buff));
    ck_assert_str_eq(rpn_to_infix("lmn^/o*p-", sizeof(buff), buff, sizeof(buff), scratchbuff, sizeof(scratchbuff)), "l/m^n*o-p");
}
END_TEST

START_TEST(inf_p_p_v_div_w_p_pow_x_p_mul_p_y_sub_z_p)
{
    memset(buff, 0, sizeof(buff));
    ck_assert_str_eq(rpn_to_infix("vw/x^yz-*", sizeof(buff), buff, sizeof(buff), scratchbuff, sizeof(scratchbuff)), "(v/w)^x*(y-z)");
}
END_TEST

START_TEST(inf_the_big_one)
{
    memset(buff, 0, sizeof(buff));
    ck_assert_str_eq(rpn_to_infix("ag+ba-c+cedf^*+^*", sizeof(buff), buff, sizeof(buff), scratchbuff, sizeof(scratchbuff)), "(a+g)*(b-a+c)^(c+e*d^f)");
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
    tcase_add_test(tc_core, a_plus_p_b_minus_c_p);
    tcase_add_test(tc_core, a_plus_b_minus_c_minus_d);
    tcase_add_test(tc_core, p_a_plus_b_p_minus_c);
    tcase_add_test(tc_core, abcdef);
    tcase_add_test(tc_core, abcdef_short);
    tcase_add_test(tc_core, l_div_m_pow_n_mult_o_minus_p);
    tcase_add_test(tc_core, l_div_m_pow_n_mult_o_minus_p_with_parens);
    tcase_add_test(tc_core, p_p_v_div_w_p_pow_x_p_mul_p_y_sub_z_p);
    tcase_add_test(tc_core, the_big_one);

    suite_add_tcase(s, tc_core);
    return s;
}

Suite *inf_suite()
{
    Suite *s = suite_create("inf");
    TCase *tc_core;
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, inf_a_plus_b);
    tcase_add_test(tc_core, inf_a_plus_b_plus_c);
    tcase_add_test(tc_core, inf_a_plus_b_minus_c);
    tcase_add_test(tc_core, inf_l_div_m_pow_n_mult_o_minus_p);
    tcase_add_test(tc_core, inf_l_div_m_pow_n_mult_o_minus_p_with_parens);
    tcase_add_test(tc_core, inf_p_p_v_div_w_p_pow_x_p_mul_p_y_sub_z_p);
    tcase_add_test(tc_core, inf_the_big_one);

    suite_add_tcase(s, tc_core);
    return s;
}

int main()
{
    Suite *s = rpn_suite();//FIXME: free
    SRunner *sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    int number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    
    Suite *inf_s = inf_suite();//FIXME: free
    SRunner *inf_sr = srunner_create(inf_s);
    srunner_run_all(inf_sr, CK_NORMAL);
    int inf_number_failed = srunner_ntests_failed(inf_sr);
    srunner_free(inf_sr);
    
    return number_failed + inf_number_failed;
}
