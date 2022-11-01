/**
* Copyright (c) 2019 Gabriel Ferreira <gabrielinuz@gmail.com>. All rights reserved. 
* This file is part of CPPSET.
* Released under the GPL3 license
* https://opensource.org/licenses/GPL-3.0
**/
#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <iostream>
#include <memory>
#include <cppset/ConfigCreate.h>

typedef std::shared_ptr<ConfigCreate> ConfigCreatePtr;

class Configuration {
public:
    Configuration() { }
    virtual ~Configuration() { }

    void initConfiguration() {
        ConfigCreatePtr configCreate( new ConfigCreate() ) ;
        configCreate->Open("configuration.ini") ;

        configCreate->PutSection("GENERAL") ;
        configCreate->PutValue("applicationName", "Application") ;
        configCreate->PutValue("componentsPath", "./web/backend/lib/") ;
        configCreate->PutValue("actionsPath", "actions/") ;  
        configCreate->PutBlankLine() ;
        configCreate->PutSection("SQLITE") ;
        configCreate->PutValue("path", "./web/backend/database/todo.db") ;

        configCreate->Close() ;
        
    }
} ;

#endif // CONFIGURATION_H