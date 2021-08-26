This a very simple and maybe naive inter process comm implementatation.
The endianess aspect is not taken into account
The core of the server is impelemented in the server_app.cpp, the core 
of the client is implemented in client_app.cpp

Unitary tests:
There is small unitary test module to be found in the test/ directory.
Before compiling the unitary tests you have to run a make clean in the parent directory
in ortder to cleanup the already created object files
