/*
  ==============================================================================

    pMixLogger.h
    Author:  Oliver Larkin

  ==============================================================================
*/

#pragma once


#include "JuceHeader.h"
#include <list>

class PMixLogger  : public Logger
                  , public ChangeBroadcaster
{
public:
  PMixLogger()
  {
    
  }
  
  ~PMixLogger()
  {
    
  }
  
  void logMessage (const String& message)
  {
   // printf("size %i\n", messageList.size());
    String str = message;
    messageList.push_back(str);
    sendChangeMessage();
    //sendSynchronousChangeMessage();
  }
  
  bool getLastMessage(String& theMessage)
  {
    while (messageList.size())
    {
      theMessage = messageList.front();
      messageList.pop_front();
      return true;
    }
    
    return false;
  }
  
private:
  std::list<String> messageList;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PMixLogger)
};


