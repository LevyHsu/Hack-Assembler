#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include <string>
#include <vector>
#include <map>
#include <inttypes.h>

class instructions
{
    private:
    bool verbose;

    std::string current_line;
    std::string current_type;

    char* asm_file_name;
    char* output_file_name;
    int Variable_Symbols_index;

    std::map<std::string,unsigned int> Variable_Symbols;
    std::map<std::string,unsigned int> Predefined_Symbols;
    std::map<std::string,unsigned int> Lable;
    std::map<std::string,unsigned int> C_instruction_comp;
    std::map<std::string,unsigned int> C_instruction_dest;
    std::map<std::string,unsigned int> C_instruction_jump;

    //File content without whitespace
    std::vector<std::string> file_content;
    std::vector<uint16_t> binarycode;
    
    public:
    instructions(char* file,bool debug);
    ~instructions();
    void remove_comment_whitespace();
    void check_lable();
    bool is_lable();
    bool is_numeric_A();
    bool is_symbol_A();
    void decode();
    void writefile();
};

#endif