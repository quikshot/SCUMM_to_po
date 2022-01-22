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
    tm *ltm = localtime(&now);

    // convert now to string form
    char* dt = ctime(&now);
    std::string dateString (dt);
    
    dateString=dateString.substr(0,(int)dateString.length()-1);
    std::stringstream stream;
    stream << "# POT File created using scummtr2po \n" 
           << "# https://github.com/quikshot/SCUMM_to_po\n" 
           << "# Context extracted from strings created using scummtr.exe tool\n" 
           << "msgid \"\"\n" 
           << "msgstr \"\"\n" 
           << "\"Project-Id-Version: \\n\"\n" 
           << "\"POT-Creation-Date: " <<  1900 + ltm->tm_year << "-" << 1 + ltm->tm_mon <<"-"<< ltm->tm_mday  <<" "<< ltm->tm_hour << ":"<<ltm->tm_min << "+0100"<< "\\n\"\n" 
           << "\"PO-Revision-Date: \\n\"\n" 
           << "\"Language-Team: \\n\"\n" 
           << "\"MIME-Version: 1.0\\n\"\n" 
           << "\"Content-Type: text/plain; charset=UTF-8\\n\"\n" 
           << "\"Last-Translator: \\n\"\n" 
           << "\"Content-Transfer-Encoding: 8bit\\n\"\n" 
           << "\"X-Generator: Poedit 3.0\"\n\n";
    return stream.str();
}
    
scummtr2po::scummtr2po( const std::string& stringsFilename, const std::string& poFilename, const std::string& configFilename, const bool debug ):
stringsFilename_(stringsFilename),
poFilename_(poFilename),
configFilename_(configFilename),
debug_(debug)
{

    //strReplaceTest();
    //exit(0);
}

void scummtr2po::addTranslation(std::string lang, std::string translationFile)
{
    extraFile* tr = new extraFile(lang, translationFile);
    translations_.push_back( tr );
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
        stream <<"Room: " << strId.roomNumber_ << " Text related to object: " << stringId::removeFinalAt(obj.getObjectName( stoi(strId.typeNumber_) ));
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
        auto currentPositionInFile = pos.tellg();
        
        int loopFile=0;
        pos.seekg(0,pos.beg);
        while( loopFile <= 1 )
        {
            while( getline(pos, lineStr) )
            {
                //std::cout << "getStringFromPoFile: "<<lineStr<<std::endl;
                // Read PO file 
                // if line contains: "#. stringId:", read stringId number.
                std::string searchStr = "#. stringId:" + std::to_string(stringNumber) + "_";
                //std::cout << "search: "<<searchStr << "-->";
                std::size_t found = lineStr.find(searchStr);
                
                if( found != std::string::npos)
                {
                    foundId = true;
                    //std::cout << "FOUND id"<<std::endl;
                }
                else if( foundId == true )
                {
                    //std::cout << "search msgstr...";
                    std::size_t found = lineStr.find("msgstr \"");
                    if( found != std::string::npos)
                    {
                        //std::cout << "FOUND msg"<<std::endl;
                        std::string cut=lineStr.substr(8); //remove initial part "msgstr \""
                        msgstr_ = cut.substr(0,cut.length()-1);
                        while( getline(pos, lineStr) )
                        {
                            if( lineStr.substr(0,1)=="\"")
                            {
                               msgstr_ = msgstr_ + lineStr.substr(1,lineStr.length()-2);     
                               std::cout << "MULTILINE: " << msgstr_<<std::endl;
                            }
                            else
                            {
                                
                                break;
                            }
                        }
                        //ret=true;
                        //break;
                        return true;
                    }
                    else
                    {
                        //std::cout << "not found. continue"<<std::endl;
                        continue;
                    }
                        
                }else
                {
                    //std::cout << "not found initial stringId. continue"<<std::endl;
                    continue;
                }
            }
            //std::cout << "RESTARTING SEARCH FROM BEGINNING OF FILE"<<std::endl;
            ++loopFile;
            pos.seekg(0,pos.beg);
        }
        //std::cout << "Looped 2. NOT FOUND. returning position file to "<<currentPositionInFile<<std::endl;
        
        //string not found after 2 loops in file.
        //move file reading position to last string found.
        pos.seekg(currentPositionInFile);
        return false;
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
        if(debug_) { std::cout << "Looping.." <<std::endl; }
        
        while( getline(configos, lineStr) )
        {
 
            if(debug_)
            {
                std::cout << "CFG: " << lineStr << std::endl;
            }
            //storeContext from config file. Map:
            
            //if duplicated string, get string from context map.
            
            //read input string
            //search translation
            int stringToSearch = lineNumber;
            
            
            // Is string duplicated?
            std::size_t foundDupl = lineStr.find("DUPLICATED_TO_");
            if( foundDupl != std::string::npos )
            {
                if(debug_) { std::cout << "DUPLICATED:"<<lineStr.substr(foundDupl+14) << std::endl; }
                stringToSearch = std::stoi(lineStr.substr(foundDupl+14));
            }
            std::size_t found = 0;
            
            // Is string empty?
            if( (found = lineStr.find("SCUMMTR2PO_EMPTY_STRING")) != std::string::npos )
            {
                if(debug_) { std::cout << "EMPTY_STRING" << std::endl; }
                std::string substitution;
        
                substitution = lineStr.substr( 0 ,found);
                strs << substitution << std::endl;
            
                            
            }else if( getStringFromPoFile( pos, stringToSearch, msgstr) )
            {
                //TODO: If duplicated, search from stored context.
                //write output translated string
                //get initial part 
                std::string substitution;
                std::size_t found = lineStr.find("SCUMMTR2PO_STRING_ID_");
                if( (found != std::string::npos) || foundDupl )
                {
                    substitution = lineStr.substr( 0 ,found) + msgstr;
                }else{
                    std::cerr <<"ERR: wrong config file! "<< configFilename_ << " should contain string substitutions like SCUMMTR2PO_STRING_ID_NNN" << std::endl;
                }
                    
                if( debug_ )
                {
                    std::cout << "ORIGINAL  : " << lineNumber << ": " << lineStr << std::endl; 
                    std::cout << "TRANSLATED: " << lineNumber << ": " << msgstr << std::endl;
                    std::cout << "SUBST     : " << lineNumber << ": " << substitution << std::endl<<std::endl;
                }
                //replace extended ASCII characters (and special chars) for ASCII code in text
                stringId::strReplaceFromCharToCode( substitution );
                strs << substitution << std::endl;
                
            } else {
                std::cerr << "ERR: String "<< std::to_string(lineNumber) << " not found in file " << poFilename_ << std::endl;
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

bool stringDuplicated( std::map<int, stringId>& contextMap, stringId& str, int &duplicatedId  )
{
    for (auto &it : contextMap)
    {
        //int id = static_cast<int>(it.first);
        std::string sid = (it.second).getMsgId();
        //std::cout << std::to_string(id)  << ':' << str.getMsgId()<< " =? " << sid << std::endl;
        
        if ( sid  == str.getMsgId() )
        {
            //stringDuplicated
            duplicatedId = (it.second).stringIdentifier_;
            return true;
        }
    }
    return false;
}

void scummtr2po::scummToPo( bool test )
{
    std::map<int, stringId> contextMap;
    std::string context="";
    if(debug_)
    {
        std::cout << "Strings Filename: " << stringsFilename_ << std::endl;
        std::cout << "Po Filename: " << poFilename_ << std::endl;
        std::cout << "Intermediate config Filename: " << configFilename_ << std::endl;
    }
    
    std::fstream is;
    std::fstream os;
    std::fstream configos;
    //std::list<std::fstream> tranStreams;
    
    int lineNumber = 0;
    objects objs = objects(stringsFilename_, "objects.txt");
    
    is.open( stringsFilename_, std::ios::in ); 
    os.open( poFilename_, std::ios::out );  
    configos.open( configFilename_, std::ios::out );  
    
    if(os.is_open()) 
    {
        os << this->getHeader();   
    }

    
    if (is.is_open()){  
        std::string lineStr;
        
        // read strings file line by line and process 
        while( getline(is, lineStr) )
        {
            // Process line 
            stringId str = stringId( lineStr , lineNumber );
            stringId::strReplaceFromCodeToChar ( str.stringId_ );
            std::string strId = str.getMsgId();
            
            std::string translationStr="";
            
            for (auto& ex: translations_ )
            {
                ex->getLineIntoString();
            }
            
            bool duplicated = false;
            if( str.isNotEmpty() && os.is_open()) 
            {
                auto contextStr = createContext(objs, str); 
                std::string internalContext = str.roomNumber_;
                // store all strings related to this context
                // search duplicates of current string in same context
                if( context != internalContext )
                {
                    if(debug_) {std::cout << "New context: " << contextStr <<std::endl;}
                    context = internalContext;
                    contextMap.clear();
                    contextMap.insert( std::pair<int,stringId>( lineNumber, str ) );

                }else{
                    //search if current string already exists.
                    //if it already exists, mark as duplicate.
                    int duplicatedId = 0;
                    if( (duplicated = stringDuplicated( contextMap, str, duplicatedId ) ) )
                    { 
                        str.setStringDuplicated( duplicatedId );
                        if(debug_) {std::cout << "Found duplicated string: " << duplicatedId << std::endl;}
                    }
                    contextMap.insert( std::pair<int,stringId>( lineNumber, str ) );
                }
                // Only write to PO file if string is not duplicated in the same context.
                if( !duplicated )
                {
                    // Write POT file 
                    //os << "#. RAW: " << lineStr << std::endl;
                    os << "#. stringId:"<<lineNumber<<"_"<<std::endl;
                    
                    //Add extra translations
                    for (auto& ex: translations_ )
                    {
                        os << "#. "<< ex->strLine_ <<std::endl;
                    }                        
                    //os << str.getReference();
                    os << contextStr;
                    os << strId;   
                    os << str.getMsgStr( test );   
                    os << std::endl;   
                }
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
    
    for (auto& ex: translations_ )
    {
        delete ex;
    }   
}


void scummtr2po::convertFromAscii( )
{
    std::string outputFile = stringsFilename_ +".utf8.txt";
    if(debug_)
    {
        std::cout << "convertFromAscii =========" << std::endl;
        std::cout << "Strings Filename: " << stringsFilename_ << std::endl;
        std::cout << "Converted: " << outputFile << std::endl;
    }
    
    std::fstream is;
    std::fstream os;
    
    int lineNumber = 0;

    
    is.open( stringsFilename_, std::ios::in ); 
    os.open( outputFile, std::ios::out );  
    
    
    if (is.is_open()){  
        std::string lineStr;
        
        // read strings file line by line and process 
        while( getline(is, lineStr) )
        {
            // Process line 
            stringId::strReplaceFromCodeToCharWithoutSpecial ( lineStr ,false);
            lineStr = stringId::removeFinalAt(lineStr);
            if(os.is_open()) 
            {
                os<<lineStr<<std::endl; 
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




/***************************************
 * TEST
 */

void scummtr2po::strReplaceTest()
{
    //test
    bool dbg=debug_;
    debug_=true;
    //std::string s ="r\\161o. Ara ve un wait \\255\\003 i despres una var amb params: \\255\\007\\016\\133. Bon dia, a ca\\135ar.";
//[001:LSCR#0201](D8)\255\010\230\196\255\010\001\000\255\010\010\000\255\010\000\000No creo que pueda traducir lo dem\160s.
//[002:OBNA#0030](__)playa@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    
    std::string s="219_: \\255\\005\\110\\000 \\255\\006\\109\\000@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@";
    stringId sid = stringId("[001:LSCR#0201](D8)\\255\\010\\230\\196\\255\\010\\001\\000\\255\\010\\010\\000\\255\\010\\000\\000No creo que pueda traducir lo dem\\160s.\\255\\001\\222\\1111\\255\\004\\122\\221", 0 );
    std::string obj="baul@@@@@@@@@@@@@@@@@@@@@@@@@@@@@";
    std::string inputs(sid.stringId_);
    std::cout << "TEST!!!!"<<std::endl;
    std::cout << "RAW: "<<sid.rawText_<<std::endl;
    
    sid.strReplaceFromCodeToChar( sid.stringId_ , true);
    std::string outputs(sid.stringId_);
    sid.strReplaceFromCharToCode(sid.stringId_);
    std::string back(sid.stringId_);

    debug_=dbg;
    std::cout << "RESULT:"<<std::endl;
    std::cout << "Input:"<<inputs<<std::endl;
    std::cout << "Output:"<<outputs<<std::endl;
    std::cout << "Back:"<<back<<std::endl;
    if( inputs == back)
        std::cout << "check: OK!" << std::endl;
    else
        std::cout << "check: ERROR!" << std::endl;
    
    std::string objRemoved = stringId::removeFinalAt(obj);
    std::cout << "OBJ        : "<<obj<<std::endl;
    std::cout << "OBJ REMOVED: "<<objRemoved<<std::endl;
}
