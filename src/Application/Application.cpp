/**
* Copyright (c) 2019 Gabriel Ferreira <gabrielinuz@gmail.com>. All rights reserved. 
* This file is part of CPPSET.
* Released under the GPL3 license
* https://opensource.org/licenses/GPL-3.0
**/

#include <iostream>
#include <memory>
#include <cppset/ApplicationInterface.h>
#include <cppset/ActionInterface.h>
#include <cppset/ComponentFactory.h>
#include <cppset/IniReader.h>
#include <cppset/RequestHandlerInterface.h>
#include <cppset/ResponseHandlerInterface.h> //
#include <cppset/DatabaseHandlerInterface.h> //
#include <SqliteDatabaseHandler/SqliteDatabaseHandler.h> //
#include <nlohmann/json.hpp>

typedef std::shared_ptr<ApplicationInterface> IApplicationPtr;
typedef std::shared_ptr<ActionInterface> IActionPtr;
typedef std::shared_ptr<ComponentFactory> ComponentFactoryPtr;
typedef std::shared_ptr<IniReader> IniReaderPtr;
typedef std::shared_ptr<RequestHandlerInterface> IRequestHandlerPtr;
typedef std::shared_ptr<ResponseHandlerInterface> IResponseHandlerPtr; //
typedef std::shared_ptr<DatabaseHandlerInterface> IDatabaseHandlerPtr; //

class Application : public ApplicationInterface
{
    public:
        Application()
        {                     
            std::shared_ptr<IniReader> iniReader( new IniReader() ); //            
            iniReader->open("configuration.ini");
            this->actionsPath = iniReader->selectSection("GENERAL")->getValue("actionsPath");
            this->sqlitePath = iniReader->selectSection("SQLITE")->getValue("path") ; //            
            iniReader->close() ;
            dbh = std::make_shared<SqliteDatabaseHandler>(sqlitePath) ; //
            dbh->prepareQuery("CREATE TABLE IF NOT EXISTS Notes (id INTEGER PRIMARY KEY AUTOINCREMENT, note TEXT)") ; //            
            dbh->execute() ; //             
        }
        
        virtual ~Application(){ }

        void run()
        {                        
            ComponentFactoryPtr componentFactory(new ComponentFactory());                        
            IRequestHandlerPtr requestHandler = componentFactory->create<RequestHandlerInterface>("RequestHandler");                    
            nlohmann::json request = requestHandler->getRequest(); 
            IResponseHandlerPtr responseHandler = componentFactory->create<ResponseHandlerInterface>("ResponseHandler");
            std::string actionName ;

            if (request.empty()) {
                nlohmann::json response = {
                        {"message", "No hay contenido para mostrar."},
                        {"error_status", true},                                                                        
                    } ; 
                responseHandler->setResponse(response, 403) ; // forbidden
                exit(EXIT_FAILURE) ;
            }
            
            try{
                actionName = request["action"] ;                
            } catch(nlohmann::json::exception& e) {
                nlohmann::json action_exception = {
                        {"message", "Se ha producido un error."},
                        {"error_status", true}                        
                    };
                responseHandler->setResponse(action_exception, 400) ; // bad request
                exit(EXIT_FAILURE) ;
            } 
                       
            IActionPtr action = componentFactory->create<ActionInterface>(this->actionsPath + actionName) ;                   
            action->execute( request ) ;
        }                 

    private:
        std::string actionsPath;
        std::string sqlitePath ; //
        IDatabaseHandlerPtr dbh ;
};

extern "C" IApplicationPtr create(std::string);

IApplicationPtr create(std::string typeinfo)
{
    return ( typeid(ApplicationInterface).name() == typeinfo )? 
        std::make_shared<Application>() 
            : nullptr;
}         