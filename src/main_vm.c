#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "vm.h"

/*
 * Starting point of the VM
 */
int
main (int argc, char *argv[])
{
    /*
     * Parse command line options
     */
    int c;
    char *file;

    while ((c = getopt (argc, argv, "vh")) != -1) {
        switch (c) {
        case 'v':
            /*
             * Display version info
             */
            version ();
            exit (0);
        case 'h':
            /*
             * Display help info
             */
            help ();
            exit (0);
        }
    }

    /*
     * Get filename from last argument
     */
    if (argc > 1) {
        file = argv[argc - 1];
    } else {
        printf ("Error: no filename specified.\n");
        sys_dump (0x0);
        sys_halt (0x0);
    }

    /*
     * Initialize the VM
     */
    mem_init ();
    int st = mem_load (file);

    /*
     * Check for loading errors
     */
    if (st == -1) {
        printf ("Error: invalid file (%s).\n", file);
        sys_dump (0x0);
        sys_halt (0x0);
    }

    /* Run the vm */
    vm_run ();
}
