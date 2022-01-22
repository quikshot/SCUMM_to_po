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
#include <ctime>
#include <sstream>
#include <map>
#include <stdlib.h>
#include <list>
/**
 * @todo write docs
 */

#include "stringId.h"
#include "objects.h"


class scummtr2po
{
public:
    
    scummtr2po( const std::string& inputFilename, const std::string& oututFilename, const std::string& configFilename, const bool debug = false );
    ~scummtr2po();
    void scummToPo( bool test = false);
    void poToScumm();
    const std::string createContext(objects& obj, stringId& strId);
    const std::string getHeader(void);
    void addTranslation(std::string lang, std::string translationFile);

    void strReplaceTest();


private: 
    class extraFile
    {
    public:
        extraFile(std::string filename, std::string lang):
            filename_(filename),
            lang_(lang){
                //TODO: open stream. if error exit.
                 fs_.open( filename_, std::ios::in );  
                 if(fs_.is_open())
                 {
                    std::cout << "extraFile: " << lang_<<":"<<filename_<<std::endl;
                 }else
                 {
                    std::cerr << "CANNOT OPEN FILE:"<<":"<<filename_<<std::endl;
                }
            }
        ~extraFile(){ //TODO: close stream
            if( fs_.is_open() ) 
            {
                fs_.close();
            }
        };  
        
        //Read a line and store it for later
        void getLineIntoString()
        {
            if( fs_.is_open() ) 
            {
                getline(fs_, strLine_);
                stringId::strReplaceFromCodeToChar(strLine_);
                strLine_ = lang_ + ":" + strLine_;
            }else{
                std::cerr << "ERR:getLineIntoString:"<<filename_<<std::endl;
            }
        }    
                 
        std::string filename_;
        std::string lang_;
        std::fstream fs_;
        std::string strLine_;
    };
    std::string stringsFilename_;
    std::string poFilename_;
    std::string configFilename_;
    std::list<extraFile*> translations_;
    bool debug_;
};

#endif // SCUMMTR2PO_H
