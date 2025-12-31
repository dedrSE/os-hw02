#include "types.h"
#include "user.h"

int main (void) {
    int i, j, a = 0, pid;
    if ((pid = fork()) > 0)
        wait();
    else if (pid < 0) {
        printf(1, "fork error\n");
        exit();
    }
    else {
        for (i = 0, j = 3; i < 1e8; i++, j += i) {
            a += i;
            j = a + i;
            a *= (j-1) / (i+1);
        }
        printf(1, "result : %d\n", a);
    }

    exit();
}
