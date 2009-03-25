/*void
_add (int reg1, int reg2, int reg3)
{
  return;
}

void
_sub (int reg1, int reg2, int reg3)
{
  return;
}

void
_mul (int reg1, int reg2, int reg3)
{
  return;
}

void
_div (int reg1, int reg2, int reg3)
{
  return;
}

void
_beq (int reg1, int reg2, int reg3)
{
  int oldpc = prog_counter;

  if (mem.registers[reg2] == mem.registers[reg3])
    {
      prog_counter = mem.registers[reg1];
    }

  reg_set (0x0, 0x1, oldpc);
}

void
_bgt (int reg1, int reg2, int reg3)
{
  int oldpc = prog_counter;

  if (mem.registers[reg2] > mem.registers[reg3])
    prog_counter = mem.registers[reg1];

  reg_set (0x0, 0x1, oldpc);
}

void
_ld (int reg, int src, int pos)
{
  if (mem.registers[src] + pos > 255)
    {
      printf ("Error: invalid address %#x.", (mem.registers[src] + pos));
      sys_dump (0x0);
      sys_halt (0x0);
    }

  int data = mem.data[mem.registers[src] + pos];
  reg_set (0x1, reg, data);
}

void
_st (int reg, int src, int val)
{
}

void
_lc (int reg, int val)
{
  reg_set (0x1, reg, val);
}

void
_jmp (int reg, int newpc)
{
  int oldpc = prog_counter;
  prog_counter = newpc;
  reg_set (0x0, 0x1, oldpc);
}

void
_inc (int reg, int val)
{
  reg_set (0x1, reg, mem.registers[reg] + val);
}

void
_sys (int reg, int code)
{
  sys_call (code, reg);
}
*/
void
execute (unsigned short int ins)
{
  /* seperate instruction into parts */
  short int a0 = (ins >> 12) % 16;
  short int a1 = (ins >> 8) % 16;
  short int a2 = (ins >> 4) % 16;
  short int a3 = (ins >> 0) % 16;
}
