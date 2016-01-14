# pMixPlugin - Plug-in version

A VST2, VST3 and AudioUnit version of pMix. Possibly LV2 in the future.

##Dependencies:

See main README.md for pMix standalone.

##Compiling:

Once you have built and installed the above in the correct locations you should be able to compile the standalone app version of pMix by opening the Xcode project: /Builds/MacOSX/pMixPlugin.xcodeproj

If you need to change any paths etc, you should do so using the JUCE IntroJucer rather than modifing the xcode project.

See http://www.juce.com/learn/introjucer

##Notes:

Currently JUCE must be patched in order to avoid a deadlock when AudioProcessorGraph is used in a plug-in setting. The included JuceLibraryCode should allready be patched, but if you update it you may need to repatch. See http://www.juce.com/forum/topic/ableton-live-deadlock-plugin-audioprocessorgraph


**License:**

pMix - Copyright Oliver Larkin 2008 - 2016

pMix v2    
Contemporary Music Research Centre  
Music Department  
University of York  
License: GPL
  
www.cmrcyork.org | www.olilarkin.co.uk