/*
 * chronos.h
 *
 *  Created on: 12/05/2013
 *      Author: dev
 */

#ifndef CHRONOS_H_
#define CHRONOS_H_

void start_timer();
void stop_timer();
FILE *open_stat_file(char *);
void write_stat(FILE *, char *, char *, int, int, int, unsigned long long int);

#endif /* CHRONOS_H_ */
