/*
 * Producer.c
 * ECE254 Group 01
 * By :  Rushan Yogaratnam and Ameen Patel
 * University of Waterloo Computer Engineering
 * Spring 2014
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <string.h>

//Include helper functions and global constants
#include "producer.h"
#include "common.h"

int main(int argc, char **argv) {

	int queue_size;
	int process_count;

	//check if the arguments are valid if so set them
	if (process_arguments(argc, argv, &queue_size, &process_count)) {
		printf("Invalid arguments\n");
		return 1;
	}

	//create queue attributes
	struct mq_attr queue_attributes;
	queue_attributes.mq_maxmsg = queue_size;
	queue_attributes.mq_msgsize = sizeof(int);
	queue_attributes.mq_flags = 0;

	mqd_t queue_descriptor;
	mode_t permissions = S_IRUSR | S_IWUSR;

	//attempt to open the queue
	queue_descriptor = mq_open(queue_name, O_RDWR | O_CREAT, permissions,
			&queue_attributes);

	//check if the queue was opened
	if (queue_descriptor == -1) {
		printf("error creating the queue %s\n", strerror(errno));
		return 1;
	}

	double time_before_fork;

	if (spawn_consumer("consumer", argv, queue_descriptor, &time_before_fork)
			!= -1) {
		double time_afer_fork = get_time_in_seconds();
		produce_and_send_elements(process_count, queue_descriptor);
		wait_on_child(time_before_fork, time_afer_fork);
	}

	//Close and mark the queue for deletion
	if (mq_close(queue_descriptor) == -1) {
		perror("mq_close failed");
		exit(2);
	}

	if (mq_unlink(queue_name) != 0) {
		perror("mq_unlink failed");
		exit(3);
	}

	return 0;

}
int process_arguments(int argc, char* argv[], int * queue_size,
		int * proc_count) {

	if (argc < 3) {
		return 1;
	} else {
		*proc_count = atoi(argv[1]);
		*queue_size = atoi(argv[2]);

		return ((*proc_count <= 0 || *queue_size <= 0)) ? 1 : 0;
	}

}

int spawn_consumer(char* program, char **arg_list, mqd_t queue_descriptor,
		double * fork_time) {

	arg_list[0] = program;
	pid_t child_pid;

	//get the time before fork.
	*fork_time = get_time_in_seconds();
	child_pid = fork();

	if (child_pid > 0) {
		return child_pid;
	} else if (child_pid < 0) {
		printf("error creating the child process %s\n", strerror(errno));
		return -1;
	} else {
		execvp("./consume", arg_list);
		printf("error occurred in execvp %s\n", strerror(errno));
		abort();
	}

}

void produce_and_send_elements(int process_count, mqd_t queue_descriptor) {

	srand(time(0));
	int i;

	for (i = 1; i <= process_count; ++i) {

		int message = ((rand() % 80));

		if (mq_send(queue_descriptor, (char*) &message, sizeof(int), 0) == -1) {
			printf("error sending the message %s\n", strerror(errno));
		}
	}
}

int wait_on_child(double time_before_fork, double time_after_fork) {

	int child_status;
	wait(&child_status);

	if (WIFEXITED(child_status)) {

		double time_after_last_consumed = get_time_in_seconds();

		printf("time to initialize system: %f seconds\n",
				time_after_fork - time_before_fork);
		printf("time to transmit data: %f seconds\n",
				time_after_last_consumed - time_after_fork);
		return 0;

	} else {
		printf("child process exited abnormally \n");
		return 1;
	}

}
double get_time_in_seconds() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec + tv.tv_usec / 1000000.0);

}

