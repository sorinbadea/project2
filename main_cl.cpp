#include <thread>
#include "client_app.h"

int main() {

    int loops = 16;
    client_app a;
    while( loops-- ) {
       a.request_test();
    }

    return 0;
} 
