#include <stdio.h>

/*
 * REGISTERS_LEN - number of registers the VM holds
 * DATA_LEN - number of user-defined data the VM can hold
 * INSTRUCTIONS_LEN - number of instructions the VM can hold
*/
#define REGISTERS_LEN 16
#define DATA_LEN 256
#define INSTRUCTIONS_LEN 256

/*
 * Data structure holding memory of the VM
 * registers - Holds the values of the vm registers (r0, r1, etc.)
 *   NOTE:
 *     r0 - not user writable, holds constant 0
 *     r1 - not user writable, used by instructions for values related
 *            to their computations, eg. div uses r1 for the remainder
 * data - Holds the loaded program data
 * instructions - Holds the instructions of the loaded program
 */
struct ant_memory {
  int registers[REGISTERS_LEN];
  int data[DATA_LEN];
  int instructions[INSTRUCTIONS_LEN];
};

/*
 * Variables
 * mem - Holds the memory for the VM
 * prog_counter - (PC), holds the location of the next instruction
 *   that the vm will execute
 */
struct ant_memory mem;
unsigned int prog_counter;

/*
 * Sets the initial values of the memory of the VM by giving
 * instructions invalid values and setting all other data, registers,
 * and the program counter to 0
 */
void
mem_init(void)
{
  int i;

  /* set all locations in instruction memory to 0xffff,
   * a value unused by the instruction set, to show that
   * these locations do not contain valid instructions
   */
  for (i = 0; i < INSTRUCTIONS_LEN; i++)
      mem.instructions[i] = 0xffff;

  /* set all locations in data memory to 0 */
  for (i = 0; i < DATA_LEN; i++)
      mem.data[i] = 0x0;

  /* set all registers to 0 */
  for (i = 0; i < REGISTERS_LEN; i++)
    {
      mem.registers[i] = 0x0;
    }

  /* initialize program counter to 0 */
  prog_counter = 0x0;
}

/*
 * Loads in data and instructions from a compiled file
 * fn - filename of the compiled VM program to load
 * returns - 1 on success, -1 on failure
 */
int
mem_load(char *fn)
{
  /* open up the file for reading */
  FILE *f = fopen(fn, "r");

  /* sanity check */
  if (f != NULL)
    {
      /* read the file into memory */
      while (!feof(f))
	{
	}

      /* return success */
      fclose(f);
      return 1;
    }
  else
    {
      /* return error */
      fclose(f);
      return -1;
    }
}

/*
 * Dumps the current state of the VM's memory out to a
 * file, cant.core in the current directory
 */
void
mem_dump(void)
{
  int i;
  FILE *f = fopen("cant.core", "w");

  if (f != NULL)
    {
      /* Dump instructions */
      fprintf(f, "--Instruction memory--\n");
      fprintf(f, "PC:\t%i\n", prog_counter);

      for (i = 0; i < INSTRUCTIONS_LEN; i++)
	fprintf(f, "i%i:\t%i\n", i, mem.instructions[i]);

      /* Dump data */
      fprintf(f, "--Data memory--\n");
      for (i = 0; i < DATA_LEN; i++)
	fprintf(f, "d%i:\t%i\n", i, mem.data[i]);

      /* Dump register values */
      fprintf(f, "--Register memory--\n");
      for (i = 0; i < REGISTERS_LEN; i++)
	fprintf(f, "r%i:\t%i\n", i, mem.registers[i]);
    }

  /* close the file */
  fclose(f);
}
