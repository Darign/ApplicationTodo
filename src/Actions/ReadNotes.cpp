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
#include <cppset/ResponseHandlerInterface.h>
#include <cppset/DatabaseHandlerInterface.h> //
#include <cppset/ComponentFactory.h>
#include <SqliteDatabaseHandler/SqliteDatabaseHandler.h>
#include <nlohmann/json.hpp>
#include <cppset/IniReader.h> //

typedef std::shared_ptr<ActionInterface> IActionPtr;
typedef std::shared_ptr<ComponentFactory> ComponentFactoryPtr;
typedef std::shared_ptr<ResponseHandlerInterface> IResponseHandlerPtr ;
typedef std::shared_ptr<IniReader> IniReaderPtr; //
typedef std::shared_ptr<DatabaseHandlerInterface> IDatabaseHandlerPtr; //

typedef std::unordered_map< std::string, std::string > Row; 
typedef std::vector< Row > Table; 

class ReadNotes : public ActionInterface
{
    public:
        ReadNotes() { }
        virtual ~ReadNotes() {}
        
        void execute(nlohmann::json request) {
            std::shared_ptr<ComponentFactory> componentFactory( new ComponentFactory()) ;                                                   
            IResponseHandlerPtr responseHandler = componentFactory->create<ResponseHandlerInterface>("ResponseHandler") ;
                                                                       
            std::shared_ptr<IniReader> iniReader( new IniReader() ); //            
            iniReader->open("configuration.ini"); //                  
            std::string sqlitePath = iniReader->selectSection("SQLITE")->getValue("path") ; // 
            iniReader->close() ;

            IDatabaseHandlerPtr dbh = std::make_shared<SqliteDatabaseHandler>(sqlitePath) ;                        
            dbh->prepareQuery("SELECT * FROM Notes ORDER BY id");
            Table noteList = dbh->fetchAll() ;                         
            nlohmann::json row = nlohmann::json::array({ }) ;                 

            for ( size_t i = 0; i < noteList.size(); i++ ) {                    
                row.push_back( {{"id", noteList[i]["id"]},
                                {"note", noteList[i]["note"]}
                                }) ;                                                        
            } 
                 
            responseHandler->setResponse( row ) ;                               
        }       
};

extern "C" IActionPtr create(std::string);

IActionPtr create(std::string typeinfo)
{
	return ( typeid(ActionInterface).name() == typeinfo )? std::make_shared<ReadNotes>() : nullptr;
}