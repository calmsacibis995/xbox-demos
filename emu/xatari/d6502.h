#ifndef D6502_HXX
#define D6502_HXX

class D6502;
class M6502;
class System;

#include "bspf.h"

/**
  This is a base class for 6502 debuggers.  This class provides the 
  basic functionality needed for interactive debuggers.

  @author  Bradford W. Mott
  @version $Id: D6502.hxx,v 1.3 1998/08/29 15:29:56 bwmott Exp $ 
*/
class D6502
{
  public:
    /**
      Create a new 6502 debugger for the specified system

      @param system The system the debugger should operate on
    */
    D6502(System* system);

    /**
      Destructor
    */
    virtual ~D6502();

  public:
    /**
      Disassemble a single instruction at the specified address into 
      the given buffer and answer the number of bytes disassembled.  
      The buffer should be at least 20 characters long.

      @param address The address to disassemble code at
      @param buffer The buffer where the ASCII disassemble should be stored
      @return The number of bytes disassembled
    */
    uInt16 disassemble(uInt16 address, char* buffer);

  public:
    /**
      Get the value of the accumulator

      @return The accumulator's value
    */
    uInt8 a();

    /**
      Change value of the accumulator

      @param value The value to set the accumulator to
    */
    void a(uInt8 value);

    /**
      Get value of the program counter

      @return The program counter's value
    */
    uInt16 pc();

    /**
      Change value of the program counter

      @param value The value to set the program counter to
    */
    void pc(uInt16 value);

    /**
      Get the value of the processor status register

      @return The processor status register's value
    */
    uInt8 ps();

    /**
      Change value of the processor status register

      @param value The value to set the processor status register to
    */
    void ps(uInt8 value);

    /**
      Get the value of the stack pointer

      @return The stack pointer's value
    */
    uInt8 sp();

    /**
      Change value of the stack pointer

      @param value The value to set the stack pointer to
    */
    void sp(uInt8 value);

    /**
      Get the value of the X index register

      @return The X register's value
    */
    uInt8 x();

    /**
      Change value of the X index register

      @param value The value to set the X register to
    */
    void x(uInt8 value);
   
    /**
      Get the value of the Y index register

      @return The Y register's value
    */
    uInt8 y();

    /**
      Change value of the Y index register

      @param value The value to set the Y register to
    */
    void y(uInt8 value);

  protected:
    // Pointer to the system I'm debugging
    System* mySystem;
};
#endif

