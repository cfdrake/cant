#include "version.c"
#include "mem.c"
#include "sys.c"

/*
 * Starting point of the VM
 */
int
main(int argc, char *argv[])
{
  /* Parse command line options */
  int c;
  while ((c = getopt(argc, argv, "vh")) != -1)
    {
      switch(c)
	{
	case 'v':
	  /* Display version info */
	  version();
	  exit(0);
	case 'h':
	  /* Display help info */
	  help();
	  exit(0);
	}
    }

  /* Get filename from last argument */
  char *file = argv[argc - 1];

  /* Initialize the VM */
  mem_init();
  int st = mem_load(file);

  /* Check for loading errors */
  if (st == -1)
    {
      printf("Error: Invalid file (%s).\n", file);
      sys_halt(0x0);
    }
}
