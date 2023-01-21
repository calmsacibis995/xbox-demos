#ifndef DEVICE_HXX
#define DEVICE_HXX

class System;

#include "bspf.h"

/**
  Abstract base class for devices which can be attached to a 6502
  based system.

  @author  Bradford W. Mott
  @version $Id: Device.hxx,v 1.2 1998/07/15 20:51:34 bwmott Exp $
*/
class Device
{
  public:
    /**
      Create a new device
    */
    Device();

    /**
      Destructor
    */
    virtual ~Device();

  public:
    /**
      Get a null terminated string which is the device's name (i.e. "M6532")

      @return The name of the device
    */
    virtual const char* name() const = 0;

    /**
      Reset device to its power-on state
    */
    virtual void reset() = 0;

    /**
      Notification method invoked by the system right before the
      system resets its cycle counter to zero.  It may be necessary 
      to override this method for devices that remember cycle counts.
    */
    virtual void systemCyclesReset();

    /**
      Install device in the specified system.  Invoked by the system
      when the device is attached to it.

      @param system The system the device should install itself in
    */
    virtual void install(System& system) = 0;

  public:
    /**
      Get the byte at the specified address

      @return The byte at the specified address
    */
    virtual uInt8 peek(uInt16 address) = 0;

    /**
      Change the byte at the specified address to the given value

      @param address The address where the value should be stored
      @param value The value to be stored at the address
    */
    virtual void poke(uInt16 address, uInt8 value) = 0;

  protected:
    /// Pointer to the system the device is installed in or the null pointer
    System* mySystem;
};
#endif
 
