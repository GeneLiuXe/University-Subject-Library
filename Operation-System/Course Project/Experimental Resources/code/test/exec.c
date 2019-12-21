#include "syscall.h"

int main() {
	Exec("../test/exit.noff");
	Exec("../test/halt.noff");
    Exit(0);
}
