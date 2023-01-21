
#include "stdafx.h"

/**
  Compare the two strings s1 and s2 ignoring the case of the 
  characters.  Answers true iff they are equal.

  @param s1 The first string to compare
  @param s2 The second string to compare
  @return true iff the two strings are equal
*/
static bool compare(string s1, string s2)
{
  if(s1.length() != s2.length())
  {
    return false;
  }

  for(uInt32 i = 0; i < s1.length(); ++i)
  {
    if(tolower(s1.letter(i)) != tolower(s2.letter(i)))
    {
      return false;
    }
  }

  return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Console::Console( const uInt8* image,
				  uInt32 size,
				  const char* filename,
				  const Event& event,
				  PropertiesSet& propertiesSet,
				  Sound& sound )
: myEvent( event )
{
  myControllers[0] = 0;
  myControllers[1] = 0;
  myMediaSource = 0;
  mySwitches = 0;
  mySystem = 0;
  myProperties = defaultProperties();

  // Get the MD5 message-digest for the ROM image
  string md5 = MD5(image, size);

  // Search through the properties set to see if some exist for this game
  for(uInt32 i = 0; i < propertiesSet.size(); ++i)
  {
    const Properties& properties = propertiesSet.get(i);

    if(properties.get("Cartridge.MD5") == md5)
    {
      // We have a match so let's use those properties
      myProperties = properties;
      break;
    }
  } 

  // If there was no MD5 match then let's search based on filename
  if(md5 != myProperties.get("Cartridge.MD5"))
  {
    for(uInt32 i = 0; i < propertiesSet.size(); ++i)
    {
      const Properties& properties = propertiesSet.get(i);

      if(compare(properties.get("Cartridge.Filename"), filename))
      {
        // We have a match so let's use those properties
        myProperties = properties;
        break;
      }
    } 
  }

  // TODO: At some point I belive we'll need to set the properties'
  // MD5 value so the user will be able to edit it.  
  // myProperties.save(cout);

  // Setup the controllers based on properties
  string left = myProperties.get("Controller.Left");
  string right = myProperties.get("Controller.Right");

  // Construct left controller
  if(left == "Booster-Grip")
  {
    myControllers[0] = new BoosterGrip(Controller::Left, myEvent);
  }
  else if(left == "Driving")
  {
    myControllers[0] = new Driving(Controller::Left, myEvent);
  }
  else if((left == "Keyboard") || (left == "Keypad"))
  {
    myControllers[0] = new Keyboard(Controller::Left, myEvent);
  }
  else if(left == "Paddles")
  {
    myControllers[0] = new Paddles(Controller::Left, myEvent);
  }
  else
  {
    myControllers[0] = new Joystick(Controller::Left, myEvent);
  }
  
  // Construct right controller
  if(right == "Booster-Grip")
  {
    myControllers[1] = new BoosterGrip(Controller::Right, myEvent);
  }
  else if(right == "Driving")
  {
    myControllers[1] = new Driving(Controller::Right, myEvent);
  }
  else if((right == "Keyboard") || (right == "Keypad"))
  {
    myControllers[1] = new Keyboard(Controller::Right, myEvent);
  }
  else if(right == "Paddles")
  {
    myControllers[1] = new Paddles(Controller::Right, myEvent);
  }
  else
  {
    myControllers[1] = new Joystick(Controller::Right, myEvent);
  }

  // Create switches for the console
  mySwitches = new Switches(myEvent, myProperties);

  // Now, we can construct the system and components
  mySystem = new System(13, 6);

  M6502* m6502;
  if((myProperties.get("Emulation.CPU") == "High") ||
      ((myProperties.get("Emulation.CPU") == "Auto-detect") && !(size % 8448)))
  {
    m6502 = new M6502High(1);
  }
  else
  {
    m6502 = new M6502Low(1);
  }

  M6532* m6532 = new M6532(*this);
  TIA* tia = new TIA(*this, sound);
  Cartridge* cartridge = Cartridge::create(image, size, myProperties);

  mySystem->attach(m6502);
  mySystem->attach(m6532);
  mySystem->attach(tia);
  mySystem->attach(cartridge);

  // Remember what my media source is
  myMediaSource = tia;

  // Reset, the system to its power-on state
  mySystem->reset();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Console::Console(const Console& console)
    : myEvent(console.myEvent)
{
  // TODO: Write this method
  assert(false);
}
 
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Console::~Console()
{
  delete mySystem;
  delete mySwitches;
  delete myControllers[0];
  delete myControllers[1];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
const Properties& Console::properties() const
{
  return myProperties;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Console& Console::operator = (const Console&)
{
  // TODO: Write this method
  assert(false);

  return *this;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
const Properties& Console::defaultProperties()
{
  // Make sure the <key,value> pairs are in the default properties object
  ourDefaultProperties.set("Cartridge.Filename", "");
  ourDefaultProperties.set("Cartridge.MD5", "");
  ourDefaultProperties.set("Cartridge.Manufacturer", "");
  ourDefaultProperties.set("Cartridge.ModelNo", "");
  ourDefaultProperties.set("Cartridge.Name", "Untitled");
  ourDefaultProperties.set("Cartridge.Note", "");
  ourDefaultProperties.set("Cartridge.Rarity", "");
  ourDefaultProperties.set("Cartridge.Type", "Auto-detect");

  ourDefaultProperties.set("Console.LeftDifficulty", "B");
  ourDefaultProperties.set("Console.RightDifficulty", "B");
  ourDefaultProperties.set("Console.TelevisionType", "Color");

  ourDefaultProperties.set("Controller.Left", "Joystick");
  ourDefaultProperties.set("Controller.Right", "Joystick");

  ourDefaultProperties.set("Display.Format", "NTSC");
  ourDefaultProperties.set("Display.XStart", "0");
  ourDefaultProperties.set("Display.Width", "160");
  ourDefaultProperties.set("Display.YStart", "38");
  ourDefaultProperties.set("Display.Height", "210");

  ourDefaultProperties.set("Emulation.CPU", "Auto-detect");
  ourDefaultProperties.set("Emulation.HmoveBlanks", "Yes");

  return ourDefaultProperties;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Properties Console::ourDefaultProperties;

