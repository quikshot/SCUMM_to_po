#include <iostream>
#include <fstream>
#include "third-party/cxxopts/include/cxxopts.hpp"
#include "scummtr2po.h"


bool fileExists ( std::string filepath )
{
   bool fileExists = false;
   std::ifstream ifile;
   ifile.open( filepath );
   if( ifile ) {
      fileExists = true;
      if( ifile.is_open())
      { 
          ifile.close();
      }
   }
   return fileExists;
}

const std::string checkParameterIsSetAndFileExists( const cxxopts::ParseResult& result, 
                                                    const std::string& paramFilename,
                                                    const std::string& errorToPrint
)
{
    if( result.count( paramFilename ) == 0 )
    {
        std::cerr << "ERR: Missing parameter: " << errorToPrint << std::endl;
        exit(0);
    }
    else
    {
        return result[ paramFilename ].as<std::string>();
    }
}

int main(int argc, char** argv)
{
    bool exportTr = false; 
    bool importTr = false;
    bool debug = false;
    bool test = false;
    
    std::string fileStrings = "";
    std::string filePo = "";
    std::string fileStringsConfig = "";
    std::string fileAddTranslation = "";
    
    cxxopts::Options options("ScummTranslations", "\nConverts strings extracted from Scumm games into PO/POT format\nConverts PO translations into strings formated for Scumm games\nhttps://github.com/quikshot/SCUMM_to_po");

    options.add_options()
        ("s,strings", "strings file (generated from scummtr.exe)", cxxopts::value<std::string>())
        ("p,po", "po file", cxxopts::value<std::string>())
        ("c,config", "config file (default: stringsFile.cfg)", cxxopts::value<std::string>())
        ("a,add-translation", "add a translation as notes(i.e another language)", cxxopts::value<std::vector<std::string>>())   
        ("e,export", "export translations from game", cxxopts::value<bool>()->default_value("false"))
        ("i,import", "import translations to game", cxxopts::value<bool>()->default_value("false"))
        ("t,test", "duplicate string as translation", cxxopts::value<bool>()->default_value("false"))
        ("d,debug", "Enable debugging", cxxopts::value<bool>()->default_value("false"))
        ("convertFromAscii", "convert strings file from ascii.output extension.utf8.txt", cxxopts::value<bool>()->default_value("false"))        
        ("h,help", "\nexport:\n   scummtr2po -e -s stringsEN.txt -p atlantis.pot\nimport:\n   scummtr2po -i -s stringsEN.txt -p ca.po\n")
    ;
    options.custom_help("-i/-e -s STRINGS_FILE -p PO_FILE [-d] [-h] [-c CONFIG_FILE]");
    auto result = options.parse(argc, argv);

    if (result.count("help"))
    {
      std::cout << options.help() << std::endl;
      
      exit(0);
    }
    

    // Check options are correct:
    importTr = result["import"].as<bool>();
    exportTr = result["export"].as<bool>();
    
    // Check only one action is requested:
    if( importTr == exportTr )
    {
        std::cerr << "You need to specify if you want to export translations from game (-e) or import translations to the game (-i)" << std::endl;
        exit(0);
    }
    // Check filenames are provided and exist
    fileStrings = checkParameterIsSetAndFileExists( result, "strings", "Provide: -s STRINGS_FILE" );
    if( !result.count("convertFromAscii") )
    {
        filePo = checkParameterIsSetAndFileExists( result, "po", "Provide: -p PO_FILE" );
    }   
    if( result.count("config") == 1)
    {
        fileStringsConfig = result["config"].as<std::string>();
    }else
    {
        fileStringsConfig = fileStrings + ".cfg";
    }
    if ( importTr )
    {
        if ( !fileExists(fileStringsConfig) ) 
        {
            std::cerr << "ERR: a valid config file is needed. Default file is: "<< fileStringsConfig << "\n Provide a valid file with: -c CONFIG_FILE" << std::endl;
            exit(0);
        }
        if ( fileExists(fileStrings) )
        {
            std::cerr << "WARNING: importing to an existing strings file: "<< fileStrings << "\n Overwrite? [y/N]" << std::endl;
            std::string userInput;
            std::cin >> userInput;
            if( ! ( (userInput == "y") || (userInput == "Y") ) )
            {   
                std::cerr << "ERR: Exiting" << std::endl;
            }else
            {
                std::cerr << "\nWARNING: " << fileStrings << " will be overwritten!!"<< std::endl;
            }
        }
    }
        
    debug = result["debug"].as<bool>();
    test  = result["test"].as<bool>();


    // Create converter with strings file and pot file.
    
    scummtr2po converter = scummtr2po( fileStrings, filePo , fileStringsConfig, debug );
    
    if( result.count("add-translation") )
    {
        auto& ff = result["add-translation"].as<std::vector<std::string>>();
        
        int i=1;
        for (auto& translationFile : ff)
        {
            std::cout << translationFile << std::endl;
            std::string lang=std::to_string(i);
            
            converter.addTranslation(translationFile,lang);
            ++i;
        }        
        
    }
    
    
    
    // Execute action: import or export.
    if( exportTr )
    {
            // Check parameters
        if( result.count("convertFromAscii") )
        {
            converter.convertFromAscii();
        }else{
            converter.scummToPo( test );
        }
    }
    else if ( importTr )
    {
        converter.poToScumm();
    }

    
    return 0;
}
