#include "objects.h"

/*************************************
 * 
 *            OBJECTS
 *
 *************************************/

objects::objects( const std::string& stringsFilename, const std::string& objectsFilename)
{
    stringsFilename_ = stringsFilename;
    objectsFilename_ = objectsFilename;
    
    createObjectTable();
}

objects::~objects()
{
    
}
        
const std::string objects::getObjectName( int objectNumber )
{
    std::map<int, std::string>::const_iterator iter =  objectNumberToName.find( objectNumber );
    std::string objectName = "";
    if (iter != objectNumberToName.end())
    {
        objectName = iter->second;
    }
    else
    {
        std::cout << "ERROR: object not found: " << objectNumber <<std::endl;
    }
    return objectName;
}
        
    
void objects::createObjectTable()
{
    std::fstream is;
    std::fstream objects;
    
    std::cout << "Creating table of objects" << std::endl;
    
    is.open( stringsFilename_, std::ios::in ); 
    objects.open ( objectsFilename_, std::ios::out);
    
    if (is.is_open()){  
        std::string line;
        
        while( getline(is, line) )
        {
            //std::cout << line << "\n"; 
            stringId str = stringId( line , 0 );
            if ( str.type_ == "OBNA" )
            {
                int objectNumber = stoi(str.typeNumber_);
                
                // map object name and object number 
                stringId::strReplaceFromCodeToChar(str.stringId_);
                objectNumberToName.insert(std::make_pair(objectNumber, str.stringId_));
                
                // Write relationship in objects.txt file
                if(objects.is_open()) 
                {
                    objects <<  objectNumber << " : "<< str.stringId_ << std::endl;
                }            
            }
        }
        is.close(); 
        if(objects.is_open()) 
        {
            objects.close();    
        }                

    }
    std::cout << "..done" << std::endl;
    
}


