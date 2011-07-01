########## The following definition must be set for qOS >= V2.8x #############
CFLAGS=-DNO_TYPEDEF_OS_FLAGS

########## Path for qOS core source files #################################
QOS_SRC=../../../src

########## Path for qOS LINUX port source files ###########################
QOS_PORT_SRC=../../port_src

########## Path for qOS LINUX demo source files ########################
QOS_PORT_DEMO=./

########## Name of Example bin file ########################################
BIN=demo


all: main.c os_cfg.h makefile $(QOS_PORT_SRC)/os_cpu.h $(QOS_PORT_SRC)/os_cpu.c $(QOS_SRC)/qos.c $(QOS_SRC)/qos.h
	@echo --- Recompiling the application ---
	@gcc -g -D_REENTRANT $(CFLAGS) \
		-I$(QOS_SRC) -I$(QOS_PORT_SRC) -I$(QOS_PORT_DEMO) \
		main.c \
		$(QOS_SRC)/qos.c \
		$(QOS_PORT_SRC)/os_cpu.c \
		-lm -lpthread -o $(BIN)

clean:
	@rm -f *.o
	@rm -f *.bak
	@rm -f *~

cleanall:
	@rm -f *.o
	@rm -f *.bak
	@rm -f *~
	@rm -f *.a
	@rm -f $(BIN)
