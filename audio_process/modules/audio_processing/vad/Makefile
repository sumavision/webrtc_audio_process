DRV_RULES_DIR = $(CURDIR)/../../..
COMMON_DIR =$(DRV_RULES_DIR)/common
VADSRC_DIR = $(COMMON_DIR)/common_audio/vad
OBJ_DIR    = $(CURDIR)/.objs

include $(DRV_RULES_DIR)/Rules.make
include $(COMMON_DIR)/common.mak

RELEASE_DIR = $(DRV_RULES_DIR)/release
HEADER_DIR = $(DRV_RULES_DIR)/include
MKDIRS = $(OBJ_DIR)

NAME = $(notdir $(CURDIR))
DIR = $(dir $(CURDIR))

LIB_NAME = libwebrtcvad.a


# archiver and its options
ARFLAGS = -r

# The pre-processor options used by the cpp (man cpp for more).  
CPPFLAGS  = -Wall -Werror 

# The pre-processor and compiler options.   
CFLAGS  = -g -O2  -DWEBRTC_POSIX  -DWEBRTC_LINUX
CXXFLAGS= -g -O2  -DWEBRTC_POSIX  -DWEBRTC_LINUX -std=gnu++11

#libs 
LDFLAGS = -lpthread -pthread



HEADERS := $(wildcard $(addprefix *,$(HDREXTS)))
src     := $(wildcard $(addprefix *,$(SRCEXTS)))

objects = $(addsuffix .o, $(basename $(src)))
executables := $(dir $(objects))

VAD_HEADERS = $(wildcard $(addprefix $(VADSRC_DIR)/*,$(HDREXTS)))
VAD_SOURCES = $(wildcard $(addprefix $(VADSRC_DIR)/*,$(SRCEXTS)))
VAD_OBJECTS = $(addsuffix .o, $(basename $(addprefix $(OBJ_DIR)/,$(notdir $(VAD_SOURCES)))))
HEADERS :=   $(COMM_HEADERS) #$(VAD_HEADERS)
SOURCES :=   $(COMM_SOURCES) #$(VAD_SOURCES)
OBJECTS :=   $(COMM_OBJECTS) #$(VAD_OBJECTS)

INCDIRS := -I. -I$(HEADER_DIR) -I$(DRV_RULES_DIR) -I$(COMMON_DIR)

CFLAGS   += $(INCDIRS)
CXXFLAGS += $(INCDIRS)

all: $(LIB_NAME)
$(LIB_NAME) : $(MKDIRS) $(OBJECTS)
	$(AR) $(ARFLAGS) $(LIB_NAME) $(OBJECTS)

objs:$(MKDIRS) $(OBJECTS)

$(OBJECTS):
%.o:%.c
	$(CC)  $(CFLAGS)   $(CPPFLAGS) -c $< -o $@
%.o:%.cc
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

$(MKDIRS):
	test -d $@ || mkdir -p $@;cp $(SOURCES) $@

install:
	rm -rf $(addprefix $(OBJ_DIR)/*,$(SRCEXTS))
	if test ! -d $(HEADER_DIR);then mkdir -p $(HEADER_DIR);fi
	if test ! -d $(RELEASE_DIR);then mkdir -p $(RELEASE_DIR);fi
	cp -f $(VADSRC_DIR)/include/webrtc_vad.h  $(HEADER_DIR)
	cp -f $(LIB_NAME) $(RELEASE_DIR)

clean:
	rm -rf $(OBJ_DIR) *.a

distclean: clean
	rm -f *.o
	

# Show variables (for debug use only.)  
show :
	@echo 'src            :' $(src)
	@echo 'uti_src        :' $(uti_src)
	@echo 'objects        :' $(objects)
