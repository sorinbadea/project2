#include "server_app.h"

int main(int argc, char** argv) {

    server_app app(server_type::SERVER_TCP, PORT, QUEUE_SIZE);
    app.start();
    return 0;

}
