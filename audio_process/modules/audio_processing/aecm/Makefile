DRV_RULES_DIR =../../..
include $(DRV_RULES_DIR)/Rules.make

NAME = $(notdir $(CURDIR))
DIR = $(dir $(CURDIR))

UTI_DIR = $(CURDIR)/../utility

LIB_NAME = libWebRtcAECM.a

RELEASE_DIR = $(DRV_RULES_DIR)/release
HEADER_DIR = $(DRV_RULES_DIR)/include

# archiver and its options
ARFLAGS = -r

EXE_DIR = .

# The pre-processor options used by the cpp (man cpp for more).  
CPPFLAGS  = -Wall -Werror 

# The pre-processor and compiler options.  
# Users can override those variables from the command line.  
CFLAGS  = -g -O2  -DWEBRTC_POSIX
CXXFLAGS= -g -O2  -DWEBRTC_POSIX

LDFLAGS = -lpthread -pthread


# .c indicates C source files, and others C++ ones.  
SRCEXTS = .c .cc  
  
# The header file types.  
HDREXTS = .h 


HEADERS := $(wildcard $(addprefix *,$(HDREXTS)))
src     := $(wildcard $(addprefix *,$(SRCEXTS)))
uti_src := $(wildcard $(addprefix $(UTI_DIR)/*,$(SRCEXTS)))
uti_headers := $(wildcard $(addprefix $(UTI_DIR)/*,$(HDREXTS)))
objects = $(addsuffix .o, $(basename $(src))) $(addsuffix .o, $(basename $(uti_src)))
executables := $(patsubst %.c,%,$(src))

INCDIRS := -I. -I$(COMPILE_INCLUDE) -I$(HEADER_DIR) -I$(DRV_RULES_DIR) -I$(DRV_RULES_DIR)/common -I$(UTI_DIR) 

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
		rm -f $(EXE_DIR)/$$dir; \
	done

distclean: clean
	rm -f *.o
	

# Show variables (for debug use only.)  
show :
	@echo 'src            :' $(src)
	@echo 'uti_src        :' $(uti_src)
	@echo 'objects        :' $(objects)
