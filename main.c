#include "funcPack.h"

int main() {
    pid_t pid, pid2;
    pid = create_process();
    pid2 = shared_memory();
    exec_look();
    return 0;
}