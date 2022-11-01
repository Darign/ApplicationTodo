/**
* Copyright (c) 2016 Gabriel Ferreira <gabrielinuz@gmail.com>. All rights reserved. 
* This file is part of CPPSET.
* Released under the GPL3 license
* https://opensource.org/licenses/GPL-3.0
**/
#ifndef RESPONSE_HANDLER_INTERFACE_H
#define RESPONSE_HANDLER_INTERFACE_H

#include <iostream>
#include <nlohmann/json.hpp>

class ResponseHandlerInterface
{
    public:
    	virtual void setResponse(nlohmann::json response, int status_code = 200) = 0;            
};

#endif // RESPONSE_HANDLER_INTERFACE_H