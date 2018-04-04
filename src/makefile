include makefile.define

default:all

PATS1 = ListenEvent.o IotTimer.o ClientManager.o

LIB_BASE_OBJS1 = ListenEvent.o IotTimer.o ClientManager.o

LIB_BASE1 = Iot_Timer_Server

all:$(LIB_BASE1) makefile

$(LIB_BASE1):$(PATS1)
	$(CC) -rdynamic -o $(LIB_BASE1) $(LIB_BASE_OBJS1) $(LIBS)

clean:
	rm -rf *.o  $(LIB_BASE1) $(LIB_BASE_OBJS1)
cl:
	rm -rf *.o 
