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

#ifndef SCUMMTR2PO_H
#define SCUMMTR2PO_H

#include <iostream>
#include <fstream>
#include <string>

/**
 * @todo write docs
 */


class scummtr2po
{
public:
    
    scummtr2po( const std::string& inputFilename, const std::string& oututFilename);
    ~scummtr2po();
    void process();
    
    const std::string getHeader(void);
    
    void createObjectTable( const std::string& filePath );

private:

    class stringId
    {
    public:
        stringId( const std::string& line);
        ~stringId();
        
        std::string rawText_;
        std::string completeStringId_;
        std::string stringId_;
        std::string roomNumber_;
        std::string type_;
        std::string typeNumber_;
        std::string op_;
        
        /* 
         * Convert: 
        //[097:LSCR#0210](27)\255\010\056\245\255\010\107\000\255\010\010\000\255\010\000\000Brace the door with a\016rock!
        
           To:
        #. [001:VERB#0026](D8)
        #. Object interaction: shelves
        msgctxt "Room:001 VERB#0026"
        msgid "Here's a cartouche from\016The\016Well\016of\016Souls."
        msgstr ""
        */
        
        const std::string getMsgId();
        const std::string getReference();
        const std::string getMsgContext();
        const std::string getMsgStr();
        
    };
    
    std::string inputFilename_;
    std::string oututFilename_;
};

#endif // SCUMMTR2PO_H
