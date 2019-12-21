#include "syscall.h"

int main() {
	Exec("../test/exit.noff");
	Yield();
    Exit(0);
}
