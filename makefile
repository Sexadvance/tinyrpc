PATH_BIN = bin
PATH_LIB = lib
PATH_OBJ = obj

PATH_ROCKET = rocket
PATH_COMM = $(PATH_ROCKET)/common
PATH_NET = $(PATH_ROCKET)/net

PATH_TESTCASES = testcases

# will install lib to /usr/local/lib/libsocket.a
PATH_INSTALL_LIB_ROOT = /usr/local/lib

#will install all header file to /usr/local/include/socket
PATH_INSTALL_INC_ROOT = /usr/local/include

PATH_INSTALL_INC_COMM  = $(PATH_INSTALL_INC_ROOT)/$(PATH_COMM)
PATH_INSTALL_INC_NET = $(PATH_INSTALL_INC_ROOT)/$(PATH_NET)

#PATH_PROTOBUF = /usr/local/include/google
#PATH_TINYXML = /usr/include/tinyxml

CXX := g++

CXXFLAGES += -g -O0 -std=c++11 -Wall -Wno-deprecated -Wno-unused-but-set-variable

CXXFLAGES += -I ./ -I$(PATH_ROCKET) -I$(PATH_COMM) -I$(PATH_NET)

LIBS  += /usr/local/lib/libprotobuf.a /usr/local/lib/libtinyxml.a

COMM_OBJ := $(patsubst $(PATH_COMM)/%.cc,$(PATH_OBJ)/%.o,$(wildcard $(PATH_COMM)/*.cc)) 
NET_OBJ := $(patsubst $(PATH_NET)/%.cc,$(PATH_OBJ)/%.o,$(wildcard $(PATH_NET)/*.cc))

ALL_TESTS : $(PATH_BIN)/test_log $(PATH_BIN)/test_eventloop

TEST_CASE_OUT := $(PATH_BIN)/test_log $(PATH_BIN)/test_eventloop

LIB_OUT := $(PATH_LIB)/librocket.a

$(PATH_BIN)/test_log:$(LIB_OUT)
	$(CXX) $(CXXFLAGES) $(PATH_TESTCASES)/test_log.cc -o $@ $(LIB_OUT) $(LIBS) -pthread

$(PATH_BIN)/test_eventloop:$(LIB_OUT)
	$(CXX) $(CXXFLAGES) $(PATH_TESTCASES)/test_eventloop.cc -o $@ $(LIB_OUT) $(LIBS) -pthread 

$(LIB_OUT):$(COMM_OBJ) $(NET_OBJ)
	cd $(PATH_OBJ) && ar rcv librocket.a *.o && cp librocket.a ../lib/

$(PATH_OBJ)/%.o:$(PATH_COMM)/%.cc
	$(CXX) $(CXXFLAGES) -c $< -o $@

$(PATH_OBJ)/%.o:$(PATH_NET)/%.cc
	$(CXX) $(CXXFLAGES) -c $< -o $@

# print something test
# like this :make PRINT-PATH_BIN,and then will variable PATH_BIN
PRINT-%: ; @echo $* = $($*)

# to clean
clean:
	rm -f $(COMM_OBJ) $(NET_OBJ) $(TESTCASES) $(TEST_CASE_OUT) $(PATH_LIB)/librocket.a $(PATH_OBJ)/librocket.a

# install
install:
	mkdir -p $(PATH_INSTALL_INC_COMM) $(PATH_INSTALL_INC_NET) \
	&& cp $(PATH_COMM)/*.h $(PATH_INSTALL_INC_COMM) \
	&& cp $(PATH_NET)/*.h $(PATH_INSTALL_INC_NET) \
	&& cp $(LIB_OUT) $(PATH_INSTALL_LIB_ROOT)/

# uninstall
uninstall:
	rm -rf $(PATH_INSTALL_INC_ROOT)/ROCKET && rm -f $(PATH_INSTALL_LIB_ROOT)/librocket.a


