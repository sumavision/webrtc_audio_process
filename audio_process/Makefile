NAME = $(notdir $(CURDIR))
APM_DIR = $(CURDIR)/modules/audio_processing
RELEASE_DIR = ../../../release
HEADER_DIR = ../../../include

.PHONY:  install all all_clean aecm agc ns vad clean help

all :
	$(MAKE) -fMakefile -s -C $(APM_DIR)/aecm
	$(MAKE) -fMakefile -s -C $(APM_DIR)/agc
	$(MAKE) -fMakefile -s -C $(APM_DIR)/ns
	$(MAKE) -fMakefile -s -C $(APM_DIR)/vad

aecm :
	$(MAKE) -fMakefile -s -C $(APM_DIR)/aecm

agc :
	$(MAKE) -fMakefile -s -C $(APM_DIR)/agc
	
ns :
	$(MAKE) -fMakefile -s -C $(APM_DIR)/ns
	
vad :
	$(MAKE) -fMakefile -s -C $(APM_DIR)/vad

all_clean:
clean :
	$(MAKE) -fMakefile -s -C $(APM_DIR)/aecm clean
	$(MAKE) -fMakefile -s -C $(APM_DIR)/agc clean
	$(MAKE) -fMakefile -s -C $(APM_DIR)/ns clean
	$(MAKE) -fMakefile -s -C $(APM_DIR)/vad clean
	rm -rf $(CURDIR)/include $(CURDIR)/release
	
install :
	$(MAKE) -fMakefile -s -C $(APM_DIR)/aecm install
	$(MAKE) -fMakefile -s -C $(APM_DIR)/agc install
	$(MAKE) -fMakefile -s -C $(APM_DIR)/ns install
	$(MAKE) -fMakefile -s -C $(APM_DIR)/vad install
	cp -f $(CURDIR)/include/*.h $(HEADER_DIR)
	cp -f $(CURDIR)/release/*.a $(RELEASE_DIR)
	cp -f $(CURDIR)/common/typedefs.h     $(HEADER_DIR)
	

# Show variables (for debug use only.)  
show :
	@echo 'HEADERS        :' $(HEADERS)
	@echo 'SOURCES        :' $(SOURCES)
	@echo 'OBJECTS        :' $(OBJECTS)

################################################
help:
	@echo "webrtc audio process module make help"
	@echo 
	@echo "Available build targets are :"
	@echo "aecm       : Build acoustic echo cancellation for mobile "
	@echo "agc        : Build automatic gain control"
	@echo "ns         : Build noise suppression"
	@echo "vad        : Build voice activity detection"
	@echo "install    : install libs"
	@echo "clean      : Clean all packages"
	@echo "all        : Rebuild all package"
	@echo "all_clean  : Clean all package"
	@echo 

################################################	