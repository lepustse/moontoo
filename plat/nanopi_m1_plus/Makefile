#====================================================================
# File		 : Makefile
# Change Logs:
# Date			Author			Notes
# 2019.3.18		never			the first version
#====================================================================

#--------------------------------------------------------------------
# baremetal: y
#--------------------------------------------------------------------
Baremetal := y

#--------------------------------------------------------------------
# target
#--------------------------------------------------------------------
ifeq ($(Baremetal), y)
TARGET := start.elf
MCPU := cortex-a7
else
TARGET := demo
CCFLAGS := -O2
endif

#--------------------------------------------------------------------
# compile option
#--------------------------------------------------------------------

ifeq ($(Baremetal), y)
export CROSS_COMPILE ?= arm-none-eabi-

DEVICE := -O2 -fno-strict-aliasing -fno-common -mfpu=vfpv3 -fno-builtin\
	-ffreestanding -nostdinc -isystem -pipe -marm  -mabi=aapcs-linux\
   	-std=gnu99 -mno-thumb-interwork -Wall -mcpu=$(MCPU)

export CCFLAGS := $(DEVICE)
export CCFLAGS += -Wstrict-prototypes -fno-stack-protector -D_SYS_SUPPORT_

export ASFLAGS := $(DEVICE)
export ASFLAGS += -D__KERNEL__ -D__ASSEMBLY__ -D__ARM__ -DCONFIG_ARM -mcpu=$(MCPU)

export LDFLAGS := -static -nostartfiles -Tlink.ld -nostdlib
# 以下是cm4的编译参数
#DEVICE  += -mthumb -mfpu=fpv4-sp-d16 \
#	-mfloat-abi=hard -ffunction-sections -fdata-sections
#
#export CCFLAGS := -O2
#CCFLAGS += $(DEVICE)
#CCFLAGS += -g -Wall -DSTM32F429xx -D__ASSEMBLY__
#
#export ASFLAGS := -c
#ASFLAGS += $(DEVICE)
#ASFLAGS += -x assembler-with-cpp -Wa,-mimplicit-it=thumb
#
#export LDFLAGS := $(DEVICE)
##LDFLAGS += -lm -lgcc -lc
#LDFLAGS += -nostdlib -nostartfiles -Wl,--gc-sections,-u,Reset_Handler
#LDFLAGS += -T stm32_rom.ld

else
export CROSS_COMPILE ?=
export CCFLAGS := -O2
endif

export CC := $(CROSS_COMPILE)gcc
export AR := $(CROSS_COMPILE)ar
export LD := $(CROSS_COMPILE)ld
export CP := ${CROSS_COMPILE}objcopy
export DP := ${CROSS_COMPILE}objdump
export NM := ${CROSS_COMPILE}nm

#--------------------------------------------------------------------
# echo option
#--------------------------------------------------------------------
ifeq ("$(origin V)", "command line")
KBUILD_VERBOSE = $(V)
endif
ifndef KBUILD_VERBOSE
KBUILD_VERBOSE = 0
endif
ifeq ($(KBUILD_VERBOSE),1)
Q =
MAKE_SILENT =
else
Q = @
MAKE_SILENT = -s
endif
export Q KBUILD_VERBOSE MAKE_SILENT

#--------------------------------------------------------------------
# output file
#--------------------------------------------------------------------
O ?= out
export O

D ?= dump

srcdir := $(CURDIR)

ifeq ($(O),)
objdir := $(srcdir)
rm-dir :=
else
objdir := $(O)
rm-dir := $(objdir)
endif

ifeq ($(D),)
dumpdir := $(D)
rm-dir +=
else
dumpdir := $(D)
rm-dir += $(dumpdir)
endif

#--------------------------------------------------------------------
# make all
#--------------------------------------------------------------------
target_deps := $(objdir)/built-in.o
OLDTARGET := $(TARGET)
DUMPTARGET := $(dumpdir)/$(OLDTARGET)
TARGET := $(objdir)/$(TARGET)

all: $(TARGET)
	$(Q)echo "GEN $(DUMPTARGET).dump"
	$(Q)$(DP) -d $(TARGET) >$(DUMPTARGET).dump
	$(Q)echo "GEN $(DUMPTARGET).sym"
	$(Q)$(NM) $(TARGET) | sort >$(DUMPTARGET).sym
	$(Q)echo "GEN $(DUMPTARGET).map"
	$(Q)$(DP) -h $(TARGET) >$(DUMPTARGET).map

ifeq ($(Baremetal), y)
$(TARGET): $(target_deps) | $(objdir) $(dumpdir)
	$(Q)echo "GEN $(TARGET)"
	$(Q)$(CC) $(LDFLAGS) $(target_deps) -o $@
	$(Q)echo "GEN $(TARGET).bin"
	$(Q)$(CP) -O binary $(TARGET) $(TARGET).bin
else
$(TARGET): $(target_deps) | $(objdir) $(dumpdir)
	$(Q)echo "GEN $(TARGET)"
	$(Q)$(CROSS_COMPILE)gcc $(target_deps) -o $@
endif

ifneq ($(O),)
$(objdir):
	$(Q)if [ ! -d $@ ]; then mkdir -p $(objdir); fi
endif

ifneq ($(D),)
$(dumpdir):
	$(Q)if [ ! -d $@ ]; then mkdir -p $(dumpdir); fi
endif

$(sort $(target_deps)): $(srcdir);
$(srcdir):
	$(Q)make $(MAKE_SILENT) -f Makefile.build obj=$(srcdir)

clean:
	$(Q)echo "CLEAN"
	$(Q)set -e
	$(Q)make $(MAKE_SILENT) -f Makefile.build obj=$(srcdir) clean
	$(Q)rm -rf $(TARGET) $(rm-dir)

.PHONY: all clean FORCE $(srcdir) $(dumpdir)
