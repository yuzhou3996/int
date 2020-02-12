CROSS_COMPILE 	?= arm-linux-gnueabihf-
TARGET		  	?= int

CC 				:= $(CROSS_COMPILE)gcc
LD				:= $(CROSS_COMPILE)ld
OBJCOPY 		:= $(CROSS_COMPILE)objcopy
OBJDUMP 		:= $(CROSS_COMPILE)objdump

INCDIRS 		:= imx6ull \
				   bsp/clk \
				   bsp/led \
				   bsp/delay  \
				   bsp/beep \
				   bsp/gpio \
				   bsp/key \
				   bsp/int
				   			   
SRCDIRS			:= project \
				   bsp/clk \
				   bsp/led \
				   bsp/delay \
				   bsp/beep \
				   bsp/gpio \
				   bsp/key \
				   bsp/int
				   
				   
INCLUDE			:= $(patsubst %, -I %, $(INCDIRS))

SFILES			:= $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.S))
CFILES			:= $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.c))

SFILENDIR		:= $(notdir  $(SFILES))
CFILENDIR		:= $(notdir  $(CFILES))

SOBJS			:= $(patsubst %, obj/%, $(SFILENDIR:.S=.o))
COBJS			:= $(patsubst %, obj/%, $(CFILENDIR:.c=.o))
OBJS			:= $(SOBJS) $(COBJS)

VPATH			:= $(SRCDIRS)							#设置全局路径

.PHONY: clean
	
$(TARGET).bin : $(OBJS)
	$(LD) -T imx6ull.lds $^ -o $(TARGET).elf 			#链接文件
	$(OBJCOPY) -O binary -S  $(TARGET).elf $@ 			#生成二进制文件
	$(OBJDUMP) -D -m arm $(TARGET).elf > $(TARGET).dis	#生成反汇编文件

$(SOBJS) : obj/%.o : %.S
	$(CC) -Wall -nostdlib -c -O2  $(INCLUDE) $< -o $@ 	#只编译不链接

$(COBJS) : obj/%.o : %.c
	$(CC) -Wall -nostdlib -c -O2  $(INCLUDE) $< -o $@ 	#只编译不链接
	
clean:
	rm -rf $(TARGET).elf $(TARGET).dis $(TARGET).bin $(COBJS) $(SOBJS)

	
