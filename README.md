# pMix - Preset Mixer (v2)

pMix (short for preset mixer) is a sound design, composition and performance tool that facilitates the control of multiple parameters using an intuitive graphical interface. It includes a graphical patcher for creating a network of audio synthesis or processing nodes. A node could be a VST2, VST3, AU or LADSPA plug-in or it could a JIT compiled FAUST script. FAUST is a first class citizen in pMix and an integrated code editor and SVG display makes it a nice environment for FAUST development.

Presets created for each node are represented by colour-coded balls that are positioned on a 2D plane. The size of each ball and its proximity to the node's cursor affects the weight of the associated preset in the interpolation. 

Morphing between presets can result in the discovery of interesting hybrid sounds. By constraining sound manipulations within a predesigned "interpolation space" complex transitions can be achieved that would otherwise be hard to manage. 

pMix version 2 will eventually work on Mac, Linux and Windows and can operate as a standalone application or as a plug-in.

Currently it is being developed using JUCE on Mac with Xcode 7.2 and the 10.11 SDK, with OSX deployment target set to 10.9. It won't yet compile/work well on other platforms.

##Dependencies:

Due to the complexity of this project please closely follow the instructions:

**VST3 SDK**

The .jucer projects expect the Steinberg VST 3 SDK to be installed in this location: 

~/SDKs/VST3 SDK 

You can download the VST3 SDK here: https://www.steinberg.net/en/company/developers.html

vstsdk365_12_11_2015_build_67.zip was used

see note below regarding VST2.4 SDK.

**JUCE_FAUSTLLVM Module Dependencies**

There are more dependencies which are described in the juce_faustllvm [README.md](https://github.com/CMRCYork/juce_faustllvm)


##Compiling:

Once you have built and installed the above in the correct locations you should be able to compile the standalone app version of pMix by opening the Xcode project: /Builds/MacOSX/pMix.xcodeproj

If you need to change any paths etc, you should do so using the .jucer project rather than modifing the xcode project.

##Notes:

* pMix has been developed on Mac OSX and there hasn't yet been time to get it working on the other intended platforms! Hopefully there should not be any platform dependencies that prevent it working on Linux and Windows.
* JUCE's VST2 plug-in support is most mature, so VST2 plug-ins are preferable to VST3, however Steinberg no longer distribute the VST2.4 SDK so VST3 support must be enabled in order to find the VST2.4 headers in the VST3 SDK.
* If you don't care about plugin hosting support, you can disable it on the juce_audio_processors module tab by opening the pMix.jucer.
* AU plug-in hosting is currently disabled, you can turn it on via the .jucer project.

**License:**

pMix - Copyright Oliver Larkin 2008 - 2016

pMix v2    
Contemporary Music Research Centre  
Music Department  
University of York  
License: GPL

www.cmrcyork.org | www.olilarkin.co.uk