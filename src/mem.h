#ifndef MEM_H
#define MEM_H

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
struct cant_memory
{
  int registers[REGISTERS_LEN];
  unsigned char data[DATA_LEN];
  short int instructions[INSTRUCTIONS_LEN];
};

struct cant_memory mem;

/*
 * Variables
 * mem - Holds the memory for the VM
 * prog_counter - (PC), holds the location of the next instruction
 *   that the vm will execute
 * instruction_pos - keeps track of current instruction position for reading
 * data_pos - keeps track of current data position for reading
 */
unsigned char prog_counter;
int instruction_pos, data_pos;

int reg_set (int user, int reg, int value);
void mem_set (unsigned char addr, int value);
void mem_int(void);
int mem_load(char *fn);
void mem_dump(void);

#endif // MEM_H
