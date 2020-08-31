#ifndef CODE_INTERPRETER_H
#define CODE_INTERPRETER_H

#include <string>
#include <vector>
#include <iostream>

extern int CAPACITY, BASE, PC, SP, BA;
extern std::vector<int> S;

void Run();

#endif