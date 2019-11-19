CC=g++ -std=c++11
CFLAGS=-c -I. -I../
CPPFLAGS += -I. -I../
LDFLAGS= -pthread -lstdc++

SOURCES_TEST= $(addprefix ../, exception_srv.cpp server.cpp server_app.cpp utils.cpp) mock.cpp server_test.cpp
OBJECTS_TEST=$(SOURCES_TEST:.cpp=.o)

EXE_TEST=app_test

all: $(EXE_TEST)

$(EXE_TEST): $(OBJECTS_TEST)
	$(CC) $(LDFLAGS) -I../ $(OBJECTS_TEST) -g -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS_TEST) 
	rm -f $(EXE_TEST)  

