DRV_RULES_DIR =../../..
include $(DRV_RULES_DIR)/Rules.make

NAME = $(notdir $(CURDIR))
DIR = $(dir $(CURDIR))

LIB_NAME = libWebRtcAGC.a

RELEASE_DIR = $(DRV_RULES_DIR)/release
HEADER_DIR = $(DRV_RULES_DIR)/include

# archiver and its options
ARFLAGS = -r

EXE_DIR = .

# The pre-processor options used by the cpp (man cpp for more).  
CPPFLAGS  = -Wall -Werror 

# The pre-processor and compiler options.  
# Users can override those variables from the command line.  
CFLAGS  = -g -O2  -DWEBRTC_POSIX  -DWEBRTC_LINUX
CXXFLAGS= -g -O2  -DWEBRTC_POSIX  -DWEBRTC_LINUX

LDFLAGS = -lpthread -pthread


# .c indicates C source files, and others C++ ones.  
SRCEXTS = .c .cc  
  
# The header file types.  
HDREXTS = .h 


HEADERS := $(wildcard $(addprefix *,$(HDREXTS)))
src     := $(wildcard $(addprefix *,$(SRCEXTS)))

objects = $(addsuffix .o, $(basename $(src)))
executables := $(dir $(objects))

INCDIRS := -I. -I$(HEADER_DIR) -I$(DRV_RULES_DIR) -I$(DRV_RULES_DIR)/common

CFLAGS += $(INCDIRS)
CXXFLAGS += $(INCDIRS) -std=gnu++11

all: LIB_NAME
LIB_NAME : $(objects)
	$(AR) $(ARFLAGS) $(LIB_NAME) $(objects)

$(objects):
%.o:%.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@
%.o:%.cc
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) $< -o $@
	

install:
	cp *.h  $(HEADER_DIR)
	cp $(LIB_NAME) $(RELEASE_DIR)
#	install -d $(EXEC_DIR)
#	install $(executables) $(EXEC_DIR)

clean:
	rm -f *.o *.a
	@for dir in $(executables); do \
		rm -rf $$dir/*.o $$dir/*.a; \
	done

distclean: clean
	rm -f *.o
	

# Show variables (for debug use only.)  
show :
	@echo 'src            :' $(src)
	@echo 'uti_src        :' $(uti_src)
	@echo 'objects        :' $(objects)
