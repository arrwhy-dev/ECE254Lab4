
#include <stdio.h>

#include <stdlib.h>

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

	return 0;

}

int detect_user_error(int argc, char* argv[]) {

	if (argc < 3) {

		return 1;

	}

	return 0;

}
