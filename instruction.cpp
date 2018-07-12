#include <iostream>
#include <fstream>
#include <iomanip>
#include <string.h>
#include <stdlib.h>
#include <bitset>
#include <algorithm>

#include "instruction.h"

using namespace std;

instructions::instructions(char* file,bool debug)
{
    verbose = debug;
    asm_file_name = new char;
    output_file_name = new char;
    Variable_Symbols_index = 16;
    strcpy (asm_file_name,file);

    //output file name handle
    char keys[] = ".";
    int i = strcspn (file,keys);
    strncpy ( output_file_name, file, i );
    strcat(output_file_name,".hack");

    //map symbol
    Predefined_Symbols.insert ( pair<string,unsigned int>("SP",0) );
    Predefined_Symbols.insert ( pair<string,unsigned int>("LCL",1) );
    Predefined_Symbols.insert ( pair<string,unsigned int>("ARG",2) );
    Predefined_Symbols.insert ( pair<string,unsigned int>("THIS",3) );
    Predefined_Symbols.insert ( pair<string,unsigned int>("THAT",4) );
    Predefined_Symbols.insert ( pair<string,unsigned int>("SCREEN",16384) );
    Predefined_Symbols.insert ( pair<string,unsigned int>("KBD",24576) );

    //Comp a=0
    C_instruction_comp.insert ( pair<string,unsigned int>("0",42) );
    C_instruction_comp.insert ( pair<string,unsigned int>("1",63) );
    C_instruction_comp.insert ( pair<string,unsigned int>("-1",58) );
    C_instruction_comp.insert ( pair<string,unsigned int>("D",12) );
    C_instruction_comp.insert ( pair<string,unsigned int>("A",48) );
    C_instruction_comp.insert ( pair<string,unsigned int>("!D",13) );
    C_instruction_comp.insert ( pair<string,unsigned int>("!A",49) );
    C_instruction_comp.insert ( pair<string,unsigned int>("-D",15) );
    C_instruction_comp.insert ( pair<string,unsigned int>("D+1",31) );
    C_instruction_comp.insert ( pair<string,unsigned int>("A+1",55) );
    C_instruction_comp.insert ( pair<string,unsigned int>("D-1",14) );
    C_instruction_comp.insert ( pair<string,unsigned int>("A-1",50) );
    C_instruction_comp.insert ( pair<string,unsigned int>("D+A",2) );
    C_instruction_comp.insert ( pair<string,unsigned int>("D-A",19) );
    C_instruction_comp.insert ( pair<string,unsigned int>("A-D",7) );
    C_instruction_comp.insert ( pair<string,unsigned int>("D&A",0) );
    C_instruction_comp.insert ( pair<string,unsigned int>("D|A",21) );

    //Comp a=1
    C_instruction_comp.insert ( pair<string,unsigned int>("M",112) );
    C_instruction_comp.insert ( pair<string,unsigned int>("!M",113) );
    C_instruction_comp.insert ( pair<string,unsigned int>("-M",115) );
    C_instruction_comp.insert ( pair<string,unsigned int>("M+1",119) );
    C_instruction_comp.insert ( pair<string,unsigned int>("M-1",114) );
    C_instruction_comp.insert ( pair<string,unsigned int>("D+M",66) );
    C_instruction_comp.insert ( pair<string,unsigned int>("D-M",83) );
    C_instruction_comp.insert ( pair<string,unsigned int>("M-D",71) );
    C_instruction_comp.insert ( pair<string,unsigned int>("D&M",64) );
    C_instruction_comp.insert ( pair<string,unsigned int>("D|M",85) );

    //dest
    C_instruction_dest.insert ( pair<string,unsigned int>("",0) );
    C_instruction_dest.insert ( pair<string,unsigned int>("M",1) );
    C_instruction_dest.insert ( pair<string,unsigned int>("D",2) );
    C_instruction_dest.insert ( pair<string,unsigned int>("MD",3) );
    C_instruction_dest.insert ( pair<string,unsigned int>("A",4) );
    C_instruction_dest.insert ( pair<string,unsigned int>("AM",5) );
    C_instruction_dest.insert ( pair<string,unsigned int>("AD",6) );
    C_instruction_dest.insert ( pair<string,unsigned int>("AMD",7) );

    //jump
    C_instruction_jump.insert ( pair<string,unsigned int>("",0) );
    C_instruction_jump.insert ( pair<string,unsigned int>("JGT",1) );
    C_instruction_jump.insert ( pair<string,unsigned int>("JEQ",2) );
    C_instruction_jump.insert ( pair<string,unsigned int>("JGE",3) );
    C_instruction_jump.insert ( pair<string,unsigned int>("JLT",4) );
    C_instruction_jump.insert ( pair<string,unsigned int>("JNE",5) );
    C_instruction_jump.insert ( pair<string,unsigned int>("JLE",6) );
    C_instruction_jump.insert ( pair<string,unsigned int>("JMP",7) );

    if(verbose)
    {
        cout<<"Instruction constructed"<<endl<<endl;
    }  
}

void instructions::remove_comment_whitespace()
{
    ifstream asmfile (asm_file_name);
    if (!asmfile.is_open())
        perror("ERROR OPENING ASM FILE");

    while(getline(asmfile, current_line)) 
    {
        for(unsigned int i=0;i<current_line.length();i++)
        {              
            //REMOVE COMMENT
            if (current_line[i]=='/')
                current_line.erase (i,current_line.length());
        }
        //REMOVE WHITESPACE
        current_line.erase( remove( current_line.begin(), current_line.end(), ' ' ), current_line.end() );

        //IF NOT EMPTY LINE, PUSH IT TO VECTOR
        if ( !current_line.empty() && current_line[0]!='	')
        {
            file_content.push_back(current_line);
        }
    }

    if(verbose)
    {
        unsigned int index = 0;
        cout<<"----------BOF----------"<<endl;
        for (vector<string>::const_iterator i = file_content.begin(); i != file_content.end(); ++i)
        {
            cout <<"ASM LINE["<<index<<"]: "<< *i << endl;
            index++;
        }
        cout<<"----------EOF----------"<<endl;
    }

    asmfile.close();
}

void instructions::check_lable()
{
    unsigned int index = 0;
   for (vector<string>::const_iterator i = file_content.begin(); i != file_content.end(); ++i) 
    {
        current_line = *i;
        //Matches (XXX)
         if ( current_line[0] == '('&& (current_line[current_line.length()-1]==')' ))
         {
            Lable.insert ( pair<string,unsigned int>(current_line.substr(1,current_line.length()-2),index) );
            if(verbose)
            {
                cout<<"check_lable(): "<<current_line<<endl;
                cout<<"Lable Found: "<<current_line.substr(1,current_line.length()-2)<<" at: "<<index<<endl<<endl;
            } 
            index--;
         }
         index++;  
    }
}

bool instructions::is_lable()
{
    map<string,unsigned int>::iterator it;
    it = Lable.find(current_line.substr(1,current_line.length()-1));
    if ( current_line[0] == '('&& (current_line[current_line.length()-1]==')' ))
    {
        return false;
    }
    if (it != Lable.end())
        return true;
    return false;
}

bool instructions::is_numeric_A()
{
    if (current_line[0]!='@')
    {
         return false;
    }  
	for(unsigned int i = 1; i < current_line.length() ; i++) 
    {
		if( !isdigit(current_line[i]) )
            return false;
	} 
    current_type = "numeric_A";
    return true;
}

bool instructions::is_symbol_A()
{
   if (current_line[0]!='@')
    {
         return false;
    }

    // R0 - R15
    if (current_line[1]=='R' && isdigit(current_line[2]) )
    {   
        if(verbose)
            cout<<"R"<<endl;
        current_type="R";
         return true;
    }

    //Predefined Symbols
    map<string,unsigned int>::iterator it;
    it = Predefined_Symbols.find(current_line.substr(1,current_line.length()-1));
    if (it != Predefined_Symbols.end())
    {
        if(verbose)
            cout<<"Predefined_Symbols"<<endl;
        current_type="Predefined_Symbols";
        return true;
    }
        
    //Variable Symbols
    for (unsigned int i=1; i<(current_line.substr(1,current_line.length()-1)).length(); i++ )
    {
        if (!isalpha((current_line.substr(1,current_line.length()-1))[i]))
            return false;
    }
    current_type="Variable_Symbols";
    if(verbose)
        cout<<"Variable_Symbols";
    return true;
}

void instructions::decode()
{
    for (vector<string>::const_iterator i = file_content.begin(); i != file_content.end(); ++i) 
    {
        current_line = *i;
        if(verbose)
        {
            cout<<"Decode Line: "<<current_line<<endl;
        }
        // Lable with ( )
         if ( current_line[0] == '('&& (current_line[current_line.length()-1]==')' ))
         {
             if(verbose)
            {
                cout<<"Ignore: "<<current_line<<endl;
            }
         }  
         //Lable i.e: (BAR)'s BAR
         else if (is_lable())
         {
             //Erase '@'
            current_line.erase (current_line.begin());
            binarycode.push_back(Lable[current_line]);
            if(verbose)
            {
                cout<<"is_lable() Binary:"<<setfill('0')<<setw(16)<<bitset<16>(Lable[current_line])<<endl<<endl;
            }
         }
         //numeric A instructions
         else if (is_numeric_A()) 
         {
            current_line.erase (current_line.begin());
            binarycode.push_back(stoi (current_line,NULL,10));
             if(verbose)
            {
                cout<<"is_numeric_A() Binary:"<<setfill('0')<<setw(16)<<bitset<16>(stoi (current_line,NULL,10))<<endl<<endl;
            }       
         }
         else if (is_symbol_A()) 
         {
             //R0-R15
            if(current_type=="R")
            {
                //Erase '@R'
                current_line.erase (current_line.begin(),current_line.begin()+2);
                binarycode.push_back(stoi (current_line,NULL,10));
                if(verbose)
                {
                    cout<<"is_symbol_A(R) Binary:"<<setfill('0')<<setw(16)<<bitset<16>(stoi (current_line,NULL,10))<<endl<<endl;
                }      
            }
            //SP LCL KBD SCREEN...
            if(current_type=="Predefined_Symbols")
            {
                //Erase '@'
                current_line.erase (current_line.begin());
                binarycode.push_back(Predefined_Symbols[current_line]);
                if(verbose)
                {
                    cout<<"is_symbol_A(P) Binary:"<<setfill('0')<<setw(16)<<bitset<16>(Predefined_Symbols[current_line])<<endl<<endl;
                }  
            }
            //USER DEFINED SYMBOL
            if(current_type=="Variable_Symbols")
            {
                //Erase '@'
                current_line.erase (current_line.begin());
                //If Exist
                map<string,unsigned int>::iterator it;
                it = Variable_Symbols.find(current_line);
                if (it != Variable_Symbols.end())
                {
                    binarycode.push_back(Variable_Symbols[current_line]);
                }
                //If Not
                else
                {
                   Variable_Symbols.insert( pair<string,unsigned int>(current_line,Variable_Symbols_index) );
                   binarycode.push_back(Variable_Symbols_index);
                   Variable_Symbols_index++;
                }
                if(verbose)
                {
                    cout<<"is_symbol_A(V) Binary:"<<setfill('0')<<setw(16)<<bitset<16>(Variable_Symbols_index - 1)<<endl<<endl;
                }  
            }
            //C-instruction
         }
         else
            {
                uint16_t C = 7,comp,dest,jump;
                string comp_str;
                string dest_str;
                string jump_str;

                size_t found_equal_sign = current_line.find('=');
                size_t found_colon_sign = current_line.find(';');

                // = ;
                if (found_equal_sign!=string::npos && found_colon_sign!=string::npos)
                {
                    dest_str = current_line.substr(0,found_equal_sign);
                    jump_str = current_line.substr(found_colon_sign + 1, 3);
                    comp_str = current_line.substr(found_equal_sign + 1, current_line.length() - dest_str.length() - jump_str.length() - 2 );
                    
                }
                // = 
                else if (found_equal_sign!=string::npos && found_colon_sign==string::npos)
                {
                    dest_str = current_line.substr(0,found_equal_sign);
                    comp_str = current_line.substr(found_equal_sign + 1, current_line.length() - dest_str.length() - 1 );
                    jump_str = "";
                }
                // ; 
                else if (found_equal_sign==string::npos && found_colon_sign!=string::npos)
                {
                    dest_str = "";
                    jump_str = current_line.substr(found_colon_sign + 1, 3 );
                    comp_str = current_line.substr( 0, current_line.length() - jump_str.length() - 1 );
                }
                //nothing
                else 
                {
                    dest_str = "";
                    comp_str = current_line;
                    jump_str = "";
                }

                comp = C_instruction_comp[comp_str];
                dest = C_instruction_dest[dest_str];
                jump = C_instruction_jump[jump_str];

                uint16_t result = ( C << 13 ) + ( comp << 6 ) + ( dest << 3) + jump;
                binarycode.push_back(result);

                if(verbose)
                {
                    cout<<"------------C INS------------"<<endl;
                    cout<<"comp: "<<comp_str<<"  value:  "<<setfill('0')<<setw(7)<<bitset<16>(comp)<<endl;
                    cout<<"dest: "<<dest_str<<"  value:  "<<setfill('0')<<setw(3)<<bitset<16>(dest)<<endl;
                    cout<<"jump: "<<jump_str<<"  value:  "<<setfill('0')<<setw(3)<<bitset<16>(jump)<<endl;
                    cout<<"Result:"<<setfill('0')<<setw(7)<<bitset<16>(result)<<endl;
                    cout<<"------------C END------------"<<endl<<endl;
                } 
            }
    }
}

void instructions::writefile()
{
    ofstream hackfile;
    hackfile.open (output_file_name);
    if (!hackfile.is_open())
        perror("ERROR OPENING HACK FILE");

    //Read vector and append to file    
    for (vector<uint16_t>::const_iterator i = binarycode.begin(); i != binarycode.end(); ++i)
    {
            hackfile<<setfill('0')<<setw(16)<<bitset<16>(*i).to_string()<<endl;
    }
    hackfile.close();
}

