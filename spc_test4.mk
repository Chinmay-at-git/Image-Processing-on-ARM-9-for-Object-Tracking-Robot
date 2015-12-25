#############################################################
#
# builds and installs spc_test4 to the TARGET_DIR
# for inclusion into the rootfs
#
# This is an example of how to add your own program
# to the build system to make a complete rootfs with
# your own binaries in conjunction with the standard
# Linux system and libraries already built and installed
# so you dont have to re-invent the wheel
#############################################################
SPJDIR_spc_test4:=$(BASE_DIR)/packages/spj/dummy/spc_test4

$(SPJDIR_spc_test4)/spc_test4:
	$(MAKE)	CC=$(TARGET_CC) \
	CFLAGS="-I$(HOST_DIR)/include" \
	LDFLAGS="-L$(HOST_DIR)/lib -lz" \
	-C $(SPJDIR_spc_test4)
	
$(TARGET_DIR)/usr/bin/spc_test4: $(SPJDIR_spc_test4)/spc_test4  	# now spc_test4 bin exists, so we dont build again
	mkdir -p $(TARGET_DIR)/usr/bin			# make the install dir if not exists
	cp $(SPJDIR_spc_test4)/spc_test4 $(TARGET_DIR)/usr/bin  	# copy to the rootfs
	$(TARGET_STRIP) $(TARGET_DIR)/usr/bin/spc_test4  	# shrinks the file size (no Debug syms)
	

# If you need this binary to be used by other apps
# you need to put it in the HOST_DIR/bin to allow it
# to be called by the system when building something else.

$(HOST_DIR)/bin/spc_test4: $(SPJDIR_spc_test4)/spc_test4	
	mkdir -p $(HOST_DIR)/bin			# make the host/bin dir if not exists
	cp $(SPJDIR_spc_test4)/spc_test4 $(HOST_DIR)/bin		# copy spc_test4 to the host/bin dir so
							# other programs can use it.
	cp $(SPJDIR_spc_test4)/main.h $(HOST_DIR)/include	# copy header files to host/include so
							# other programs can use it.
	cp $(SPJDIR_spc_test4)/main.so	$(HOST_DIR)/lib		# copy shared library to host/lib so
							# other programs can link it

spc_test4: zlib-example $(TARGET_DIR)/usr/bin/spc_test4	# spc_test4 needs zlib.h and libz.so
							# to build, so we call zlib to make
							# sure that it is built first

spc_test4-clean:
	-$(MAKE) -C $(SPJDIR_spc_test4) clean		   
	-@rm -f $(TARGET_DIR)/usr/bin/spc_test4 

#############################################################
#
# Toplevel Makefile options
#
#############################################################
ifeq ($(strip $(PACKAGE_spc_test4)),y)       # grab spc_test4 from Config.in
TARGETS+=spc_test4				 # This is to include spc_test4 into the toplevel makefile
endif
