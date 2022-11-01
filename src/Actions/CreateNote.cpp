/**
* Copyright (c) 2019 Gabriel Ferreira <gabrielinuz@gmail.com>. All rights reserved. 
* This file is part of CPPSET.
* Released under the GPL3 license
* https://opensource.org/licenses/GPL-3.0
**/

#include <iostream>
#include <memory>
#include <string>

#include <cppset/ActionInterface.h>
#include <cppset/ResponseHandlerInterface.h> //
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
typedef std::shared_ptr<DatabaseHandlerInterface> IDatabaseHandlerPtr; //
typedef std::shared_ptr<ValidatorInterface> IValidatorPtr; //

typedef std::unordered_map< std::string, std::string > Row; 


class CreateNote : public ActionInterface
{
    public:        
        CreateNote() { }           
            
        virtual ~CreateNote(){ }       

        void execute(nlohmann::json request) {                                   
            std::shared_ptr<ComponentFactory> componentFactory( new ComponentFactory()) ;  
            IResponseHandlerPtr responseHandler = componentFactory->create<ResponseHandlerInterface>("ResponseHandler") ;
            IValidatorPtr validator = componentFactory->create<ValidatorInterface>("Validator") ;            
                
            if ( validator->required(request, "note", "La nota no puede estar vacía.") &&
                    validator->maxLength(request, "note", 64, "La nota no puede superar los 64 caracteres.") ) 
               {        
                std::shared_ptr<IniReader> iniReader( new IniReader() ); // 
                iniReader->open("configuration.ini"); //                
                std::string sqlitePath = iniReader->selectSection("SQLITE")->getValue("path") ; //                
                iniReader->close() ;
                IDatabaseHandlerPtr dbh = std::make_shared<SqliteDatabaseHandler>(sqlitePath) ;                 
                dbh->prepareQuery("INSERT INTO Notes(note) VALUES(?)") ;
                dbh->addParameter(1, request["note"] ) ;
                dbh->execute() ;
                
                dbh->prepareQuery("SELECT last_insert_rowid() as id" ) ;               
                Row row = dbh->fetch() ;
                
                nlohmann::json response = {
                                {"message","¡Nota creada con éxito!"},                                
                                {"error_status", false},                                
                                {"data",{ 
                                    {"id", row["id"]},
                                    {"note", request["note"]}                                    
                                }}
                            };                
                
                responseHandler->setResponse( response ) ;  

            } else {
                responseHandler->setResponse( validator->getMessage(), 422 ) ; // Unprocessable entity 
            }
        }            
} ;

extern "C" IActionPtr create(std::string);

IActionPtr create(std::string typeinfo)
{
	return ( typeid(ActionInterface).name() == typeinfo )? std::make_shared<CreateNote>() : nullptr;
}           