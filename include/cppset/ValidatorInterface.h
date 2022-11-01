#ifndef VALIDATOR_INTERFACE_H
#define VALIDATOR_INTERFACE_H

#include <nlohmann/json.hpp>
#include <string>

class ValidatorInterface {
public:
    virtual bool validate(nlohmann::json request, std::string property) = 0 ;
    virtual bool required(nlohmann::json request, std::string property, std::string msg) = 0 ;
    virtual bool maxLength(nlohmann::json request, std::string property, size_t max_length, std::string msg) = 0 ; 
    virtual nlohmann::json getMessage() = 0 ;    
} ;

#endif