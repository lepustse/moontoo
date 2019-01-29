###############################################################################
# 配置项
###############################################################################
export MARCH := armv7-a
export TARGET := start.elf
export VFLAG := 0

# PLAT
export PLAT := qemu

ifeq ($(VFLAG), 0)
V := @
else
V := 
endif
export DELETE := rm -f
export RMDIR  := rm -rf
export MKDIR  := mkdir
export DEBUG  := -g

export CCPREFIX := arm-none-eabi-

export CC := $(CCPREFIX)gcc
export AR := $(CCPREFIX)ar 
export LD := $(CCPREFIX)ld
export CP := ${CCPREFIX}objcopy
export DP := ${CCPREFIX}objdump
export NM := ${CCPREFIX}nm

#define
USE_COHERENT_MEM := 1
#------------------------------------------------------------------------------
# path
#------------------------------------------------------------------------------
TOP := $(shell pwd)
OUT_DIR := $(TOP)/out
DUMP_DIR := $(TOP)/dump

#CCFLAGS := -O0 -fno-strict-aliasing -fno-common -mfpu=vfpv3 -fno-builtin
CCFLAGS := -g -fno-strict-aliasing -fno-common -msoft-float -fno-builtin
CCFLAGS += -ffreestanding -nostdinc -isystem -pipe -marm  -mabi=aapcs-linux -std=gnu99
CCFLAGS += -mno-thumb-interwork -Wall -Wstrict-prototypes -fno-stack-protector
CCFLAGS += -march=$(MARCH) -D_SYS_SUPPORT_

ASFLAGS := -g -fno-strict-aliasing -fno-common -msoft-float -fno-builtin
#ASFLAGS := -Os -fno-strict-aliasing -fno-common -mfpu=vfpv3 -fno-builtin
ASFLAGS += -ffreestanding -nostdinc -isystem -pipe -marm  -mabi=aapcs-linux 
ASFLAGS += -mno-thumb-interwork -Wall 
ASFLAGS += -march=$(MARCH)
ASFLAGS += -D__KERNEL__ -D__ASSEMBLY__ -D__ARM__ -DCONFIG_ARM

LDFLAGS := -static -Tlink.ld -nostartfiles

ifeq (${USE_COHERENT_MEM}, 1)
CCFLAGS += -DUSE_COHERENT_MEM=1
ASFLAGS += -DUSE_COHERENT_MEM=1
endif

TOS_ENTRY=0x70000000
CCFLAGS += -DTOS_ENTRY=$(TOS_ENTRY)
ASFLAGS += -DTOS_ENTRY=$(TOS_ENTRY)

#save fpu
CCFLAGS += -DSAVE_FPU
ASFLAGS += -DSAVE_FPU

###############################################################################
#  头文件
###############################################################################
INC_DIR := $(TOP)
CCFLAGS += -I$(INC_DIR)
ASFLAGS += -I$(INC_DIR)
CCFLAGS += -I$(INC_DIR)/include
ASFLAGS += -I$(INC_DIR)/include

###############################################################################
#  源文件
###############################################################################
# monitor
SRC_DIR := $(TOP)/monitor
SOBJ := tz_init.o
PSOBJ := $(addprefix $(OUT_DIR)/, $(SOBJ))
VPATH := $(SRC_DIR)

# COMMON
SRC_DIR := $(TOP)/common/printf
SOBJ := console.o vsprintf.o
PSOBJ += $(addprefix $(OUT_DIR)/, $(SOBJ))
VPATH += $(SRC_DIR)
CCFLAGS += -I$(SRC_DIR)/include

# PLAT
SRC_DIR := $(TOP)/plat/$(PLAT)
SOBJ := qemu_uart.o qemu.o
PSOBJ += $(addprefix $(OUT_DIR)/, $(SOBJ))
VPATH += $(SRC_DIR)
CCFLAGS += -I$(SRC_DIR)/include
ASFLAGS += -I$(SRC_DIR)/include

#------------------------------------------------------------------------------
##############################################################################
# 链接库
##############################################################################
#SHARE_LIB := $(TOP)/../lib/bin/share_lib.a
##############################################################################
# 目标文件
##############################################################################
.PHONY: all
all:$(TARGET) $(DUMP_DIR)
	$(V)echo " (gen) $(TARGET).dump"
	$(V)$(DP) -d $(TARGET) >$(DUMP_DIR)/$(TARGET).dump
	$(V)echo " (gen) $(TARGET).sym"
	$(V)$(NM) $(TARGET) | sort >$(DUMP_DIR)/$(TARGET).sym
	$(V)echo " (gen) $(TARGET).map"
	$(V)$(DP) -h $(TARGET) >$(DUMP_DIR)/$(TARGET).map

$(DUMP_DIR):
	$(V)mkdir -p $@
	
$(TARGET):$(OUT_DIR) $(SHARE_LIB) $(POBJ) $(PSOBJ) $(TOP)/plat/$(PLAT)/link.ld Makefile
	$(if $(V), $(V)echo " (LD) $@")
	$(V)$(CC) -nostdlib -T $(TOP)/plat/$(PLAT)/link.ld -o $@ $(POBJ) $(PSOBJ) $(SHARE_LIB) -lgcc

-include $(DEPENDENCY_FILE)
$(DEPENDENCY_FILE): $(deps-y)
	@echo "where is dep file"
	$(V)cat $(deps-y) > $(DEPENDENCY_FILE)

$(OUT_DIR)/%.dep:%.c
#	@echo "gen dep file"
	$(V)mkdir -p `dirname $@`
	$(if $(V), @echo " (cc-dep)    $(subst $(OUT_DIR)/,,$@)")
	$(V)echo -n `dirname $@`/ > $@
	$(V)$(CC) $(CCFLAGS) -I`dirname $<` -MM $< >> $@

$(OUT_DIR)/%.dep:%.S
#	@echo "gen dep file"
	$(V)mkdir -p `dirname $@`
	$(if $(V), @echo " (as-dep)    $(subst $(OUT_DIR)/,,$@)")
	$(V)echo -n `dirname $@`/ > $@
	$(V)$(CC) $(ASFLAGS) -I`dirname $<` -MM $< >> $@
		
$(OUT_DIR)/%.o:%.c
	$(if $(V), @echo " (cc)    $(subst $(OUT_DIR)/,,$@)")
	$(V)$(CC) $(CCFLAGS) -c $< -o $@
	
$(OUT_DIR)/%.o:%.S
	$(if $(V), @echo " (as)    $(subst $(OUT_DIR)/,,$@)")
	$(V)$(CC) $(ASFLAGS) -c $< -o $@
	
$(OUT_DIR):
	$(if $(V), $(V)echo "$(MKDIR) $@")
	-$(V)$(MKDIR) $@
				
clean:
	-$(RMDIR) $(OUT_DIR)
	-$(RMDIR) $(DUMP_DIR)
	-${DELETE} ${TARGET}
