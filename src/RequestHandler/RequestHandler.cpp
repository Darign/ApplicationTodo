/**
* Copyright (c) 2019 Gabriel Ferreira <gabrielinuz@gmail.com>. All rights reserved. 
* This file is part of CPPSET.
* Released under the GPL3 license
* https://opensource.org/licenses/GPL-3.0
**/

#include <iostream>
#include <memory>
#include <string>
#include <map> //
#include <nlohmann/json.hpp>
#include <cppset/RequestHandlerInterface.h>
#include <cppset/CGIhelper.h> //

typedef std::shared_ptr<RequestHandlerInterface> IRequestHandlerPtr;

class RequestHandler : public RequestHandlerInterface{
public:
    RequestHandler() { }
    
    virtual ~RequestHandler(){ }                     
    
    nlohmann::json getRequest() { //        
        nlohmann::json response = { } ;

        const char* result = Post::getPostdata() ;                
                
        if ( result != NULL && result[0] != '\0' ) {                                                       
            std::map<std::string, std::string> pair ;            
            CGI::uriTranslator( result, pair ) ;            
            std::map<std::string, std::string>::iterator iter ;                                   
            for ( iter = pair.begin(); iter != pair.end(); iter++ ) {                 
                response[iter->first] = CGI::decodeURLString( pair[iter->first] ) ;                
            }                                             
        }                                                                
        
        return response ;
    }                 
};

extern "C" IRequestHandlerPtr create(std::string);

IRequestHandlerPtr create(std::string typeinfo)
{
    return ( typeid(RequestHandlerInterface).name() == typeinfo )? std::make_shared<RequestHandler>() : nullptr;
}