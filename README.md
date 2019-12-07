# pMix - Preset Mixer (v2)

pMix (short for preset mixer) is a sound design, composition and performance tool that facilitates the control of multiple parameters using an intuitive graphical interface. It includes a graphical patcher for creating a network of audio synthesis or processing nodes. A node could be a VST2, VST3, AU or LADSPA plug-in or it could a JIT compiled FAUST script. FAUST is a first class citizen in pMix and an integrated code editor and SVG display makes it a nice environment for FAUST development.

Presets created for each node are represented by colour-coded balls that are positioned on a 2D plane. The size of each ball and its proximity to the node's cursor affects the weight of the associated preset in the interpolation.

Morphing between presets can result in the discovery of interesting hybrid sounds. By constraining sound manipulations within a predesigned "interpolation space" complex transitions can be achieved that would otherwise be hard to manage.

pMix version 2 will eventually work on Mac, Linux and Windows and can operate as a standalone application or as a plug-in.

Currently it is being developed using JUCE on Mac with Xcode 8 and the 10.12 SDK, with OSX deployment target set to 10.12. It won't yet compile/work well on other platforms.

[![IMAGE ALT TEXT](/misc/pMixVimeo.png?raw=true)](https://vimeo.com/122268573 "pMix2 quick demo")


## Dependencies

Due to the complexity of this project please closely follow the instructions:

**Using brew to install dependencies**

```brew install llvm zlib ncurses openssl```

**JUCE**

pMix v2 is built using JUCE and the .jucer project expects to find the JUCE source and the juce_faustllvm juce modules source in the global module directory, which you can set in the Projucer settings.

**VST3 SDK**

The VST 3 SDK is now included as three git submodules. You will need to use ```git submodule update``` to check out the submodules.

**JUCE_FAUSTLLVM Module Dependencies**

There are more dependencies which are described in the juce_faustllvm [README.md](https://github.com/olilarkin/juce_faustllvm)


## Compiling

Once you have built and installed the above in the correct locations you should be able to compile the standalone app version of pMix by opening the Xcode project: /Builds/MacOSX/pMix.xcodeproj

If you need to change any paths etc, you should do so using the .jucer project rather than modifing the xcode project.

## Notes

* pMix has been developed on Mac OSX and there hasn't yet been time to get it working on the other intended platforms! Hopefully there should not be any platform dependencies that prevent it working on Linux and Windows.
* JUCE's VST2 plug-in support is most mature, so VST2 plug-ins are preferable to VST3, however Steinberg no longer distribute the VST2.4 SDK so VST3 support must be enabled in order to find the VST2.4 headers in the VST3 SDK.
* If you don't care about plugin hosting support, you can disable it on the juce_audio_processors module tab by opening the pMix.jucer.
* AU plug-in hosting is currently disabled, you can turn it on via the .jucer project.

**License:**

pMix - Copyright Oliver Larkin 2008 - 2019 
GNU Public License  
www.olilarkin.co.uk
