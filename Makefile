ifeq ($(OS),Windows_NT)
    RM = del /Q
    EXE = .exe
else
    RM = rm -f
    EXE = 
endif

CC = gcc
CFLAGS = -Wall -Wextra
LDLIBS = -lSDL2 -lSDL2_image -lSDL2_ttf -lm

SRCDIR = src
BUILDDIR = build
TARGET = $(BUILDDIR)/game$(EXE)

SOURCES := $(wildcard $(SRCDIR)/*.c)
OBJECTS := $(SOURCES:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)

.PHONY: all clean debug run

all: $(TARGET) copy_assets

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

clean:
	$(RM) $(BUILDDIR)/*.o $(TARGET)

debug: CFLAGS += -DDEBUG -g
debug: all

run: all
	./$(TARGET)

copy_assets: 
	rsync -r --update $(SRCDIR)/assets $(BUILDDIR)/