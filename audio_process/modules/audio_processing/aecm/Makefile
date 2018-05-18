DRV_RULES_DIR = $(CURDIR)/../../..
COMMON_DIR =$(DRV_RULES_DIR)/common
UTI_DIR = $(CURDIR)/../utility
OBJ_DIR = $(CURDIR)/.objs

include $(DRV_RULES_DIR)/Rules.make
include $(COMMON_DIR)/common.mak
include $(UTI_DIR)/apm_uti.mak

RELEASE_DIR = $(DRV_RULES_DIR)/release
HEADER_DIR = $(DRV_RULES_DIR)/include
MKDIRS = $(OBJ_DIR)

NAME = $(notdir $(CURDIR))
DIR = $(dir $(CURDIR))

LIB_NAME = libwebrtcaecm.a

# archiver and its options
ARFLAGS = -r

# The pre-processor options used by the cpp (man cpp for more).  
CPPFLAGS  = -Wall -Werror 

# The pre-processor and compiler options.   
CFLAGS  = -g -O2               -DWEBRTC_POSIX -DWEBRTC_LINUX
CXXFLAGS= -g -O2  -std=gnu++11 -DWEBRTC_POSIX -DWEBRTC_LINUX

#libs 
LDFLAGS = -lpthread -pthread

AECM_HEADERS = $(wildcard $(addprefix *,$(HDREXTS)))
AECM_SOURCES = $(wildcard $(addprefix *,$(SRCEXTS)))
AECM_OBJECTS = $(addsuffix .o, $(basename $(addprefix $(OBJ_DIR)/,$(notdir $(AECM_SOURCES)))))
HEADERS :=  $(COMM_HEADERS) $(UTI_HEADERS) $(AECM_HEADERS)
SOURCES :=   $(COMM_SOURCES) $(UTI_SOURCES) $(AECM_SOURCES)
OBJECTS :=   $(COMM_OBJECTS) $(UTI_OBJECTS) $(AECM_OBJECTS)

INCDIRS := -I. -I$(HEADER_DIR) -I$(DRV_RULES_DIR) -I$(COMMON_DIR) -I$(UTI_DIR) 

CFLAGS   += $(INCDIRS)
CXXFLAGS += $(INCDIRS)

all:$(LIB_NAME) 
$(LIB_NAME): $(MKDIRS) $(OBJECTS)
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
	cp $(CURDIR)/echo_control_mobile.h  $(HEADER_DIR)
	cp $(LIB_NAME) $(RELEASE_DIR)

clean:
	rm -rf $(OBJ_DIR) *.a

distclean: clean
	rm -f *.o
	

# Show variables (for debug use only.)  
show :
	@echo 'HEADERS        :' $(HEADERS)
	@echo 'SOURCES        :' $(SOURCES)
	@echo 'OBJECTS        :' $(OBJECTS)
