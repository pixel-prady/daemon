#ifndef  CONFIG_PARSER_HPP
#define  CONFIG_PARSER_HPP

#include <string> 
#include <unordered_map>

class ConfigParser{
    public : 
        ConfigParser ( const std :: string & filepath); 

        bool loadConfig();

        std :: string getString ( const std :: string & key , const std :: string default= "") ; 
        int getInt ( const std :: string & key , int default =0) ; 
        double getDouble ( const std :: string & key , double default = 0.0) ;

    private: 
        std :: string filepath ; 
        std :: unordered_map<std :: string , std :: string > configData; 


        bool parseYaml() ; 
        bool parseJson() ;
};  

#endif; 