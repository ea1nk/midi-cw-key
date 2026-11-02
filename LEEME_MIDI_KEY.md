# MIDI-PADDLE

Manipulador de palas (keyer) y manipulador vertical para Arduino Leonardo que se presenta por USB como dispositivo MIDI con el nombre **MIDI-PADDLE**.

Cada pala y el manipulador vertical envían una nota MIDI por el canal 1 (Note On al pulsar, Note Off al soltar):

| Entrada              | Pin | Nota      |
|----------------------|-----|-----------|
| Pala 1               | D2  | C4 (60)   |
| Pala 2               | D3  | C#4 (61)  |
| Manipulador vertical | D4  | D4 (62)   |

## Conexionado

Cada pala y el manipulador vertical van entre su pin y GND. Los pines usan la resistencia de pull-up interna (`INPUT_PULLUP`), así que no hace falta ninguna resistencia externa. El firmware aplica un antirrebote de 15 ms.

## Requisitos

- Arduino Leonardo (u otra placa con ATmega32U4 y USB nativo)
- [arduino-cli](https://arduino.github.io/arduino-cli/)
- Plataforma `arduino:avr`
- Librería `MIDIUSB`

### Instalación de dependencias

```sh
arduino-cli core update-index
arduino-cli core install arduino:avr
arduino-cli lib install MIDIUSB
```

Si estás detrás de un proxy y `arduino-cli` da timeouts al descargar, indícale el proxy explícitamente:

```sh
export ARDUINO_NETWORK_PROXY="$https_proxy"
```

## Compilar y flashear

Con la placa conectada, comprueba su puerto:

```sh
arduino-cli board list
```

Compila y sube (sustituye `/dev/ttyACM0` por el puerto de tu placa):

```sh
arduino-cli compile --fqbn arduino:avr:leonardo \
  --build-property 'build.usb_product="MIDI-PADDLE"' \
  -u -p /dev/ttyACM0 .
```

La propiedad `build.usb_product` es la que da el nombre USB **MIDI-PADDLE**. Si la omites, la placa se identificará como "Arduino Leonardo".

### Cambiar el nombre USB

`arduino-cli` guarda en caché el core compilado y no lo recompila al cambiar `build.usb_product`. Si cambias el nombre, borra antes esa caché y compila con `--clean`:

```sh
rm -rf ~/.cache/arduino/cores/arduino_avr_leonardo_*
arduino-cli compile --clean --fqbn arduino:avr:leonardo \
  --build-property 'build.usb_product="NUEVO-NOMBRE"' \
  -u -p /dev/ttyACM0 .
```

El fabricante siempre aparecerá como "Arduino LLC". El core de Arduino lo fuerza mientras se use el VID USB de Arduino (`0x2341`).

## Comprobación

En Linux, tras flashear la placa debería aparecer como dispositivo MIDI:

```sh
$ amidi -l
Dir Device    Name
IO  hw:3,0,0  MIDI-PADDLE MIDI 1
```

Para ver las notas que envía al pulsar las palas o el manipulador vertical:

```sh
aseqdump -p MIDI-PADDLE
```
