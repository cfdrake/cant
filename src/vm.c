#include "vm.h"
#include "mem.h"
#include "sys.h"
#include "instructions.h"

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
          execute (instruction);
        }
    }

  /* Exit nicely */
  sys_halt (0x0);
}
