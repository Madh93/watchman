# Executables
CC=gcc
CP=cp -r
MD=mkdir -p
RM=rm -rf
CHM=chmod -R +x
CHMALL=chmod -R 755

# Directories
DIR_BUILD=build
DIR_BIN=bin
DIR_SRC=src

# Config
TARGET=watchman
PREFIX?=/usr/local
SHARE=$(PREFIX)/share/$(TARGET)
INITD=/etc/init.d

# Rules
all: directories $(TARGET)

$(TARGET): main.o watchman.o
	$(CC) $(DIR_BUILD)/watchman.o $(DIR_BUILD)/main.o -o $(DIR_BIN)/$(TARGET)

main.o: $(DIR_SRC)/main.c
	$(CC) -Iheaders -c $(DIR_SRC)/main.c -o $(DIR_BUILD)/main.o

watchman.o: $(DIR_SRC)/watchman.c
	$(CC) -Iheaders -c $(DIR_SRC)/watchman.c -o $(DIR_BUILD)/watchman.o

directories:
	$(MD) $(DIR_BIN) $(DIR_BUILD)

install:
	$(CP) $(DIR_BIN)/$(TARGET) $(PREFIX)/bin/$(TARGET)
	$(CP) init.d/$(TARGET).sh $(INITD)/$(TARGET)
	$(MD) $(SHARE)
	$(CP) share/* $(SHARE)
	$(CHMALL) $(SHARE)

uninstall:
	$(RM) $(PREFIX)/bin/$(TARGET)
	$(RM) $(INITD)/$(TARGET)

clean:
	$(RM) $(DIR_BIN) $(DIR_BUILD)
