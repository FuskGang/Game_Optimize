ifeq ($(OS),Windows_NT)
    RM = del
    EXE = .exe
    LDLIBS = -lmingw32 -lSDL2main
    CPY = xcopy /s /i /e
    CPY_FOLDER = assets
    DELMTR = \\

else
    RM = rm -f
    EXE = 
    LDLIBS = -lm
    CPY = rsync -r --update
    CPY_FOLDER = 
    DELMTR = /
endif

CC = gcc
CFLAGS = -Wall -Wextra
LDLIBS += -lSDL2 -lSDL2_image -lSDL2_ttf
SRCDIR = src
BUILDDIR = build
TARGET = $(BUILDDIR)$(DELMTR)game$(EXE)

SOURCES := $(wildcard $(SRCDIR)/*.c)
OBJECTS := $(SOURCES:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)

.PHONY: all clean debug run

all: $(TARGET) copy_assets

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR):
	mkdir $(BUILDDIR)

clean:
	$(RM) $(BUILDDIR)$(DELMTR)*.o $(TARGET)

debug: CFLAGS += -DDEBUG -g
debug: all

run: all
	./$(TARGET)

copy_assets: 
	$(CPY) $(SRCDIR)$(DELMTR)assets $(BUILDDIR)$(DELMTR)$(CPY_FOLDER)