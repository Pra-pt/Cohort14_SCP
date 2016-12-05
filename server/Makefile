#yet to create a Makefile
HEADERS = task_mgr.h message_handling.h
OBJECTS = client-management.o server_epoll.o processMessage.o

default: server.xx

%.o: %.c $(HEADERS)
	gcc -c $< -o $@

server.xx: $(OBJECTS)
	gcc -pthread $(OBJECTS) -o $@

clean:
	-rm -f $(OBJECTS)
	-rm -f program

