

#include "scummtr2po.h"

  
int main(int argc, char** argv)
{
    //TODO: add getopts to parse
    
    std::cout << "You have entered " << argc
         << " arguments:" << "\n";
  
    for (int i = 0; i < argc; ++i)
        std::cout << argv[i] << "\n";
  
    std::string inputFilename = argv[1];
    std::string outputFilename = argv[2];
    
    
    scummtr2po parser = scummtr2po( inputFilename, outputFilename );
    
    parser.process();
    
    return 0;
}
