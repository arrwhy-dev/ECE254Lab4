
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char **argv) {

	printf("printing from the consumer\n");


	//lets try to open the queue in the consumer.

	mode_t mode = S_IRUSR | S_IWUSR;
	struct mq_attr attr;
	char * queue_name = "/mailbox_ece254_ryo_ap";

	int queue_size = atoi(argv[2]);

	attr.mq_msgsize = sizeof(int);
	attr.mq_flags = 0;
	attr.mq_maxmsg = queue_size;

	mqd_t qdes;

	qdes = mq_open(queue_name, O_RDONLY, mode, &attr);

	if (qdes == -1) {
		printf("there was an error making the queue");
		return 1;
	} else {
		printf("queue was opened in consumer\n");
		return 0;
	}



	return 0;

}
