#ifndef CGI_HELPER_H
#define CGI_HELPER_H

#include <iostream>
#include <string>
#include <map>

class Post {
public:
    Post() { }
    virtual ~Post() { }
    
    static const char* getPostdata(){               
        char* lenstr = getenv( "CONTENT_LENGTH") ;              
        int len = atoi(lenstr);
        char* postdata = new char[len+1] ;
        fgets( postdata, len+1, stdin ) ;
        
        return postdata ;       
    }

} ;

class CGI {
public:
    CGI() { }
    virtual ~CGI() { }       
     
    static void uriTranslator( std::string _str, std::map< std::string, std::string > &mapa ) {
        std::string key ;
        std::string value ;
        std::string str = _str ;
        std::string equal = "=" ;
        std::string ampersand = "&" ;
        size_t pos = 0 ;
        while( ( pos = str.find( equal ) ) != ( std::string::npos ) ) {
            key = str.substr(0, pos) ;
            str.erase(0, pos + equal.length()) ;
            value = str.substr(0, str.find( ampersand )) ;
            str.erase(0, str.find( ampersand ) + ampersand.length()) ;            
                mapa[ key ] = value ;            
        }
    }

    static std::string decodeURLString(std::string URLstr) {
        const int len = URLstr.length();
        std::string result;
        for(int i = 0; i < len; i++) {
        if(URLstr[i] == '+')
            result += ' ';
        else if(URLstr[i] == '%') {
            result +=
            translateHex(URLstr[i + 1]) * 16 +
            translateHex(URLstr[i + 2]);
            i += 2; // Move past hex code
        } else // An ordinary character
            result += URLstr[i];
        }
        return result;
    }

    static char translateHex(char hex) {
        if(hex >= 'A')
        return (hex & 0xdf) - 'A' + 10;
        else
        return hex - '0';
    }    

} ;

#endif