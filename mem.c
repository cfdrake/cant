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
struct cant_memory {
  int registers[REGISTERS_LEN];
  unsigned char data[DATA_LEN];
  short int instructions[INSTRUCTIONS_LEN];
};

/*
 * Variables
 * mem - Holds the memory for the VM
 * prog_counter - (PC), holds the location of the next instruction
 *   that the vm will execute
 * instruction_pos - keeps track of current instruction position for reading
 * data_pos - keeps track of current data position for reading
 */
struct cant_memory mem;
unsigned char prog_counter;
int instruction_pos,
  data_pos;

/*
 * Sets a register to a value
 * user - Whether the user is setting this, or if it is interally
 *          1 = user, other = internal
 * reg - register to set
 * val - value to set the register to
 */
int
reg_set(int user, int reg, int value)
{
  /* Check if the user is trying to set r0 or r1 */
  if (user == 1 && (reg == 0 || reg == 1))
    return -1;

  /* Check for out of bounds registry number */
  if (reg < 0 || reg > REGISTERS_LEN)
    return -2;

  /* We should be alright, set the value and return success */
  mem.registers[reg] = value;
  return 1;
}

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
  FILE *f = fopen(fn, "rb");

  /* sanity check */
  if (f != NULL)
    {
      /* read the file into memory */
      int read_mode = 0;
      instruction_pos = 0;
      data_pos = 0;

      while (feof(f) == 0)
	{
	  /* mode 0 read instructions, mode != 0 read data */
	  if (read_mode == 0)
	    {
	      /* read in 2 bytes, add to mem, and update counter */
	      short int ins;
	      fread(&ins, sizeof(ins), 1, f);

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
	      fread(&data, sizeof(data), 1, f);
	      mem.data[data_pos] = data;
	      data_pos++;
	    }
	}

      /* return success */
      fclose(f);
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
mem_dump(void)
{
  int i;
  FILE *f = fopen("cant.core", "w");

  if (f != NULL)
    {
      /* Dump program data */
      fprintf(f, "%s %s\n\n", PROGRAM_NAME, PROGRAM_VERSION);

      /* Dump instructions */
      fprintf(f, "--Instruction memory--\n");
      fprintf(f, "PC:\t%#x\n", prog_counter);

      for (i = 0; i < INSTRUCTIONS_LEN; i++)
	if (mem.instructions[i] != 0xffffffff)
	  fprintf(f, "i%#x:\t%#x\n", i, mem.instructions[i]);

      /* Dump data */
      fprintf(f, "--Data memory--\n");
      for (i = 0; i < DATA_LEN; i++)
	if (mem.data[i] != 0x0)
	  fprintf(f, "d%#x:\t%#x\n", i, mem.data[i]);

      /* Dump register values */
      fprintf(f, "--Register memory--\n");
      for (i = 0; i < REGISTERS_LEN; i++)
	fprintf(f, "r%#x:\t%#x\n", i, mem.registers[i]);
    }

  /* close the file */
  fclose(f);
}
