DRV_RULES_DIR =../../..
include $(DRV_RULES_DIR)/Rules.make

NAME = $(notdir $(CURDIR))
DIR = $(dir $(CURDIR))

LIB_NAME = libWebRtcNS.a

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

LDFLAGS = -lpthread -pthread

HEADERS = $(wildcard *.h)
src     := $(wildcard *.c)
objects := $(patsubst %.c,%.o,$(src))
executables := $(patsubst %.c,%,$(src))

INCDIRS := -I. -I$(COMPILE_INCLUDE) -I$(HEADER_DIR) -I$(DRV_RULES_DIR) -I$(DRV_RULES_DIR)/common

CFLAGS += $(INCDIRS)

all: LIB_NAME
LIB_NAME : $(objects)
	$(AR) $(ARFLAGS) $(LIB_NAME) $(objects)

$(objects): 
%.o: %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

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