CC=g++ -std=c++11
CFLAGS=-c
LDFLAGS= -pthread

SOURCES_CL=exception_cl.cpp client.cpp main_cl.cpp
SOURCES_SRV=exception_srv.cpp server.cpp server_app.cpp worker.cpp main_srv.cpp
SOURCES_TEST= exception_srv.cpp server.cpp server_app.cpp client_app.cpp worker.cpp server_test.cpp

OBJECTS_SRV=$(SOURCES_SRV:.cpp=.o)
OBJECTS_CL=$(SOURCES_CL:.cpp=.o)
OBJECTS_TEST=$(SOURCES_TEST:.cpp=.o)

EXE_CL=app_client
EXE_SRV=app_server
EXE_TEST=app_test
client: $(SOURCES_CL) $(EXE_CL)
server: $(SOURCES_SRV) $(EXE_SRV)

$(EXE_CL): $(OBJECTS_CL)
	$(CC) $(LDFLAGS) $(OBJECTS_CL) -g -o $@

$(EXE_SRV): $(OBJECTS_SRV)
	$(CC) -DCLS_DEBUG $(LDFLAGS) $(OBJECTS_SRV) -g -o $@

$(EXE_TEST): $(OBJECTS_TEST)
	$(CC) -DTEST $(LDFLAGS) $(OBJECTS_TEST) -g -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(EXE_CL) $(OBJECTS_CL) $(OBJECTS_TEST) 
	rm -f $(EXE_SRV) $(EXE_TEST) $(OBJECTS_SRV)   

