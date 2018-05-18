DRV_RULES_DIR =../../../..
COMMON_DIR =$(DRV_RULES_DIR)/common
include $(DRV_RULES_DIR)/Rules.make

NAME = $(notdir $(CURDIR))
DIR = $(dir $(CURDIR))

TARGET = $(NAME)

RELEASE_DIR = $(DRV_RULES_DIR)/release
HEADER_DIR = $(DRV_RULES_DIR)/include

# The pre-processor options used by the cpp (man cpp for more).  
CPPFLAGS  = -Wall -Werror 

# The pre-processor and compiler options.  
# Users can override those variables from the command line.  
CFLAGS  = -g -O2  -DWEBRTC_POSIX -DWEBRTC_LINUX

LDFLAGS = -lpthread -pthread

HEADERS = $(wildcard *.h)
src     := $(wildcard *.c)
objects := $(patsubst %.c,%.o,$(src))
executables := $(patsubst %.c,%,$(src))

INCDIRS := -I. -I$(DRV_RULES_DIR) -I$(HEADER_DIR) -I$(COMMON_DIR)

CFLAGS  += $(INCDIRS)
LDFLAGS += -L$(RELEASE_DIR) -lwebrtcagc

all: $(TARGET)
$(TARGET) : $(objects)
	$(CXX) $^ -o $@ $(LDFLAGS)

$(objects): 
%.o: %.c
	$(CXX) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

install:
	cp *.h  $(HEADER_DIR)
	#cp $(TARGET) $(RELEASE_DIR)
	
clean:
	rm -f *.o *.a

distclean: clean
	rm -f *.o