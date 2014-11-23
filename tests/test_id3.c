#include <string.h>
#include <CUnit/Basic.h>
#include "../source/id3.h"
#include <stdlib.h>
#include <stdbool.h>


static const char *file_missing = "bin/test_data/file_missing.test";
static const char *file_small  = "bin/test_data/file_small.test";
static const char *file_no_id3 = "bin/test_data/file_no_id3.test";
static const char *file_has_id3 = "bin/test_data/file_has_id3.test";

int init_suite_1 (void) {
    return 0;
}
int clean_suite_1(void) {
    return 0;
}

void test_ID3_open(void) {

    CU_ASSERT(ID3_open(file_missing) == NULL);
    CU_ASSERT(ID3_open(file_small) == NULL);
    CU_ASSERT(ID3_open(file_no_id3) == NULL);
    ID3_file *file = ID3_open(file_has_id3);
    CU_ASSERT(file != NULL);
    ID3_close(&file);
}

void test_ID3_close(void) {
    ID3_file *file = ID3_open(file_has_id3);
    CU_ASSERT(file != NULL);
    ID3_close(&file);
    CU_ASSERT(file == NULL);
}

void test_ID3_read(void) {
    ID3_file *file = ID3_open(file_has_id3);
    CU_ASSERT(file != NULL);

    ID3_frame *frame = NULL;


    bool TPE1 = false; // "Tolvan Tolvansson"
    bool TYER = false; // "1912"
    bool TALB = false; // "Best of Tolvan"
    bool TIT2 = false; // "Apostlarna"
    bool TRCK = false; // "1/12"
    bool TCON = false; // "Twelve tone music"


#define FRAME_ID_EQUAL(tag) (strcmp(ID3_frame_id(frame), tag) == 0)

    while ((frame = ID3_read(file))) {

             if (FRAME_ID_EQUAL("TIT2") && strcmp(ID3_frame_data(frame), "Apostlarna") == 0)        { TIT2 = true; }
        else if (FRAME_ID_EQUAL("TPE1") && strcmp(ID3_frame_data(frame), "Tolvan Tolvansson") == 0) { TPE1 = true; }
        else if (FRAME_ID_EQUAL("TALB") && strcmp(ID3_frame_data(frame), "Best of Tolvan") == 0)    { TALB = true; }
        else if (FRAME_ID_EQUAL("TRCK") && strcmp(ID3_frame_data(frame), "1/12") == 0)              { TRCK = true; }
        else if (FRAME_ID_EQUAL("TCON") && strcmp(ID3_frame_data(frame), "Twelve tone music") == 0) { TCON = true; }
        else if (FRAME_ID_EQUAL("TYER") && strcmp(ID3_frame_data(frame), "1912") == 0)              { TYER = true; }

        ID3_frame_free(&frame);
    }
#undef FRAME_ID_EQUAL

    CU_ASSERT(TPE1 == true);
    CU_ASSERT(TYER == true);
    CU_ASSERT(TALB == true);
    CU_ASSERT(TIT2 == true);
    CU_ASSERT(TRCK == true);
    CU_ASSERT(TCON == true);


    ID3_close(&file);
}


void test_ID3_frame_free(void) {
    ID3_file *file = ID3_open(file_has_id3);
    CU_ASSERT(file != NULL);

    ID3_frame *frame = NULL;

    frame = ID3_read(file);
    CU_ASSERT(frame != NULL);

    ID3_frame_free(&frame);
    CU_ASSERT(frame == NULL);

    ID3_close(&file);
    CU_ASSERT(file == NULL);
}

void test_ID3_frame_id(void) {
    ID3_file *file = ID3_open(file_has_id3);
    CU_ASSERT(file != NULL);

    ID3_frame *frame = NULL;
    frame = ID3_read(file);
    CU_ASSERT(frame != NULL);

    CU_ASSERT(strcmp(ID3_frame_id(frame), "TPE1") == 0);

    ID3_frame_free(&frame);
    CU_ASSERT(frame == NULL);

    ID3_close(&file);
    CU_ASSERT(file == NULL);
}

void test_ID3_frame_data(void) {
    ID3_file *file = ID3_open(file_has_id3);
    CU_ASSERT(file != NULL);

    ID3_frame *frame = NULL;
    frame = ID3_read(file);
    CU_ASSERT(frame != NULL);

    char *data = ID3_frame_data(frame);
    CU_ASSERT(strcmp(data, "Tolvan Tolvansson") == 0);
    free(data);

    ID3_frame_free(&frame);
    CU_ASSERT(frame == NULL);

    ID3_close(&file);
    CU_ASSERT(file == NULL);
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
            (NULL == CU_add_test(pSuite1, "test of ID3_open                ", test_ID3_open)) ||
            (NULL == CU_add_test(pSuite1, "test of ID3_read                ", test_ID3_read)) ||
            (NULL == CU_add_test(pSuite1, "test of ID3_close               ", test_ID3_close)) ||
            (NULL == CU_add_test(pSuite1, "test of ID3_frame_free          ", test_ID3_frame_free)) ||
            (NULL == CU_add_test(pSuite1, "test of ID3_frame_id            ", test_ID3_frame_id)) ||
            (NULL == CU_add_test(pSuite1, "test of ID3_frame_data          ", test_ID3_frame_data))
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

