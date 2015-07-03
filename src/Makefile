# autoconf generated makefile configuration
include ../make.local

PROTOC			= $(PB_BIN)/protoc
CXX	            = g++
CXXFLAGS        = -g -Wall
INCLUDE         = -I$(UTIL) -I$(EXTERNAL) -isystem $(PB_INC) -I.
LIBVERSION      = 0.1.0
LDFLAGS         = -lpthread

ifndef DEBUG
CXXFLAGS       += -O2
endif
TARG            = libtrpc.so
TARG2           = libtrpc.a
V               = @

.DEFAULT_GOAL := all

all: $(TARG) $(TARG2)

PROTO =\
    proto/comm.proto

PROTO_CC = $(PROTO:.proto=.pb.cc)
PROTO_H = $(PROTO:.proto=.pb.h)
PROTO_O = $(PROTO_CC:.cc=.o)

LIB_CC =\
    connection.cc\
    rpc_channel.cc\
    rpc_controller.cc\
    debug.cc
LIB_O = $(LIB_CC:.cc=.o)

DEPS =\
        $(LIB_CC) \
        $(PROTO_CC)

%.o:%.cc
	@echo + cc $<
	$(V)$(CXX) -c $(CXXFLAGS) $(INCLUDE) -o $@ $<

$(PROTO_CC) $(PROTO_H): $(PROTO)
	@echo + generating protocol buffers
	$(V)$(PROTOC) --cpp_out=. --java_out=./java/ $^

proto/%.o:proto/%.cc
	@echo + cc $<
	$(V)$(CXX) -c $(CXXFLAGS) $(INCLUDE) -o $@ $<

libtrpc.so: CXXFLAGS += -fPIC 
libtrpc.so: $(LIB_O) $(PROTO_O)
	@echo + ld $@
	$(V)$(CXX) $(CXXFLAGS) $(LDFLAGS) -fPIC -shared \
        -Wl,-soname,libtrpc.so.${LIBVERSION} \
        -o $@ $^ -L$(PB_LIB) -Wl,-Bdynamic -lprotobuf -Wl,-Bdynamic
	$(V)if [ ! -e $@.${LIBVERSION} ] ; then ln -s $@ $@.${LIBVERSION} ; fi

libtrpc.a: $(LIB_O) $(PROTO_O)
	@echo + ar $@
	$(V)$(AR) crv $@ $^

-include depend
depend: $(DEPS) Makefile
	$(V)gcc $(INCLUDE) $(DYNCXXFLAGS) -MM $(DEPS) > depend

clean:
	$(V)rm -f core core.* *.core *.o $(TARG) $(TARG).* $(TARG2) $(PROTO_O) depend 

distclean: clean
	$(V)rm -f $(PROTO_CC) $(PROTO_H) 
