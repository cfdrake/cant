/* Constants for types of variables to check */
#define TYPE_SRC    0
#define TYPE_DES    1
#define TYPE_REG    2
#define TYPE_CONST8 3
#define TYPE_CONST4 4

/*
 * Determines if the specified arg is of a type (des, reg, etc.)
 */
void
check (int arg, int type, int param)
{
  /* Determine what type to check for */
  switch (type)
    {
      case TYPE_SRC:
        if (!(arg >= 0 && arg < REGISTERS_LEN))
          {
            printf ("Error: expected SRC for param %i.\n", param);
            sys_dump (0x0);
            sys_halt (0x0);
          }
      case TYPE_DES:
        if (!(arg >= 2 && arg < REGISTERS_LEN))
          {
            printf ("Error: expected DES for param %i.\n", param);
            sys_dump (0x0);
            sys_halt (0x0);
          }
      case TYPE_REG:
        if (!(arg >= 0 && arg < REGISTERS_LEN))
          {
            printf ("Error: expected REG for param %i.\n", param);
            sys_dump (0x0);
            sys_halt (0x0);
          }
      case TYPE_CONST8:
        break;
      case TYPE_CONST4:
        if (!(arg >= 0 && arg <= 15))
          {
            printf ("Error: expected CONST4 for param %i.\n", param);
            sys_dump (0x0);
            sys_halt (0x0);
          }
    }
}

/*
 * Integer addition
 * des - destination register
 * src1 - addend
 * src2 - addend
 */
void
_add (int des, int src1, int src2)
{
  check (des, TYPE_DES, 1);
  check (src1, TYPE_SRC, 2);
  check (src2, TYPE_SRC, 3);
  printf ("add\n");

  /* Calculate and store, and update r1 */
  int sum = mem.registers[src1] + mem.registers[src2];
  reg_set (0x1, des, sum);

  if (sum > 127)
    {
      reg_set (0x0, 0x1, 1);
    }
  else if (sum < -128)
    {
      reg_set (0x0, 0x1, -1);
    }
  else
    {
      reg_set (0x0, 0x1, 0);
    }
}

/*
 * Integer subtraction
 * des - destination register
 * src1 - minuend
 * src2 - subtrahend
 */
void
_sub (int des, int src1, int src2)
{
  check (des, TYPE_DES, 1);
  check (src1, TYPE_SRC, 2);
  check (src2, TYPE_SRC, 3);

  /* Calculate and store/update r1 */
  int diff = mem.registers[src1] + mem.registers[src2];
  reg_set (0x1, des, diff);
  
  if (diff > 127)
    {
      reg_set (0x0, 0x1, 1);
    }
  else if (diff < -128)
    {
      reg_set (0x0, 0x1, -1);
    }
  else
    {
      reg_set (0x0, 0x1, 0);
    }
}

/*
 * Integer multiplication
 * des - destination register
 * src1 - factor
 * src2 - factor
 */
void
_mul (int des, int src1, int src2)
{
  check (des, TYPE_DES, 1);
  check (src1, TYPE_SRC, 2);
  check (src2, TYPE_SRC, 3);
  
  /* Calculate and store/update r1 */
  int prod = src1 * src2;

  reg_set (0x1, des, prod);
  reg_set (0x0, 0x1, prod);
}

/*
 * Integer division
 * des - destination register
 * src1 - dividend
 * src2 - divisor
 */
void
_div (int des, int src1, int src2)
{
  check (des, TYPE_DES, 1);
  check (src1, TYPE_SRC, 2);
  check (src2, TYPE_SRC, 3);
  
  /* Check for divide-by-zero error, and divide if possible */
  if (mem.registers[src2] == 0)
    {
      printf ("Error: division by zero.\n");
      sys_dump (0x0);
      sys_halt (0x0);
    }
  else
    {
      reg_set (0x1, des, ((int)mem.registers[src1] / mem.registers[src2]));
      reg_set (0x0, 0x1, mem.registers[src1] % mem.registers[src2]);
    }
}

/*
 * Branch if equal to
 * reg1 - location to jump to
 * reg2 - arg1
 * reg3 - arg2
 */
void
_beq (int reg1, int reg2, int reg3)
{
  check (reg1, TYPE_REG, 1);
  check (reg2, TYPE_REG, 2);
  check (reg3, TYPE_REG, 3);
  
  /* Check for condition and set pc if needed */
  int oldpc = prog_counter;

  if (mem.registers[reg2] == mem.registers[reg3])
    {
      prog_counter = mem.registers[reg1];
    }
  
  reg_set (0x0, 0x1, oldpc);
}

/*
 * Branch if greater than
 * reg1 - location to jump to
 * reg2 - arg1
 * reg3 - arg2
 */
void
_bgt (int reg1, int reg2, int reg3)
{
  check (reg1, TYPE_REG, 1);
  check (reg2, TYPE_REG, 2);
  check (reg3, TYPE_REG, 3);
  
  /* Check for condition and set pc if needed */
  int oldpc = prog_counter;

  if (mem.registers[reg2] > mem.registers[reg3])
    {
      prog_counter = mem.registers[reg1];
    }
  
  reg_set (0x0, 0x1, oldpc);
}

/*
 * Load a value from memory into a register
 * des - destination register
 * src1 - first part of addr
 * const4 - second part of addr
 * addr = src1 + const4
 */
void
_ld (int des, int src1, int const4)
{
  check (des, TYPE_DES, 1);
  check (src1, TYPE_SRC, 2);
  check (const4, TYPE_CONST4, 3);

  /* Calculate address and store */
  int addr = src1 + const4;

  if (addr > 255)
    {
      printf ("Error: invalid address %#x.\n", addr);
      sys_dump (0x0);
      sys_halt (0x0);
    }
  else
    {
      reg_set (0x1, des, mem.data[addr]);
    }
}

/*
 * Saves a register into memory
 * reg - register to store
 * src1 - first part of addr
 * const4 - second part of addr
 * addr = src1 + const4
 */
void
_st (int reg, int src1, int const4)
{
  check (reg, TYPE_REG, 1);
  check (src1, TYPE_SRC, 2);
  check (const4, TYPE_CONST4, 3);
  
  /* Store value of reg into memory */
  int addr = mem.registers[src1] + const4;

  if (addr > 255)
    {
      printf ("Error: invalid address %#x.\n", addr);
      sys_dump (0x0);
      sys_halt (0x0);
    }
  else
    {
      mem_set (addr, mem.registers[reg]);
    }
}

/*
 * Loads a constant into a register
 * des - destination register
 * const8 - constant to save
 */
void
_lc (int des, int const8)
{
  check (des, TYPE_DES, 1);
  check (const8, TYPE_CONST8, 2);

  /* Set des to const8 */
  reg_set (0x1, des, const8);
}

/*
 * Jump to an instruction
 * reg - discarded
 * const8 - prog counter number
 */
void
_jmp (int reg, int const8)
{
  check (reg, TYPE_REG, 1);
  check (const8, TYPE_CONST8, 2);

  /* Set new pc */
  int oldpc = prog_counter;
  prog_counter = const8;

  /* Store old pc in reg1 */
  reg_set (0x0, 0x1, oldpc);
}

/*
 * Increases the value of a selected register by a constant
 * des - destination register
 * const8 - constant to add
 */
void
_inc (int des, int const8)
{
  check (des, TYPE_DES, 1);
  check (const8, TYPE_CONST8, 2);
  
  /* Set des to des + const8 */
  reg_set (0x1, des, mem.registers[des] + const8);
  
  /* Set r1 */
  reg_set (0x0, 0x1, ((mem.registers[des] > 127) ? 1 : -1));
}

/*
 * Executes a system call
 * reg - register to pass to syscall
 * const8 - syscall code to run
 */
void
_sys (int reg, int const8)
{
  check (reg, TYPE_REG, 1);
  check (const8, TYPE_CONST8, 2);

  /* Execute system call */
  sys_call (reg, const8);
}

/*
 * Executes a given instruction, or errors out
 * ins - instruction to run
 */
void
execute (unsigned short int ins)
{
  /* seperate instruction into parts */
  short int a0 = (ins >> 12) % 16;
  short int a1 = (ins >> 8) % 16;
  short int a2 = (ins >> 4) % 16;
  short int a3 = (ins >> 0) % 16;

  /* Run associated instruction */
  switch(a0)
    {
      case 0x0:
        _add (a1, a2, a3);
        break;
      case 0x1:
        _sub (a1, a2, a3);
        break;
      case 0x2:
        _mul (a1, a2, a3);
        break;
      case 0x3:
        _div (a1, a2, a3);
        break;
      case 0x6:
        _beq (a1, a2, a3);
        break;
      case 0x7:
        _bgt (a1, a2, a3);
        break;
      case 0x8:
        _ld (a1, a2, a3);
        break;
      case 0x9:
        _st (a1, a2, a3);
        break;
      case 0xa:
        _lc (a1, a3);
        break;
      case 0xb:
        _jmp (a1, a3);
        break;
      case 0xc:
        _inc (a1, a3);
        break;
      case 0xf:
        _sys (a1, a3);
        break;
      default:
        printf ("Error: invalid opcode %#x.\n", a0);
        sys_dump (0x0);
        sys_halt (0x0);
    }
}
