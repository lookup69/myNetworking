#SOURCES  = interface.cpp socket.cpp udp.cpp multicasting.cpp main.cpp mDNS.cpp debugUtility.c
#OBJECTS  = $(addsuffix .o, $(basename $(SOURCES)))
#DEPS     = $(addsuffix .d, $(basename $(SOURCES)))
LDFLAGS  = 
CPPFLAGS = -g -Wall -fpermissive -std=c++11
CFLAGS   = -g -Wall

CORE_LIB_SRCS       = ../socket.cpp ../tcpNetwork.cpp ../udpNetwork.cpp 
CORE_LIB_OBJS       = $(addsuffix .o, $(basename $(CORE_LIB_SRCS)))
CORE_LIB_DEPS       = $(addsuffix .d, $(basename $(CORE_LIB_SRCS)))

TCP_SERVER_TARGET  = tcpsrv
TCP_SERVER_SRCS    = tcpsrv.cpp 
TCP_SERVER_OBJS    = $(addsuffix .o, $(basename $(TCP_SERVER_SRCS)))
TCP_SERVER_DEPS    = $(addsuffix .d, $(basename $(TCP_SERVER_SRCS)))

TCP_CLIENT_TARGET  = tcpcli
TCP_CLIENT_SRCS    = tcpcli.cpp 
TCP_CLIENT_OBJS    = $(addsuffix .o, $(basename $(TCP_CLIENT_SRCS)))
TCP_CLIENT_DEPS    = $(addsuffix .d, $(basename $(TCP_CLIENT_SRCS)))

UDP_SERVER_TARGET  = udpsrv
UDP_SERVER_SRCS    = udpsrv.cpp 
UDP_SERVER_OBJS    = $(addsuffix .o, $(basename $(TCP_SERVER_SRCS)))
UDP_SERVER_DEPS    = $(addsuffix .d, $(basename $(TCP_SERVER_SRCS)))

UDP_CLIENT_TARGET  = udpcli
UDP_CLIENT_SRCS    = udpcli.cpp 
UDP_CLIENT_OBJS    = $(addsuffix .o, $(basename $(TCP_CLIENT_SRCS)))
UDP_CLIENT_DEPS    = $(addsuffix .d, $(basename $(TCP_CLIENT_SRCS)))



all: TCP_CLIENT TCP_SERVER UDP_CLIENT UDP_SERVER

TCP_SERVER: $(CORE_LIB_OBJS) $(TCP_SERVER_OBJS) 
	g++ -o $(TCP_SERVER_TARGET) $(TCP_SERVER_OBJS) $(CORE_LIB_OBJS) $(LDFLAGS)

TCP_CLIENT: $(CORE_LIB_OBJS) $(TCP_CLIENT_OBJS) 
	g++ -o $(TCP_CLIENT_TARGET) $(TCP_CLIENT_OBJS) $(CORE_LIB_OBJS) $(LDFLAGS)

UDP_SERVER: $(CORE_LIB_SRCS) $(UDP_SERVER_OBJS) 
	g++ -o $(UDP_SERVER_TARGET) $(UDP_SERVER_OBJS) $(CORE_LIB_OBJS) $(LDFLAGS)

UDP_CLIENT: $(CORE_LIB_OBJS) $(UDP_CLIENT_OBJS) 
	g++ -o $(UDP_CLIENT_TARGET) $(UDP_CLIENT_OBJS) $(CORE_LIB_OBJS) $(LDFLAGS)



$(CORE_LIB_OBJS) : $(CORE_LIB_SRCS) 
	g++ $(CPPFLAGS) -c $*.cpp -o $@

$(TCP_SERVER_OBJS) : $(TCP_SERVER_SRCS)
	g++ $(CPPFLAGS) -c $*.cpp -o $@

$(TCP_CLIENT_OBJS) : $(TCP_CLIENT_SRCS)
	g++ $(CPPFLAGS) -c $*.cpp -o $@

$(UDP_SERVER_OBJS) : $(UDP_SERVER_SRCS)
	g++ $(CPPFLAGS) -c $*.cpp -o $@

$(UDP_CLIENT_OBJS) : $(UDP_CLIENT_SRCS)
	g++ $(CPPFLAGS) -c $*.cpp -o $@


#include dependency
#-include $(CORE_LIB_DEPS)
#-include $(TCP_SERVER_DEPS)
#-include $(TCP_CLIENT_DEPS)


$(CORE_LIB_DEPS) : $(CORE_LIB_SRCS) 
	g++ $(CPPFLAGS) -MM $< | sed 's/$*.o/& $@/g' > $@

$(TCP_SERVER_DEPS) : $(TCP_SERVER_SRCS) 
	g++ $(CPPFLAGS) -MM $< | sed 's/$*.o/& $@/g' > $@

$(TCP_CLIENT_DEPS) : $(TCP_CLIENT_SRCS) 
	g++ $(CPPFLAGS) -MM $< | sed 's/$*.o/& $@/g' > $@

$(UDP_SERVER_DEPS) : $(UDP_SERVER_SRCS) 
	g++ $(CPPFLAGS) -MM $< | sed 's/$*.o/& $@/g' > $@

$(UDP_CLIENT_DEPS) : $(UDP_CLIENT_SRCS) 
	g++ $(CPPFLAGS) -MM $< | sed 's/$*.o/& $@/g' > $@


clean:
	rm -f *.d
	rm -f $(CORE_LIB_OBJS)
	rm -f $(TCP_SERVER_OBJS) 
	rm -f $(TCP_SERVER_TARGET)
	rm -f $(TCP_CLIENT_OBJS) 
	rm -f $(TCP_CLIENT_TARGET)
	rm -f $(UDP_SERVER_OBJS) 
	rm -f $(UDP_SERVER_TARGET)
	rm -f $(UDP_CLIENT_OBJS) 
	rm -f $(UDP_CLIENT_TARGET)
