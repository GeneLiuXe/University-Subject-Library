#include <iostream>
#include <fstream>
#include <map>
#include "lexical_analyzer.h"

std::string keywords[29] = {".", "const", ",", ";", "=", "var", "procedure", ":=", "begin", "end", "odd", "+", "-", "*", "/", "<>", "<", "<=", ">", ">=", "if", "then", "call", "while", "do", "read", "write", "(", ")"};
const int keytype = 28, tagtype = keytype + 1, numtype = keytype + 2;

std::map<std::string, int> mp;
std::vector<int> SYM;
std::vector<std::string> ID;
std::vector<long long> NUM;

void SYM_OUTPUT(char *outdir) {
    std::ofstream output(outdir);
    int p2 = 0, p3 = 0, sz = SYM.size();
    for(int p1 = 0; p1 < sz; p1++){
        if(SYM[p1] <= keytype) output << keywords[SYM[p1]] << std::endl;
        else if(SYM[p1] == tagtype) output << ID[p2++] << std::endl;
        else output << NUM[p3++] << std::endl;
    }
    output.close();
}

bool GETSYM(char *indir) {
    // init
    std::ifstream input(indir);
    mp.clear(); SYM.clear(); ID.clear(); NUM.clear();
    for(int i = 0; i < 30; i++)
        mp[keywords[i]] = i;
    // lexical analysis
    std::string line;
    while(getline(input, line)){
        for(int i = 0; line[i]; i++){
            if(line[i] == ' ' || line[i] == '\n' || (int)line[i] <= 32){
                continue;
            }
            else if(line[i] >= '0' && line[i] <= '9'){
                // identify number
                long long number = line[i]-'0';
                while(line[i+1] && line[i+1] >= '0' && line[i+1] <= '9'){
                    number = number * 10 + (int)(line[++i] - '0');
                }
                SYM.push_back(numtype);
                NUM.push_back(number);
            }
            else if(line[i] >= 'a' && line[i] <= 'z'){
                // identify variable
                std::string tmp = "\0";
                tmp += line[i];
                while(line[i+1] && ((line[i+1] >= '0' && line[i+1] <= '9') || (line[i+1] >= 'a' && line[i+1] <= 'z'))){
                    tmp += line[++i];
                }
                if(mp.find(tmp) != mp.end()) SYM.push_back(mp[tmp]);
                else SYM.push_back(tagtype), ID.push_back(tmp);
            }
            else{
                std::string tmp = "\0";
                tmp += line[i];
                while((mp.find(tmp+line[i+1]) != mp.end() || mp.find(tmp) == mp.end()) && line[i+1] && line[i+1] != ' ' && line[i+1] != '\n' && (int)line[i+1] != 13){
                    tmp += line[++i];
                }
                if(mp.find(tmp) != mp.end()) SYM.push_back(mp[tmp]);
                else {input.close(); return 0;}
            }
        }
    }
    input.close();
    return 1;
}