#include <thread>
#include "client_app.cpp"

/**
 * client callback
   this methd is called when the request result is ready
*/
void client_callback(const request_result_t& r) {
   if (r.res == result::SUCCESS) {
     std::cout << "message id:" << r.message_id << " OK" << std::endl;
   }
   else {
     std::cout << "message id:" << r.message_id << " KO" << std::endl;
   }
}

void send_request_test() {
    
    int loops = 3;

    /** instanciate a client app for a \"test\" message */
    client_app<message_test_t> app(client_type::CLIENT_TCP, client_callback);

    while (loops--) {
       /** send the request */
       app.send_request({1,2,1.66});
    }
}

void send_request_registration() {
    
    int loops = 3;
    /** instanciate a client app for a \"registration\" message */
    client_app<message_registration_t> app(client_type::CLIENT_TCP, client_callback);
    
    while (loops--) {
       /** send the request */
       app.send_request({2,6453});
    }
}


int main() {

    std::thread thread_l1(&send_request_test);
    std::thread thread_l2(&send_request_test);
    std::thread thread_l3(&send_request_test);
    std::thread thread_l4(&send_request_registration);
    thread_l1.join();
    thread_l2.join();
    thread_l3.join();
    thread_l4.join();
    return 0;
} 
