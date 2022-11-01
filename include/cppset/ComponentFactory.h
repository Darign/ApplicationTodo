/**
* Copyright (c) 2016-2019 Gabriel Ferreira <gabrielinuz@gmail.com>. All rights reserved. 
* This file is part of COMPSET.
* Released under the GPL3 license
* https://opensource.org/licenses/GPL-3.0
**/

#ifndef COPONENT_FACTORY_H
#define COPONENT_FACTORY_H

#include <iostream>
#include <stdlib.h>
#include <memory>
#include <typeinfo>
#include <cppset/LibraryLoader.h>
#include <cppset/IniReader.h>

typedef std::shared_ptr<LibraryLoader> LoaderPtr;
typedef std::shared_ptr<IniReader> IniReaderPtr;

class ComponentFactory
{
    public:
        ComponentFactory()
        {                        
            // THE LOADER:
            this->loader = std::make_shared<LibraryLoader>();
            // INIREADER:
            IniReaderPtr iniReader( new IniReader() );
            iniReader->open("configuration.ini");
            this->componentsPath = iniReader->selectSection("GENERAL")->getValue("componentsPath");                          
        }
        virtual ~ComponentFactory()
        {
            this->loader->freeLibrary();            
        }
        
        template<typename InterfaceType> std::shared_ptr<InterfaceType> create(std::string componentName)
        {
            typedef std::shared_ptr<InterfaceType> InterfaceTypePtr;

            //LOAD:                     
            void* load = this->loader->loadLibrary(this->componentsPath + componentName); 
            
            //NULL COMPONENT OBJECT:
            InterfaceTypePtr componentObject = nullptr;            
            if(load)
            {
                //CREATE:
                typedef InterfaceTypePtr ( *Factory ) (std::string);
                Factory factory = ( Factory ) this->loader->getExternalFunction( "create" );

                if(factory)
                {
                    InterfaceTypePtr createdComponent = factory(typeid(InterfaceType).name());//construyo una sola vez
                    if( std::dynamic_pointer_cast<InterfaceType> (createdComponent) != nullptr )
                    //if( dynamic_cast<InterfaceType*> (createdComponent.get()) != nullptr )
                    {
                        componentObject = createdComponent;
                    }
                    else
                    {
                        this->loader->freeLibrary();
                        std::cout << "Error: The component doesn't implement the interface: " << typeid(InterfaceType).name() << std::endl;
                        exit(-1);
                    }
                }
                else
                {
                    this->loader->freeLibrary();
                    std::cout << "Error:  Failed creating a component from "                            
                            << componentName << ", there is no external function \"create(void)\"." << std::endl;
                    exit(-1);
                }
            }
            else
            {
                this->loader->freeLibrary();
                std::cout << "Error: Failed to load the component: " << componentName << std::endl;
                exit(-1);
            }
            return componentObject;
        }        

    private:
        LoaderPtr loader;
        std::string componentsPath;                        
};
#endif // COPONENT_FACTORY_H