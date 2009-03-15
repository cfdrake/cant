/*
 * Halts VM immediately 
 * Call 0x0
 */
void
sys_halt(int reg)
{
  reg_set(0x0, 0x1, 0x0);
  exit(0);
}

/*
 * Dumps memory information of the VM
 * Call 0x1
 */
void
sys_dump(int reg)
{
  reg_set(0x0, 0x1, 0x0);
  mem_dump();
}

/*
 * Displays a register's int value on the screen
 * Call 0x2
 */
void
sys_put_int(int reg)
{
  reg_set(0x0, 0x1, 0x0);
  printf("%i", mem.registers[reg]);
}

/*
 * Displays a register's char value on the screen
 * Call 0x3
 */
void
sys_put_char(int reg)
{
  reg_set(0x0, 0x1, 0x0);
  printf("%c", mem.registers[reg]);
}

/*
 * Displays a string on the screen
 * Call 0x4
 */
void
sys_put_str(int reg)
{
  reg_set(0x0, 0x1, 0x0);

  int i = 0x0;
  char c = 0x42;  /* heh heh */

  /* Loop from data starting point until we find
   *   a null character, printing out each char
   */
  while (c != 0x000)
    {
      /* Retrieve next char and print it */
      c = mem.data[reg + i];
      printf("%c", c);

      /* Move onto the next location */
      i++;
    }
}

/* 
 * Reads an int from input, and saves it in a register
 * Call 0x5
 */
void
get_int(int reg)
{
  /* Get input */
  
  reg_set(0x1, reg, in);

  /* Set r1 */
  reg_set(0x0, 0x1, 0x0);
}

/*
 * Reads a char from input, and saves it in a register as an int
 * Call 0x6
 */
void
get_char(int reg)
{
  /* Get input */
  /* Set r1 */
  reg_set(0x0, 0x1, 0x0);
}
