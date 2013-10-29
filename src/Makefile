#============================================================
# Name            : Makefile
#
# This Makefile builds the toollogger client library and 
# the ToolLogger UDP server.
#
# ----------------------C r e a t e d------------------------
#
# Created: November  5, 2001
# Creator: Peter R. Torpman (peter@torpman.com)
#
#============================================================
#

############################################################
#         USER SETTINGS
############################################################

CC       := gcc
CFLAGS   := -O2 -Wall -Werror -Wextra
CCC      := g++
CXXFLAGS := -O2 -Wall -Werror -Wextra
INCLUDES := 

# Where the client library should be installed
INCDIR   := /tmp/include
LIBDIR   := /tmp/lib

# Where the server should be installed
BINDIR   := /tmp/bin


############################################################
#         SOURCE FILES
############################################################

CLIENT_LIB := libtoollogger.a
CLIENT_SRC := toollogger.c
CLIENT_OBJ := $(CLIENT_SRC:.c=.o)

SERVER     := loggerserver
SERVER_SRC := server.cc
SERVER_OBJ := $(SERVER_SRC:.cc=.o)

SAMPLE     := sample
SAMPLE_SRC := sample.c
SAMPLE_OBJ := $(SAMPLE_SRC:.c=.o)

############################################################
#         RULES
############################################################

# C Source 
.c.o: 
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

# C++ Source
.cc.o: 
	$(CCC) $(CXXFLAGS) $(INCLUDES) -c $<

############################################################
#         TARGETS
############################################################

all: $(CLIENT_LIB) $(SERVER) $(SAMPLE)

$(CLIENT_LIB): $(CLIENT_OBJ)
	ar rcv $@ $^
	ranlib $@

$(SERVER): $(SERVER_OBJ)
	$(CCC) -o $@ $^ 

$(SAMPLE): $(SAMPLE_OBJ)
	$(CC) -o $@ $^ -L. -ltoollogger

install: all
	install -d $(INCDIR)
	install -d $(LIBDIR)
	install -d $(BINDIR)
	@echo "Installing header file in $(INCDIR)"
	install -m 664 toollogger.h  $(INCDIR)
	install -m 664 $(CLIENT_LIB) $(LIBDIR)
	@echo "Installing ToolLogger Server in $(BINDIR)"
	install -m 755 $(SERVER) $(BINDIR)


clean:	
	-rm -rf *.o $(CLIENT_OBJ) $(CLIENT_LIB) $(SERVER_OBJ) $(SERVER) $(LOGGER_SERVER) $(SAMPLE_OBJ) $(SAMPLE)
