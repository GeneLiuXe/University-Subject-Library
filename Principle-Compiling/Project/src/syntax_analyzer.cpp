#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include "lexical_analyzer.h"
#include "syntax_analyzer.h"

const std::string CONSTANT = "CONSTANT", VARIABLE = "VARIABLE", PROCEDURE = "PROCEDURE";
const int DX = 3;
int TX = 0, LEV = 0, ADR = DX, p1, p2, p3, sz;

std::vector<Table> TABLE;
std::vector<int> empty_vec;
std::vector<std::string> label;
std::vector<std::vector<int> > G;
std::vector<Instruction> CODE;

void DFS_OUTPUT(int TableNum, int EntryNum, int flag, std::ofstream &output) {
    // Table Header Output
    if(flag == 1){
        output << std::left << std::setfill('-') << std::setw(100) << "-" << std::endl;
        output << "TX: " << std::left << std::setfill(' ') << std::setw(10) << TableNum;
        if(EntryNum >= TABLE[TableNum].table.size()){
            output << std::endl;
            return;
        }
        flag = 0;
    }
    else output << "    " << std::left << std::setfill(' ') << std::setw(10) << " ";

    // Table Information Output
    output << "NAME: " << std::left << std::setw(20) << TABLE[TableNum].table[EntryNum].NAME;
    output << "KIND: " << std::left << std::setw(25) << TABLE[TableNum].table[EntryNum].KIND;
    if(TABLE[TableNum].table[EntryNum].KIND == CONSTANT)
        output << "VAL: " << std::left << std::setw(15) << TABLE[TableNum].table[EntryNum].VAL << std::endl;
    else{
        if(TABLE[TableNum].table[EntryNum].KIND == PROCEDURE) {
            output << "LEVEL: " << std::left << std::setw(15) << TABLE[TableNum].table[EntryNum].LEVEL
                  << "ADR: " << " " << std::endl;
        }
        else{
            output << "LEVEL: " << std::left << std::setw(15) << TABLE[TableNum].table[EntryNum].LEVEL
                  << "ADR: " << TABLE[TableNum].table[EntryNum].ADR << std::endl;
        }
    }

    // Recursion & Backtrack Output
    if(TABLE[TableNum].table[EntryNum].KIND == PROCEDURE) DFS_OUTPUT(TABLE[TableNum].table[EntryNum].NXT, 0, 1, output), flag = 1;    
    if(EntryNum+1 < TABLE[TableNum].table.size()) DFS_OUTPUT(TableNum, EntryNum+1, flag, output);
}

void TABLE_OUTPUT(char *outdir) {
    std::ofstream output(outdir);
    DFS_OUTPUT(0, 0, 1, output);
    output << std::left << std::setfill('-') << std::setw(100) << "-" << std::endl;
    output.close();
}

void GRAPH_OUTPUT(char *outdir) {
    std::ofstream output(outdir);
    int n = label.size();
    output << n << std::endl;
    for(int i = 0; i < n; i++)
        output << label[i] << " \n"[i==n-1];
    for(int i = 0; i < n; i++){
        int sz = G[i].size();
        if(sz == 0) output << std::endl;
        for(int j = 0; j < sz; j++)
            output << G[i][j] << " \n"[j==sz-1];
    }
    output.close();
}

void CODE_OUTPUT(char *outdir) {
    std::ofstream output(outdir);
    int sz = CODE.size();
    for(int i = 0; i < sz; i++)
        output << i+1 << " " << CODE[i].f << " " << CODE[i].l << " " << CODE[i].a << std::endl;
    output.close();
}

std::string ll2string(long long x){
    std::string result = "";
    if(x == 0) result += '0';
    else{
        while(x){
            result += '0'+(x%10);
            x /= 10;
        }
    }
    reverse(result.begin(), result.end());
    return result;
}

void Error(std::string error) {
    std::cout << "error: expected " << error << " at the position " << ll2string(p1) << "." << std::endl;
    std::cout << "Syntax Analyzer Error!" << std::endl;
    exit(0);
}

int Graph_init(std::string name, int fa){
    // Graph Construct
    label.push_back(name);
    G.push_back(empty_vec);
    int id = label.size()-1;
    G[fa].push_back(id);
    return id;
}

void BLOCK() {
    TABLE.push_back(Table());
    p1 = p2 = p3 = 0; sz = SYM.size()-1;
    label.clear(); G.clear();
    Procedure();
}

/*------------------- Procedure Function -------------------*/
void Procedure() {
    // Graph Construct
    label.push_back("程序");
    G.push_back(empty_vec);
    int id = label.size()-1;

    // the first code: jump to main procedure
    CODE.push_back(Instruction("jmp",0,0));
    SubProcedure(id, 1);

    if(SYM[p1] == 0 && p1 == sz){
        Graph_init(keywords[0], id);
        CODE.push_back(Instruction("opr", 0, 0));
    }
    else Error("'.' in the end of main procedure");
}

void SubProcedure(int fa, int op = 0) {
    int id = Graph_init("分程序", fa);
    
    if(SYM[p1] == 1) ConstSpecification(id);
    if(SYM[p1] == 5) VariableSpecification(id);
    if(SYM[p1] == 6) ProcedureSpecification(id);
    if(op){
        // main procedure add INT code and backfill JMP addr
        CODE.push_back(Instruction("int", 0, TABLE[0].VNUM+3));
        CODE[0].a = CODE.size();
    }
    Statement(id);
}

/*------------------- Specification Function -------------------*/
void ConstDefinition(int fa) {
    int id = Graph_init("常量定义", fa);
    TableEntry entry;
    if(SYM[p1] == tagtype) {
        // std::cout << p1 << "," << ID[p2] << "," << SYM[p1] << std::endl;
        entry.NAME = ID[p2++];
        entry.KIND = CONSTANT;
        p1++;
        Graph_init(entry.NAME, id);
    }
    else Error("'tagtype' in const definition");

    Judge(4, "'=' in const definition");
    Graph_init(keywords[4], id);

    if(SYM[p1] == numtype) {
        entry.VAL = NUM[p3++];
        entry.LEVEL = LEV;
        // redifinition const error
        if(TABLE[TX].mp1.find(entry.NAME) != TABLE[TX].mp1.end()) Error("undeclared 'variable name' in const definition");
        // assign value to the map of TABLE[TX], const's second dimensionality is 0
        TABLE[TX].mp1[entry.NAME] = std::make_pair(entry.VAL, 0);
        TABLE[TX].table.push_back(entry);
        p1++;
        Graph_init(ll2string(entry.VAL), id);
    }
    else Error("'numtype' in const definition");
}

void ConstSpecification(int fa) {
    int id = Graph_init("常量说明部分", fa);
    Judge(1, "'const' in const declaration");
    Graph_init(keywords[1], id);

    ConstDefinition(id);

    while(SYM[p1] == 2){
        p1++;
        Graph_init(keywords[2], id);
        ConstDefinition(id);
    }
    Judge(3, "';' in const declaration");
    Graph_init(keywords[3], id);
}

void RecognizeFlag(std::string flag, int fa, int op = 0) {
    TableEntry entry;
    entry.NAME = ID[p2++];
    entry.KIND = flag;
    entry.LEVEL = LEV;
    entry.ADR = ADR++;
    if(op){
        // redifinition procedure error
        if(TABLE[TX].mp2.find(entry.NAME) != TABLE[TX].mp2.end()) Error("different 'procedure name' in procedure declaration");
        // assign value to the map of TABLE[TX]
        TABLE[TX].mp2[entry.NAME] = CODE.size()+1;
    }
    else{
        // redifinition variable error
        if(TABLE[TX].mp1.find(entry.NAME) != TABLE[TX].mp1.end()) return Error("different 'variable name' in variable / const declaration");
        // assign value to the map of TABLE[TX], variable's second dimensionality is 1
        TABLE[TX].mp1[entry.NAME] = std::make_pair(entry.ADR, 1);
        // the number of variable in TABLE[TX] ++
        TABLE[TX].VNUM++;
    }
    TABLE[TX].table.push_back(entry);
    p1++;
    Graph_init(entry.NAME, fa);
}

void VariableSpecification(int fa) {
    int id = Graph_init("变量说明部分", fa);
    Judge(5, "'var' in variable declaration");
    Graph_init(keywords[5], id);
    if(SYM[p1] != tagtype) Error("'tagtype' in variable declaration");
    RecognizeFlag(VARIABLE, id);
    
    while(SYM[p1] == 2) {
        p1++;
        Graph_init(keywords[2], id);
        if(SYM[p1] != tagtype) Error("'tagtype' in variable declaration");
        RecognizeFlag(VARIABLE, id);
    }
    Judge(3, "';' in variable declaration");
    Graph_init(keywords[3], id);
}

void ProcedureSpecification(int fa) {
    int id = Graph_init("过程说明部分", fa);
    Judge(6, "'procedure' in procedure declaration");
    Graph_init(keywords[6], id);
    if(SYM[p1] == tagtype){
        RecognizeFlag(PROCEDURE, id, 1);
        TABLE.push_back(Table());
        TABLE[TABLE.size()-1].PreTable = TX;
        TABLE[TABLE.size()-1].PreEntry = TABLE[TX].table.size()-1;
        TABLE[TX].table[TABLE[TX].table.size()-1].NXT = TABLE.size()-1;
        TX = TABLE.size()-1, LEV++, ADR = DX;
    }
    else return Error("'tagtype' in procedure declaration");

    Judge(3, "';' in procedure declaration");
    Graph_init(keywords[3], id);
    
    // add the first INT code of procedure
    int index = CODE.size();
    CODE.push_back(Instruction("int",0,0));
    SubProcedure(id);
    // backfill the size of INT code
    CODE[index].a = TABLE[TX].VNUM + 3;

    Judge(3, "';' in procedure declaration");
    Graph_init(keywords[3], id);
    LEV--;
    int PreEntry = TABLE[TX].PreEntry;
    TX = TABLE[TX].PreTable;
    ADR = ADR - 1 + TABLE[TX].table[PreEntry].ADR;
    TABLE[TX].table[PreEntry].ADR = ADR++;
    CODE.push_back(Instruction("opr", 0, 0));

    while(SYM[p1] == 6) ProcedureSpecification(id);
}

/*------------------- Statement Function -------------------*/
std::pair<int,int> SearchPosition(std::string NAME, int op = 0) {
    /*
        constant: (-1, val), variable: (l, a), procedure: (l, addr)
        variable name should differ from const name 
        procedure name could be same with variable or const name
    */
    int now = TX, cnt = 0;
    while(now != -1) {
        if(!op && TABLE[now].mp1.find(NAME) != TABLE[now].mp1.end()){
            if(TABLE[now].mp1[NAME].second == 0)
                return std::make_pair(-1, TABLE[now].mp1[NAME].first);
            else
                return std::make_pair(cnt, TABLE[now].mp1[NAME].first);
        }
        // search procedure
        if(op && TABLE[now].mp2.find(NAME) != TABLE[now].mp2.end())
            return std::make_pair(cnt, TABLE[now].mp2[NAME]);
        now = TABLE[now].PreTable, cnt++;
    }
    return std::make_pair(-3, -1);
}

void Statement(int fa) {
    if(SYM[p1] == tagtype) AssignStatement(Graph_init("语句", fa));
    else if(SYM[p1] == 20) ConditionStatement(Graph_init("语句", fa));
    else if(SYM[p1] == 23) LoopStatement(Graph_init("语句", fa));
    else if(SYM[p1] == 22) CallStatement(Graph_init("语句", fa));
    else if(SYM[p1] == 25) ReadStatement(Graph_init("语句", fa));
    else if(SYM[p1] == 26) WriteStatement(Graph_init("语句", fa));
    else if(SYM[p1] == 8) CompoundStatement(Graph_init("语句", fa));
}

void AssignStatement(int fa) {
    int id = Graph_init("赋值语句", fa);
    if(SYM[p1] == tagtype) Graph_init(ID[p2++], id), p1++;
    else return Error("'tagtype' in assign statement");

    // acquire the position of variable
    std::pair<int, int> pos = SearchPosition(ID[p2-1]);
    // pos must be variable
    if(pos.first < 0) return Error("declared 'variable name' in assign statement");

    Judge(7, "':=' in assign statement");
    Graph_init(keywords[7], id);
    Expression(id);
    // assign code
    CODE.push_back(Instruction("sto", pos.first, pos.second));
}

void ConditionStatement(int fa) {
    int id = Graph_init("条件语句", fa);    
    Judge(20, "'if' in condition statement");
    Graph_init(keywords[20], id);
    Condition(id);
    CODE.push_back(Instruction("jpc", 0, 0));
    int index = CODE.size();    

    Judge(21, "'then' in condition statement");
    Graph_init(keywords[21], id);
    Statement(id);
    CODE[index-1].a = CODE.size()+1;
}

void LoopStatement(int fa) {
    int id = Graph_init("当型循环语句", fa);
    Judge(23, "'while' in loop statement");
    Graph_init(keywords[23], id);
    int index1 = CODE.size();
    Condition(id);
    CODE.push_back(Instruction("jpc", 0, 0));
    int index2 = CODE.size();    

    Judge(24, "'do' in loop statement");
    Graph_init(keywords[24], id);
    Statement(id);
    CODE.push_back(Instruction("jmp", 0, index1+1));
    CODE[index2-1].a = CODE.size()+1;
}

void CallStatement(int fa) {
    int id = Graph_init("过程调用语句", fa);
    Judge(22, "'call' in call procedure statement");
    Graph_init(keywords[22], id);
    if(SYM[p1] == tagtype) {
        // acquire the position of precedure
        std::pair<int, int> pos = SearchPosition(ID[p2], 1);
        // pos must be precedure
        if(pos.first >= 0) CODE.push_back(Instruction("cal", pos.first, pos.second));
        else Error("declared 'procedure name' in call procedure statement");
        
        Graph_init(ID[p2++], id), p1++;
    }
    else return Error("declared 'procedure name' in call procedure statement");
}

void ReadStatement(int fa) {
    int id = Graph_init("读语句", fa);
    Judge(25, "'read' in read statement");
    Graph_init(keywords[25], id);
    Judge(27, "'(' in read statement");
    Graph_init(keywords[27], id);
    if(SYM[p1] == tagtype) {
        CODE.push_back(Instruction("opr", 0, 16));
        // acquire the position of variable
        std::pair<int, int> pos = SearchPosition(ID[p2]);
        // pos must be variable
        if(pos.first >= 0) CODE.push_back(Instruction("sto", pos.first, pos.second));
        else Error("declared 'variable name' in read statement");
        
        Graph_init(ID[p2++], id), p1++;
    }
    else Error("declared 'variable name' in read statement");

    while(SYM[p1] == 2){
        p1++;
        Graph_init(keywords[2], id);
        if(SYM[p1] == tagtype) {
            CODE.push_back(Instruction("opr", 0, 16));
            // acquire the position of variable
            std::pair<int, int> pos = SearchPosition(ID[p2]);
            // pos must be variable
            if(pos.first >= 0) CODE.push_back(Instruction("sto", pos.first, pos.second));
            else Error("declared 'variable name' in read statement");

            Graph_init(ID[p2++], id), p1++;
        }
        else Error("declared 'variable name' in read statement");
    }

    Judge(28, "')' in read statement");
    Graph_init(keywords[28], id);
}

void WriteStatement(int fa) {
    int id = Graph_init("写语句", fa);
    Judge(26, "'write' in write statement");
    Graph_init(keywords[26], id);
    Judge(27, "'(' in write statement");
    Graph_init(keywords[27], id);
    Expression(id);
    CODE.push_back(Instruction("opr", 0, 14));
    CODE.push_back(Instruction("opr", 0, 15));

    while(SYM[p1] == 2){
        p1++;
        Graph_init(keywords[2], id);
        Expression(id);
        CODE.push_back(Instruction("opr", 0, 14));
        CODE.push_back(Instruction("opr", 0, 15));
    }

    Judge(28, "')' in write statement");
    Graph_init(keywords[28], id);
}

void CompoundStatement(int fa) {
    int id = Graph_init("复合语句", fa);
    Judge(8, "'begin' in compound statement");
    Graph_init(keywords[8], id);
    Statement(id);
    while(SYM[p1] == 3){
        p1++;
        Graph_init(keywords[3], id);
        Statement(id);
    }
    Judge(9, "'end' in compound statement");
    Graph_init(keywords[9], id);
}

/*------------------- Other Functions -------------------*/
void Item(int fa) {
    int id = Graph_init("项", fa), op = 0;
    Factor(id);
    while(SYM[p1] == 13 || SYM[p1] == 14){
        if(SYM[p1] == 13) op = 1;
        else op = -1;
        Graph_init(keywords[SYM[p1++]], id);
        Factor(id);
        // solve *、/
        if(op == 1) CODE.push_back(Instruction("opr", 0, 4));
        else CODE.push_back(Instruction("opr", 0, 5));
    }
}

void Factor(int fa) {
    int id = Graph_init("因子", fa);
    if(SYM[p1] == tagtype) {
        // acquire the position of variable / constant
        std::pair<int, int> pos = SearchPosition(ID[p2]);
        // pos must be variable
        if(pos.first >= 0) CODE.push_back(Instruction("lod", pos.first, pos.second));
        else if(pos.first == -1) CODE.push_back(Instruction("lit", 0, pos.second));
        else Error("declared 'variable name' as a factor");

        Graph_init(ID[p2++], id), p1++;
        return;
    }
    if(SYM[p1] == numtype) {
        CODE.push_back(Instruction("lit", 0, NUM[p3]));
        Graph_init(ll2string(NUM[p3++]), id), p1++;
        return;
    }
    if(SYM[p1] == 27) {
        p1++;
        Graph_init(keywords[27], id);
        Expression(id);
        Judge(28, "')' in the end of expression statement as a factor");
        Graph_init(keywords[28], id);
        return;
    }
    return Error("'tagtype' / 'numtype' / 'expression statement' as a factor");
}

void Condition(int fa) {
    int id = Graph_init("条件", fa);
    if(SYM[p1] == 10){
        p1++;
        Graph_init(keywords[10], id);
        Expression(id);
        CODE.push_back(Instruction("opr", 0, 6));
    }
    else{
        Expression(id);
        if(SYM[p1] == 4 || (SYM[p1] >= 15 && SYM[p1] <= 19)) {
            Graph_init(keywords[SYM[p1++]], id);
            int op = SYM[p1-1];
            Expression(id);
            if(op == 4) CODE.push_back(Instruction("opr", 0, 8));
            else CODE.push_back(Instruction("opr", 0, op-6));
        }
        else Error("'operator specifier' between expression statements");
    }
}

void Expression(int fa) {
    int id = Graph_init("表达式", fa), op = 0;
    if(SYM[p1] == 11 || SYM[p1] == 12) {
        if(SYM[p1] == 12) op = -1;
        Graph_init(keywords[SYM[p1++]], id);
    }
    Item(id);
    // solve uminus
    if(op == -1) {
        CODE.push_back(Instruction("lit", 0, -1));
        CODE.push_back(Instruction("opr", 0, 4));
    }
    while(SYM[p1] == 11 || SYM[p1] == 12) {
        if(SYM[p1] == 11) op = 1;
        else op = -1;
        Graph_init(keywords[SYM[p1++]], id);
        Item(id);
        // solve +、-
        if(op == 1) CODE.push_back(Instruction("opr", 0, 2));
        else CODE.push_back(Instruction("opr", 0, 3));
    }
}