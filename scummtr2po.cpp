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


    
scummtr2po::scummtr2po( const std::string& inputFilename, const std::string& oututFilename )
{
    inputFilename_ = inputFilename;
    oututFilename_ = oututFilename;
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

void scummtr2po::poToScumm()
{
}
void scummtr2po::scummToPo()
{
    std::cout << "Input Filename: " << inputFilename_ << std::endl;
    std::cout << "Output Filename: " << oututFilename_ << std::endl;
    std::fstream is;
    std::fstream os;
    int lineNumber = 0;
    objects objs = objects(inputFilename_, "objects.txt");

    os.open(oututFilename_,std::ios::out);  
    
    if(os.is_open()) 
    {
        os << this->getHeader();   
    }
    
    is.open( inputFilename_, std::ios::in ); 

    if (is.is_open()){  
        std::string tp;

        while( getline(is, tp) )
        {
            //std::cout << tp << "\n"; 
            stringId str = stringId( tp , lineNumber );
            
            
            if(os.is_open()) 
            {
                
                os << "#. RAW: " << tp << std::endl;
                os << "#. stringId:"<<lineNumber<<std::endl;
                os << str.getReference();
                os << createContext(objs, str);   
                os << str.getMsgId();   
                os << str.getMsgStr();   
                os << std::endl;   
                
            }  
            ++lineNumber;
        }

        is.close(); 
    }
    
    if(os.is_open()) 
    {
        os.close();    
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

    if ( found != std::string::npos )
    {
        stringId_ = completeStringId_.substr((int)found);
    }
    else
    {
        stringId_ = completeStringId_;
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

const std::string stringId::getMsgStr()
{
    return "msgstr \"\"\n";
}



/*************************************
 * 
 *            OBJECTS
 *
 *************************************/

objects::objects( const std::string& inputFilename, const std::string& objectsFilename)
{
    inputFilename_ = inputFilename;
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
    
    is.open( inputFilename_, std::ios::in ); 
    objects.open ( objectsFilename_, std::ios::out);
    
    if (is.is_open()){  
        std::string line;
        
        while( getline(is, line) )
        {
            //std::cout << tp << "\n"; 
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


