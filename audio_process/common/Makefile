DRV_RULES_DIR =..
include $(DRV_RULES_DIR)/Rules.make

NAME = $(notdir $(CURDIR))
DIR = $(dir $(CURDIR))

LIB_NAME = libWebRtcCommon.a

RELEASE_DIR = $(DRV_RULES_DIR)/release
HEADER_DIR = $(DRV_RULES_DIR)/include
EXE_DIR = .

# archiver and its options
ARFLAGS = -r

# The pre-processor options used by the cpp (man cpp for more).  
CPPFLAGS  = -Wall -Werror 

# The pre-processor and compiler options.  
# Users can override those variables from the command line.  
CFLAGS  = -g -O2  -DWEBRTC_POSIX -DWEBRTC_LINUX
CXXFLAGS= -g -O2  -DWEBRTC_POSIX -DWEBRTC_LINUX

LDFLAGS = -lpthread -pthread

# .c indicates C source files, and others C++ ones.  
SRCEXTS = .c .cc  
  
# The header file types.  
HDREXTS = .h 


OBJ_DIR=.objs
MKDIRS = $(OBJ_DIR)


#the current dir;Recursive with three-level maxdepth;
DIRS := $(shell find $(CURDIR) -maxdepth 3 -type d)
#the c++/c file
SOURCES = $(foreach dir,$(DIRS),$(wildcard $(addprefix $(dir)/*,$(SRCEXTS))))
#the headers file
HEADERS = $(foreach dir,$(DIRS),$(wildcard $(addprefix $(dir)/*,$(HDREXTS)))) 

objects = $(addsuffix .o, $(basename $(SOURCES)))

filterDir   := $(filter-out %$(OBJ_DIR) %/include,$(DIRS))
executables := $(filterDir)

INCDIRS := -I. -I$(CURDIR) -I$(HEADER_DIR)
CFLAGS   += $(INCDIRS)
CXXFLAGS += $(INCDIRS) -std=gnu++11

all:$(LIB_NAME)

$(LIB_NAME) : $(objects)
	$(AR) $(ARFLAGS) $(LIB_NAME) $(objects)

$(objects): 
%.o:%.c
	$(CC)  -c $(CFLAGS)   $(CPPFLAGS) $< -o $@
%.o:%.cc
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) $< -o $@


$(MKDIRS):
	mkdir $@

install:
	cp $(HEADERS)  $(HEADER_DIR)
	cp $(LIB_NAME) $(RELEASE_DIR)
	@for dir in $(executables); do \
	  mkdir -p $$dir/$(OBJ_DIR);    \
	  mv $$dir/*.o  $$dir/$(OBJ_DIR); \
	done
#	install -d $(EXEC_DIR)
#	install $(executables) $(EXEC_DIR)

clean:
	rm -f *.o *.a
	@for dir in $(executables); do \
		rm -rf $$dir/$(OBJ_DIR)/*.o $$dir/*.a; \
	done

distclean: clean
	rm -f *.o
	
	
# Show variables (for debug use only.)  
show: 
#	@echo 'DIRS        :' $(DIRS)
#	@echo 'SOURCES     :' $(SOURCES)
#	@echo 'HEADERS     :' $(HEADERS)
#	@echo 'objects     :' $(objects) 
#	@echo 'INCHDERS    :' $(INCHDERS)
	@echo 'filterDir    :' $(filterDir)
	@echo 'executables    :' $(executables)



.PHONY: distclean clean install show all