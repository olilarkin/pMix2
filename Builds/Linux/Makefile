# Automatically generated makefile, created by the Projucer
# Don't edit this file! Your changes will be overwritten when you re-save the Projucer project!

# (this disables dependency generation if multiple architectures are set)
DEPFLAGS := $(if $(word 2, $(TARGET_ARCH)), , -MMD)

ifndef STRIP
  STRIP=strip
endif

ifndef AR
  AR=ar
endif

ifndef CONFIG
  CONFIG=Debug
endif

ifeq ($(CONFIG),Debug)
  JUCE_BINDIR := build
  JUCE_LIBDIR := build
  JUCE_OBJDIR := build/intermediate/Debug
  JUCE_OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -m64
  endif

  JUCE_CPPFLAGS := $(DEPFLAGS) -DLINUX=1 -DDEBUG=1 -D_DEBUG=1 -DJUCE_PLUGINHOST_LADSPA=1 -DJUCER_LINUX_MAKE_7346DA2A=1 -DJUCE_APP_VERSION=0.0.2 -DJUCE_APP_VERSION_HEX=0x2 $(shell pkg-config --cflags alsa freetype2 libcurl x11 xext xinerama) -pthread -I../../JuceLibraryCode -I../../../../juce/modules -I../../../../Projects
  JUCE_CFLAGS += $(CFLAGS) $(JUCE_CPPFLAGS) $(TARGET_ARCH) -g -ggdb -O0 -std=gnu++0x
  JUCE_CXXFLAGS += $(CXXFLAGS) $(JUCE_CFLAGS) -std=c++11
  JUCE_LDFLAGS += $(LDFLAGS) $(TARGET_ARCH) -L$(JUCE_BINDIR) -L$(JUCE_LIBDIR) -L/usr/X11R6/lib/ -L/usr/lib/llvm-3.7/lib/ $(shell pkg-config --libs alsa freetype2 libcurl x11 xext xinerama) -lGL -ldl -lpthread -lrt 

  TARGET := pMix
  BLDCMD = $(CXX) -o $(JUCE_OUTDIR)/$(TARGET) $(OBJECTS) $(JUCE_LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
  CLEANCMD = rm -rf $(JUCE_OUTDIR)/$(TARGET) $(JUCE_OBJDIR)
endif

ifeq ($(CONFIG),Release)
  JUCE_BINDIR := build
  JUCE_LIBDIR := build
  JUCE_OBJDIR := build/intermediate/Release
  JUCE_OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -m64
  endif

  JUCE_CPPFLAGS := $(DEPFLAGS) -DLINUX=1 -DNDEBUG=1 -DJUCE_PLUGINHOST_LADSPA=1 -DJUCER_LINUX_MAKE_7346DA2A=1 -DJUCE_APP_VERSION=0.0.2 -DJUCE_APP_VERSION_HEX=0x2 $(shell pkg-config --cflags alsa freetype2 libcurl x11 xext xinerama) -pthread -I../../JuceLibraryCode -I../../../../juce/modules -I../../../../Projects
  JUCE_CFLAGS += $(CFLAGS) $(JUCE_CPPFLAGS) $(TARGET_ARCH) -Os -std=gnu++0x
  JUCE_CXXFLAGS += $(CXXFLAGS) $(JUCE_CFLAGS) -std=c++11
  JUCE_LDFLAGS += $(LDFLAGS) $(TARGET_ARCH) -L$(JUCE_BINDIR) -L$(JUCE_LIBDIR) -fvisibility=hidden -L/usr/X11R6/lib/ -L/usr/lib/llvm-3.7/lib/ $(shell pkg-config --libs alsa freetype2 libcurl x11 xext xinerama) -lGL -ldl -lpthread -lrt 

  TARGET := pMix
  BLDCMD = $(CXX) -o $(JUCE_OUTDIR)/$(TARGET) $(OBJECTS) $(JUCE_LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
  CLEANCMD = rm -rf $(JUCE_OUTDIR)/$(TARGET) $(JUCE_OBJDIR)
endif

OBJECTS := \
  $(JUCE_OBJDIR)/pMixInterpolationSpaceLayout_cb9ddbe1.o \
  $(JUCE_OBJDIR)/pMixInterpolationSpaceCrosshairs_d34850c8.o \
  $(JUCE_OBJDIR)/pMixInterpolationSpaceActions_92513270.o \
  $(JUCE_OBJDIR)/pMixInterpolationSpace_b9dffb17.o \
  $(JUCE_OBJDIR)/pMixGraphEditor_f95221f0.o \
  $(JUCE_OBJDIR)/pMixGraphEditorParts_4bc4cb1a.o \
  $(JUCE_OBJDIR)/pMixGraphEditorActions_e9275577.o \
  $(JUCE_OBJDIR)/pMixPrefsGeneral_1c47026d.o \
  $(JUCE_OBJDIR)/pMixPrefsPlugins_1e6e5525.o \
  $(JUCE_OBJDIR)/pMixPrefsAudio_a26ee25b.o \
  $(JUCE_OBJDIR)/pMixPrefsColours_f5ed3e2c.o \
  $(JUCE_OBJDIR)/pMixPrefsControllers_939ed25c.o \
  $(JUCE_OBJDIR)/pMixPrefsComponent_29159002.o \
  $(JUCE_OBJDIR)/pMixCodeEditor_9c72070f.o \
  $(JUCE_OBJDIR)/pMixSVGDisplay_81b3cd13.o \
  $(JUCE_OBJDIR)/pMixFileBrowser_37eaaf21.o \
  $(JUCE_OBJDIR)/pMixConsole_f8903cec.o \
  $(JUCE_OBJDIR)/pMixGenericEditor_f1457b39.o \
  $(JUCE_OBJDIR)/pMixRHPTabContainer_91db8e9b.o \
  $(JUCE_OBJDIR)/pMixMainAppWindow_68a956cd.o \
  $(JUCE_OBJDIR)/pMixApp_f4968cb6.o \
  $(JUCE_OBJDIR)/pMixLookAndFeel_bf43c93.o \
  $(JUCE_OBJDIR)/pMixDocument_9b99d1f0.o \
  $(JUCE_OBJDIR)/pMixPluginWindow_8e0c1d8.o \
  $(JUCE_OBJDIR)/pMixFloatWindow_b286ca81.o \
  $(JUCE_OBJDIR)/pMixMainComponent_71d6859.o \
  $(JUCE_OBJDIR)/pMixInternalPluginFormat_69d4d33c.o \
  $(JUCE_OBJDIR)/pMixAudioEngine_de4ed8cd.o \
  $(JUCE_OBJDIR)/juce_audio_basics_6b797ca1.o \
  $(JUCE_OBJDIR)/juce_audio_devices_a742c38b.o \
  $(JUCE_OBJDIR)/juce_audio_formats_5a29c68a.o \
  $(JUCE_OBJDIR)/juce_audio_processors_dea3173d.o \
  $(JUCE_OBJDIR)/juce_audio_utils_c7eb679f.o \
  $(JUCE_OBJDIR)/juce_core_75b14332.o \
  $(JUCE_OBJDIR)/juce_cryptography_6de2ebff.o \
  $(JUCE_OBJDIR)/juce_data_structures_72d3da2c.o \
  $(JUCE_OBJDIR)/juce_events_d2be882c.o \
  $(JUCE_OBJDIR)/juce_faustllvm_3d2bdc9.o \
  $(JUCE_OBJDIR)/juce_graphics_9c18891e.o \
  $(JUCE_OBJDIR)/juce_gui_basics_8a6da59c.o \
  $(JUCE_OBJDIR)/juce_gui_extra_4a026f23.o \
  $(JUCE_OBJDIR)/juce_opengl_cd70b4c2.o \

.PHONY: clean

$(JUCE_OUTDIR)/$(TARGET): check-pkg-config $(OBJECTS) $(RESOURCES)
	@echo Linking pMix
	-@mkdir -p $(JUCE_BINDIR)
	-@mkdir -p $(JUCE_LIBDIR)
	-@mkdir -p $(JUCE_OUTDIR)
	@$(BLDCMD)

check-pkg-config:
	@command -v pkg-config >/dev/null 2>&1 || { echo >&2 "pkg-config not installed. Please, install it."; exit 1; }
	@pkg-config --print-errors alsa freetype2 libcurl x11 xext xinerama

clean:
	@echo Cleaning pMix
	@$(CLEANCMD)

strip:
	@echo Stripping pMix
	-@$(STRIP) --strip-unneeded $(JUCE_OUTDIR)/$(TARGET)

$(JUCE_OBJDIR)/pMixInterpolationSpaceLayout_cb9ddbe1.o: ../../Source/pMixInterpolationSpaceLayout.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling pMixInterpolationSpaceLayout.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/pMixInterpolationSpaceCrosshairs_d34850c8.o: ../../Source/pMixInterpolationSpaceCrosshairs.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling pMixInterpolationSpaceCrosshairs.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/pMixInterpolationSpaceActions_92513270.o: ../../Source/pMixInterpolationSpaceActions.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling pMixInterpolationSpaceActions.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/pMixInterpolationSpace_b9dffb17.o: ../../Source/pMixInterpolationSpace.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling pMixInterpolationSpace.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/pMixGraphEditor_f95221f0.o: ../../Source/pMixGraphEditor.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling pMixGraphEditor.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/pMixGraphEditorParts_4bc4cb1a.o: ../../Source/pMixGraphEditorParts.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling pMixGraphEditorParts.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/pMixGraphEditorActions_e9275577.o: ../../Source/pMixGraphEditorActions.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling pMixGraphEditorActions.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/pMixPrefsGeneral_1c47026d.o: ../../Source/pMixPrefsGeneral.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling pMixPrefsGeneral.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/pMixPrefsPlugins_1e6e5525.o: ../../Source/pMixPrefsPlugins.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling pMixPrefsPlugins.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/pMixPrefsAudio_a26ee25b.o: ../../Source/pMixPrefsAudio.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling pMixPrefsAudio.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/pMixPrefsColours_f5ed3e2c.o: ../../Source/pMixPrefsColours.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling pMixPrefsColours.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/pMixPrefsControllers_939ed25c.o: ../../Source/pMixPrefsControllers.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling pMixPrefsControllers.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/pMixPrefsComponent_29159002.o: ../../Source/pMixPrefsComponent.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling pMixPrefsComponent.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/pMixCodeEditor_9c72070f.o: ../../Source/pMixCodeEditor.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling pMixCodeEditor.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/pMixSVGDisplay_81b3cd13.o: ../../Source/pMixSVGDisplay.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling pMixSVGDisplay.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/pMixFileBrowser_37eaaf21.o: ../../Source/pMixFileBrowser.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling pMixFileBrowser.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/pMixConsole_f8903cec.o: ../../Source/pMixConsole.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling pMixConsole.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/pMixGenericEditor_f1457b39.o: ../../Source/pMixGenericEditor.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling pMixGenericEditor.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/pMixRHPTabContainer_91db8e9b.o: ../../Source/pMixRHPTabContainer.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling pMixRHPTabContainer.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/pMixMainAppWindow_68a956cd.o: ../../Source/pMixMainAppWindow.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling pMixMainAppWindow.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/pMixApp_f4968cb6.o: ../../Source/pMixApp.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling pMixApp.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/pMixLookAndFeel_bf43c93.o: ../../Source/pMixLookAndFeel.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling pMixLookAndFeel.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/pMixDocument_9b99d1f0.o: ../../Source/pMixDocument.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling pMixDocument.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/pMixPluginWindow_8e0c1d8.o: ../../Source/pMixPluginWindow.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling pMixPluginWindow.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/pMixFloatWindow_b286ca81.o: ../../Source/pMixFloatWindow.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling pMixFloatWindow.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/pMixMainComponent_71d6859.o: ../../Source/pMixMainComponent.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling pMixMainComponent.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/pMixInternalPluginFormat_69d4d33c.o: ../../Source/pMixInternalPluginFormat.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling pMixInternalPluginFormat.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/pMixAudioEngine_de4ed8cd.o: ../../Source/pMixAudioEngine.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling pMixAudioEngine.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_audio_basics_6b797ca1.o: ../../JuceLibraryCode/juce_audio_basics.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_audio_basics.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_audio_devices_a742c38b.o: ../../JuceLibraryCode/juce_audio_devices.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_audio_devices.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_audio_formats_5a29c68a.o: ../../JuceLibraryCode/juce_audio_formats.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_audio_formats.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_audio_processors_dea3173d.o: ../../JuceLibraryCode/juce_audio_processors.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_audio_processors.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_audio_utils_c7eb679f.o: ../../JuceLibraryCode/juce_audio_utils.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_audio_utils.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_core_75b14332.o: ../../JuceLibraryCode/juce_core.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_core.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_cryptography_6de2ebff.o: ../../JuceLibraryCode/juce_cryptography.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_cryptography.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_data_structures_72d3da2c.o: ../../JuceLibraryCode/juce_data_structures.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_data_structures.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_events_d2be882c.o: ../../JuceLibraryCode/juce_events.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_events.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_faustllvm_3d2bdc9.o: ../../JuceLibraryCode/juce_faustllvm.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_faustllvm.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_graphics_9c18891e.o: ../../JuceLibraryCode/juce_graphics.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_graphics.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_gui_basics_8a6da59c.o: ../../JuceLibraryCode/juce_gui_basics.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_gui_basics.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_gui_extra_4a026f23.o: ../../JuceLibraryCode/juce_gui_extra.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_gui_extra.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_opengl_cd70b4c2.o: ../../JuceLibraryCode/juce_opengl.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_opengl.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)
