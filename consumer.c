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
int main(int argc, char **argv) {

	printf("printing from the consumer\n");

	//lets try to open the queue in the consumer.

	mode_t mode = S_IRUSR | S_IWUSR;
	struct mq_attr queue_attributes;

	int queue_size = atoi(argv[2]);

	char * queue_name = "/mailbox_ece254_ryo_ap";

	queue_attributes.mq_maxmsg = queue_size;
	queue_attributes.mq_msgsize = sizeof(int);
	queue_attributes.mq_flags = 0;

	mqd_t qdes;

	qdes = mq_open(queue_name, O_RDONLY, mode, &queue_attributes);

	if (qdes == -1) {
		printf("there was an error opening the queue in the consumer");
		printf("the error is %s \n", strerror(errno));
		return 1;
	} else {
		printf("queue was opened in consumer\n");
	}

	int i;
	if (mq_receive(qdes, (char*) &i, sizeof(int), 0) == -1) {
		perror("errore retrieveing from the queue");
		return 1;
	} else {
		printf("the value consumed is %i \n", i);

	}

	if (mq_close(qdes) == -1) {
		perror("mq_close90 failed");
		exit(2);
	}

	if (mq_unlink(queue_name) != 0) {
		perror("mq_unlink failed");
		exit(3);
	}

	return 0;

}
