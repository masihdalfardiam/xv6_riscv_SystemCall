//
// Created by masih on 11/24/22.
//
/*#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"*/

#include "user.h"


int

main(int argc, char *argv[]) {


    int count = 0;
    int mem = 0;
    int n = 0;

    struct run *r;

    acquire(&kmem.lock);

    r = kmem.freelist;

    while (r != 0) {
        count += 1;
        r = r->next;
    }
    release(&kmem.lock);
    n = getpagesize();
    count = (n * count);
    return count;
    printf("xv6 free pages: %d\n");

    exit(0);


}
