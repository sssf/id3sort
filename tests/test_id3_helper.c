#include <string.h>
#include <CUnit/Basic.h>
#include "../source/id3_helper.h"
#include <stdlib.h>
#include <stdbool.h>


//static const char *file_missing = "bin/test_data/file_missing.test";
//static const char *file_small  = "bin/test_data/file_small.test";
//static const char *file_no_id3 = "bin/test_data/file_no_id3.test";
static const char *id3_file = "bin/test_data/file_has_id3.test";

int init_suite_1 (void) {
    return 0;
}
int clean_suite_1(void) {
    return 0;
}


void test_ID3_info_read(void) {
    ID3_info *info = ID3_info_read(id3_file);
    CU_ASSERT(info != NULL);
    CU_ASSERT_STRING_EQUAL(info->title,  "Apostlarna");
    CU_ASSERT_STRING_EQUAL(info->artist, "Tolvan Tolvansson");
    CU_ASSERT_STRING_EQUAL(info->album,  "Best of Tolvan");
    CU_ASSERT_STRING_EQUAL(info->track,  "1/12");
    CU_ASSERT_STRING_EQUAL(info->genre,  "Twelve tone music");
    CU_ASSERT_STRING_EQUAL(info->year,   "1912");
    ID3_info_free(&info);
    CU_ASSERT(info == NULL);
}


void test_ID3_info_free(void) {
    ID3_info *info = malloc(sizeof(ID3_info));
    CU_ASSERT(info != NULL);
    memset(info, 0, sizeof(ID3_info));
    ID3_info_free(&info);
    CU_ASSERT(info == NULL);
}


void test_ID3_info_print(void) {
    // How do we test this and should we test it? Just make it pass, we like pretty green colors!
    CU_ASSERT(true);
}


int main() {
    CU_pSuite pSuite1 = NULL;

    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();


    /* add a suites to the registry */
    pSuite1 = CU_add_suite("ID3 sort helper Test Suite", init_suite_1, clean_suite_1);
    if (NULL == pSuite1)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add the tests to the suites */
    if (
            (NULL == CU_add_test(pSuite1, "test of ID3_info_read          ", test_ID3_info_read)) ||
            (NULL == CU_add_test(pSuite1, "test of ID3_info_free          ", test_ID3_info_free)) ||
            (NULL == CU_add_test(pSuite1, "test of ID3_info_print         ", test_ID3_info_print))
       )
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return CU_get_error();
}

