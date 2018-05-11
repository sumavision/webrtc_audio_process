DRV_RULES_DIR =../../../..
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
CFLAGS  = -g -O2  -DWEBRTC_POSIX

LDFLAGS = -lpthread -pthread

HEADERS = $(wildcard *.h)
src     := $(wildcard *.c)
objects := $(patsubst %.c,%.o,$(src))
executables := $(patsubst %.c,%,$(src))

INCDIRS := -I. -I$(COMPILE_INCLUDE) -I$(HEADER_DIR) -I$(CURDIR)/.. -I$(DRV_RULES_DIR)/common

CFLAGS  += $(INCDIRS)
LDFLAGS += -L$(RELEASE_DIR) -lWebRtcNS -lWRtcACommon

all: $(TARGET)
$(TARGET) : $(objects)
	$(CXX) $^ -o $@ $(LDFLAGS)

$(objects): 
%.o: %.c
	$(CXX) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

install:
	cp *.h  $(HEADER_DIR)
	cp $(TARGET) $(RELEASE_DIR)
#	install -d $(EXEC_DIR)
#	install $(executables) $(EXEC_DIR)

clean:
	rm -f *.o *.a
	@for dir in $(executables); do \
		rm -f $(EXE_DIR)/$$dir; \
	done

distclean: clean
	rm -f *.o