/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2021  <copyright holder> <email>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "scummtr2po.h"


/*************************************
 * 
 *      scummtr2po, main class
 *
 *************************************/

const std::string scummtr2po::getHeader(void)
{
    // current date/time based on current system
    time_t now = time(0);
   
    // convert now to string form
    char* dt = ctime(&now);
    
    std::stringstream stream;
    stream << "# POT File created using scummtr2po \n" 
           << "# https://github.com/quikshot/SCUMM_to_po\n" 
           << "# Context extracted from strings created using scummtr.exe tool\n" 
           << "msgid \"\"\n" 
           << "msgstr \"\"\n" 
           << "\"Project-Id-Version: \\n\"\n" 
           << "\"POT-Creation-Date: " <<  dt <<  "\\n\"\n" 
           << "\"PO-Revision-Date: \\n\"\n" 
           << "\"Language-Team: \\n\"\n" 
           << "\"MIME-Version: 1.0\\n\"\n" 
           << "\"Content-Type: text/plain\"\n\n";
    return stream.str();
}


    
scummtr2po::scummtr2po( const std::string& stringsFilename, const std::string& poFilename, const std::string& configFilename, const bool debug ):
stringsFilename_(stringsFilename),
poFilename_(poFilename),
configFilename_(configFilename),
debug_(debug)
{
    //TODO: check if file exists
}

scummtr2po::~scummtr2po()
{
}

const std::string scummtr2po::createContext( objects& obj, stringId& strId )
{
    std::stringstream stream;
    stream << "msgctxt \"" ;
    if( strId.type_ == "VERB" ) 
    {
        stream <<"Room: " << strId.roomNumber_ << " Text related to object: " << obj.getObjectName( stoi(strId.typeNumber_) );
    }
    else if( strId.type_ == "OBNA" ) 
    {
        stream <<"Room: " << strId.roomNumber_ << " Object name";
    }
    else if( strId.type_ == "ENCD" ) 
    {
        stream <<"Room: " << strId.roomNumber_ << " Entry screenplaying";
    }
    else if( strId.type_ == "EXCD" ) 
    {
        stream <<"Room: " << strId.roomNumber_ << " Exit screenplaying";
    }    
    else if( strId.type_ == "LSCR" ) 
    {
        stream <<"Room: " << strId.roomNumber_ << " Script "<< strId.typeNumber_;
    }
    else if( strId.type_ == "SCRP" ) 
    {
        stream <<"Room: " << strId.roomNumber_ << " Screenplay "<< strId.typeNumber_;
    }
    else
    {
        stream <<"Room: " << strId.roomNumber_ << " Other: " << strId.type_ << "#" << strId.typeNumber_;
    }
    stream <<"\""<< std::endl;
    return stream.str();
}

void copyFileTo(std::string sourceFile, std::string destFile )
{
    std::ifstream source(sourceFile, std::ios::binary);
    std::ofstream dest(destFile, std::ios::binary);
    std::cout << "Copying " << sourceFile << " to: " << destFile << std::endl; 
    dest << source.rdbuf();

    source.close();
    dest.close();
}

bool getStringFromPoFile( std::fstream& pos, const int stringNumber, std::string& msgstr_)
{
    bool ret=false;
    std::string lineStr;
    std::string msgstr;
    bool foundId = false;
    if (pos.is_open() ){ 
        std::string lineStr;
        std::string msgstr="NOT FOUND";
        while( getline(pos, lineStr) )
        {
            //std::cout << "getStringFromPoFile: "<<lineStr<<std::endl;
            // Read PO file 
            // if line contains: "#. stringId:", read stringId number.
            std::string searchStr = "#. stringId:" + std::to_string(stringNumber);
            std::size_t found = lineStr.find(searchStr);
            
            if( found != std::string::npos)
            {
                foundId = true;
            }
            else if( foundId == true )
            {
                std::size_t found = lineStr.find("msgstr \"");
                if( found != std::string::npos)
                {
                    std::string cut=lineStr.substr(8); //remove initial part "msgstr \""
                    msgstr_ = cut.substr(0,cut.length()-1);
                    ret=true;
                    break;
                }
                else
                {
                    continue;
                }
                    
            }else
            {
                continue;
            }
        }
    }else
    {
        std::cerr << "ERR: String stream is not opened" << std::endl;
    }
    
    return ret;
}

void scummtr2po::poToScumm()
{
    // Read po file.
    // read comment #. stringId:20
    // find next msgstr "XXX20"
    // store XXX
    // read config file and substitute  SCUMMTR2PO_STRING_ID_20 for XXX20
    // write to new stringsEN_ca.txt
    std::fstream strs;
    std::fstream pos;
    std::fstream configos;
    int lineNumber = 0;
    // Copy config file into destination file
    
    //copyFileTo (configFilename_, stringsFilename_ );
    
    //objects objs = objects(stringsFilename_, "objects.txt");

    strs.open( stringsFilename_, std::ios::out ); 
    pos.open( poFilename_, std::ios::in );  
    configos.open( configFilename_, std::ios::in );  

    if(debug_)
    {
        std::cout << "Strings Filename: " << stringsFilename_ << " - " <<  strs.is_open() <<std::endl;
        std::cout << "Po Filename: " << poFilename_ << " - " <<  pos.is_open() << std::endl;
        std::cout << "Intermediate config Filename: " << configFilename_ << " - " <<  configos.is_open() << std::endl;
    }
    
    if( pos.is_open() && configos.is_open() && strs.is_open() ){  
        std::string lineStr;
        std::string msgstr;
        std::cout << "Looping.." <<std::endl;
        
        while( getline(configos, lineStr) )
        {
             
            //read input string
            //search translation
            if( getStringFromPoFile( pos, lineNumber, msgstr) )
            {
                //write output translated string
                //get initial part 
                std::string substitution;
                std::size_t found = lineStr.find("SCUMMTR2PO_STRING_ID_");
                if( found != std::string::npos)
                {
                    substitution = lineStr.substr( 0 ,found) + msgstr;
                }
                else{
                    std::cerr <<"ERR: wrong config file! "<< configFilename_ << " should contain string substitutions like SCUMMTR2PO_STRING_ID_NNN" << std::endl;
                }
                    
                
                if( debug_ )
                {
                    std::cout << "ORIGINAL  : " << lineNumber << ": " << lineStr << std::endl; 
                    std::cout << "TRANSLATED: " << lineNumber << ": " << msgstr << std::endl;
                    std::cout << "SUBST     : " << lineNumber << ": " << substitution << std::endl<<std::endl;
                }
                
                strs << substitution << std::endl;
                
            } else {
                std::cerr << " String "<< std::to_string(lineNumber) << " not found in file " << poFilename_ << std::endl;
            }
            
            ++lineNumber;
        }
        pos.close(); 
    }
    
    if(strs.is_open()) 
    {
        strs.close();    
    }
    if(configos.is_open()) 
    {
        configos.close();    
    }        
}

void scummtr2po::scummToPo( bool test )
{
    if(debug_)
    {
        std::cout << "Strings Filename: " << stringsFilename_ << std::endl;
        std::cout << "Po Filename: " << poFilename_ << std::endl;
        std::cout << "Intermediate config Filename: " << configFilename_ << std::endl;
    }
    
    std::fstream is;
    std::fstream os;
    std::fstream configos;
    int lineNumber = 0;
    objects objs = objects(stringsFilename_, "objects.txt");

    os.open( poFilename_, std::ios::out );  
    configos.open( configFilename_, std::ios::out );  
    
    if(os.is_open()) 
    {
        os << this->getHeader();   
    }

    is.open( stringsFilename_, std::ios::in ); 
    
    if (is.is_open()){  
        std::string lineStr;

        while( getline(is, lineStr) )
        {
            // Write POT file 
            stringId str = stringId( lineStr , lineNumber );
            if(os.is_open()) 
            {
                os << "#. RAW: " << lineStr << std::endl;
                os << "#. stringId:"<<lineNumber<<std::endl;
                os << str.getReference();
                os << createContext(objs, str);   
                os << str.getMsgId();   
                os << str.getMsgStr( test );   
                os << std::endl;   
            }  
            
            // Write intermediate config file
            if(configos.is_open()) 
            {
                configos << str.configText_ << std::endl;
            }  
            
            ++lineNumber;
        }
        is.close(); 
    }
    
    if(os.is_open()) 
    {
        os.close();    
    }
    if(configos.is_open()) 
    {
        configos.close();    
    }    
}



/*************************************
 * 
 *   stringId to process line by line
 *
 *************************************/


stringId::stringId( const std::string& line, int id)
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
    
    
    
    std::size_t found = completeStringId_.find_first_not_of("0123456789\\");
    if( found != std::string::npos)
    {
        configText_ = line.substr(0, 19+(int)found) + "SCUMMTR2PO_STRING_ID_" +std::to_string( id ) ;
    }else{
        configText_ = line + "SCUMMTR2PO_STRING_ID_" +std::to_string( id ) ;
    }
    
    if ( found != std::string::npos )
    {
        stringId_ = completeStringId_.substr((int)found);
    }
    else
    {
        stringId_ = "";
    }

    /* Convert: 
    //[097:LSCR#0210](27)\255\010\056\245\255\010\107\000\255\010\010\000\255\010\000\000Brace the door with a\016rock!
    */
}

stringId::~stringId()
{}

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
}


