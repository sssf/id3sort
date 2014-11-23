GCOV = -fprofile-arcs -ftest-coverage
COMPILER = gcc -std=c99
BUILD_FLAGS = -Wall -pg -ggdb -c
LINK_FLAGS = -Wall -pg -ggdb -o
TARGET = bin/main

TEST_PASS = 's/...passed/\x1b[1;32m   PASSED\x1b[m/g'
TEST_FAIL = 's/...FAILED/\x1b[1;31m   FAILED\x1b[m/g'
FORMAT_TEST_OUTPUT = ./tests/format_test_output.sh

#Build and run
all: main

main: main.o id3.o io.o id3_helper.o id3_sort.o
	mkdir -p bin
	mkdir -p bin/music
	cp data/* bin/data/
	$(COMPILER) $(LINK_FLAGS) $(TARGET) main.o id3.o io.o id3_helper.o id3_sort.o remember.o
	./$(TARGET) bin/music bin/data/*.mp3

id3.o: source/id3.c source/id3.h
	$(COMPILER) $(BUILD_FLAGS) source/id3.c

main.o: source/main.c source/id3.h source/id3.c
	$(COMPILER) $(BUILD_FLAGS) source/main.c

io.o: source/io.c source/io.h
	$(COMPILER) $(BUILD_FLAGS) source/io.c

id3_helper.o: source/id3_helper.c source/id3_helper.h
	$(COMPILER) $(BUILD_FLAGS) source/id3_helper.c

id3_sort.o: source/id3_sort.c source/id3_sort.h
	$(COMPILER) $(BUILD_FLAGS) source/id3_sort.c

# Build and run test
test: test_io test_id3 test_id3_helper test_id3_sort
	@echo;
	@echo "ALL YOUR BASE ARE BELONING TO US";


test_io: source/io.h source/io.c source/io.h
	@mkdir -p bin
	@$(COMPILER) $(LINK_FLAGS) bin/test_io source/io.c tests/test_io.c -lcunit
	@./bin/test_io | sed $(TEST_PASS)  | sed $(TEST_FAIL)


test_id3: source/id3.h source/id3.c tests/test_id3.c
	@mkdir -p bin
	@$(COMPILER) $(LINK_FLAGS) bin/test_id3 source/id3.c tests/test_id3.c -lcunit
	@./bin/test_id3 | sed $(TEST_PASS)  | sed $(TEST_FAIL)


test_id3_helper: source/id3.c source/id3.h source/id3_helper.h source/id3_helper.c tests/test_id3_helper.c
	@mkdir -p bin
	@$(COMPILER) $(LINK_FLAGS) bin/test_id3_helper source/id3.c source/id3_helper.c tests/test_id3_helper.c -lcunit
	@./bin/test_id3_helper | sed $(TEST_PASS)  | sed $(TEST_FAIL)

test_id3_sort: source/id3_sort.c source/id3_sort.h source/id3.c source/id3.h source/io.c source/io.h source/id3_helper.c source/id3_sort.h
	@mkdir -p bin/test_music
	@rm -rf bin/test_music/*
	@cp bin/test_data/music0.mp3  bin/test_data/file_has_all_frames.mp3
	@cp bin/test_data/music39.mp3 bin/test_data/file_missing_album.mp3
	@cp bin/test_data/music38.mp3 bin/test_data/file_missing_frames.mp3
	@$(COMPILER) $(LINK_FLAGS) bin/test_id3_sort source/id3_sort.c source/id3.c source/io.c source/id3_helper.c tests/test_id3_sort.c -lcunit
	@./bin/test_id3_sort | sed $(TEST_PASS)  | sed $(TEST_FAIL)



valgrind: clean
	mkdir -p bin
	mkdir -p bin/music
	cp data/* bin/data/
	valgrind --leak-check=full -v ./$(TARGET) bin/music bin/data/*.mp3

clean:
	#rm $(TARGET)
	rm -f main.o id3.o io.o id3_helper.o id3_sort.o *.c.gcov *.gcda *.gcno
	rm -rf bin/music/*

tar:
	tar cfv source.tar source/*

#.PHONY: valgrind
