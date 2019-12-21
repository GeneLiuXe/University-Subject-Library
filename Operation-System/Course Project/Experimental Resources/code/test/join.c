#include "syscall.h"

int main() {
    SpaceId newProc = Exec("../test/yield.noff");
    Join(newProc);
    Exit(0);
}

