CC=g++ -std=c++11
CFLAGS=-c
LDFLAGS= -pthread -lstdc++

SOURCES_CL=exception_cl.cpp client.cpp utils.cpp main_cl.cpp
SOURCES_SRV=exception_srv.cpp server.cpp server_app.cpp worker.cpp main_srv.cpp

OBJECTS_SRV=$(SOURCES_SRV:.cpp=.o)
OBJECTS_CL=$(SOURCES_CL:.cpp=.o)

EXE_CL=app_client
EXE_SRV=app_server

all: $(EXE_CL) $(EXE_SRV) $(EXE_TEST)

$(EXE_CL): $(OBJECTS_CL)
	$(CC) $(LDFLAGS) $(OBJECTS_CL) -g -o $@

$(EXE_SRV): $(OBJECTS_SRV)
	$(CC) $(LDFLAGS) $(OBJECTS_SRV) -g -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(EXE_CL) $(OBJECTS_CL)
	rm -f $(EXE_SRV) $(OBJECTS_SRV) 

