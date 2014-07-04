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
#include <sys/time.h>
#include "common.h"

//function prototypes for helped functions.
double get_time_in_seconds();
int wait_on_child(double time_before_fork, double time_after_fork);
int detect_user_error(int, char**);
void produce_and_send_elements (int,mqd_t);
int produce_element();

int main(int argc, char **argv) {

	if (detect_user_error(argc, argv)) {
		printf("Invalid arguments\n");
		return 0;
	}

	int process_count = atoi(argv[1]);
	int queue_size = atoi(argv[2]);

	mode_t mode = S_IRUSR | S_IWUSR;

	struct mq_attr queue_attributes;

	queue_attributes.mq_maxmsg = queue_size;
	queue_attributes.mq_msgsize = sizeof(int);
	queue_attributes.mq_flags = 0;

	mqd_t queue_descriptor;

	queue_descriptor = mq_open(queue_name, O_RDWR | O_CREAT, mode,
			&queue_attributes);

	if (queue_descriptor == -1) {
		printf("there was an error making the queue");
		return 1;
	}

	pid_t child_pid;
	double time_before_fork = get_time_in_seconds();
	double time_afer_fork;

	child_pid = fork();

	if (child_pid != 0) {

		time_afer_fork = get_time_in_seconds();
		srand(time(0));

		produce_and_send_elements(process_count,queue_descriptor);


} else {

	execv("./consumer", argv);
	//We only reach this point if the consumer failed to properly exec.
	printf("error which exec'ing the consumer");
}

wait_on_child(time_before_fork, time_afer_fork);

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

int detect_user_error(int argc, char* argv[]) {

if (argc < 3) {

	return 1;
}

return 0;

}

//produce a random number
int produce_element() {

return ((rand() % 80));
}

void produce_and_send_elements(int process_count,mqd_t  queue_descriptor) {
int i;
for (i = 0; i < process_count; ++i) {

	int element = produce_element();

	if (mq_send(queue_descriptor, (char*) &element, sizeof(int), 0) == -1) {
		perror("send operation failed");
	}
}

}

int wait_on_child(double time_before_fork, double time_after_fork) {
int child_status;
wait(&child_status);

if (WIFEXITED(child_status)) {

	double time_after_last_consumed = get_time_in_seconds();

	printf("Time to initialize system: %f seconds\n",
			time_after_fork - time_before_fork);
	printf("Time to transmit data: %f seconds\n",
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

