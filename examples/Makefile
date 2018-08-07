#SOURCES  = interface.cpp socket.cpp udp.cpp multicasting.cpp main.cpp mDNS.cpp debugUtility.c
#OBJECTS  = $(addsuffix .o, $(basename $(SOURCES)))
#DEPS     = $(addsuffix .d, $(basename $(SOURCES)))
LDFLAGS  = 
CPPFLAGS = -g -Wall -fpermissive -std=c++11
CFLAGS   = -g -Wall

TCP_LIB_SRCS       = ../tcpNetwork.cpp
TCP_LIB_OBJS       = $(addsuffix .o, $(basename $(TCP_LIB_SRCS)))
TCP_LIB_DEPS       = $(addsuffix .d, $(basename $(TCP_LIB_SRCS)))

TCP_SERVER_TARGET  = tcpSrv2
TCP_SERVER_SRCS    = tcpSrv2.cpp 
TCP_SERVER_OBJS    = $(addsuffix .o, $(basename $(TCP_SERVER_SRCS)))
TCP_SERVER_DEPS    = $(addsuffix .d, $(basename $(TCP_SERVER_SRCS)))

TCP_CLIENT_TARGET  = tcpCli2
TCP_CLIENT_SRCS    = tcpCli2.cpp 
TCP_CLIENT_OBJS    = $(addsuffix .o, $(basename $(TCP_CLIENT_SRCS)))
TCP_CLIENT_DEPS    = $(addsuffix .d, $(basename $(TCP_CLIENT_SRCS)))


all: TCP_CLIENT TCP_SERVER

TCP_SERVER: $(TCP_LIB_OBJS) $(TCP_SERVER_OBJS) 
	g++ -o $(TCP_SERVER_TARGET) $(TCP_SERVER_OBJS) $(TCP_LIB_OBJS) $(LDFLAGS)

TCP_CLIENT: $(TCP_LIB_OBJS) $(TCP_CLIENT_OBJS) 
	g++ -o $(TCP_CLIENT_TARGET) $(TCP_CLIENT_OBJS) $(TCP_LIB_OBJS) $(LDFLAGS)


# create dependency file (.d)
#%.d: %.c
#	gcc $(CFLAGS) -MM $< | sed 's/$*.o/& $@/g' > $@

# create dependency file (.d)
#%.d: %.cpp
#	g++ $(CPPFLAGS) -MM $< | sed 's/$*.o/& $@/g' > $@ 



$(TCP_LIB_OBJS) : $(TCP_LIB_SRCS) 
	g++ $(CPPFLAGS) -c $< -o $@

$(TCP_SERVER_OBJS) : $(TCP_SERVER_SRCS)
	g++ $(CPPFLAGS) -c $< -o $@

$(TCP_CLIENT_OBJS) : $(TCP_CLIENT_SRCS)
	g++ $(CPPFLAGS) -c $< -o $@

#include dependency
#-include $(TCP_LIB_DEPS)
#-include $(TCP_SERVER_DEPS)
#-include $(TCP_CLIENT_DEPS)


$(TCP_LIB_DEPS) : $(TCP_LIB_SRCS) 
	g++ $(CPPFLAGS) -MM $< | sed 's/$*.o/& $@/g' > $@

$(TCP_SERVER_DEPS) : $(TCP_SERVER_SRCS) 
	g++ $(CPPFLAGS) -MM $< | sed 's/$*.o/& $@/g' > $@

$(TCP_CLIENT_DEPS) : $(TCP_CLIENT_SRCS) 
	g++ $(CPPFLAGS) -MM $< | sed 's/$*.o/& $@/g' > $@


#%.o : $(TCP_LIB_SRC) $(TCP_SERVER_SRCS) $(TCP_CLIENT_SRCS)
#	g++ $(CPPFLAGS) -c $< -o $@
#%.o : %.cpp
#	g++ $(CPPFLAGS) -c $< -o $@
#%.o : %.c
#	gcc $(CFLAGS) -c $< -o $@

clean:
	rm -f *.d
	rm -f $(TCP_LIB_OBJS)
	rm -f $(TCP_SERVER_OBJS) 
	rm -f $(TCP_SERVER_TARGET)
	rm -f $(TCP_CLIENT_OBJS) 
	rm -f $(TCP_CLIENT_TARGET)
