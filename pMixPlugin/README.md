# pMixPlugin - Plug-in version

A VST2, VST3 and AudioUnit version of pMix. Possibly LV2 in the future.

##Dependencies:

See main README.md for pMix standalone.

##Compiling:

Once you have built and installed the above in the correct locations you should be able to compile the standalone app version of pMix by opening the Xcode project: /Builds/MacOSX/pMixPlugin.xcodeproj

If you need to change any paths etc, you should do so using the JUCE .jucer project.

##Notes:

Currently JUCE must be modified in order to avoid a deadlock when AudioProcessorGraph is used in a plug-in setting. The JUCE.patch file has a temporary hack, until a better solution is found.

For further info, see http://www.juce.com/forum/topic/ableton-live-deadlock-plugin-audioprocessorgraph


**License:**

pMix  
Copyright Oliver Larkin 2008 - 2016  
License: GPL  
www.olilarkin.co.uk