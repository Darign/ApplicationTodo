#include <iostream>
#include <cppset/ComponentFactory.h>
#include <cppset/ApplicationInterface.h>
#include <cppset/ConfigCreate.h>
#include <cppset/Configuration.h>

typedef std::shared_ptr<ComponentFactory> ComponentFactoryPtr;
typedef std::shared_ptr<ApplicationInterface> IApplicationPtr;
typedef std::shared_ptr<IniReader> IniReaderPtr;
typedef std::shared_ptr<Configuration> ConfigurationPtr;


int main() {                  
        
        IniReaderPtr iniReader( new IniReader() );
        std::string APPLICATION_NAME = "" ;
        bool foundConfig = iniReader->open("configuration.ini") ;              

        if (!foundConfig) {
            iniReader->close() ;   
            ConfigurationPtr configuration(new Configuration()) ;
            configuration->initConfiguration() ;  
            iniReader->open("configuration.ini") ;
            APPLICATION_NAME = iniReader->selectSection("GENERAL")->getValue("applicationName");            
        } else {            
            APPLICATION_NAME = iniReader->selectSection("GENERAL")->getValue("applicationName");            
        }   

        iniReader->close() ;              

        std::shared_ptr<ComponentFactory> componentFactory( new ComponentFactory()) ;
        IApplicationPtr app = componentFactory->create<ApplicationInterface>(APPLICATION_NAME);
        app->run();                
                        
        return 0 ;
}