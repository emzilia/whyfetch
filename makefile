CC = gcc
CC_FLAGS = -Wall

INSTALL_DIR = ~/.local/bin

SRC = whyfetch.c
TARGET = whyfetch

$(TARGET): $(SRC)
	$(CC) $(CC_FLAGS) $< -o $@

install:
	cp $(TARGET) $(INSTALL_DIR)

uninstall:
	rm $(INSTALL_DIR)/$(TARGET)

clean:
	rm $(TARGET)

.PHONY: all install uninstall clean
