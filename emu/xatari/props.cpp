#include "stdafx.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Properties::Properties(const Properties* defaults)
{
  myDefaults = defaults;
  myCapacity = 16;
  myProperties = new Property[myCapacity];
  mySize = 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Properties::Properties(const Properties& properties)
{
  copy(properties);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Properties::~Properties()
{
  // Free the properties array
  delete[] myProperties;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
string Properties::get(const string& key) const
{
  // Try to find the named property and answer its value
  for(uInt32 i = 0; i < mySize; ++i)
  {
    if(key == myProperties[i].key)
    {
      return myProperties[i].value;
    }
  }

  // Oops, property wasn't found so ask defaults if we have one
  if(myDefaults != 0)
  {
    // Ask the default properties object to find the key
    return myDefaults->get(key);
  } 
  else
  {
    // No default properties object so just return the empty string
    return "";
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Properties::set(const string& key, const string& value)
{
  // See if the property already exists
  for(uInt32 i = 0; i < mySize; ++i)
  {
    if(key == myProperties[i].key)
    {
      myProperties[i].value = value;
      return;
    }
  }

  // See if the array needs to be resized
  if(mySize == myCapacity)
  {
    // Yes, so we'll make the array twice as large
    Property* newProperties = new Property[myCapacity * 2];

    for(uInt32 i = 0; i < mySize; ++i)
    {
      newProperties[i] = myProperties[i];
    }

    delete[] myProperties;

    myProperties = newProperties;
    myCapacity *= 2;
  } 

  // Add new property to the array
  myProperties[mySize].key = key;
  myProperties[mySize].value = value;

  ++mySize;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Properties::load(istream2& in)
{
  // Empty my property array
  mySize = 0;

  // Loop reading properties
  for(;;)
  {
    // Get the key associated with this property
    string key = readQuotedString(in);

    // Make sure the stream is still okay
    if(!in.eof())
    {
      return;
    }

    // A null key signifies the end of the property list
    if(key == "")
    {
      break;
    }

    // Get the value associated with this property
    string value = readQuotedString(in);

    // Make sure the stream is still okay
    if(!in.eof())
    {
      return;
    }

    // Set the property 
    set(key, value);
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Properties::save(ostream2& out)
{
  // Write out each of the key and value pairs
  for(uInt32 i = 0; i < mySize; ++i)
  {
    writeQuotedString(out, myProperties[i].key);
    out.put(' ');
    writeQuotedString(out, myProperties[i].value);
    out.put('\n');
  }

  // Put a trailing null string so we know when to stop reading
  writeQuotedString(out, string(""));
  out.put('\n');
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
string Properties::readQuotedString(istream2& in)
{
  char c;

  // Read characters until we see a quote
  while(in.get(c))
  {
    if(c == '"')
    {
      break;
    }
  }

  // Read characters until we see the close quote
  string s;
  while(in.get(c))
  {
    if((c == '\\') && (in.peek() == '"'))
    {
      in.get(c);
    }
    else if((c == '\\') && (in.peek() == '\\'))
    {
      in.get(c);
    }
    else if(c == '"')
    {
      break;
    }
    else if(c == '\r')
    {
      continue;
    }

    s += c;
  }

  return s;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Properties::writeQuotedString(ostream2& out, string& s)
{
  out.put('"');
  for(uInt32 i = 0; i < s.length(); ++i)
  {
    if(s.letter(i) == '\\')
    {
      out.put('\\');
      out.put('\\');
    }
    else if(s.letter(i) == '\"')
    {
      out.put('\\');
      out.put('"');
    }
    else
    {
      out.put(s.letter(i));
    }
  }
  out.put('"');
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Properties& Properties::operator = (const Properties& properties)
{
  // Do the assignment only if this isn't a self assignment
  if(this != &properties)
  {
    // Free the properties array
    delete[] myProperties;

    // Now, make myself a copy of the given object
    copy(properties);
  }

  return *this;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Properties::copy(const Properties& properties)
{
  // Remember the defaults to use
  myDefaults = properties.myDefaults;

  // Create an array of the same size as properties
  myCapacity = properties.myCapacity;
  myProperties = new Property[myCapacity];

  // Now, copy each property from properties
  mySize = properties.mySize;
  for(uInt32 i = 0; i < mySize; ++i)
  {
    myProperties[i] = properties.myProperties[i];
  }
}

