/*
NOTAS:
1) Baseado no programa original time.c

   `time' utility to display resource usage of processes.
   Copyright (C) 1990, 91, 92, 93, 96 Free Software Foundation, Inc.

   Licensed under GPLv2, see file LICENSE in this tarball for details.

   Originally written by David Keppel <pardo@cs.washington.edu>.
   Heavily modified by David MacKenzie <djm@gnu.ai.mit.edu>.
   Heavily modified for busybox by Erik Andersen <andersen@codepoet.org>

3) Uso indicado:
    Restrito a apenas um programa (não é multithread)
    pois possui apenas uma variável de controle (resp).
    a) no início do programa chamar: start_time();
    b) no final chamar             : stop_time();
    c) depois de stop_resource     : write_resource(stderr);
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>

#include "chronos.h"

/* estrutura que registra os recursos (privada no módulo)*/
typedef struct
 {
   struct rusage ru;
   struct timeval start, elapsed; /* Wallclock time of process.  */
 } resource_t;

/* variável estática, ou seja, pública apenas no módulo (neste programa)*/
static resource_t resp;

/* inicia a contagem de tempo */
void start_timer()
{
     gettimeofday (&resp.start, (struct timezone *) 0);
}

/* para a contagem de tempo e recolhe informações de recursos */
void stop_timer()
{
     getrusage(RUSAGE_SELF, &resp.ru);
     gettimeofday (&resp.elapsed, (struct timezone *) 0);
     resp.elapsed.tv_sec -= resp.start.tv_sec;
     if (resp.elapsed.tv_usec < resp.start.tv_usec) {
         /* Manually carry a one from the seconds field.  */
         resp.elapsed.tv_usec += 1000000;
         --resp.elapsed.tv_sec;
     };
     resp.elapsed.tv_usec -= resp.start.tv_usec;
}

// Abre um arquivo de análise estatística e coloca cabeçalho se vazio
FILE *open_stat_file(char *arq)
{
    FILE *harq;

    /* Abre o arquivo para escrita (APPEND) */
    if((harq = fopen(arq ,"a"))) {
        //se arquivo vazio, grava o cabeçalho
        if (ftell(harq) == 0)
           fprintf(harq, "id; method; n; m; k; cost; real; user; sys\n");
        return harq;
    }
    fprintf(stderr, "Error on open analysis file: %s\n", arq);
    exit(9);
    return NULL;

}

// escreve dados para análise
void write_stat(FILE *harq, char *id, char *method, int n, int m, int k,
        unsigned long long int cost)
{
        //grava a linha de dados
        fprintf(harq, "%s;%s;%d;%d;%d;%lld;%ld.%06ld;%ld.%06ld;%ld.%06ld\n",
            id, method, n, m, k, cost,
            resp.elapsed.tv_sec, resp.elapsed.tv_usec,
            resp.ru.ru_utime.tv_sec, resp.ru.ru_utime.tv_usec,
            resp.ru.ru_stime.tv_sec, resp.ru.ru_stime.tv_usec);

}

