# autoconf generated makefile configuration
include ../make.local

PROTOC			= $(PB_BIN)/protoc
CXX	            = g++
CXXFLAGS        = -g -Wall
INCLUDE         = -I$(UTIL) -I$(EXTERNAL) -isystem $(PB_INC) -I../src -I.
LDFLAGS         = -lpthread -lprotobuf -L../src -Wl,-Bstatic -ltrpc -Wl,-Bdynamic

ifndef DEBUG
CXXFLAGS       += -O2
endif
TARG            = test_client test_server
V               = @

.DEFAULT_GOAL := all

all: $(TARG)

PROTO =\
    proto/hello.proto

PROTO_CC = $(PROTO:.proto=.pb.cc)
PROTO_H = $(PROTO:.proto=.pb.h)
PROTO_O = $(PROTO_CC:.cc=.o)

DEPS =\
        $(PROTO_CC)\
        test_client.cc\
        test_server.cc\
        hello_service.cc

%.o:%.cc
	@echo + cc $<
	$(V)$(CXX) -c $(CXXFLAGS) $(INCLUDE) -o $@ $<

$(PROTO_CC) $(PROTO_H): $(PROTO)
	@echo + generating protocol buffers
	$(V)$(PROTOC) --cpp_out=. $^

proto/%.o:proto/%.cc
	@echo + cc $<
	$(V)$(CXX) -c $(CXXFLAGS) $(INCLUDE) -o $@ $<

test_client: test_client.o $(PROTO_O)
	$(V)$(CXX) $(CXXFLAGS) $(INCLUDE) -o $@ $^ $(LDFLAGS)

test_server: test_server.o hello_service.o $(PROTO_O)
	$(V)$(CXX) $(CXXFLAGS) $(INCLUDE) -o $@ $^ $(LDFLAGS)

-include depend
depend: $(DEPS) Makefile
	$(V)gcc $(INCLUDE) $(DYNCXXFLAGS) -MM $(DEPS) > depend

clean:
	rm -f core core.* *.core *.o $(TARG) $(PROTO_O) depend 

distclean: clean
	rm -f $(PROTO_CC) $(PROTO_H) 
