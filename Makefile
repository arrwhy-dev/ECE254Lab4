all: producer

producer: producer.o
	gcc -o producer producer.o -lrt

producer.o: consumer
	gcc -c producer.c -lrt

consumer: consumer.o
	gcc -o consumer consumer.o -lrt

consumer.o: consumer.c
	 gcc -c consumer.c -lrt
