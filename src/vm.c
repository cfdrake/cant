#include "version.c"
#include "mem.c"
#include "sys.c"
#include "instructions.c"

/*
 * Main loop of the vm
 */
void
vm_run()
{
  /* Reset prog counter as sanity check and execute instructions in memory */
  for(prog_counter = 0; prog_counter < instruction_pos; prog_counter++)
    {
      /* Grab next instruction and execute it */
      short int instruction = mem.instructions[prog_counter];

      if (instruction == 0xffffffff)
        {
          /* Special invalid instruction, signal to quit */
          break;
        }
      else
        {
          execute(instruction);
        }
    }

  /* Exit nicely */
  sys_halt (0x0);
}

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

  while ((c = getopt (argc, argv, "vh")) != -1)
    {
      switch (c)
	    {
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
  if (argc > 1)
    {
      file = argv[argc - 1];
    }
  else
    {
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
  if (st == -1)
    {
      printf ("Error: invalid file (%s).\n", file);
      sys_dump (0x0);
      sys_halt (0x0);
    }

  /* Run the vm */
  vm_run ();
}
