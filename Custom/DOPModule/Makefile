##===- lib/Transforms/Hello/Makefile -----------------------*- Makefile -*-===##
#
#                     The LLVM Compiler Infrastructure
#
# This file is distributed under the University of Illinois Open Source
# License. See LICENSE.TXT for details.
#
##===----------------------------------------------------------------------===##

LEVEL = ../../..
LIBRARYNAME = LLVMDOPModule
LOADABLE_MODULE = 1
#BUILD_ARCHIVE = 1
#USEDLIBS =

# If we don't need RTTI or EH, there's no reason to export anything
# from the hello plugin.
ifneq ($(REQUIRES_RTTI), 1)
ifneq ($(REQUIRES_EH), 1)
#EXPORTED_SYMBOL_FILE = $(PROJ_SRC_DIR)/DFG.exports
endif
endif

include $(LEVEL)/Makefile.common
LIBS += -lboost_thread -lboost_serialization -lboost_iostreams -lboost_filesystem -lboost_system
