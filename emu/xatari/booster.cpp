
#include "stdafx.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
BoosterGrip::BoosterGrip(Jack jack, const Event& event)
    : Controller(jack, event)
{
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
BoosterGrip::~BoosterGrip()
{
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool BoosterGrip::read(DigitalPin pin)
{
  switch(pin)
  {
    case One:
      return (myJack == Left) ? (myEvent.get(Event::JoystickZeroUp) == 0) : 
          (myEvent.get(Event::JoystickOneUp) == 0);

    case Two:
      return (myJack == Left) ? (myEvent.get(Event::JoystickZeroDown) == 0) : 
          (myEvent.get(Event::JoystickOneDown) == 0);

    case Three:
      return (myJack == Left) ? (myEvent.get(Event::JoystickZeroLeft) == 0) : 
          (myEvent.get(Event::JoystickOneLeft) == 0);

    case Four:
      return (myJack == Left) ? (myEvent.get(Event::JoystickZeroRight) == 0) :
          (myEvent.get(Event::JoystickOneRight) == 0);

    case Six:
      return (myJack == Left) ? (myEvent.get(Event::JoystickZeroFire) == 0) : 
          (myEvent.get(Event::JoystickOneFire) == 0);

    default:
      return true;
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Int32 BoosterGrip::read(AnalogPin pin)
{
  // The CBS Booster-grip has two more buttons on it.  These buttons are
  // connected to the inputs usually used by paddles.

  switch(pin)
  {
    case Five:
      if(myJack == Left)
      {
        return (myEvent.get(Event::BoosterGripZeroBooster) != 0) ? 
            minimumResistance : maximumResistance;
      }
      else
      {
        return (myEvent.get(Event::BoosterGripOneBooster) != 0) ? 
            minimumResistance : maximumResistance;
      }

    case Nine:
      if(myJack == Left)
      {
        return (myEvent.get(Event::BoosterGripZeroTrigger) != 0) ? 
            minimumResistance : maximumResistance;
      }
      else
      {
        return (myEvent.get(Event::BoosterGripOneTrigger) != 0) ? 
            minimumResistance : maximumResistance;
      }

    default:
      return maximumResistance;
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void BoosterGrip::write(DigitalPin, bool)
{
  // Writing doesn't do anything to the booster grip...
}

