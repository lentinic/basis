BASIS_ROOT      	:= $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))

BASIS_SOURCES   	:= string.cpp
BASIS_OBJECTS       := $(BASIS_SOURCES:%.cpp=$(INTERMEDIATE_DIR)/basis/%.o)
BASIS_INCLUDE_DIR	:= $(BASIS_ROOT)/include

DEFINES      += $(BASIS_DEFINES)
INCLUDE_DIRS += $(BASIS_INCLUDE_DIR)
OBJECTS      += $(BASIS_OBJECTS)

$(INTERMEDIATE_DIR)/basis/%.o : $(BASIS_ROOT)/src/%.cpp
	@echo "basis:" $<
	@mkdir -p $(dir $@)
	@$(CXX) $< $(CPPFLAGS) -I$(BASIS_INCLUDE_DIR) -MMD -c -o $@