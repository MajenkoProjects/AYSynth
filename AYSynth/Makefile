BOARD=chipKIT:pic32:chipkit_DP32
SKETCH=AYSynth
PORT?=/dev/ttyACM0

SUBDIR=$(subst :,.,$(BOARD))
BIN=bin/$(SKETCH).ino.hex

$(BIN): $(SKETCH).ino
	arduino-cli compile --fqbn $(BOARD):USB=z_custom --output-dir bin --build-path build

install: $(BIN)
	arduino-cli upload --fqbn $(BOARD) -p $(PORT) --input-dir bin

clean:
	rm -rf build bin

watch:
	echo ${SKETCH}.ino | entr -c -s 'make'
