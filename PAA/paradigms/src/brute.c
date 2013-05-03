#include "file.h"
#include "brute.h"


int main(int argc, char argv[])
{
    FILE file = open_file("inputs/00_small_input");
    printf("file: %d\n", file);

    int min_seq_size = get_seq_size(&file);

    while(min_seq_size != 0)
    {
        Word word0 = word_handler(&file);
        Word word1 = word_handler(&file);

        min_seq_size = get_seq_size(&file);

        printf("k: %d\nword0: %s\nword1: %s\n", min_seq_size, word0.letters, word1.letters);
    }
    
    return 0;
}
