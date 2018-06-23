#include <iostream>
#include <fstream>
#include <string>
#include "instruction.h"

using namespace std;

int main(int argc, char* argv[]) 
{
    string arg;
    bool debug = false;
    for (int i = 1; i < argc; i++) 
    {
	    arg = string(argv[i]);
	    if (arg == "-v")  // Verbose output
	        debug = true;
    }
    
    instructions* asmfile;
    asmfile = new instructions(argv[1], debug);
    asmfile->remove_comment_whitespace();
	asmfile->check_lable();
    asmfile->decode();
    asmfile->writefile();
    
    return 0;
}