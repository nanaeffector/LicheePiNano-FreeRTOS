#
# Machine makefile
#

BUILDDIR = build
	
# toolchain
TOOL         = $(TOOLCHAIN)arm-none-eabi-
CC           = $(TOOL)gcc
CPP          = $(TOOL)gcc
DP           = $(TOOL)objdump
CP           = $(TOOL)objcopy
SZ           = $(TOOL)size
AS           = $(TOOL)gcc -c -x assembler-with-cpp
LD           = $(TOOL)ld

# user specific
SRCS += \
	$(wildcard ../lib/*.c) \
	$(wildcard ../lib/driver/*.c) \
	$(wildcard ../lib/freertos/*.c) \
	$(wildcard ../lib/freertos/portable/GCC/ARM926EJ-S/*.c) \
	$(wildcard ../lib/freertos/portable/MemMang/*.c) \

SRCS += \
	$(wildcard ./src/*.c) \
	$(wildcard ./src/*.S) \
	$(wildcard ./src/interrupt/*.c) \
	$(wildcard ./src/interrupt/*.S) \
	$(wildcard ./src/startup/*.c) \
	$(wildcard ./src/startup/*.S) 


# include
# LIBS 		= -lgcc -lm -lc_nano -lnosys
LIBS 		= -lm -lgcc -lc_nano -lnosys 
INCLUDES	+= -I../lib -I../lib/driver -I../lib/freertos/include -I./src/ -I../lib/freertos/portable/GCC/ARM926EJ-S

ELF = $(BUILDDIR)/$(PROJECT_NAME).elf
BIN = $(BUILDDIR)/$(PROJECT_NAME).bin

# defines
DEFS += -D__ARM32_ARCH__=5 -D__ARM926EJS__

OBJS := $(addprefix $(BUILDDIR)/,$(notdir $(SRCS:.c=.o)))
OBJS := $(addprefix $(BUILDDIR)/,$(notdir $(OBJS:.S=.o)))
vpath %.c $(sort $(dir $(SRCS)))
vpath %.S $(sort $(dir $(SRCS)))

# MC_FLAGS = -march=armv5te -mtune=arm926ej-s -mfloat-abi=soft -marm -mno-thumb-interwork -g -ggdb -Wall -O0 -fdata-sections -ffunction-sections -ffreestanding -std=gnu99
#MC_FLAGS = -march=armv5te -mtune=arm926ej-s -mfloat-abi=soft -marm -mno-thumb-interwork -g -gdwarf-2 -Wall -O2 -ffreestanding -std=gnu99 -nostdlib
MC_FLAGS = -march=armv5te -mtune=arm926ej-s -mfloat-abi=soft -marm -mno-thumb-interwork -g -gdwarf-2 -Wall -O0 -ffreestanding -std=gnu99 -nostdlib

ASFLAGS += $(MC_FLAGS) -Wall $(DEFS)
CFLAGS += $(MC_FLAGS) $(OPT) -Wall $(DEFS)
LDFLAGS += -nostartfiles -Xlinker --gc-sections -T$(LINK_SCRIPT) -nostdlib -Wl,-M -Wl,-Map=$(BUILDDIR)/$(PROJECT_NAME).map,--cref,-gc-sections

#
# makefile rules
#
$(BIN): $(ELF)

	@$(CP) -O binary $< $@
	@echo   ---------------------------------------------------
	@$(SZ) $(ELF)

$(ELF): $(OBJS)
	@echo.
	@echo link: $@
	@echo.
	@$(CPP) $(LDFLAGS) $(OBJS) -o $@ $(LIBS)

$(BUILDDIR)/%.o: %.c
#	mkdir $(dir $@)
	@if not exist $(BUILDDIR) mkdir $(BUILDDIR)
	@echo compile: $<
	@$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

$(BUILDDIR)/%.o: %.S
	@if not exist $(BUILDDIR) mkdir $(BUILDDIR)
	@echo assemble: $<
	@$(AS) $(ASFLAGS) $(INCLUDES) $< -o $@

size:
	@echo   ---------------------------------------------------
	@$(SZ) $(ELF)

clean:
	@del /Q $(BUILDDIR)\*