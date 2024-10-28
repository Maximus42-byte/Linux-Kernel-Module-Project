obj-m+=mym.o

all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
	$(CC) my.c -o my
clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean
	rm my
	