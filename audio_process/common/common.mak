#COMMOBJ_DIR = $(COMMON_DIR)/.objs
# .c indicates C source files, and others C++ ones.  
ifndef SRCEXTS
	SRCEXTS = .c .cc  
endif  
  
# The header file types.  
ifndef  HDREXTS
	HDREXTS = .h 
endif

#the current dir;Recursive with three-level maxdepth;
COMM_DIRS := $(shell find $(COMMON_DIR) -maxdepth 3 -type d)
#the c++/c file
COMM_SOURCES = $(foreach dir,$(COMM_DIRS),$(wildcard $(addprefix $(dir)/*,$(SRCEXTS))))
#the headers file
COMM_HEADERS = $(foreach dir,$(COMM_DIRS),$(wildcard $(addprefix $(dir)/*,$(HDREXTS)))) 
#objects file
COMM_OBJECTS = $(addsuffix .o, $(basename $(addprefix $(OBJ_DIR)/,$(notdir $(COMM_SOURCES)))))
