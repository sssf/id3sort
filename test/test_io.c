#include <string.h>
#include <CUnit/Basic.h>
#include "../source/io.h"
#include <stdlib.h>


static const char *file_exist        = "file_exist.test";
static const char *file_missing      = "file_missing.test";
static const char *directory_exist   = "directory_exist";
static const char *directory_missing = "directory_missing";


int init_suite_1 (void) {
    IO_directory_create(directory_exist);

    FILE* file = fopen(file_exist, "wb");
    char buffer[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'k'};
    fwrite (buffer , sizeof(char), sizeof(buffer), file);
    fclose (file);

    return 0;
}


int clean_suite_1(void) {
    IO_directory_remove(directory_exist);
    IO_directory_remove(directory_missing);
    IO_file_remove(file_exist);
    return 0;
}


void test_IO_directory_exist (void) {

    CU_ASSERT(IO_directory_exist(directory_exist)  == true);
    CU_ASSERT(IO_directory_exist(directory_missing) == false);
}


void test_IO_directory_create(void) {

    CU_ASSERT(IO_directory_create(directory_missing) == true);
    CU_ASSERT(IO_directory_create(directory_exist)  == false);

    IO_directory_remove(directory_missing);
}


void test_IO_directory_remove(void) {

    CU_ASSERT(IO_directory_remove(directory_missing) == false);
    CU_ASSERT(IO_directory_remove(directory_exist)  == true);
    CU_ASSERT(IO_directory_create(directory_exist) == true);
}


void test_IO_file_size(void) {
    CU_ASSERT(IO_file_size(file_exist) == 10);
    CU_ASSERT(IO_file_size(file_missing) == -1);
}


void test_IO_file_exist(void) {
    CU_ASSERT(IO_file_exist(file_exist) == true);
    CU_ASSERT(IO_file_exist(file_missing) == false);
}


void test_IO_file_move(void) {

    char *file_moved = "file_moved.test";
    char *file_to_move = "file_to_move.test";

    system("touch file_to_move.test");

    CU_ASSERT(IO_file_move(file_missing, file_moved) == false);
    CU_ASSERT(IO_file_move(file_to_move, file_exist) == false);
    CU_ASSERT(IO_file_move(file_to_move, file_moved) == true);
    CU_ASSERT(IO_file_exist(file_to_move) == false);
    CU_ASSERT(IO_file_exist(file_moved) == true);
    CU_ASSERT(IO_file_remove(file_moved) == true);

    //IO_file_move(file_exist, file_moved);
}


void test_IO_file_remove(void) {

    char *file_to_remove = "file_to_remove.test";
    system("touch file_to_remove.test");

    CU_ASSERT(IO_file_remove(file_missing) == false);
    CU_ASSERT(IO_file_exist(file_to_remove)    == true);
    CU_ASSERT(IO_file_remove(file_to_remove)   == true);
    CU_ASSERT(IO_file_exist(file_to_remove)    == false);
}


int main() {
    CU_pSuite pSuite1 = NULL;

    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();


    /* add a suites to the registry */
    pSuite1 = CU_add_suite("IO Test Suite", init_suite_1, clean_suite_1);
    if (NULL == pSuite1)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add the tests to the suites */
    if (
            (NULL == CU_add_test(pSuite1, "test of IO_directory_exist     ", test_IO_directory_exist)) ||
            (NULL == CU_add_test(pSuite1, "test of IO_directory_create    ", test_IO_directory_create)) ||
            (NULL == CU_add_test(pSuite1, "test of IO_directory_remove    ", test_IO_directory_remove)) ||
            (NULL == CU_add_test(pSuite1, "test of IO_file_size           ", test_IO_file_size)) ||
            (NULL == CU_add_test(pSuite1, "test of IO_file_exist          ", test_IO_file_exist)) ||
            (NULL == CU_add_test(pSuite1, "test of IO_file_move           ", test_IO_file_move)) ||
            (NULL == CU_add_test(pSuite1, "test of IO_file_remove         ", test_IO_file_remove))
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

