#UTIOBJ_DIR = $(UTI_DIR)/.objs
# .c indicates C source files, and others C++ ones.  
ifndef SRCEXTS
	SRCEXTS = .c .cc  
endif

# The header file types.
ifndef  HDREXTS
	HDREXTS = .h 
endif

#the c++/c file
UTI_SOURCES = $(wildcard $(addprefix $(UTI_DIR)/*,$(SRCEXTS)))
#the headers file
UTI_HEADERS = $(wildcard $(addprefix $(UTI_DIR)/*,$(HDREXTS)))
#objects file
UTI_OBJECTS = $(addsuffix .o, $(basename $(addprefix $(OBJ_DIR)/,$(notdir $(UTI_SOURCES)))))
