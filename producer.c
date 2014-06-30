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
#include <unistd.h>

int detect_user_error();

int main(int argc, char **argv) {
	if (detect_user_error(argc, argv)) {
		printf("Invalid arguments\n");
		return 0;
	}

	int process_count = atoi(argv[1]);

	int queue_size = atoi(argv[2]);

	printf("The num of ints to process is %d and the queue size is %d\n",

	process_count, queue_size);

	mode_t mode = S_IRUSR | S_IWUSR;
	struct mq_attr attr;
	char * queue_name = "/mailbox_ece254_ryo_ap";

	attr.mq_maxmsg = queue_size;
	attr.mq_msgsize = sizeof(int);
	attr.mq_flags = 0;

	mqd_t qdes;

	qdes = mq_open(queue_name, O_RDWR | O_CREAT, mode, &attr);

	if (qdes == -1) {
		printf("there was an error making the queue");
		return 1;
	} else {
		printf("queue was opened\n");
	}

	pid_t child_pid;

	child_pid = fork();

	if (child_pid != 0) {
		printf("printing from the parent\n");
	} else {

		execv("./consumer", argv);

		printf("error making the consumer");
	}

	return 0;

}

int detect_user_error(int argc, char* argv[]) {

	if (argc < 3) {

		return 1;

	}

	return 0;

}
