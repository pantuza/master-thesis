#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>

#include "chronos.h"
#include "strtype.h"
#include "file.h"
#include "dynamic.h"
#include "brute.h"
#include "greedy.h"

void rotate(char* word, int size, char start, char end) {
    for (int i = size-1; i >= 0; i--)
    {
        if (word[i] >= end)
            word[i] = start;
        else
        {
            word[i]++;
            break;
        }
    }
}

void generate_test(FILE* output,
        int k, int sw0, char start, char end, char* word1)
{
    int i;
    char word0[sw0+1];
    int sw1 = strlen(word1);

    for(i = 0; i < sw0; i++)
        word0[i] = 'a';
    word0[sw0] = 0;

    int max = pow(end - start + 1, sw0);
    long preview = max*(sw0+sw1+4);
    printf("Generating %d tests in %ld bytes...\n", max, preview);
    for (i = 1; i <= max; i++)
    {
        fprintf(output, "%d\n", k);
        fprintf(output, "%s\n", word0);
        fprintf(output, "%s\n", word1);
        rotate(word0, sw0, start, end);
    }
}

void generate_full_test(FILE* output,
        int k, int sw0, char start0, char end0, 
               int sw1, char start1, char end1)
{
    int i, j;
    char word0[sw0+1], word1[sw1+1];

    for(i = 0; i < sw0; i++)
        word0[i] = 'a';
    word0[sw0] = 0;

    int max0 = pow(end0 - start0 + 1, sw0);
    int max1 = pow(end1 - start1 + 1, sw1);
    long preview = max0*max1*(sw0+sw1+4);
    printf("Generating %d tests in %ld bytes...\n", max0*max1, preview);

    for (i = 1; i <= max0; i++)
    {
        for(j = 0; j < sw1; j++)
            word1[j] = 'a';
        word1[sw1] = 0;
	for (j = 1; j <= max1; j++) {
            fprintf(output, "%d\n", k);
            fprintf(output, "%s\n", word0);
            fprintf(output, "%s\n", word1);
            rotate(word1, sw1, start1, end1);
        }
        rotate(word0, sw0, start0, end0);
    }
}

void generate_random_test(FILE* output,
        int k, int sw0, int sw1)
{
    char word0[sw0+1], word1[sw1+1];
    int i;
    for(i = 0; i < sw0; i++)
        word0[i] = (char)('a' + (rand() % 26));
    word0[sw0] = 0;

    for(i = 0; i < sw1; i++)
        word1[i] = (char)('a' + (rand() % 26));

    word1[sw1] = 0;

    fprintf(output, "%d\n", k);
    fprintf(output, "%s\n", word0);
    fprintf(output, "%s\n", word1);

}

unsigned long long int brute_full_test(FILE* output,
        int k, int sw0, char start0, char end0,
               int sw1, char start1, char end1)
{
    int i, j;
    int l0, l1;
    char word0[sw0+1], word1[sw1+1];

    for(i = 0; i < sw0; i++)
        word0[i] = 'a';
    word0[sw0] = 0;

    unsigned long long int err = 0, tot = 0;
    unsigned long long int max0 = pow(end0 - start0 + 1, sw0);
    unsigned long long int max1 = pow(end1 - start1 + 1, sw1);
    unsigned long long int preview = max0*max1;
    int msg = 0;
    printf("Tests = %lld \n", preview);

    for (i = 1; i <= max0; i++)
    {
        for(j = 0; j < sw1; j++)
            word1[j] = 'a';
        word1[sw1] = 0;
        for (j = 1; j <= max1; j++) {
            tot++;
            if (++msg >= 100000) {
                msg = 0;
                printf("Executados %lld de %lld testes (com %lld erros).\n", tot, preview, err);
            }
            l0 = dynamic(k, word0, sw0, word1, sw1);
            l1 = brute_force(k, word0, sw0, word1, sw1);
            if (l0 != l1) {
                fprintf(output, "%d,%d,%d,%d,%d,%s,%s\n", l0, l1,
                        sw0, sw1, k, word0, word1);
                err++;
            }
            rotate(word1, sw1, start1, end1);
        }
        rotate(word0, sw0, start0, end0);
    }
    fprintf(output,"Total = %lld\n", tot);
    fprintf(output,"Error = %lld\n", err);
    printf("Finalizados %lld de %lld testes (com %lld erros).\n", tot, preview, err);
    return err;
}

void test_case1(char *file)
{
    char filename[1024];
    sprintf(filename, "inputs/%s", file);

    FILE *f = fopen(filename, "w");
    int max = 1000;
    printf("Random test file %s\n", file);
    printf("Generating %d tests...\n", 4*max);
    for (int i = 0; i <= 100; i++) {
        generate_random_test(f, 4, 20, 20);
        generate_random_test(f, 3, 15, 15);
        generate_random_test(f, 2, 12, 12);
        generate_random_test(f, 1, 8, 8);
    }

    fprintf(f, "0");
    fclose(f);
}

void test_case2(char *file)
{
    char filename[1024];
    sprintf(filename, "inputs/%s", file);

    FILE *f = fopen(filename, "w");
    printf("Test file %s\n", file);
    generate_test(f, 2, 12, 'a', 'c', "aaaaa");
    generate_test(f, 3, 10, 'a', 'd', "abcdeabcd");
    fprintf(f, "0");
    fclose(f);
}

void test_case3(char *file)
{
    char filename[1024];
    sprintf(filename, "inputs/%s", file);

    FILE *f = fopen(filename, "w");
    printf("Full test file %s\n", file);
    generate_full_test(f, 3, 8, 'a', 'd', 7, 'a', 'c');
    //generate_full_test(f, 3, 6, 'a', 'c', 5, 'a', 'd');
    fprintf(f, "0");
    fclose(f);
}

void sweep_input_compare(FILE *file, FILE *diff0, FILE *diff1)
{
    if (!(diff0 || diff1))
        return;

    int lcs[3], tot, fail, err[2];
    // the string must contain space for termination mark ('\0')
    char word0[WORD_SIZE + 1], word1[WORD_SIZE + 1];

    int size0, size1;

    // read the first k (zero implies end of execution)
    int k = get_segment_size(file);

    tot = 0;
    fail = 0;
    err[0] = 0;
    err[1] = 0;
    lcs[0] = 0;
    lcs[1] = 0;
    lcs[2] = 0;

    // while exist input case to execute (test case)
    while(k != 0)
    {
        tot++;
        // reads both words
        size0 = get_word(file, word0);
        size1 = get_word(file, word1);

        // is really there words?
        if (size0 && size1) {
            // sort the arguments (words)
            if (size0 <= size1)
            {
                lcs[0] = dynamic(k, word0, size0, word1, size1);
                if (diff0)
                    lcs[1] = brute_force(k, word0, size0, word1, size1);
                if (diff1)
                    lcs[2] = greedy(k, word0, size0, word1, size1);
            } else {
                lcs[0] = dynamic(k, word1, size1, word0, size0);
                if (diff0)
                    lcs[1] = brute_force(k, word1, size1, word0, size0);
                if (diff1)
                    lcs[2] = greedy(k, word1, size1, word0, size0);
            }
            if (diff0 && lcs[0] != lcs[1]) {
                fprintf(diff0, "%d,%d,%d,%d,%d,%s,%s\n", lcs[0], lcs[1], 
                        size0, size1, k, word0, word1);
                err[0]++;
            }
            if (diff1 && lcs[0] != lcs[2]) {
                fprintf(diff1, "%d,%d,%d,%d,%d,%s,%s\n", lcs[0], lcs[2],
                        size0, size1, k, word0, word1);
                err[1]++;
            }
        }
        else
        {
            // returns zero, that always is a valid size of LCS,
            // even in this system error condition (empty line or word)
            if (diff0)
                fprintf(diff0, "-,-,%d,%d,%d,%s,%s\n",
                        size0, size1, k, word0, word1);
            if (diff1)
                fprintf(diff1, "-,-,%d,%d,%d,%s,%s\n",
                        size0, size1, k, word0, word1);
            fail++;
        }

        // read another instance of k (zero implies end of execution)
        k = get_segment_size(file);
    }
    if (diff0)
    {
        fprintf(diff0,"Total = %d\n", tot);
        fprintf(diff0,"Fails = %d\n", fail);
        fprintf(diff0,"Error = %d\n", err[0]);
    }
    if (diff1)
    {
        fprintf(diff1,"Total = %d\n", tot);
        fprintf(diff1,"Fails = %d\n", fail);
        fprintf(diff1,"Error = %d\n", err[1]);
    }
}

void do_compare_brute(char *file)
{
    char filename[1024];
    char diff0_filename[1024];

    sprintf(filename, "inputs/%s", file);
    sprintf(diff0_filename, "outputs/%s-diff-brute.txt", file);

    printf("Input: %s \t Output: %s\n", filename, diff0_filename);
    FILE *f, *d0;
    d0 = fopen(diff0_filename, "w");
    f = open_file(filename);
    sweep_input_compare(f, d0, NULL);
    close_file(f);
    fclose(d0);
}

void do_compare_greedy(char *file)
{
    char filename[1024];
    char diff0_filename[1024];

    sprintf(filename, "inputs/%s", file);
    sprintf(diff0_filename, "outputs/%s-diff-greedy.txt", file);

    printf("Input: %s \t Output: %s\n", filename, diff0_filename);
    FILE *f, *d0;
    d0 = fopen(diff0_filename, "w");
    f = open_file(filename);
    sweep_input_compare(f, NULL, d0);
    close_file(f);
    fclose(d0);
}

void do_compare(char *file)
{
    char filename[1024];
    char diff0_filename[1024];
    char diff1_filename[1024];

    sprintf(filename, "inputs/%s", file);
    sprintf(diff0_filename, "outputs/%s-diff-brute.txt", file);
    sprintf(diff1_filename, "outputs/%s-diff-greedy.txt", file);

    printf("Input: %s \t Output: %s, %s\n", filename,
                     diff0_filename, diff1_filename);
    FILE *f, *d0, *d1;
    d0 = fopen(diff0_filename, "w");
    d1 = fopen(diff1_filename, "w");
    f = open_file(filename);
    sweep_input_compare(f, d0, d1);
    close_file(f);
    fclose(d0);
    fclose(d1);
}

unsigned long long int test_case4(int k, int w0, int w1)
{
    char filename[1024];
    sprintf(filename, "outputs/k%d-%d-%d-mem-brute.txt", k, w0, w1);
    FILE *f = fopen(filename, "w");
    unsigned long long int err;
    err = brute_full_test(f, k, w0, 'a', 'd', w1, 'a', 'd');
    fclose(f);
    printf("Full test file %s\n", filename);
    return err;
}

void test_case4_1(int n)
{
    unsigned long long int err = 0;
    for (int k = 1; k <= n; k++)
        for (int w0 = 1; w0 <= n; w0++)
            for (int w1 = 1; w1 <= n; w1++)
                err += test_case4(k, w0, w1);
    printf("Total errors = %lld\n", err);
}
int main(int argc, char** argv)
{
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);

    //    char* filename1 = "01_test_input";
    //char* filename2 = "02_test_input";
    //char* filename3 = "04_test_input";


//    test_case1(filename1);
    //test_case2(filename2);
    //test_case3(filename3);
    test_case4_1(6);
/*
    do_test(filename1, "brute_force", brute_force);
    do_test(filename2, "brute_force", brute_force);
    
    do_test(filename1, "dynamic", dynamic);
    do_test(filename2, "dynamic", dynamic);

    do_test(filename1, "greedy", greedy);
    do_test(filename2, "greedy", greedy);
*/

//    do_compare(filename1);
    //do_compare_brute(filename2);
    //do_compare_brute(filename3);

    return 0;
}

