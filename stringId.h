#ifndef STRINGID_
#define STRINGID_

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <sstream>
#include <map>
#include <stdlib.h>


static const std::map<std::string, std::string> replaceMap_ = {
{"\\173","¡"},
{"\\168","¿"},
{"\\135","ç"},
{"\\128","Ç"},
{"\\164","ñ"},
{"\\165","Ñ"},
{"\\143","Á"},
{"\\160","á"},
{"\\133","à"},
{"\\183","À"},
{"\\144","É"},
{"\\130","é"},
{"\\136","ê"},
{"\\138","è"},
{"\\212","È"},
{"\\214","Í"},
{"\\161","í"},
{"\\139","ï"},
{"\\216","Ï"},
{"\\224","Ó"},
{"\\162","ó"},
{"\\149","ò"},
{"\\227","Ò"},
{"\\233","Ú"},
{"\\163","ú"},
{"\\129","ü"},
{"\\015","™"},
{"\\255\\001","/newLine/"}, 
{"\\254\\001","/new2Line/"}, 
{"\\255\\002","/keepText/"}, 
{"\\255\\003","/wait/"}, //wait()
{"\\255\\004","/getInt/"}, //
{"\\255\\006","/getName/"}, //
{"\\255\\007","/getString/"},
{"\\254\\008","/unknownOp/"}};    

static const std::map<std::string, std::string> replaceMapClean_ = {
{"\\173","¡"},
{"\\168","¿"},
{"\\135","ç"},
{"\\128","Ç"},
{"\\164","ñ"},
{"\\165","Ñ"},
{"\\143","Á"},
{"\\160","á"},
{"\\133","à"},
{"\\183","À"},
{"\\144","É"},
{"\\130","é"},
{"\\136","ê"},
{"\\138","è"},
{"\\212","È"},
{"\\214","Í"},
{"\\161","í"},
{"\\139","ï"},
{"\\216","Ï"},
{"\\224","Ó"},
{"\\162","ó"},
{"\\149","ò"},
{"\\227","Ò"},
{"\\233","Ú"},
{"\\163","ú"},
{"\\129","ü"},
{"\\015","™"},
{"\\255\\001"," "}, 
};    

class stringId
{
public:
    stringId(const std::__cxx11::string& line, int id);
    ~stringId();
    
    std::string rawText_;
    std::string completeStringId_;
    std::string stringId_;
    std::string roomNumber_;
    std::string type_;
    std::string typeNumber_;
    std::string op_;
    std::string configText_;
    int stringIdentifier_;
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
    bool isNotEmpty();
    const std::string getMsgId();
    const std::string getReference();
    const std::string getMsgContext();
    const std::string getMsgStr( bool test = false);
    void  setStringDuplicated( int duplicatedId );

    static void strReplaceFromCodeToChar( std::string& str , bool debug_ = false);
    static void strReplaceFromCharToCode( std::string& str , bool debug_ = false);    
    static std::string removeFinalAt(std::string str);
    static void strReplaceFromCodeToCharWithoutSpecial ( std::string& str , bool debug_ = false);

private:
    
    bool isEmptyString_;
    int duplicatedId_;
    
    //String replacement for extended ASCII chars. 
    
    //https://www.ascii-codes.com/cp850.html
    //TODO: if game is in another laguage (i.e. german/french) more keys must be added!
    
};
#endif
