#ifndef SYNTAX_ANALYZER_H
#define SYNTAX_ANALYZER_H

#include <string>
#include <vector>
#include <map>

#define Judge(i, NAME) if(SYM[p1] == i) {p1++;} \
                 else {return Error(NAME);}

struct TableEntry {
    std::string NAME, KIND;
    int VAL, LEVEL, ADR, NXT;
    TableEntry() { ADR = NXT = -1; }
    TableEntry(std::string t1, int t2, int t3, int t4, int t5, int t6) {
        NAME = t1; KIND = t2; VAL = t3; LEVEL = t4; ADR = t5; NXT = t6;
    }
};

struct Instruction {
    std::string f;
    int l, a;
    Instruction() { f = "", l = a = 0; }
    Instruction(std::string f1, int l1, int a1) {
        f = f1; l = l1; a = a1;
    }
};

struct Table {
    int PreTable, PreEntry, VNUM;
    std::vector<TableEntry> table;
    std::map<std::string, std::pair<int, int> > mp1;
    std::map<std::string, int> mp2;
    Table() {
        PreTable = PreEntry = -1;
        VNUM = 0; // 记录变量个数
        table.clear();
        mp1.clear(); // 用于记录常量-(数值,0)，变量-(相对地址,1)
        mp2.clear(); // 用于记录过程-入口地址
    }
    Table(int t1, int t2) : PreTable(t1), PreEntry(t2) {}
};

extern std::vector<Table> TABLE;
extern std::vector<std::string> label;
extern std::vector<std::vector<int> > G;
extern std::vector<Instruction> CODE;

void BLOCK();
void TABLE_OUTPUT(char *outdir);
void GRAPH_OUTPUT(char *outdir);
void CODE_OUTPUT(char *outdir);

/*------------------- Procedure Function -------------------*/
void Procedure();
void SubProcedure(int, int);

/*------------------- Specification Function -------------------*/
void ConstDefinition(int);
void ConstSpecification(int);
void VariableSpecification(int);
void ProcedureSpecification(int);

/*------------------- Statement Function -------------------*/
void Statement(int);
void AssignStatement(int);
void ConditionStatement(int);
void LoopStatement(int);
void CallStatement(int);
void ReadStatement(int);
void WriteStatement(int);
void CompoundStatement(int);

/*------------------- Other Functions -------------------*/
void Item(int);
void Factor(int);
void Condition(int);
void Expression(int);

#endif