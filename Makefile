UTIL_CC=gcc
UTIL_CFLAGS=-c -Wall
UTIL_LDFLAGS=-static
UTIL_SRC=klog.c
UTIL_OBJ=$(UTIL_SRC:.c=.o)
UTIL=klog

MOD_OBJ=kmsg_log.o
obj-m += $(MOD_OBJ)

all: $(UTIL)
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	rm -fr $(UTIL) $(UTIL_OBJ)
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

$(UTIL): $(UTIL_OBJ)
	$(UTIL_CC) $(UTIL_LDFLAGS) $(UTIL_OBJ) -o $@

.c.o:
	$(UTIL_CC) $(UTIL_CFLAGS) $< -o $@
