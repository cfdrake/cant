#include "mem.c"
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
}
