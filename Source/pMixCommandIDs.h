/*
  ==============================================================================

    CommandIDs.h
    Author:  Oliver Larkin

  ==============================================================================
*/

#ifndef COMMANDIDS_H_INCLUDED
#define COMMANDIDS_H_INCLUDED


namespace CommandIDs
{
  static const int open                   = 0x30000;
  static const int save                   = 0x30001;
  static const int saveAs                 = 0x30002;
  static const int showPluginListEditor   = 0x30100;
  static const int showAudioSettings      = 0x30200;
  static const int aboutBox               = 0x30300;
  static const int copy                   = 0x30010;
  static const int paste                  = 0x30020;
  static const int undo                   = 0x30030;
  static const int redo                   = 0x30040;
  static const int zoomIn                 = 0xf20013;
  static const int zoomOut                = 0xf20014;
  static const int zoomNormal             = 0xf20015;
  static const int showISpace             = 0xf20016;
  static const int showGraphEditor        = 0xf20017;
  static const int showCodeEditor         = 0xf20018;
  static const int showParameters         = 0xf20019;
  
  static const int newAudioInput          = 0xe20000;
  static const int newAudioOutput         = 0xe20001;
  static const int newMIDIInput           = 0xe20002;
  static const int newMIDIOutput          = 0xe20003;
  static const int newFaustEffect         = 0xe20004;
}


#endif  // COMMANDIDS_H_INCLUDED
