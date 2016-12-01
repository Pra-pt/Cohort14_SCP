#yet to create a Makefile
HEADERS = task_mgr.h
OBJECTS = client-management.o server_epoll.o

default: server.xx

%.o: %.c $(HEADERS)
	gcc -c $< -o $@

server.xx: $(OBJECTS)
	gcc -pthread $(OBJECTS) -o $@

clean:
	-rm -f $(OBJECTS)
	-rm -f program

