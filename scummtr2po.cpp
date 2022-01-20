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
    std::string dateString (dt);
    
    dateString=dateString.substr(0,(int)dateString.length()-1);
    std::stringstream stream;
    stream << "# POT File created using scummtr2po \n" 
           << "# https://github.com/quikshot/SCUMM_to_po\n" 
           << "# Context extracted from strings created using scummtr.exe tool\n" 
           << "msgid \"\"\n" 
           << "msgstr \"\"\n" 
           << "\"Project-Id-Version: \\n\"\n" 
           << "\"POT-Creation-Date: " <<  dateString <<  "\\n\"\n" 
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
    //https://www.ascii-codes.com/cp850.html
    //TODO: check if file exists
    //init substitutions:
    fromCode["\\173"]="¡";
    fromCode["\\168"]="?";
    fromCode["\\135"]="ç";
    fromCode["\\128"]="Ç";
    fromCode["\\164"]="ñ";
    fromCode["\\165"]="Ñ";
    fromCode["\\143"]="Á";
    fromCode["\\160"]="á";
    fromCode["\\133"]="à";
    fromCode["\\183"]="À";
    fromCode["\\144"]="É";
    fromCode["\\130"]="é";
    fromCode["\\136"]="ê";
    fromCode["\\138"]="è";
    fromCode["\\212"]="È";
    fromCode["\\214"]="Í";
    fromCode["\\161"]="í";
    fromCode["\\139"]="ï";
    fromCode["\\216"]="Ï";
    fromCode["\\224"]="Ó";
    fromCode["\\162"]="ó";
    fromCode["\\149"]="ò";
    fromCode["\\227"]="Ò";
    fromCode["\\233"]="Ú";
    fromCode["\\163"]="ú";
    fromCode["\\129"]="ü";
    fromCode["\\015"]="™";
    fromCode["\\255\\001"]="/newLine/"; 
    fromCode["\\254\\001"]="/new2Line/"; 
    fromCode["\\255\\002"]="/keepText/"; 
    fromCode["\\255\\003"]="/wait/"; //wait()
    fromCode["\\255\\004"]="/getInt/"; //
    fromCode["\\255\\006"]="/getName/"; //
    fromCode["\\255\\007"]="/getString/";
    fromCode["\\254\\008"]="/unknownOp/";
    
    for (const auto& kv : fromCode) {    
        if(debug_)
        {    std::cout << "Replacements: code:" << kv.first << " value:" << kv.second << std::endl; }
        
        toCode[kv.second] = kv.first;
    }
    //strReplaceTest();
    //exit(0);
}

void scummtr2po::strReplaceTest()
{
    //test
    bool dbg=debug_;
    debug_=true;
    //std::string s ="r\\161o. Ara ve un wait \\255\\003 i despres una var amb params: \\255\\007\\016\\133. Bon dia, a ca\\135ar.";
    std::string s="219_: \\255\\005\\110\\000 \\255\\006\\109\\000@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@";

    std::string inputs(s);
    std::cout << "TEST!!!!"<<std::endl;
    std::cout << "1: "<<s<<std::endl;
    strReplaceFromCodeToChar(s);
    std::string outputs(s);
    std::cout << "2: "<<s<<std::endl;
    strReplaceFromCharToCode(s);
    std::string back(s);
    std::cout << "3: "<<s<<std::endl;
    std::cout << "TEST!!!!"<<std::endl;
    debug_=dbg;
    std::cout << "RESULT:"<<std::endl;
    std::cout << "Input:"<<inputs<<std::endl;
    std::cout << "Output:"<<outputs<<std::endl;
    std::cout << "Back:"<<back<<std::endl;
    if( inputs == back)
        std::cout << "check: OK!" << std::endl;
    else
        std::cout << "check: ERROR!" << std::endl;
}

void scummtr2po::strReplaceFromCodeToChar(std::string& str)
{
    if(debug_)
    {
        /*for (const auto& kv : toCode) {    
            std::cout << "key:" << kv.first << " value:" << kv.second << std::endl;
        } */       
        std::cout << "FromCode: input: "<<str<<std::endl;
    }
    std::map<std::string,std::string>::iterator it;
    for (const auto& kv : fromCode) 
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
        std::cout << "DIGITS: "<< str.substr(found,4) << std::endl;
        //check if 3 following chars ar digits.
        if( isdigit((int)(str.substr(found+1,1).c_str()[0])) && isdigit((int)((char)str.substr(found+2,1).c_str()[0])) && isdigit((int)((char)str.substr(found+3,1).c_str()[0])))
        {
            
            str.replace(found, 1, "//");
        }
        

        //find next substitution in string
        found = str.find("\\",found+4);
    }    
}


void scummtr2po::strReplaceFromCharToCode( std::string& str )
{

    if(debug_)
    {
        std::cout << "ToCode: input: "<<str<<std::endl;
    }
    std::map<std::string,std::string>::iterator it;
    for (const auto& kv : toCode) 
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
                strReplaceFromCharToCode( substitution );
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
            strReplaceFromCodeToChar ( str.stringId_ );
            std::string strId = str.getMsgId();
            
            
            bool duplicated = false;
            if( str.isNotEmpty() && os.is_open()) 
            {
                auto contextStr = createContext(objs, str); 
                
                // store all strings related to this context
                // search duplicates of current string in same context
                if( context != contextStr )
                {
                    if(debug_) {std::cout << "New context: " << contextStr <<std::endl;}
                    context = contextStr;
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
                    os << "#. RAW: " << lineStr << std::endl;
                    os << "#. stringId:"<<lineNumber<<"_"<<std::endl;
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
}



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
    std::cout << "..done" << std::endl;
    
}


