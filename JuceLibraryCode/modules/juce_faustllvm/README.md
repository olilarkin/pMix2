# FAUST LLVM JIT Module for JUCE

This is a JUCE Module for embedding the Faust LLVM JIT compiler as a JUCE AudioProcessor/AudioPluginInstance. It includes supporting classes such as a Faust syntax highlighter for the JUCE code editor.

Should work on OSX, Linux and eventually Windows. Only tested on OSX 10.9 thusfar.

THE FOLLOWING IS WORK IN PROGRESS - NEEDS TO BE CHECKED ON A FRESH SYSTEM!

**Requirements:**

* LLVM 3.5 (install via macports/linux package manager) e.g. sudo port install llvm-3.5 +universal
* FAUST (faust2 branch) : http://sourceforge.net/p/faudiostream/
* libncurses e.g. sudo port install ncurses +universal
* zlib e.g. sudo port install zlib +universal
* libcrypto from openssl e.g. sudo port install openssl +universal


YOU MAY NEED TO ADD THE FOLLOWING TO YOUR JUCER PROJECT:

**Extra Linker Flags:**
(run llvm-config --libs on command line to get list)


**Extra libraries to link:** faust crypto z curses

**Header search paths:**

* /usr/local/include

**Extra Library search paths:**

* /usr/local/lib
* /opt/local/lib
* /opt/local/libexec/llvm-3.5/lib


**License:**

  Written by Oliver Larkin,  
  Contemporary Music Research Centre  
  Music Department  
  University of York  
  License: GPL
  
  www.cmrcyork.org | www.olilarkin.co.uk