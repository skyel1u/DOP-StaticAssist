#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

// loop and conditional jump assignment
void vuln_f()
{
    uint64_t *p, *q;
    uint64_t old_value = 1, new_value = 2;
    uint64_t i;
    uint64_t state;
    p = &old_value, q = &new_value;
    i = 0x50;
    state = 0;
    int dummy;
    char *buf = (char *)&dummy;
    //char buf[0x20];

    // gadget dispatcher
    while (i--)
    {
        read(0, buf, 1000); // memory corruption is here
        printf("i is 0x%x state is 0x%x\n", i, state);
        if (state == 0)
        {
            // a gadget
            // assignment operation
            p = q;
        }
        else if (state == 1)
        {
            // a gadget
            // load operation
            p = *q;
        }
        else if (state == 2)
        {
            // a gadget
            // store operation
            *p = q;
        }
        else
        {
            printf("state value is out of this experiment %llu\n", state);
        }
        printf("p = %x d = %x\n", p, q);
    }
}

int main()
{
    vuln_f();
    return 0;
}