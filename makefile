COMPILER = gcc -std=c99
BUILD_FLAGS = -Wall -pg -ggdb -c
LINK_FLAGS = -Wall -pg -ggdb -o
TARGET = bin/main

TEST_PASS = 's/...passed/\x1b[1;32m   PASSED\x1b[m/g'
TEST_FAIL = 's/...FAILED/\x1b[1;31m   FAILED\x1b[m/g'

all: test_all

main: main.o id3.o io.o id3_helper.o id3_sort.o
	mkdir -p bin
	mkdir -p bin/music
	cp data/* bin/data/
	$(COMPILER) $(LINK_FLAGS) $(TARGET) main.o id3.o io.o id3_helper.o id3_sort.o
	./$(TARGET) bin/music bin/data/*.mp3
	cp data/* bin/data/
	#echo;
	#tree bin/music

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


#commit: clean test_all
#	git pull; git commit; git push


test_all: test_io test_id3 test_id3_helper


test_io: test/test_io.c source/io.c source/io.h
	mkdir -p bin
	$(COMPILER) $(LINK_FLAGS) bin/test_io source/io.c test/test_io.c -lcunit
	./bin/test_io | sed $(TEST_PASS)  | sed $(TEST_FAIL)


test_id3: test/test_id3.c source/id3.c source/id3.h
	mkdir -p bin
	$(COMPILER) $(LINK_FLAGS) bin/test_id3 source/id3.c test/test_id3.c -lcunit
	./bin/test_id3 | sed $(TEST_PASS)  | sed $(TEST_FAIL)


test_id3_helper: test/test_id3_helper.c source/id3_helper.c source/id3_helper.h id3.o
	mkdir -p bin
	$(COMPILER) $(LINK_FLAGS) bin/test_id3_helper source/id3_helper.c test/test_id3_helper.c id3.o -lcunit
	./bin/test_id3_helper | sed $(TEST_PASS)  | sed $(TEST_FAIL)



valgrind:
	mkdir -p bin
	mkdir -p bin/music
	cp data/* bin/data/
	valgrind --leak-check=yes ./$(TARGET) bin/music bin/data/*.mp3

clean:
	#rm $(TARGET)
	rm -f main.o id3.o io.o id3_helper.o id3_sort.o
	rm -rf bin/music/*

tar:
	tar cfv source.tar source/*

.PHONY: valgrind
