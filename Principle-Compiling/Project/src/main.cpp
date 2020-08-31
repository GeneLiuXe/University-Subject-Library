#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include "lexical_analyzer.h"
#include "syntax_analyzer.h"
#include "code_interpreter.h"

char indir[] = "data/input/PL0_code";
char outdir1[] = "data/output/PL0_lexicon.txt"; // lexical analysis result
char outdir2[] = "data/output/PL0_table.txt";   // symbol table
char outdir3[] = "data/output/PL0_graph.txt";   // syntax tree data
char outdir4[] = "data/output/PL0_code.txt";    // PL0 code

int main(int argc, char * argv[]) {
    int len = strlen(indir);
    if(argc > 1) for(int i = 0; argv[1][i]; i++) indir[len++] = argv[1][i];
    indir[len++] = '.'; indir[len++] = 'i'; indir[len++] = 'n';

    // Output keywords
    // for(int i = 0; i < 29; i++)
    //     std::cout << "| " << i << " | " << keywords[i] << " |" << std::endl;

    // Lexical Analyzer
    std::cout << "-------------------------------- PART ONE ----------------------------------" << std::endl;
    if(!GETSYM(indir)) {std::cout << "Lexical Analyzer Error!" << std::endl; return 0;}
    else std::cout << "Lexical Analyzer Succeed!" << std::endl;
    SYM_OUTPUT(outdir1);
    std::cout << "The Total Number of Keywords + Identifiers + Numbers in PL/0 Code: " << SYM.size() << '\n' << std::endl;

    // Syntax Analyzer
    std::cout << "-------------------------------- PART TWO ----------------------------------" << std::endl;
    BLOCK();
    std::cout << "Syntax Analyzer Succeed!" << std::endl;
    TABLE_OUTPUT(outdir2);
    GRAPH_OUTPUT(outdir3);
    CODE_OUTPUT(outdir4);

    // Code Interpreter
    std::cout << std::endl << "-------------------------------- PART THREE --------------------------------" << std::endl;
    Run();
    std::cout << "Program Finished Successfully!" << std::endl;
    return 0;
}