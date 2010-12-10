#include <stdio.h>
#include "version.h"

/*
 * Displays version info
 */
void
version (void)
{
  printf ("%s %s\n", PROGRAM_NAME, PROGRAM_VERSION);
}

/*
 * Displays version info and describes command line options
 */
void
help (void)
{
  version ();
  printf ("Usage: %s [OPTIONS] file\n\n", PROGRAM_NAME);
  printf ("OPTIONS:\n");
  printf (" -h\tShow this help\n");
  printf (" -v\tShow version info\n");
}
