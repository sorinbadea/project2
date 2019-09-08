#include <thread>
#include "client_app.cpp"

void send_request_test() {
    
    int loops = 3;
    int message_id = 0;
    int items = 2;
    float threshold = 3.32;
    message_test_t message;
    request_result_t* p_res;

    client_app<message_test_t> app(client_type::CLIENT_TCP);

    while (loops--) {

       message.test_id = message_id++;
       message.items = items;
       message.threshold = threshold;

       p_res = app.send_request(message);

       if (p_res != NULL) {
          if (p_res->res == result::SUCCESS) {
             std::cout << "message id:" << p_res->message_id << " OK" << std::endl;
          }
	  else {
              std::cout << "message id:" << p_res->message_id << " KO" << std::endl;
	  }
       }
    }
}

void send_request_registration() {
    
    int loops = 3;
    unsigned int test_id = 0;
    unsigned int network_id = 6555;
    message_registration_t message;
    request_result_t* p_res;

    client_app<message_registration_t> app(client_type::CLIENT_TCP);

    while (loops--) {

       message.message_id = test_id++;
       message.network_id = network_id;

       p_res = app.send_request(message);

       if (p_res != NULL) {
          if (p_res->res == result::SUCCESS) {
             std::cout << "message id:" << p_res->message_id << " OK" << std::endl;
          }
	  else {
              std::cout << "message id:" << p_res->message_id << " KO" << std::endl;
	  }
       }
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
