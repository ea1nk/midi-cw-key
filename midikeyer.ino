#include <MIDIUSB.h>

// Cada pala se conecta a GND a través del interruptor.
// Con INPUT_PULLUP, el estado por defecto es HIGH y el cierre del interruptor pasa a LOW.
const uint8_t PADDLE_1_PIN = 2; // C4
const uint8_t PADDLE_2_PIN = 3; // C#4
const uint8_t STRAIGHT_KEY_PIN = 4; // D4 (manipulador vertical)

const uint8_t NOTE_C4  = 60;  // C4
const uint8_t NOTE_C4S = 61;  // C#4
const uint8_t NOTE_D4  = 62;  // D4
const uint8_t MIDI_CHANNEL = 0;  // Canal 1 en MIDI USB (0-based)
const uint8_t NOTE_ON_VELOCITY = 127;

struct Paddle {
  uint8_t pin;
  uint8_t note;
  bool rawState;
  bool stableState;
  unsigned long lastChangeMs;
};

Paddle paddles[] = {
  { PADDLE_1_PIN, NOTE_C4,  false, false, 0 },
  { PADDLE_2_PIN, NOTE_C4S, false, false, 0 },
  { STRAIGHT_KEY_PIN, NOTE_D4, false, false, 0 }
};

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
  MidiUSB.flush();
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
  MidiUSB.flush();
}

void processPaddle(Paddle &paddle) {
  bool reading = (digitalRead(paddle.pin) == LOW); // activo en LOW

  if (reading != paddle.rawState) {
    paddle.rawState = reading;
    paddle.lastChangeMs = millis();
  }

  if ((millis() - paddle.lastChangeMs) > 15) {
    if (reading != paddle.stableState) {
      paddle.stableState = reading;

      if (reading) {
        // Pulsación detectada: enviamos Note On para que la aplicación lo vea.
        noteOn(MIDI_CHANNEL, paddle.note, NOTE_ON_VELOCITY);
      } else {
        // Soltado: enviamos Note Off para cerrar el evento.
        noteOff(MIDI_CHANNEL, paddle.note, 0);
      }
    }
  }
}

void setup() {
  pinMode(PADDLE_1_PIN, INPUT_PULLUP);
  pinMode(PADDLE_2_PIN, INPUT_PULLUP);
  pinMode(STRAIGHT_KEY_PIN, INPUT_PULLUP);

  for (auto &paddle : paddles) {
    paddle.rawState = (digitalRead(paddle.pin) == LOW);
    paddle.stableState = false;
    paddle.lastChangeMs = millis();
  }
}

void loop() {
  for (auto &paddle : paddles) {
    processPaddle(paddle);
  }
}
