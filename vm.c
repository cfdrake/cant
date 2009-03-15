#include <stdlib.h>
#include <unistd.h>
#include "mem.c"
#include "sys.c"
#define VERSION "0.1"

/*
 * Displays version info
 */
void
version(void)
{
  printf("cant (vm) %s\n", VERSION);
}

/*
 * Displays version info and describes command line options 
 */
void
help(void)
{
  version();
  printf("Usage: cant [OPTIONS] file\n\n");
  printf("OPTIONS:\n");
  printf(" -h\tShow this help\n");
  printf(" -v\tShow version info\n");
}

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

  if (st == -1)
    {
      printf("Error: Invalid file (%s).", file);
      sys_halt();
    }
}
