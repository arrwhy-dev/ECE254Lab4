/*
 * Producer.c
 * ECE254 Group 01
 * By :  Rushan Yogaratnam and Ameen Patel
 * University of Waterloo Computer Engineering
 * Spring 2014
 *
 */
#include <mqueue.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "common.h"

int main(int argc, char **argv)
{

	mode_t mode = S_IRUSR | S_IWUSR;
	struct mq_attr queue_attributes;

	int queue_size = atoi(argv[2]);
	int messages_to_consume = atoi(argv[1]);


	queue_attributes.mq_maxmsg = queue_size;
	queue_attributes.mq_msgsize = sizeof(int);
	queue_attributes.mq_flags = 0;

	mqd_t qdes;

	qdes = mq_open(queue_name, O_RDONLY, mode, &queue_attributes);

	if (qdes == -1) {
		printf("there was an error opening the queue in the consumer");
		printf("the error is %s \n", strerror(errno));
		return 1;
	}

	int i;
	for (i = 0; i < messages_to_consume; ++i) {

		int i;
		if (mq_receive(qdes, (char*) &i, sizeof(int), 0) == -1) {
			printf("failure to recieve the msg\n");
			perror("error retrieveing from the queue");
			return 1;
		} else {
			printf("%i is consumed\n", i);

		}
	}

	if (mq_close(qdes) == -1) {
		perror("mq_close90 failed");
		exit(2);
	}

	return 0;

}
