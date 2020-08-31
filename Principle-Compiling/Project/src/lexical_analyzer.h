#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H

#include <string>
#include <vector>

extern std::string keywords[29];
extern const int keytype, tagtype, numtype;
extern std::vector<int> SYM;
extern std::vector<std::string> ID;
extern std::vector<long long> NUM;

void SYM_OUTPUT(char *outdir);
bool GETSYM(char *indir);

#endif