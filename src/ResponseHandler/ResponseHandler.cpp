
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>
#include <cppset/ResponseHandlerInterface.h>

typedef std::shared_ptr<ResponseHandlerInterface> IResponseHandlerPtr;


class ResponseHandler : public ResponseHandlerInterface{
public:
    ResponseHandler() { }
    
    virtual ~ResponseHandler(){ }    

    void setResponse( nlohmann::json response, int status_code = 200 ) {
        std::cout << "Status: " << status_code << "\r\n" ; 
        std::cout << "Content-type:text/json\r\n\r\n" ;                                 

        std::cout << std::setw(4) << response << '\n'; // json beautify

        exit(0) ;
    }   

       
};

extern "C" IResponseHandlerPtr create(std::string);

IResponseHandlerPtr create(std::string typeinfo)
{
    return ( typeid(ResponseHandlerInterface).name() == typeinfo )? std::make_shared<ResponseHandler>() : nullptr;
}