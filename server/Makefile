#yet to create a Makefile
HEADERS = task_mgr.h message_handling.h
OBJECTS = task_mgr.o add_task_server.o client-management.o server_epoll.o processMessage.o 

default: server.xx

%.o: %.c $(HEADERS)
	gcc -c $< -o $@

server.xx: $(OBJECTS)
	gcc -pthread $(OBJECTS) -o $@

clean:
	-rm -f $(OBJECTS)
	-rm -f server.xx

