# build options
CONFIG ?= debug
PLATFORM ?= 
ARCH ?=

SOURCES :=
DEFINES := 
INCLUDE_DIRS :=
CPPFLAGS := -std=c++20 -Wall

ifeq ($(CONFIG),release)
	DEFINES += BUILD_CONFIG_RELEASE NDEBUG
	CPPFLAGS += -Os
else ifeq ($(CONFIG),develop)
	DEFINES += BUILD_CONFIG_DEVELOP NDEBUG
	CPPFLAGS += -Os -gfull
else ifeq ($(CONFIG),debug)
	DEFINES += BUILD_CONFIG_DEBUG _DEBUG
	CPPFLAGS += -O0 -gfull
else
	$(error Unrecognized CONFIG "$(CONFIG)" - must be release | develop | debug)
endif

INTERMEDIATE_ROOT := intermediate
OUTPUT_ROOT := bin
INTERMEDIATE_DIR := $(INTERMEDIATE_ROOT)/$(CONFIG)
OUTPUT_DIR := $(OUTPUT_ROOT)/$(CONFIG)

OBJECTS =

-include ../basis.mak

PROGRAMS := bitvector shared_mutex signal

bitvector: SOURCES += tests/bitvector.cpp
shared_mutex: SOURCES += tests/shared_mutex.cpp
signal: SOURCES += tests/signal.cpp

OBJECTS += $(SOURCES:%.cpp=$(INTERMEDIATE_DIR)/%.o)

-include $(OBJECTS:%.o=%.d)
-include $(INTERMEDIATE_DIR)/tests/bitvector.d
-include $(INTERMEDIATE_DIR)/tests/shared_mutex.d
-include $(INTERMEDIATE_DIR)/tests/signal.d

.SECONDEXPANSION:

$(INTERMEDIATE_DIR)/tests/%.o : %.cpp 
	@echo "tests:" $<
	@mkdir -p $(dir $@)
	@$(CXX) $< $(CPPFLAGS) $(INCLUDE_DIRS:%=-I%) $(DEFINES:%=-D%) -MMD -c -o $@

$(PROGRAMS): $$(OBJECTS)
	@echo "Linking:" $@
	@mkdir -p $(OUTPUT_DIR)	
	@$(CXX) $^ -o $(OUTPUT_DIR)/$@

.PHONY: clean all

clean:
	@echo "Removing intermediate files..."
	@rm -r $(INTERMEDIATE_ROOT)

all: $(PROGRAMS)