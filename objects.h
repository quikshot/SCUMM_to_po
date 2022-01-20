#ifndef OBJECTS_
#define OBJECTS_

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <sstream>
#include <map>

#include "stringId.h"

class objects
{
public:
    objects( const std::string& inputFilename, const std::string& objectsFilename);
    ~objects();
    
    const std::string getObjectName( int objectNumber );            
    
private:
    void createObjectTable();        

    std::map<int, std::string> objectNumberToName;
    std::string stringsFilename_;
    std::string objectsFilename_;     
    std::string configFilename_;
};

#endif
