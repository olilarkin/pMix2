# Automatically generated makefile, created by the Projucer
# Don't edit this file! Your changes will be overwritten when you re-save the Projucer project!

# (this disables dependency generation if multiple architectures are set)
DEPFLAGS := $(if $(word 2, $(TARGET_ARCH)), , -MMD)

ifndef CONFIG
  CONFIG=Debug
endif

ifeq ($(CONFIG),Debug)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/Debug
  OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -m64
  endif

  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D "JUCE_PLUGINHOST_LADSPA=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -D "JUCE_APP_VERSION=0.0.1" -D "JUCE_APP_VERSION_HEX=0x1" -I /usr/include -I /usr/include/freetype2 -I ~/SDKs/vstsdk2.4 -I ../../JuceLibraryCode -I ../../../../juce/modules -I ../../../../Projects
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -g -ggdb -O0 -std=gnu++0x
  CXXFLAGS += $(CFLAGS) -std=c++11
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -L/usr/X11R6/lib/ -L/usr/lib/llvm-3.7/lib/ -lGL -lX11 -lXext -lXinerama -lasound -ldl -lfreetype -lpthread -lrt 

  TARGET := pMix
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
  CLEANCMD = rm -rf $(OUTDIR)/$(TARGET) $(OBJDIR)
endif

ifeq ($(CONFIG),Release)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/Release
  OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -m64
  endif

  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "NDEBUG=1" -D "JUCE_PLUGINHOST_LADSPA=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -D "JUCE_APP_VERSION=0.0.1" -D "JUCE_APP_VERSION_HEX=0x1" -I /usr/include -I /usr/include/freetype2 -I ~/SDKs/vstsdk2.4 -I ../../JuceLibraryCode -I ../../../../juce/modules -I ../../../../Projects
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -Os -std=gnu++0x
  CXXFLAGS += $(CFLAGS) -std=c++11
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -fvisibility=hidden -L/usr/X11R6/lib/ -L/usr/lib/llvm-3.7/lib/ -lGL -lX11 -lXext -lXinerama -lasound -ldl -lfreetype -lpthread -lrt 

  TARGET := pMix
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
  CLEANCMD = rm -rf $(OUTDIR)/$(TARGET) $(OBJDIR)
endif

OBJECTS := \
  $(OBJDIR)/pMixInterpolationSpaceLayout_cb9ddbe1.o \
  $(OBJDIR)/pMixInterpolationSpaceCrosshairs_d34850c8.o \
  $(OBJDIR)/pMixInterpolationSpaceActions_92513270.o \
  $(OBJDIR)/pMixInterpolationSpace_b9dffb17.o \
  $(OBJDIR)/pMixGraphEditor_f95221f0.o \
  $(OBJDIR)/pMixGraphEditorParts_4bc4cb1a.o \
  $(OBJDIR)/pMixGraphEditorActions_e9275577.o \
  $(OBJDIR)/pMixPrefsGeneral_1c47026d.o \
  $(OBJDIR)/pMixPrefsPlugins_1e6e5525.o \
  $(OBJDIR)/pMixPrefsAudio_a26ee25b.o \
  $(OBJDIR)/pMixPrefsColours_f5ed3e2c.o \
  $(OBJDIR)/pMixPrefsControllers_939ed25c.o \
  $(OBJDIR)/pMixPrefsComponent_29159002.o \
  $(OBJDIR)/pMixCodeEditor_9c72070f.o \
  $(OBJDIR)/pMixSVGDisplay_81b3cd13.o \
  $(OBJDIR)/pMixFileBrowser_37eaaf21.o \
  $(OBJDIR)/pMixConsole_f8903cec.o \
  $(OBJDIR)/pMixGenericEditor_f1457b39.o \
  $(OBJDIR)/pMixRHPTabContainer_91db8e9b.o \
  $(OBJDIR)/pMixMainAppWindow_68a956cd.o \
  $(OBJDIR)/pMixApp_f4968cb6.o \
  $(OBJDIR)/pMixLookAndFeel_bf43c93.o \
  $(OBJDIR)/pMixDocument_9b99d1f0.o \
  $(OBJDIR)/pMixPluginWindow_8e0c1d8.o \
  $(OBJDIR)/pMixFloatWindow_b286ca81.o \
  $(OBJDIR)/pMixMainComponent_71d6859.o \
  $(OBJDIR)/pMixInternalPluginFormat_69d4d33c.o \
  $(OBJDIR)/pMixAudioEngine_de4ed8cd.o \
  $(OBJDIR)/juce_audio_basics_6b797ca1.o \
  $(OBJDIR)/juce_audio_devices_a742c38b.o \
  $(OBJDIR)/juce_audio_formats_5a29c68a.o \
  $(OBJDIR)/juce_audio_processors_dea3173d.o \
  $(OBJDIR)/juce_audio_utils_c7eb679f.o \
  $(OBJDIR)/juce_core_75b14332.o \
  $(OBJDIR)/juce_cryptography_6de2ebff.o \
  $(OBJDIR)/juce_data_structures_72d3da2c.o \
  $(OBJDIR)/juce_events_d2be882c.o \
  $(OBJDIR)/juce_faustllvm_3d2bdc9.o \
  $(OBJDIR)/juce_graphics_9c18891e.o \
  $(OBJDIR)/juce_gui_basics_8a6da59c.o \
  $(OBJDIR)/juce_gui_extra_4a026f23.o \
  $(OBJDIR)/juce_opengl_cd70b4c2.o \

.PHONY: clean

$(OUTDIR)/$(TARGET): $(OBJECTS) $(RESOURCES)
	@echo Linking pMix
	-@mkdir -p $(BINDIR)
	-@mkdir -p $(LIBDIR)
	-@mkdir -p $(OUTDIR)
	@$(BLDCMD)

clean:
	@echo Cleaning pMix
	@$(CLEANCMD)

strip:
	@echo Stripping pMix
	-@strip --strip-unneeded $(OUTDIR)/$(TARGET)

$(OBJDIR)/pMixInterpolationSpaceLayout_cb9ddbe1.o: ../../Source/pMixInterpolationSpaceLayout.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling pMixInterpolationSpaceLayout.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/pMixInterpolationSpaceCrosshairs_d34850c8.o: ../../Source/pMixInterpolationSpaceCrosshairs.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling pMixInterpolationSpaceCrosshairs.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/pMixInterpolationSpaceActions_92513270.o: ../../Source/pMixInterpolationSpaceActions.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling pMixInterpolationSpaceActions.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/pMixInterpolationSpace_b9dffb17.o: ../../Source/pMixInterpolationSpace.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling pMixInterpolationSpace.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/pMixGraphEditor_f95221f0.o: ../../Source/pMixGraphEditor.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling pMixGraphEditor.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/pMixGraphEditorParts_4bc4cb1a.o: ../../Source/pMixGraphEditorParts.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling pMixGraphEditorParts.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/pMixGraphEditorActions_e9275577.o: ../../Source/pMixGraphEditorActions.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling pMixGraphEditorActions.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/pMixPrefsGeneral_1c47026d.o: ../../Source/pMixPrefsGeneral.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling pMixPrefsGeneral.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/pMixPrefsPlugins_1e6e5525.o: ../../Source/pMixPrefsPlugins.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling pMixPrefsPlugins.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/pMixPrefsAudio_a26ee25b.o: ../../Source/pMixPrefsAudio.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling pMixPrefsAudio.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/pMixPrefsColours_f5ed3e2c.o: ../../Source/pMixPrefsColours.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling pMixPrefsColours.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/pMixPrefsControllers_939ed25c.o: ../../Source/pMixPrefsControllers.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling pMixPrefsControllers.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/pMixPrefsComponent_29159002.o: ../../Source/pMixPrefsComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling pMixPrefsComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/pMixCodeEditor_9c72070f.o: ../../Source/pMixCodeEditor.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling pMixCodeEditor.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/pMixSVGDisplay_81b3cd13.o: ../../Source/pMixSVGDisplay.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling pMixSVGDisplay.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/pMixFileBrowser_37eaaf21.o: ../../Source/pMixFileBrowser.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling pMixFileBrowser.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/pMixConsole_f8903cec.o: ../../Source/pMixConsole.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling pMixConsole.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/pMixGenericEditor_f1457b39.o: ../../Source/pMixGenericEditor.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling pMixGenericEditor.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/pMixRHPTabContainer_91db8e9b.o: ../../Source/pMixRHPTabContainer.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling pMixRHPTabContainer.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/pMixMainAppWindow_68a956cd.o: ../../Source/pMixMainAppWindow.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling pMixMainAppWindow.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/pMixApp_f4968cb6.o: ../../Source/pMixApp.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling pMixApp.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/pMixLookAndFeel_bf43c93.o: ../../Source/pMixLookAndFeel.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling pMixLookAndFeel.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/pMixDocument_9b99d1f0.o: ../../Source/pMixDocument.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling pMixDocument.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/pMixPluginWindow_8e0c1d8.o: ../../Source/pMixPluginWindow.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling pMixPluginWindow.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/pMixFloatWindow_b286ca81.o: ../../Source/pMixFloatWindow.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling pMixFloatWindow.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/pMixMainComponent_71d6859.o: ../../Source/pMixMainComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling pMixMainComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/pMixInternalPluginFormat_69d4d33c.o: ../../Source/pMixInternalPluginFormat.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling pMixInternalPluginFormat.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/pMixAudioEngine_de4ed8cd.o: ../../Source/pMixAudioEngine.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling pMixAudioEngine.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_basics_6b797ca1.o: ../../JuceLibraryCode/juce_audio_basics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_basics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_devices_a742c38b.o: ../../JuceLibraryCode/juce_audio_devices.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_devices.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_formats_5a29c68a.o: ../../JuceLibraryCode/juce_audio_formats.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_formats.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_processors_dea3173d.o: ../../JuceLibraryCode/juce_audio_processors.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_processors.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_utils_c7eb679f.o: ../../JuceLibraryCode/juce_audio_utils.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_utils.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_core_75b14332.o: ../../JuceLibraryCode/juce_core.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_core.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_cryptography_6de2ebff.o: ../../JuceLibraryCode/juce_cryptography.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_cryptography.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_data_structures_72d3da2c.o: ../../JuceLibraryCode/juce_data_structures.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_data_structures.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_events_d2be882c.o: ../../JuceLibraryCode/juce_events.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_events.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_faustllvm_3d2bdc9.o: ../../JuceLibraryCode/juce_faustllvm.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_faustllvm.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_graphics_9c18891e.o: ../../JuceLibraryCode/juce_graphics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_graphics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_basics_8a6da59c.o: ../../JuceLibraryCode/juce_gui_basics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_gui_basics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_extra_4a026f23.o: ../../JuceLibraryCode/juce_gui_extra.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_gui_extra.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_opengl_cd70b4c2.o: ../../JuceLibraryCode/juce_opengl.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_opengl.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)
