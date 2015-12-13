CC = gcc
CFLAGS = -Wall -g -O0
OBJS = anl_data.o json.o led_control_service.o sqlite.o

service:$(OBJS)
		$(CC) $(CFLAGS) -o $@ $^ -ljson -lm -lpthread -lsqlite3  


%.o:%.c
		$(CC) $(CFLAGS) -c -o $@ $^ -ljson -lm -lpthread -lsqlite3 

clean:
		$(RM) $(OBJS) service .*.sw?
