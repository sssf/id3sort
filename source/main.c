#include <stdlib.h>
#include <stdio.h>
#include "io.h"
#include "id3_sort.h"





// flags
// -v --verbose
// -t --tree
// --no_move


int main(int argc, char *argv[]) {
    // make sure we got proper amount of arguments
    if (argc < 3) {
        printf("id3_sort <target> <list of files>\n");
        return 0;
    }

    // make sure target directory exist
    char *target = argv[1];
    if (!IO_directory_exist(target)) {
        printf("directory does not exist: %s\n", target);
        return 1;
    }

    //uint32_t flags = ID3_parse_flags(arg)

    for (int n = 2; n < argc; n++) {
        ID3_sort_file(argv[n], target);
    }

    // have to flush stdout otherwise the output of tree will be printed before everything else
    fflush(stdout);
    system("tree bin/music");
    return 0;
}
