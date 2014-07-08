/*
 * Producer.h
 * ECE254 Group 01
 * By :  Rushan Yogaratnam and Ameen Patel
 * University of Waterloo Computer Engineering
 * Spring 2014
 *
 *
 *      These are a bunch of helper functions for the
 *      producer.c file. The are just functions which
 *      make the code cleaner by providing abstraction.
 *
 */

#ifndef PRODUCER_H_
#define PRODUCER_H_

double get_time_in_seconds();
int wait_on_child(double time_before_fork, double time_after_fork);
int process_arguments(int, char**, int *, int *);
int spawn_consumer(char*, char **,mqd_t ,double *);
void produce_and_send_elements(int, mqd_t);

#endif /* PRODUCER_H_ */
