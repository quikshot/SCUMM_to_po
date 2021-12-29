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

scummtr2po::stringId::stringId( const std::string& line)
{
    rawText_ = line;
    // extract substrings:
    completeStringId_ =  line.substr(19);
    //stringId_ = completeStringId_;
    roomNumber_ =  line.substr(1, 3);
    type_ = line.substr(5, 4);;
    typeNumber_ = line.substr(10, 3);
    op_ = line.substr(16, 2);;
    
    std::size_t found = completeStringId_.find_first_not_of("0123456789\\");
    std::cout << "Found: " << int(found) << std::endl;
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

scummtr2po::stringId::~stringId()
{}

const std::string scummtr2po::stringId::getMsgId()
{
    return "msgid \"" + stringId_ + "\"\n";
}

const std::string scummtr2po::stringId::getReference()
{
    return "#. ref\n";
}

const std::string scummtr2po::stringId::getMsgContext()
{
    return "msgctxt \"" +roomNumber_ + "_" + type_+ "_" + typeNumber_ + "(" + op_ + ")\"\n";
}

const std::string scummtr2po::stringId::getMsgStr()
{
    return "msgstr \"\"\n";
}

const std::string scummtr2po::getHeader(void)
{
    return "msgid \"\"\n\
\"msgstr \"\"\n\
\"Project-Id-Version: \\n\"\n\
\"POT-Creation-Date: \\n\"\n\
\"PO-Revision-Date: \\n\"\n\
\"Language-Team: \\n\"\n\
\"MIME-Version: 1.0\\n\"\n\
\"Content-Type: text/plain\"\n\n";
}


scummtr2po::scummtr2po( const std::string& inputFilename, const std::string& oututFilename )
{
    inputFilename_ = inputFilename;
    oututFilename_ = oututFilename;
    
    
}

scummtr2po::~scummtr2po()
{

}

void scummtr2po::process()
{
    std::cout << "Input Filename: " << inputFilename_ << std::endl;
    std::cout << "Output Filename: " << oututFilename_ << std::endl;
    std::fstream inputStream;
    std::fstream outputStream;
    
    outputStream.open(oututFilename_,std::ios::out);  
    
    if(outputStream.is_open()) 
    {
        outputStream << this->getHeader();   
    }
    
    inputStream.open( inputFilename_, std::ios::in ); 

    if (inputStream.is_open()){  
        std::string tp;

        while(getline(inputStream, tp))
        {
            //std::cout << tp << "\n"; 
            stringId str = stringId( tp );
            
            if(outputStream.is_open()) 
            {
                outputStream << "#. RAW: " << tp << std::endl;
                outputStream << str.getReference();
                outputStream << str.getMsgContext();   
                outputStream << str.getMsgId();   
                outputStream << str.getMsgStr();   
                outputStream << std::endl;   
            }            
        }

        inputStream.close(); 
    }
    
    if(outputStream.is_open()) 
    {
        outputStream.close();    
    }
}

void scummtr2po::createObjectTable( const std::string& filePath )
{
    
}
