# Executables
CC=gcc
CFLAGS=-std=gnu99
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
ETC=/etc
DEFAULT=$(ETC)/default
INITD=$(ETC)/init.d
RSYSLOG=$(ETC)/rsyslog.d

# Rules
all: directories $(TARGET)

$(TARGET): main.o connection.o directory.o monitor.o watchman.o
	$(CC) $(CFLAGS) $(DIR_BUILD)/connection.o $(DIR_BUILD)/directory.o $(DIR_BUILD)/watchman.o $(DIR_BUILD)/monitor.o $(DIR_BUILD)/main.o -o $(DIR_BIN)/$(TARGET)

main.o: $(DIR_SRC)/main.c
	$(CC) $(CFLAGS) -Iheaders -c $(DIR_SRC)/main.c -o $(DIR_BUILD)/main.o

connection.o: $(DIR_SRC)/connection.c
	$(CC) $(CFLAGS) -Iheaders -c $(DIR_SRC)/connection.c -o $(DIR_BUILD)/connection.o

directory.o: $(DIR_SRC)/directory.c
	$(CC) $(CFLAGS) -Iheaders -c $(DIR_SRC)/directory.c -o $(DIR_BUILD)/directory.o

monitor.o: $(DIR_SRC)/monitor.c
	$(CC) $(CFLAGS) -Iheaders -c $(DIR_SRC)/monitor.c -o $(DIR_BUILD)/monitor.o

watchman.o: $(DIR_SRC)/watchman.c
	$(CC) $(CFLAGS) -Iheaders -c $(DIR_SRC)/watchman.c -o $(DIR_BUILD)/watchman.o

directories:
	$(MD) $(DIR_BIN) $(DIR_BUILD)

install:
	$(CP) $(DIR_BIN)/$(TARGET) $(PREFIX)/bin/$(TARGET)
	$(CP) init.d/$(TARGET).sh $(INITD)/$(TARGET)
	$(CP) config/default/$(TARGET).conf $(DEFAULT)
	$(CP) config/rsyslog.d/90-$(TARGET).conf $(RSYSLOG)
	service rsyslog restart

uninstall:
	$(RM) $(PREFIX)/bin/$(TARGET)
	$(RM) $(INITD)/$(TARGET)

clean:
	$(RM) $(DIR_BIN) $(DIR_BUILD)
