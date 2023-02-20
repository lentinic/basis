BASIS_ROOT          := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))
BASIS_ROOT          := $(BASIS_ROOT:%/=%)

BASIS_SOURCES       := $(foreach srcdir, $(BASIS_ROOT)/src, $(wildcard $(srcdir)/*.cpp))
BASIS_SOURCES       := $(BASIS_SOURCES:$(BASIS_ROOT)/%=%)

BASIS_OBJECTS       := $(BASIS_SOURCES:%.cpp=$(INTERMEDIATE_DIR)/basis/%.o)

BASIS_INCLUDE_DIR   := $(BASIS_ROOT)/include
BASIS_DEFINES       :=
BASIS_CPPFLAGS      :=

$(BASIS_OBJECTS): INCLUDE_DIRS := $(BASIS_INCLUDE_DIR)
$(BASIS_OBJECTS): DEFINES += $(BASIS_DEFINES)
$(BASIS_OBJECTS): CPPFLAGS += $(BASIS_CPPFLAGS)

$(INTERMEDIATE_DIR)/basis/%.o : $(BASIS_ROOT)/%.cpp
	@echo "basis:" $(<:$(BASIS_ROOT)/%=%)
	@mkdir -p $(dir $@)
	@$(CXX) $< $(CPPFLAGS) $(INCLUDE_DIRS:%=-I%) $(DEFINES:%=-D%) -MMD -c -o $@

INCLUDE_DIRS += $(BASIS_INCLUDE_DIR)
OBJECTS += $(BASIS_OBJECTS)