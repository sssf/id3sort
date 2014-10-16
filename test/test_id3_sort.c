#include <string.h>
#include <CUnit/Basic.h>
#include "../source/id3_sort.h"
#include "../source/io.h"
#include <stdlib.h>
#include <stdbool.h>



int init_suite_1 (void) {
    return 0;
}
int clean_suite_1(void) {
    return 0;
}


void test_ID3_sort_file(void) {

    char *file_missing = "bin/test_data/file_missing.test";
    char *file_small   = "bin/test_data/file_small.test";
    char *file_no_id3  = "bin/test_data/file_no_id3.test";
    char *file_has_all_frames = "bin/test_data/file_has_all_frames.mp3";
    char *file_missing_frames = "bin/test_data/file_missing_frames.mp3";
    char *file_missing_album  = "bin/test_data/file_missing_album.mp3";
    char *target              = "bin/test_music";

    // test invalid files
    CU_ASSERT(ID3_sort_file(file_missing, target) == false);
    CU_ASSERT(ID3_sort_file(file_small, target)   == false);
    CU_ASSERT(ID3_sort_file(file_no_id3, target)  == false);

    // make sure files are properly set up
    CU_ASSERT(IO_file_exist(file_has_all_frames) == true);
    CU_ASSERT(IO_file_exist(file_missing_album)  == true);
    CU_ASSERT(IO_file_exist(file_missing_frames) == true);

    CU_ASSERT(IO_file_exist("bin/test_music/Tolvan Tolvansson/Best of Tolvan/Apostlarna.mp3") == false);
    CU_ASSERT(IO_file_exist("bin/test_music/Lady Gogo/Unbiased Dance.mp3") ==  false);
    CU_ASSERT(IO_file_exist("bin/test_music/unknown/Track 7.mp3") == false);

    // sort the file
    CU_ASSERT(ID3_sort_file(file_has_all_frames, target) == true);
    CU_ASSERT(ID3_sort_file(file_missing_album, target)  == true);
    CU_ASSERT(ID3_sort_file(file_missing_frames, target) == true);

    // make sure we get expected result
    CU_ASSERT(IO_file_exist(file_has_all_frames) == false);
    CU_ASSERT(IO_file_exist(file_missing_album)  == false);
    CU_ASSERT(IO_file_exist(file_missing_frames) == false);

    CU_ASSERT(IO_file_exist("bin/test_music/Tolvan Tolvansson/Best of Tolvan/Apostlarna.mp3") == true);
    CU_ASSERT(IO_file_exist("bin/test_music/Lady Gogo/Unbiased Dance.mp3") ==  true);
    CU_ASSERT(IO_file_exist("bin/test_music/unknown/Track 7.mp3") == true);
}


int main() {
    CU_pSuite pSuite1 = NULL;

    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();


    /* add a suites to the registry */
    pSuite1 = CU_add_suite("ID3 Test Suite", init_suite_1, clean_suite_1);
    if (NULL == pSuite1)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add the tests to the suites */
    if (
            (NULL == CU_add_test(pSuite1, "test of ID3_sort_file           ", test_ID3_sort_file))
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

