/**
* Copyright (c) 2019 Gabriel Ferreira <gabrielinuz@gmail.com>. All rights reserved. 
* This file is part of CPPSET.
* Released under the GPL3 license
* https://opensource.org/licenses/GPL-3.0
**/

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

#include <cppset/ActionInterface.h>
#include <cppset/ResponseHandlerInterface.h>
#include <cppset/DatabaseHandlerInterface.h> //
#include <cppset/ComponentFactory.h>
#include <SqliteDatabaseHandler/SqliteDatabaseHandler.h>
#include <nlohmann/json.hpp>
#include <cppset/IniReader.h> //
#include <cppset/ValidatorInterface.h> //

typedef std::shared_ptr<ActionInterface> IActionPtr;
typedef std::shared_ptr<ComponentFactory> ComponentFactoryPtr;
typedef std::shared_ptr<ResponseHandlerInterface> IResponseHandlerPtr ;
typedef std::shared_ptr<IniReader> IniReaderPtr; //
typedef std::shared_ptr<ValidatorInterface> IValidatorPtr; //
typedef std::shared_ptr<DatabaseHandlerInterface> IDatabaseHandlerPtr; //

typedef std::unordered_map< std::string, std::string > Row; 

class DeleteNote : public ActionInterface
{
    public:
        DeleteNote() { }
        virtual ~DeleteNote() {}
        
        void execute(nlohmann::json request) {
            std::shared_ptr<ComponentFactory> componentFactory( new ComponentFactory()) ;                                                   
            IResponseHandlerPtr responseHandler = componentFactory->create<ResponseHandlerInterface>("ResponseHandler") ;
            IValidatorPtr validator = componentFactory->create<ValidatorInterface>("Validator") ;
                                            
            if ( validator->required(request, "id", "Uno de los campos requeridos no ha sido especificado.") ) {// 
                std::shared_ptr<IniReader> iniReader( new IniReader() ); //            
                iniReader->open("configuration.ini"); //                  
                std::string sqlitePath = iniReader->selectSection("SQLITE")->getValue("path") ; // 
                iniReader->close() ; //

                IDatabaseHandlerPtr dbh = std::make_shared<SqliteDatabaseHandler>(sqlitePath) ;                                   
                dbh->prepareQuery("SELECT * FROM Notes WHERE id=?");
                dbh->addParameter(1, request["id"]) ;                    
                                
                try {     
                    std::string result = dbh->fetch().at("id") ;
                    
                } catch (const std::logic_error& e) {   // id inexistente  
                    std::string id = request["id"] ;
                    nlohmann::json error_response = {
                                {"message", "El id " + id + " ingresado no existe."},
                                {"error_status", true},                                
                                {"data",{                                     
                                    {"id",request["id"]}                                    
                                }}
                            };
                    responseHandler->setResponse( error_response, 400 ) ; // bad request
                    exit(EXIT_FAILURE) ;                                   
                }

                dbh->prepareQuery("DELETE FROM Notes WHERE id=?") ;
                dbh->addParameter(1, request["id"]);
                dbh->execute() ;
                        
                nlohmann::json response = {
                                {"message","¡Nota eliminada con éxito!"},                                
                                {"error_status", false},                                
                                {"data",{ 
                                    {"id", request["id"]},
                                    {"note", request["note"]}                                    
                                }}                                                               
                            };                   
                
                responseHandler->setResponse( response ) ;  
            } else {
                responseHandler->setResponse( validator->getMessage(), 422 ) ; // Unprocessable entity 
            }            
        }                
};

extern "C" IActionPtr create(std::string);

IActionPtr create(std::string typeinfo)
{
	return ( typeid(ActionInterface).name() == typeinfo )? std::make_shared<DeleteNote>() : nullptr;
}