#include "third-party/cxx_argp/cxx_argp_parser.h"
#include <iostream>
#include "scummtr2po.h"


int main(int argc, char** argv)
{
    bool exportTr = false; 
    bool importTr = false;
    
    std::string fileStrings = "";
    std::string filePoTranslation = "";
    
    
    // Command line options
    cxx_argp::parser parser;

    parser.add_option({"export", 'e', nullptr, 0, "export strings from Scumm to Po file"}, exportTr );
    
    parser.add_option({"import", 'i', nullptr, 0, "import from Po file to Scumm strings file"}, importTr );
    
    parser.add_option({nullptr, 's', "stringFile", 0, "strings file extracted with scummtr.exe"}, fileStrings);
    
    parser.add_option({nullptr, 'p', "poFile", 0, "Po/Pot file to export/import strings"}, filePoTranslation);
    
    
    if (parser.parse(argc, argv)) {
        std::cerr << "parsing OK\n";
    } else {
        std::cerr << "there was an error - exiting\n";
        return 1;
    }

    

    // Create converter with strings file and pot file.
    scummtr2po converter = scummtr2po( fileStrings, filePoTranslation );
    
    if( exportTr )
    {
        converter.scummToPo();
    }
    else if ( importTr )
    {
        converter.poToScumm();
    }
    else
    {
        std::cerr << "You need to specify import or export with -i or -e\n";
    }
        
    
    
    return 0;
}
