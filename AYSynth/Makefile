BOARD=chipKIT:pic32:chipkit_DP32
SKETCH=AYSynth
HOST=lat-01-07
PASS=wibble
PORT?=/dev/ttyACM0


SUBDIR=$(subst :,.,$(BOARD))
BIN=bin/$(SKETCH).ino.hex
#IP=$(shell avahi-resolve -n $(HOST).local | cut -f2 -d'	')
IP=192.168.0.61
OTA=~/.arduino15/packages/esp32/hardware/esp32/2.0.3/tools/espota.py

$(BIN): $(SKETCH).ino
	arduino-cli compile --fqbn $(BOARD):USB=z_custom --output-dir bin --build-path build

install: $(BIN)
	arduino-cli upload --fqbn $(BOARD) -p $(PORT) --input-dir bin

clean:
	rm -rf build bin

watch:
	echo ${SKETCH}.ino | entr -c -s 'make'
