#ifndef CONFIG_CREATE_H
#define CONFIG_CREATE_H

#include <iostream>
#include <string>

class ConfigCreate {
public:
    ConfigCreate() { }
    virtual ~ConfigCreate() { }

    void Open(const std::string& filename);
 
    void Close();
 
    void PutSection(const std::string& section);
 
    void PutValue(const std::string& key, const std::string& value);
 
    void PutComment(const std::string& comment);
 
    void PutBlankLine();
private:
    std::ofstream file ;

} ;

    void ConfigCreate::Open(const std::string& filename)
    {
        file.open(filename);
        file.clear();
    }
    
    void ConfigCreate::Close()
    {
        file.close();        
    }
    
    void ConfigCreate::PutSection(const std::string& section)
    {
        file << "[" << section << "]" << std::endl;
    }
    
    void ConfigCreate::PutValue(const std::string& key, const std::string& value)
    {
        file << key << "=" << value << std::endl;
    }
    
    void ConfigCreate::PutComment(const std::string& comment)
    {
        file << "# " << comment << std::endl;
    }
    
    void ConfigCreate::PutBlankLine()
    {
        file << std::endl;
    }



#endif