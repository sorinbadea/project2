#ifndef CL_SRV_EXCEPTION_H
#define CL_SRV_EXCEPTION_H
#include <string>

class client_exception {
   std::string p_message;
public:
   explicit client_exception(const std::string message); 
   std::string get_message() const; 
};

class server_exception {
   std::string p_message;
public:
   explicit server_exception(const std::string message); 
   std::string get_message() const;
};

#endif
