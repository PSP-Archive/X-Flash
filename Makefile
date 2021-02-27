PSPSDK                 = $(shell psp-config --pspsdk-path)
PSPDIR                 = $(shell psp-config --psp-prefix)

INCDIR                 := ./include
CFLAGS                 = -G0 -Wall -O2 -fno-strict-aliasing
LIBS                   = -lmikmod -lpspaudiolib -lpspaudio -lpsppower -lz -lm -lpsphprm -lpspusb -lpsprtc -lpspnand_driver -lpspidstorage

OBJS                   = main.o

TARGET                 = XFLASH
EXTRA_TARGETS          = EBOOT.PBP kxploit
PSP_EBOOT_TITLE        = XFLASH
EXTRA_CLEAN            = clean_kxploit
include $(PSPSDK)/lib/build.mak

clean_kxploit:
	rm -rf $(TARGET)
	rm -rf "$(TARGET)%"