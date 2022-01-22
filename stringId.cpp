#include "stringId.h"

/*************************************
 * 
 *   stringId to process line by line
 *
 *************************************/


stringId::stringId( const std::string& line, int id)
{
    duplicatedId_ = -1;
    
    if(line.length()>19)
    {
        rawText_ = line;
        // extract substrings:
        completeStringId_ =  line.substr(19);
        //stringId_ = completeStringId_;
        roomNumber_ =  line.substr(1, 3);
        type_ = line.substr(5, 4);;
        typeNumber_ = line.substr(10, 4);
        op_ = line.substr(16, 2);;
        stringIdentifier_ = id;
        
        
        // What if some translation starts with a number? "3 bananas"
        // should count groups of 4: /022
        std::size_t found = completeStringId_.find_first_not_of("0123456789\\");
        //cut the string with numbers only divisable by 4:
        int rest = (int)found%4;
        if( rest < (int)found)
        {
            found = (std::size_t)((int)found - rest);
        }

        
        if( found != std::string::npos)
        {
            stringId_ = completeStringId_.substr((int)found);
            isEmptyString_ = false;
            if( duplicatedId_ != -1 )
            {
                //configText_ = line.substr(0, 19+(int)found) + "SCUMMTR2PO_STRING_ID_" +std::to_string( id ) + "_DUPLICATED_TO_"+ std::to_string(duplicatedId_);
                configText_ = line.substr(19,(int)found) + "SCUMMTR2PO_STRING_ID_" +std::to_string( id ) + "_DUPLICATED_TO_"+ std::to_string(duplicatedId_);
            }else{
                configText_ = line.substr(19,(int)found) + "SCUMMTR2PO_STRING_ID_" +std::to_string( id );
            }
        }else{
            // empty string!
            stringId_ = "";
            isEmptyString_ = true;
            configText_ = line.substr(19) + "SCUMMTR2PO_EMPTY_STRING_" +std::to_string( id ) ;
        }
    }else{
        std::cerr << "String too short!! missing headers. Line: "<< std::to_string(id) << " " << line << std::endl;
        isEmptyString_ = true;
    }
    /* Convert: 
    //[097:LSCR#0210](27)\255\010\056\245\255\010\107\000\255\010\010\000\255\010\000\000Brace the door with a\016rock!
    */
}

stringId::~stringId()
{}

bool stringId::isNotEmpty(){
    return !isEmptyString_;
}

void stringId::setStringDuplicated( int duplicatedId ){
    duplicatedId_ = duplicatedId;
    //TODO: Reuse code
    std::size_t found = completeStringId_.find_first_not_of("0123456789\\");
    //cut the string with numbers only divisable by 4:
    int rest = (int)found%4;
    if( rest < (int)found)
    {
        found = (std::size_t)((int)found - rest);
    }
    if( found != std::string::npos)
    {
        configText_ = rawText_.substr(19, (int)found) + "SCUMMTR2PO_STRING_ID_" +std::to_string( stringIdentifier_ ) + "_DUPLICATED_TO_"+ std::to_string(duplicatedId_);
    }else{
        configText_ =  "SCUMMTR2PO_EMPTY_STRING_" +std::to_string( stringIdentifier_ ) ;

    }
}


const std::string stringId::getMsgId()
{
    return "msgid \"" + stringId_ + "\"\n";
}

const std::string stringId::getReference()
{
    return "#. ref\n";
}

const std::string stringId::getMsgContext()
{
    return "msgctxt \"" +roomNumber_ + "_" + type_+ "_" + typeNumber_ + "(" + op_ + ")\"\n";
}

const std::string stringId::getMsgStr( bool test )
{
    if ( test ) 
    {
        return "msgstr \"" + stringId_ + "\"\n";
    }
    else
    {    
        return "msgstr \"\"\n";
    }
}


void stringId::strReplaceFromCodeToChar(std::string& str, bool debug_)
{
    if(debug_)
    {
        /*for (const auto& kv : toCode) {    
            std::cout << "key:" << kv.first << " value:" << kv.second << std::endl;
        } */       
        std::cout << "FromCode: input: "<<str<<std::endl;
    }
    std::map<std::string,std::string>::iterator it;
    for (const auto& kv : replaceMap_) 
    {    

        std::string characterToSearch = kv.first;
        std::string repl = kv.second;
        std::size_t found = str.find(characterToSearch);
        while (found != std::string::npos)
        {
            // get number and substitute
            //found. replace
            str.replace(found, characterToSearch.length(), repl);
            if(debug_)
            {
                std::cout << "FromCode: replace: " << repl << std::endl;
                std::cout << "FromCode: output: " << str << std::endl;
            }  
            found = str.find(characterToSearch,found+repl.length());
        }
    }
    
    //Check any pending special chars
    std::size_t found = str.find("\\");
    while (found != std::string::npos)
    {
        //std::cout << "DIGITS: "<< str.substr(found,4) << std::endl;
        //check if 3 following chars ar digits.
        if( isdigit((int)(str.substr(found+1,1).c_str()[0])) && isdigit((int)((char)str.substr(found+2,1).c_str()[0])) && isdigit((int)((char)str.substr(found+3,1).c_str()[0])))
        {
            
            str.replace(found, 1, "//");
        }
        

        //find next substitution in string
        found = str.find("\\",found+4);
    }    
}


void stringId::strReplaceFromCharToCode( std::string& str , bool debug_ ) 
{

    if(debug_)
    {
        std::cout << "ToCode: input: "<<str<<std::endl;
    }
    std::map<std::string,std::string>::iterator it;
    for (const auto& kv : replaceMap_) 
    {    
        std::string characterToSearch = kv.second;
        std::string repl = kv.first;
        std::size_t found = str.find(characterToSearch);
        while (found != std::string::npos)
        {
            // get number and substitute
            //found. replace
            str.replace(found, characterToSearch.length(), repl);
            if(debug_)
            {
                std::cout << "ToCode: replace: " << repl << std::endl;
                std::cout << "ToCode: output: " << str << std::endl;
            }  
            found = str.find(characterToSearch,found+repl.length());
        }
    }
    //substitute /XXX to \XXX
    
    //Restore pointers
    std::size_t found = str.find("//");
    while (found != std::string::npos)
    {
        //check if 3 following chars ar digits.
        
        if( isdigit((int)(str.substr(found+2,1).c_str()[0])) && isdigit((int)((char)str.substr(found+3,1).c_str()[0])) && isdigit((int)((char)str.substr(found+4,1).c_str()[0])))
        {
            str.replace(found, 2, "\\");
            
        }
        if(debug_){std::cout << "ToCode: output: " << str << std::endl;            }

        //find next substitution in string
        found = str.find("//",found+4);
    }
    
}
std::string stringId::removeFinalAt(std::string str)
{
    int len = str.length();
    //std::cout << "str:"<< str<<std::endl;
    //std::cout << "len:"<< len<<std::endl;
    while (len > 1)
    {
        //std::cout << "last:"<< str.substr(len-1,1)<<std::endl;
        if( str.substr(len-1,1) == "@")
        {
            str = str.substr(0, str.length()-1);
        }else
        {
            break;
        }
        len = str.length();
    }
    return str;
}


void stringId::strReplaceFromCodeToCharWithoutSpecial(std::string& str, bool debug_)
{
    if(debug_)
    {
       std::cout << "FromCode: input: "<<str<<std::endl;
    }
    std::map<std::string,std::string>::iterator it;
    for (const auto& kv : replaceMapClean_) 
    {    

        std::string characterToSearch = kv.first;
        std::string repl = kv.second;
        std::size_t found = str.find(characterToSearch);
        while (found != std::string::npos)
        {
            // get number and substitute
            //found. replace
            str.replace(found, characterToSearch.length(), repl);
            if(debug_)
            {
                std::cout << "FromCode: replace: " << repl << std::endl;
                std::cout << "FromCode: output: " << str << std::endl;
            }  
            found = str.find(characterToSearch,found+repl.length());
        }
    }
    
    //Check any pending special chars
    std::size_t found = str.find("\\");
    while (found != std::string::npos)
    {
        //std::cout << "DIGITS: "<< str.substr(found,4) << std::endl;
        //check if 3 following chars ar digits.
        if( isdigit((int)(str.substr(found+1,1).c_str()[0])) && isdigit((int)((char)str.substr(found+2,1).c_str()[0])) && isdigit((int)((char)str.substr(found+3,1).c_str()[0])))
        {
            
            str.replace(found, 4, "");
            found = str.find("\\",found);
            continue;
        }
        

        //find next substitution in string
        found = str.find("\\",found+1);
    }    
}
