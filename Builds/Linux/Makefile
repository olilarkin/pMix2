# Automatically generated makefile, created by the Introjucer
# Don't edit this file! Your changes will be overwritten when you re-save the Introjucer project!

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

  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D "JUCE_PLUGINHOST_LADSPA=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -D "JUCE_APP_VERSION=0.0.1" -D "JUCE_APP_VERSION_HEX=0x1" -I /usr/include -I /usr/include/freetype2 -I ~/SDKs/VST3\ SDK -I ~/SDKs/vstsdk2.4 -I ../../JuceLibraryCode -I ../../../../juce/modules -I ../../../../Projects
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -g -ggdb -O0 -std=gnu++0x
  CXXFLAGS += $(CFLAGS) -std=c++11
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -L/usr/X11R6/lib/ -L/usr/lib/llvm-3.5/lib/ -lGL -lLLVMAArch64AsmParser -lLLVMAArch64AsmPrinter -lLLVMAArch64CodeGen -lLLVMAArch64Desc -lLLVMAArch64Disassembler -lLLVMAArch64Info -lLLVMAArch64Utils -lLLVMARMAsmParser -lLLVMARMAsmPrinter -lLLVMARMCodeGen -lLLVMARMDesc -lLLVMARMDisassembler -lLLVMARMInfo -lLLVMAnalysis -lLLVMAsmParser -lLLVMAsmPrinter -lLLVMBitReader -lLLVMBitWriter -lLLVMCodeGen -lLLVMCore -lLLVMCppBackendCodeGen -lLLVMCppBackendInfo -lLLVMDebugInfo -lLLVMExecutionEngine -lLLVMHexagonAsmPrinter -lLLVMHexagonCodeGen -lLLVMHexagonDesc -lLLVMHexagonInfo -lLLVMIRReader -lLLVMInstCombine -lLLVMInstrumentation -lLLVMInterpreter -lLLVMJIT -lLLVMLTO -lLLVMLineEditor -lLLVMLinker -lLLVMMC -lLLVMMCAnalysis -lLLVMMCDisassembler -lLLVMMCJIT -lLLVMMCParser -lLLVMMSP430AsmPrinter -lLLVMMSP430CodeGen -lLLVMMSP430Desc -lLLVMMSP430Info -lLLVMMipsAsmParser -lLLVMMipsAsmPrinter -lLLVMMipsCodeGen -lLLVMMipsDesc -lLLVMMipsDisassembler -lLLVMMipsInfo -lLLVMNVPTXAsmPrinter -lLLVMNVPTXCodeGen -lLLVMNVPTXDesc -lLLVMNVPTXInfo -lLLVMObjCARCOpts -lLLVMObject -lLLVMOption -lLLVMPowerPCAsmParser -lLLVMPowerPCAsmPrinter -lLLVMPowerPCCodeGen -lLLVMPowerPCDesc -lLLVMPowerPCDisassembler -lLLVMPowerPCInfo -lLLVMProfileData -lLLVMR600AsmPrinter -lLLVMR600CodeGen -lLLVMR600Desc -lLLVMR600Info -lLLVMRuntimeDyld -lLLVMScalarOpts -lLLVMSelectionDAG -lLLVMSparcAsmParser -lLLVMSparcAsmPrinter -lLLVMSparcCodeGen -lLLVMSparcDesc -lLLVMSparcDisassembler -lLLVMSparcInfo -lLLVMSupport -lLLVMSystemZAsmParser -lLLVMSystemZAsmPrinter -lLLVMSystemZCodeGen -lLLVMSystemZDesc -lLLVMSystemZDisassembler -lLLVMSystemZInfo -lLLVMTableGen -lLLVMTarget -lLLVMTransformUtils -lLLVMVectorize -lLLVMX86AsmParser -lLLVMX86AsmPrinter -lLLVMX86CodeGen -lLLVMX86Desc -lLLVMX86Disassembler -lLLVMX86Info -lLLVMX86Utils -lLLVMXCoreAsmPrinter -lLLVMXCoreCodeGen -lLLVMXCoreDesc -lLLVMXCoreDisassembler -lLLVMXCoreInfo -lLLVMipa -lLLVMipo -lX11 -lXext -lXinerama -lasound -lcrypto -lcurses -ldl -lfaust -lfreetype -lpthread -lrt -lz -lfaust -lcrypto -lcurses -lLLVMLTO -lLLVMObjCARCOpts -lLLVMLinker -lLLVMipo -lLLVMVectorize -lLLVMBitWriter -lLLVMIRReader -lLLVMAsmParser -lLLVMR600CodeGen -lLLVMR600Desc -lLLVMR600Info -lLLVMR600AsmPrinter -lLLVMSystemZDisassembler -lLLVMSystemZCodeGen -lLLVMSystemZAsmParser -lLLVMSystemZDesc -lLLVMSystemZInfo -lLLVMSystemZAsmPrinter -lLLVMHexagonCodeGen -lLLVMHexagonAsmPrinter -lLLVMHexagonDesc -lLLVMHexagonInfo -lLLVMNVPTXCodeGen -lLLVMNVPTXDesc -lLLVMNVPTXInfo -lLLVMNVPTXAsmPrinter -lLLVMCppBackendCodeGen -lLLVMCppBackendInfo -lLLVMMSP430CodeGen -lLLVMMSP430Desc -lLLVMMSP430Info -lLLVMMSP430AsmPrinter -lLLVMXCoreDisassembler -lLLVMXCoreCodeGen -lLLVMXCoreDesc -lLLVMXCoreInfo -lLLVMXCoreAsmPrinter -lLLVMMipsDisassembler -lLLVMMipsCodeGen -lLLVMMipsAsmParser -lLLVMMipsDesc -lLLVMMipsInfo -lLLVMMipsAsmPrinter -lLLVMAArch64Disassembler -lLLVMAArch64CodeGen -lLLVMAArch64AsmParser -lLLVMAArch64Desc -lLLVMAArch64Info -lLLVMAArch64AsmPrinter -lLLVMAArch64Utils -lLLVMARMDisassembler -lLLVMARMCodeGen -lLLVMARMAsmParser -lLLVMARMDesc -lLLVMARMInfo -lLLVMARMAsmPrinter -lLLVMPowerPCDisassembler -lLLVMPowerPCCodeGen -lLLVMPowerPCAsmParser -lLLVMPowerPCDesc -lLLVMPowerPCInfo -lLLVMPowerPCAsmPrinter -lLLVMSparcDisassembler -lLLVMSparcCodeGen -lLLVMSparcAsmParser -lLLVMSparcDesc -lLLVMSparcInfo -lLLVMSparcAsmPrinter -lLLVMTableGen -lLLVMDebugInfo -lLLVMOption -lLLVMX86Disassembler -lLLVMX86AsmParser -lLLVMX86CodeGen -lLLVMSelectionDAG -lLLVMAsmPrinter -lLLVMX86Desc -lLLVMX86Info -lLLVMX86AsmPrinter -lLLVMX86Utils -lLLVMJIT -lLLVMLineEditor -lLLVMMCAnalysis -lLLVMMCDisassembler -lLLVMInstrumentation -lLLVMInterpreter -lLLVMCodeGen -lLLVMScalarOpts -lLLVMInstCombine -lLLVMTransformUtils -lLLVMipa -lLLVMAnalysis -lLLVMProfileData -lLLVMMCJIT -lLLVMTarget -lLLVMRuntimeDyld -lLLVMObject -lLLVMMCParser -lLLVMBitReader -lLLVMExecutionEngine -lLLVMMC -lLLVMCore -lLLVMSupport -lz -ltinfo

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

  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "NDEBUG=1" -D "JUCE_PLUGINHOST_LADSPA=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -D "JUCE_APP_VERSION=0.0.1" -D "JUCE_APP_VERSION_HEX=0x1" -I /usr/include -I /usr/include/freetype2 -I ~/SDKs/VST3\ SDK -I ~/SDKs/vstsdk2.4 -I ../../JuceLibraryCode -I ../../../../juce/modules -I ../../../../Projects
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -Os -std=gnu++0x
  CXXFLAGS += $(CFLAGS) -std=c++11
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -fvisibility=hidden -L/usr/X11R6/lib/ -L/usr/lib/llvm-3.5/lib/ -lGL -lLLVMAArch64AsmParser -lLLVMAArch64AsmPrinter -lLLVMAArch64CodeGen -lLLVMAArch64Desc -lLLVMAArch64Disassembler -lLLVMAArch64Info -lLLVMAArch64Utils -lLLVMARMAsmParser -lLLVMARMAsmPrinter -lLLVMARMCodeGen -lLLVMARMDesc -lLLVMARMDisassembler -lLLVMARMInfo -lLLVMAnalysis -lLLVMAsmParser -lLLVMAsmPrinter -lLLVMBitReader -lLLVMBitWriter -lLLVMCodeGen -lLLVMCore -lLLVMCppBackendCodeGen -lLLVMCppBackendInfo -lLLVMDebugInfo -lLLVMExecutionEngine -lLLVMHexagonAsmPrinter -lLLVMHexagonCodeGen -lLLVMHexagonDesc -lLLVMHexagonInfo -lLLVMIRReader -lLLVMInstCombine -lLLVMInstrumentation -lLLVMInterpreter -lLLVMJIT -lLLVMLTO -lLLVMLineEditor -lLLVMLinker -lLLVMMC -lLLVMMCAnalysis -lLLVMMCDisassembler -lLLVMMCJIT -lLLVMMCParser -lLLVMMSP430AsmPrinter -lLLVMMSP430CodeGen -lLLVMMSP430Desc -lLLVMMSP430Info -lLLVMMipsAsmParser -lLLVMMipsAsmPrinter -lLLVMMipsCodeGen -lLLVMMipsDesc -lLLVMMipsDisassembler -lLLVMMipsInfo -lLLVMNVPTXAsmPrinter -lLLVMNVPTXCodeGen -lLLVMNVPTXDesc -lLLVMNVPTXInfo -lLLVMObjCARCOpts -lLLVMObject -lLLVMOption -lLLVMPowerPCAsmParser -lLLVMPowerPCAsmPrinter -lLLVMPowerPCCodeGen -lLLVMPowerPCDesc -lLLVMPowerPCDisassembler -lLLVMPowerPCInfo -lLLVMProfileData -lLLVMR600AsmPrinter -lLLVMR600CodeGen -lLLVMR600Desc -lLLVMR600Info -lLLVMRuntimeDyld -lLLVMScalarOpts -lLLVMSelectionDAG -lLLVMSparcAsmParser -lLLVMSparcAsmPrinter -lLLVMSparcCodeGen -lLLVMSparcDesc -lLLVMSparcDisassembler -lLLVMSparcInfo -lLLVMSupport -lLLVMSystemZAsmParser -lLLVMSystemZAsmPrinter -lLLVMSystemZCodeGen -lLLVMSystemZDesc -lLLVMSystemZDisassembler -lLLVMSystemZInfo -lLLVMTableGen -lLLVMTarget -lLLVMTransformUtils -lLLVMVectorize -lLLVMX86AsmParser -lLLVMX86AsmPrinter -lLLVMX86CodeGen -lLLVMX86Desc -lLLVMX86Disassembler -lLLVMX86Info -lLLVMX86Utils -lLLVMXCoreAsmPrinter -lLLVMXCoreCodeGen -lLLVMXCoreDesc -lLLVMXCoreDisassembler -lLLVMXCoreInfo -lLLVMipa -lLLVMipo -lX11 -lXext -lXinerama -lasound -lcrypto -lcurses -ldl -lfaust -lfreetype -lpthread -lrt -lz -lfaust -lcrypto -lcurses -lLLVMLTO -lLLVMObjCARCOpts -lLLVMLinker -lLLVMipo -lLLVMVectorize -lLLVMBitWriter -lLLVMIRReader -lLLVMAsmParser -lLLVMR600CodeGen -lLLVMR600Desc -lLLVMR600Info -lLLVMR600AsmPrinter -lLLVMSystemZDisassembler -lLLVMSystemZCodeGen -lLLVMSystemZAsmParser -lLLVMSystemZDesc -lLLVMSystemZInfo -lLLVMSystemZAsmPrinter -lLLVMHexagonCodeGen -lLLVMHexagonAsmPrinter -lLLVMHexagonDesc -lLLVMHexagonInfo -lLLVMNVPTXCodeGen -lLLVMNVPTXDesc -lLLVMNVPTXInfo -lLLVMNVPTXAsmPrinter -lLLVMCppBackendCodeGen -lLLVMCppBackendInfo -lLLVMMSP430CodeGen -lLLVMMSP430Desc -lLLVMMSP430Info -lLLVMMSP430AsmPrinter -lLLVMXCoreDisassembler -lLLVMXCoreCodeGen -lLLVMXCoreDesc -lLLVMXCoreInfo -lLLVMXCoreAsmPrinter -lLLVMMipsDisassembler -lLLVMMipsCodeGen -lLLVMMipsAsmParser -lLLVMMipsDesc -lLLVMMipsInfo -lLLVMMipsAsmPrinter -lLLVMAArch64Disassembler -lLLVMAArch64CodeGen -lLLVMAArch64AsmParser -lLLVMAArch64Desc -lLLVMAArch64Info -lLLVMAArch64AsmPrinter -lLLVMAArch64Utils -lLLVMARMDisassembler -lLLVMARMCodeGen -lLLVMARMAsmParser -lLLVMARMDesc -lLLVMARMInfo -lLLVMARMAsmPrinter -lLLVMPowerPCDisassembler -lLLVMPowerPCCodeGen -lLLVMPowerPCAsmParser -lLLVMPowerPCDesc -lLLVMPowerPCInfo -lLLVMPowerPCAsmPrinter -lLLVMSparcDisassembler -lLLVMSparcCodeGen -lLLVMSparcAsmParser -lLLVMSparcDesc -lLLVMSparcInfo -lLLVMSparcAsmPrinter -lLLVMTableGen -lLLVMDebugInfo -lLLVMOption -lLLVMX86Disassembler -lLLVMX86AsmParser -lLLVMX86CodeGen -lLLVMSelectionDAG -lLLVMAsmPrinter -lLLVMX86Desc -lLLVMX86Info -lLLVMX86AsmPrinter -lLLVMX86Utils -lLLVMJIT -lLLVMLineEditor -lLLVMMCAnalysis -lLLVMMCDisassembler -lLLVMInstrumentation -lLLVMInterpreter -lLLVMCodeGen -lLLVMScalarOpts -lLLVMInstCombine -lLLVMTransformUtils -lLLVMipa -lLLVMAnalysis -lLLVMProfileData -lLLVMMCJIT -lLLVMTarget -lLLVMRuntimeDyld -lLLVMObject -lLLVMMCParser -lLLVMBitReader -lLLVMExecutionEngine -lLLVMMC -lLLVMCore -lLLVMSupport -lz -ltinfo

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
  $(OBJDIR)/pMixWebBrowser_3e333589.o \
  $(OBJDIR)/pMixFileBrowser_37eaaf21.o \
  $(OBJDIR)/pMixConsole_f8903cec.o \
  $(OBJDIR)/pMixGenericEditor_f1457b39.o \
  $(OBJDIR)/pMixRHPTabContainer_91db8e9b.o \
  $(OBJDIR)/pMixMainAppWindow_68a956cd.o \
  $(OBJDIR)/pMixApp_f4968cb6.o \
  $(OBJDIR)/pMixLookAndFeel_bf43c93.o \
  $(OBJDIR)/pMixDocument_9b99d1f0.o \
  $(OBJDIR)/pMixPluginWindow_8e0c1d8.o \
  $(OBJDIR)/pMixMainComponent_71d6859.o \
  $(OBJDIR)/pMixInternalFilters_8e34e293.o \
  $(OBJDIR)/pMixAudioEngine_de4ed8cd.o \
  $(OBJDIR)/juce_audio_basics_50fb0bc.o \
  $(OBJDIR)/juce_audio_devices_8591c67a.o \
  $(OBJDIR)/juce_audio_formats_b413101a.o \
  $(OBJDIR)/juce_audio_processors_7d960e74.o \
  $(OBJDIR)/juce_audio_utils_450cab3a.o \
  $(OBJDIR)/juce_core_eaa9319e.o \
  $(OBJDIR)/juce_cryptography_694b3f8.o \
  $(OBJDIR)/juce_data_structures_cb7bfada.o \
  $(OBJDIR)/juce_events_46ed9f12.o \
  $(OBJDIR)/juce_faustllvm_624bbe35.o \
  $(OBJDIR)/juce_graphics_99a8a576.o \
  $(OBJDIR)/juce_gui_basics_9ed061f2.o \
  $(OBJDIR)/juce_gui_extra_f56667fa.o \
  $(OBJDIR)/juce_opengl_735c73e.o \

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

$(OBJDIR)/pMixWebBrowser_3e333589.o: ../../Source/pMixWebBrowser.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling pMixWebBrowser.cpp"
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

$(OBJDIR)/pMixMainComponent_71d6859.o: ../../Source/pMixMainComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling pMixMainComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/pMixInternalFilters_8e34e293.o: ../../Source/pMixInternalFilters.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling pMixInternalFilters.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/pMixAudioEngine_de4ed8cd.o: ../../Source/pMixAudioEngine.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling pMixAudioEngine.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_basics_50fb0bc.o: ../../../../juce/modules/juce_audio_basics/juce_audio_basics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_basics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_devices_8591c67a.o: ../../../../juce/modules/juce_audio_devices/juce_audio_devices.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_devices.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_formats_b413101a.o: ../../../../juce/modules/juce_audio_formats/juce_audio_formats.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_formats.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_processors_7d960e74.o: ../../../../juce/modules/juce_audio_processors/juce_audio_processors.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_processors.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_utils_450cab3a.o: ../../../../juce/modules/juce_audio_utils/juce_audio_utils.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_utils.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_core_eaa9319e.o: ../../../../juce/modules/juce_core/juce_core.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_core.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_cryptography_694b3f8.o: ../../../../juce/modules/juce_cryptography/juce_cryptography.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_cryptography.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_data_structures_cb7bfada.o: ../../../../juce/modules/juce_data_structures/juce_data_structures.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_data_structures.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_events_46ed9f12.o: ../../../../juce/modules/juce_events/juce_events.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_events.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_faustllvm_624bbe35.o: ../../../juce_faustllvm/juce_faustllvm.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_faustllvm.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_graphics_99a8a576.o: ../../../../juce/modules/juce_graphics/juce_graphics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_graphics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_basics_9ed061f2.o: ../../../../juce/modules/juce_gui_basics/juce_gui_basics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_gui_basics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_extra_f56667fa.o: ../../../../juce/modules/juce_gui_extra/juce_gui_extra.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_gui_extra.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_opengl_735c73e.o: ../../../../juce/modules/juce_opengl/juce_opengl.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_opengl.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)
