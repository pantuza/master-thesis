#include "file.h"
#include "brute.h"


int main(int argc, char argv[])
{
    FILE *file = open_file("inputs/00_small_input");
    printf("file: %d\n\n", file);

    int min_seq_size = get_seq_size(file);

    while(min_seq_size != 0)
    {
        Word word0 = word_handler(file);
        Word word1 = word_handler(file);

        printf("k: %d\nword0: %sword1: %s\n", min_seq_size, word0.letters, word1.letters);
        min_seq_size = get_seq_size(file);
    }
    
    close_file(file);
    return 0;
}
