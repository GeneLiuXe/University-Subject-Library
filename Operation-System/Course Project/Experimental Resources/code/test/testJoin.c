#include "syscall.h"

int main() {
    SpaceId newProc = Exec("../test/exit.noff");
    Join(newProc);
    Exit(0);
}

