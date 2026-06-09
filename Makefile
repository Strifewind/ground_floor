# ==============================
# Project Configuration (EDIT THESE)
# ==============================

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra -std=c++17 -g

# Executable names
APP     = app
TESTAPP = tests

# Source files (ADD .cpp FILES HERE)
SRC     = main.cpp vita.cpp valid.cpp
TESTSRC = tests.cpp

# Project info (for tarball submission)
PROJECT = ground_floor_resumebuilder
VERSION = 1.0

DISTFILES := Makefile \
						 $(wildcard *.cpp) \
						 $(wildcard *.h) \
						 $(wildcard README*)

# ==============================
# Derived Values (NO EDITING NEEDED)
# ==============================

OBJS     = $(SRC:.cpp=.o)
TESTOBJS = $(TESTSRC:.cpp=.o)

.PHONY: all clean run runtests edit dist valgrind valgrind-tests

# ==============================
# Build Targets
# ==============================

all: $(APP)

SETUP: 
	ifeq (.vim/session.vim)

$(APP): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(TESTAPP): $(TESTOBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

# ==============================
# Run Targets
# ==============================

run: $(APP)
	./$(APP)

runtests: $(TESTAPP)
	./$(TESTAPP)

# ==============================
# Memory Checking (Valgrind)
# ==============================

valgrind: $(APP)
	valgrind --leak-check=full \
		--show-leak-kinds=all \
		--track-origins=yes \
		./$(APP)

valgrind-tests: $(TESTAPP)
	valgrind --leak-check=full \
		--show-leak-kinds=all \
    --track-origins=yes \
    ./$(TESTAPP)

# ==============================
# Distribution (Tarball)
# ==============================

dist:
	tar -czf $(PROJECT)-$(VERSION).tar.gz \
		$(DISTFILES)

# ==============================
# Vim Helpers
# ==============================

edit:
	@if [ -f .vim/session.vim ]; then \
		vim -S .vim/session.vim; \
	else \
		vim -p *.cpp *.h; \
	fi

# ==============================
# Cleanup
# ==============================

clean:
	rm -f *.o $(APP) $(TESTAPP) *.tar.gz
