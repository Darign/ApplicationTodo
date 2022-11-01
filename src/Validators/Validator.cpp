
#include <iostream>
#include <string>

#include <nlohmann/json.hpp>
#include <cppset/ValidatorInterface.h>

typedef std::shared_ptr<ValidatorInterface> IValidatorPtr;


class Validator : public ValidatorInterface {
public:
    Validator() { }

    virtual ~Validator() { }
    
    bool required( nlohmann::json request, std::string property, std::string msg ) {
        if ( this->validate(request, property) ) {
            std::string check = request[property] ;
            if ( check.empty() ) {
                message["message"] = msg ;
                message["error_status"] = true ;                
                return false ;
            } else {
                return true ;
            }            
        }         
        return false ;       
    }   
    
    bool maxLength( nlohmann::json request, std::string property, size_t max_length, std::string msg ) {
        if ( this->validate(request, property) ) {
            std::string check = request[property] ;
            if ( check.length() > max_length ) {
                message["message"] = msg ;
                message["error_status"] = true ;
                return false ;
            } else {
                return true ;
            }                   
        }             
        return false ;       
    }

    bool validate(nlohmann::json request, std::string property) {
        std::string check ;
        try { 
            check = request[property] ;
        } catch(nlohmann::json::exception& e) {
            message["message"] = "El campo " + property + " es requerido." ; 
            message["error_status"] = true ;            
            return false ;            
        }
        return true ;
    }    

    nlohmann::json getMessage() {
        return message ;
    }

private:
    nlohmann::json message ;
} ;

extern "C" IValidatorPtr create(std::string);

IValidatorPtr create(std::string typeinfo)
{
	return ( typeid(ValidatorInterface).name() == typeinfo )? std::make_shared<Validator>() : nullptr;
}