# MIDI-PADDLE

Paddle and straight key keyer for Arduino Leonardo that shows up over USB as a MIDI device named **MIDI-PADDLE**.

Each paddle and the straight key send a MIDI note on channel 1 (Note On when pressed, Note Off when released):

| Input        | Pin | Note      |
|--------------|-----|-----------|
| Paddle 1     | D2  | C4 (60)   |
| Paddle 2     | D3  | C#4 (61)  |
| Straight key | D4  | D4 (62)   |

## Wiring

Connect each paddle and the straight key between its pin and GND. The pins use the internal pull-up resistor (`INPUT_PULLUP`), so no external resistor is needed. The firmware applies a 15 ms debounce.

## Requirements

- Arduino Leonardo (or another ATmega32U4 board with native USB)
- [arduino-cli](https://arduino.github.io/arduino-cli/)
- `arduino:avr` platform
- `MIDIUSB` library

### Installing dependencies

```sh
arduino-cli core update-index
arduino-cli core install arduino:avr
arduino-cli lib install MIDIUSB
```

If you are behind a proxy and `arduino-cli` times out while downloading, set the proxy explicitly:

```sh
export ARDUINO_NETWORK_PROXY="$https_proxy"
```

## Build and flash

With the board connected, find its port:

```sh
arduino-cli board list
```

Build and upload (replace `/dev/ttyACM0` with your board's port):

```sh
arduino-cli compile --fqbn arduino:avr:leonardo \
  --build-property 'build.usb_product="MIDI-PADDLE"' \
  -u -p /dev/ttyACM0 .
```

The `build.usb_product` property sets the USB name **MIDI-PADDLE**. Without it, the board identifies itself as "Arduino Leonardo".

### Changing the USB name

`arduino-cli` caches the compiled core and does not rebuild it when `build.usb_product` changes. If you change the name, delete that cache first and build with `--clean`:

```sh
rm -rf ~/.cache/arduino/cores/arduino_avr_leonardo_*
arduino-cli compile --clean --fqbn arduino:avr:leonardo \
  --build-property 'build.usb_product="NEW-NAME"' \
  -u -p /dev/ttyACM0 .
```

The manufacturer always shows as "Arduino LLC": the Arduino core forces it while the Arduino USB VID (`0x2341`) is used.

## Checking

On Linux, after flashing, the board should appear as a MIDI device:

```sh
$ amidi -l
Dir Device    Name
IO  hw:3,0,0  MIDI-PADDLE MIDI 1
```

To watch the notes sent when pressing the paddles or the straight key:

```sh
aseqdump -p MIDI-PADDLE
```
