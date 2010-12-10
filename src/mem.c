#include <stdio.h>
#include "mem.h"
#include "version.h"

/*
 * Sets a register to a value
 * user - Whether the user is setting this, or if it is interally
 *          1 = user, other = internal
 * reg - register to set
 * val - value to set the register to
 */
int
reg_set (int user, int reg, int value)
{
  /* Check if the user is trying to set r0 or r1 */
  if (user == 1 && (reg == 0 || reg == 1))
    {
      printf ("Error: cannot set register %#x.\n", reg);
      sys_dump (0x0);
      sys_halt (0x0);
    } 

  /* Check for out of bounds registry number */
  if (reg < 0 || reg > REGISTERS_LEN)
    {
      printf ("Error: register %#x doesn't exist.\n", reg);
      sys_dump (0x0);
      sys_halt (0x0);
    }

  /* We should be alright, set the value and return success */
  mem.registers[reg] = value;
  return 1;
}

/*
 * Sets a memory address to a value
 * addr - element ("address") in data array
 * value - value to store
 */
void
mem_set (unsigned char addr, int value)
{
  /* Sanity check and store */
  if (addr < DATA_LEN)
    {
      mem.data[addr] = value;
    }
}

/*
 * Sets the initial values of the memory of the VM by giving
 * instructions invalid values and setting all other data, registers,
 * and the program counter to 0
 */
void
mem_init (void)
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
    mem.registers[i] = 0x0;

  /* initialize program counter to 0 */
  prog_counter = 0x0;
}

/*
 * Loads in data and instructions from a compiled file
 * fn - filename of the compiled VM program to load
 * returns - 1 on success, -1 on failure
 */
int
mem_load (char *fn)
{
  /* open up the file for reading */
  FILE *f = fopen (fn, "rb");

  /* sanity check */
  if (f != NULL)
    {
      /* read the file into memory */
      int read_mode = 0;
      instruction_pos = 0;
      data_pos = 0;

      while (feof (f) == 0)
	    {
	      /* mode 0 read instructions, mode != 0 read data */
	      if (read_mode == 0)
	        {
	          /* read in 2 bytes, add to mem, and update counter */
	          short int ins;
	          fread (&ins, sizeof (ins), 1, f);

	          /* check if we have hit the end of instructions yet */
	          if (ins == 0xffffffff)
                {
                  /* special invalid opcode, end of instructions */
                  read_mode = 1;
                }
              else
                {
                  /* still reading instructions, load into memory */
                  mem.instructions[instruction_pos] = ins;
                }

              instruction_pos++;
	        }
	      else
	        {
              /* read in 1 byte, add to mem, and update counter */
              unsigned char data;
              fread (&data, sizeof (data), 1, f);
              mem.data[data_pos] = data;
              data_pos++;
            }
        }

      /* return success */
      fclose (f);
      return 1;
    }
  else
    {
      /* return error */
      return -1;
    }
}

/*
 * Dumps the current state of the VM's memory out to a
 * file, cant.core in the current directory
 */
void
mem_dump (void)
{
  int i;
  FILE *f = fopen ("cant.core", "w");

  if (f != NULL)
    {
      /* Dump program data */
      fprintf (f, "%s %s\n\n", PROGRAM_NAME, PROGRAM_VERSION);

      /* Dump instructions */
      fprintf (f, "--Instruction memory--\n");
      fprintf (f, "PC:\t%#x\n", prog_counter);

      for (i = 0; i < INSTRUCTIONS_LEN; i++)
        if (mem.instructions[i] != 0xffffffff)
          fprintf (f, "i%#x:\t%#x\n", i, mem.instructions[i]);

      /* Dump data */
      fprintf (f, "--Data memory--\n");
      for (i = 0; i < DATA_LEN; i++)
        if (mem.data[i] != 0x0)
          fprintf (f, "d%#x:\t%#x\n", i, mem.data[i]);

      /* Dump register values */
      fprintf (f, "--Register memory--\n");
      for (i = 0; i < REGISTERS_LEN; i++)
	    fprintf (f, "r%#x:\t%#x\n", i, mem.registers[i]);
    }

  /* close the file */
  fclose (f);
}
