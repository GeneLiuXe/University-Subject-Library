#include "syntax_analyzer.h"
#include "code_interpreter.h"

int CAPACITY = 100, BASE = 100, PC, SP, BA;
std::vector<int> S;

void RuntimeError(std::string error) {
    std::cout << "error: " << error << " at the CODE[" << PC << "], " 
              << CODE[PC-1].f << " " << CODE[PC-1].l << " " << CODE[PC-1].a << "." << std::endl;
    std::cout << "Code Interpreter Error!" << std::endl;
    exit(0);
}

void Push(int v) {
    if(SP+2 > S.size()) CAPACITY += BASE, S.resize(CAPACITY); 
    S[++SP] = v;
}

bool OneInstruction() {
    std::string op = CODE[PC-1].f;
    int l = CODE[PC-1].l, a = CODE[PC-1].a;
    if(op == "lit") {
        Push(a);
    }
    else if(op == "lod") {
        int tb = BA;
        while(l--) tb = S[tb];
        Push(S[tb+a]);
    }
    else if(op == "sto") {
        int tb = BA;
        while(l--) tb = S[tb];
        if(tb+a >= S.size()) RuntimeError("access the undefined stack space");
        S[tb+a] = S[SP];
        SP--;
    }
    else if(op == "cal") {
        int tb = BA;
        while(l--) tb = S[tb];
        Push(tb);
        Push(BA);
        Push(PC);
        SP -= 3;
        PC = a-1;
        BA = SP+1;
    }
    else if(op == "int") {
        SP += a;
    }
    else if(op == "jmp") {
        PC = a-1;
    }
    else if(op == "jpc") {
        if(!S[SP]) PC = a-1;
        SP--;
    }
    else if(op == "opr") {
        switch (a)
        {
            case 0:
                SP = BA-1;
                PC = S[BA+2];
                BA = S[BA+1];
                break;
            case 1:
                S[SP] = -S[SP];
                break;
            case 2:
                SP--;
                S[SP] = S[SP] + S[SP+1];
                break;
            case 3:
                SP--;
                S[SP] = S[SP] - S[SP+1];
                break;
            case 4:
                SP--;
                S[SP] = S[SP] * S[SP+1];
                break;
            case 5:
                SP--;
                if(S[SP+1] == 0) RuntimeError("divisor is 0");
                S[SP] = S[SP] / S[SP+1];
                break;
            case 6:
                S[SP] = S[SP] % 2;
                break;
            case 7:
                SP--;
                S[SP] = S[SP] % S[SP+1];
                break;
            case 8:
                SP--;
                S[SP] = (S[SP] == S[SP+1]);
                break;
            case 9:
                SP--;
                S[SP] = (S[SP] != S[SP+1]);
                break;
            case 10:
                SP--;
                S[SP] = (S[SP] < S[SP+1]);
                break;
            case 11:
                SP--;
                S[SP] = (S[SP] <= S[SP+1]);
                break;
            case 12:
                SP--;
                S[SP] = (S[SP] > S[SP+1]);
                break;
            case 13:
                SP--;
                S[SP] = (S[SP] >= S[SP+1]);
                break;
            case 14:
                std::cout << S[SP--];
                break;
            case 15:
                std::cout << "\n";
                break;
            case 16:
                Push(0);
                std::cout << "Please input an integer: ";
                std::cin >> S[SP];
                break;
            default:
                break;
        }
    }
    return true;
}

void Run() {
    S.resize(CAPACITY);
    PC = 1, SP = -1, BA = 0;
    Push(0);
    Push(0);
    Push(-1);
    SP -= 3;

    std::cout << "Choose: '0' for Run, '1' for Debug" << std::endl;
    int op;
    while(std::cin >> op) {
        if(op == 0 || op == 1) break;
        else std::cout << "Choose: '0' for Run, '1' for Debug" << std::endl;
    }
    std::cin.clear();
    std::cin.sync();
    if(!op) {
        while(PC) {
            OneInstruction();
            PC += 1;
        }
    }
    else {
        while(PC) {
            std::cout << "PC: " << PC << ", CODE[PC]: " << CODE[PC-1].f << " " << CODE[PC-1].l << " " << CODE[PC-1].a << std::endl;
            OneInstruction();
            std::cout << "One Instruction Executed, BA: " << BA << ", SP: " << SP << ", STACK[0~10]: [";
            for(int i = 0; i <= 10; i++)
                std::cout << S[i] << " ]"[i==10];
            std::cout << std::endl << "Please input '1' to continue" << std::endl;
            while(getchar() != '1');
            std::cout << std::endl;
            PC += 1;
        }   
    }
}
