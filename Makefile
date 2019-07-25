CC=g++ -std=c++11
CFLAGS=-c
LDFLAGS= -pthread

SOURCES_CL=client.cpp client_app.cpp main_cl.cpp
SOURCES_SRV=server.cpp server_app.cpp main_srv.cpp worker.cpp

OBJECTS_SRV=$(SOURCES_SRV:.cpp=.o)
OBJECTS_CL=$(SOURCES_CL:.cpp=.o)

EXE_CL=client
EXE_SRV=server
client: $(SOURCES_CL) $(EXE_CL)
server: $(SOURCES_SRV) $(EXE_SRV)

$(EXE_CL): $(OBJECTS_CL)
	$(CC) $(LDFLAGS) $(OBJECTS_CL) -g -o $@

$(EXE_SRV): $(OBJECTS_SRV)
	$(CC) $(LDFLAGS) $(OBJECTS_SRV) -g -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(EXE_CL) $(OBJECTS_CL) 
	rm -f $(EXE_SRV) $(OBJECTS_SRV)     

