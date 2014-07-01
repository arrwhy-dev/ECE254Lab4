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
#include <time.h>
#include "producer.h"

int main(int argc, char **argv) {

	if (detect_user_error(argc, argv)) {
		printf("Invalid arguments\n");
		return 0;
	}

	int process_count = atoi(argv[1]);
	int queue_size = atoi(argv[2]);

	mode_t mode = S_IRUSR | S_IWUSR;

	struct mq_attr queue_attributes;
	char * queue_name = "/mailbox_ece254_ryo_ap2";

	queue_attributes.mq_maxmsg = queue_size;
	queue_attributes.mq_msgsize = sizeof(int);
	queue_attributes.mq_flags = 0;

	mqd_t queue_descriptor;

	//open the message queue
	queue_descriptor = mq_open(queue_name, O_RDWR | O_CREAT, mode,
			&queue_attributes);

	if (queue_descriptor == -1) {
		printf("there was an error making the queue");
		return 1;
	} else {
		printf("queue was opened\n");
	}

	pid_t child_pid;

	child_pid = fork();

	if (child_pid != 0) {
		//printf("printing from the parent\n");
		srand(time(0));

		int counter;
		for (counter = 0; counter < process_count; ++counter) {
			int i = (rand() % 80);
			if (mq_send(queue_descriptor, (char*) &i, sizeof(int), 0) == -1) {
				perror("send operation failed");
			} else {
				printf("sent value of %i",i);
			}
		}

	} else {

		execv("./consumer", argv);

		printf("error making the consumer");
	}

	printf("parent going to enter wait");
	//send a message to the consumer.

	//wait on child process before terminating.

	int child_status;
	wait(&child_status);

	if (WIFEXITED(child_status)) {
		printf("the child prcess exited normally, with exit cod %d\n",
				WEXITSTATUS(child_status));
	} else {
		printf("child process exited abnormally \n");
		return 1;
	}

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

