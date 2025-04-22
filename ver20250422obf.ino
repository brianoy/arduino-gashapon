/*
 *  ╔═══════════════════════════════════════════════╗
 *  ║                                               ║
 *  ║   ██████╗ ██████╗ ███████╗██╗   ██╗███████╗   ║
 *  ║  ██╔═══██╗██╔══██╗██╔════╝██║   ██║██╔════╝   ║
 *  ║  ██║   ██║██████╔╝█████╗  ██║   ██║███████╗   ║
 *  ║  ██║   ██║██╔══██╗██╔══╝  ██║   ██║╚════██║   ║
 *  ║  ╚██████╔╝██████╔╝██║     ╚██████╔╝███████║   ║
 *  ║   ╚═════╝ ╚═════╝ ╚═╝      ╚═════╝ ╚══════╝   ║
 *  ║                                               ║
 *  ║        THIS CODE HAS BEEN OBFUSCATED          ║
 *  ║                                               ║
 *  ╚═══════════════════════════════════════════════╝
 */

// lib內需要有aceroutine
#include <Servo.h>
#include <AceRoutine.h>

using namespace ace_routine;

const int _0x243BE605 = A1;
const int _0xA521F12B = A3;

const int _0xD8A4E3D1 = 2;
const int _0xF1D5F010 = 3;
const int _0xFFC8429B = 4;
const int _0x2DF02AF2 = 8;
const int _0x8E1B2EE7 = 9;
const int _0x01CE5282 = 10;
const int _0xCA914BE1 = 11;
const int _0xE40CE96D = 12;

int _0x62E95D6C[4][2] = {
  {0, 4},
  {1, 5},
  {2, 6},
  {3, 7}
};

// 狀態變數
int _0x9472A214 = -1;
int _0xC3139859 = 0;
int _0xEA75C0A8 = 0;
int _0x80FACDF8 = 0;
int _0x8AEC5734 = 0;
int _0x423E232E = 1;
int _0xFCE237AA = 0;

static int _0x3C11ECD6 = 0;
static int _0x8C06DD98 = 0;

Servo _0xAC5DF010;

void _0x30F15299() {
  digitalWrite(_0xF1D5F010, LOW);
  shiftOut(_0xD8A4E3D1, _0xFFC8429B, LSBFIRST, 0);
  digitalWrite(_0xF1D5F010, HIGH);
}

COROUTINE(_0x00000007) {
  COROUTINE_LOOP(){
    if (_0xC3139859 > 0){
      static int _0x8C06DD98 = 0;
      for (_0x8C06DD98 = 0; _0x8C06DD98 <= 180; _0x8C06DD98++) {
        _0xAC5DF010.write(_0x8C06DD98);
        COROUTINE_DELAY(5);
      }
      COROUTINE_DELAY(2500);
      for (_0x8C06DD98 = 180; _0x8C06DD98 >= 0; _0x8C06DD98--) {
        _0xAC5DF010.write(_0x8C06DD98);
        COROUTINE_DELAY(5);
      }
      COROUTINE_DELAY(2500);
      _0x8AEC5734++;
      _0xC3139859--;
      COROUTINE_YIELD();
    }
    else if (_0xEA75C0A8 == 5 && _0xC3139859 == 0) {
      _0xEA75C0A8 = 6;
      _0x30F15299();
      COROUTINE_DELAY(1000);
      _0xEA75C0A8 = 0;
      COROUTINE_YIELD();
    }
    else{
      COROUTINE_YIELD();
    }
  }
}

void _0x1DCE59C4(int _0xF1D5F019) {
  byte _0x4967F48E = 1 << _0xF1D5F019;
  digitalWrite(_0xF1D5F010, LOW);
  shiftOut(_0xD8A4E3D1, _0xFFC8429B, LSBFIRST, _0x4967F48E);
  digitalWrite(_0xF1D5F010, HIGH);
}

COROUTINE(_0x00000006) {
  COROUTINE_LOOP(){
    if (_0xEA75C0A8 == 5 || _0xEA75C0A8 == 4){
      _0x1DCE59C4(_0xFCE237AA);
      COROUTINE_DELAY(300);
      _0x30F15299();
      COROUTINE_DELAY(300);
    }
    else{
      COROUTINE_YIELD();
    }
  }
}

COROUTINE(_0x00000005) {
  COROUTINE_LOOP() {
    if (_0xEA75C0A8 == 4){
      COROUTINE_DELAY(1500);
      if (_0x9472A214 == 0) {
        COROUTINE_DELAY(2000);
        _0xEA75C0A8 = 5;
      }
      else if (_0x9472A214 == 1){
        _0xC3139859++;
        _0xEA75C0A8 = 5;
      }
      else if (_0x9472A214 == 2){
        _0xC3139859++;
        _0xC3139859++;
        _0xEA75C0A8 = 5;
      }
      else{
        COROUTINE_DELAY(3000);
        _0xEA75C0A8 = 3;
      }
      _0x9472A214 = -1;
    }
    COROUTINE_YIELD();
  }
}

void _0xF52BBAFA(int _0x9482A214) { 
  if (random(0, 100) < _0x9482A214) {
    _0x9472A214 = 2;
    _0x80FACDF8 = 1;
  } 
  else {
    int _0xD81926E4;
    if (_0x80FACDF8%3 == 0) {
      _0xD81926E4 = random(1, 2);
      _0x80FACDF8 = 1;
    }
    else{
      _0xD81926E4 = random(0, 5);
    }
    if (_0xD81926E4 == 0) {
      _0x9472A214 = 0;
      _0x80FACDF8++;
    } 
    else if (_0xD81926E4 == 1) {
      _0x9472A214 = 1;
    } 
    else {
      _0x9472A214 = 3;
    }
  }
}

void _0x96E70466(){
  pinMode(_0xF1D5F010, OUTPUT);
  pinMode(_0xFFC8429B, OUTPUT);
  pinMode(_0xD8A4E3D1, OUTPUT);
  pinMode(_0x8E1B2EE7, INPUT);
  pinMode(_0x01CE5282, OUTPUT);
  pinMode(_0xCA914BE1, OUTPUT);
  digitalWrite(_0x01CE5282, HIGH);
  digitalWrite(_0xCA914BE1, HIGH);
  _0xAC5DF010.attach(_0xE40CE96D);
  _0xAC5DF010.write(0);
  _0x30F15299();
}

void _0x187B9D61(){
  digitalWrite(_0xF1D5F010, LOW);
  shiftOut(_0xD8A4E3D1, _0xFFC8429B, LSBFIRST, 170);
  digitalWrite(_0xF1D5F010, HIGH);
}

void _0xF5385738(){
  digitalWrite(_0xF1D5F010, LOW);
  shiftOut(_0xD8A4E3D1, _0xFFC8429B, LSBFIRST, 85);
  digitalWrite(_0xF1D5F010, HIGH);
}

void _0xC62AC5E1(){
  digitalWrite(_0xF1D5F010, LOW);
  shiftOut(_0xD8A4E3D1, _0xFFC8429B, LSBFIRST, 255);
  digitalWrite(_0xF1D5F010, HIGH);
}

COROUTINE(_0x00000001) {
  COROUTINE_LOOP() {
    if (_0xEA75C0A8 == 0) {
      _0x187B9D61();
      COROUTINE_DELAY(500);
      _0xF5385738();
      COROUTINE_DELAY(500);
    }
    else{
      COROUTINE_YIELD();
    }
  }
}

COROUTINE(_0x00000003) {
  COROUTINE_LOOP() {
    if (_0xEA75C0A8 == 1){
      _0xC62AC5E1();
      COROUTINE_DELAY(500);
      _0x30F15299();
      COROUTINE_DELAY(500);
    }
    else{
      COROUTINE_YIELD();
    }
  }
}

COROUTINE(_0x00000002) {
  COROUTINE_LOOP(){
    if ((_0xEA75C0A8 == 0) && analogRead(_0xA521F12B) > 500){
      _0xEA75C0A8 = 1;
      _0x423E232E++;
    }
    
    if ((_0xEA75C0A8 == 1) && (digitalRead(_0x8E1B2EE7) == HIGH)) {
      while (digitalRead(_0x8E1B2EE7) == HIGH);
      _0xEA75C0A8 = 2;
      delay(50);
    }
    COROUTINE_YIELD();
  }
}

COROUTINE(_0x00000008) {
  COROUTINE_LOOP(){
    if ((_0xEA75C0A8 == 0) && (digitalRead(_0x2DF02AF2) == HIGH)){
      _0xEA75C0A8 = 5;
      _0xC3139859 = 1;
    }
    COROUTINE_YIELD();
  }
}

COROUTINE(_0x00000004) {
  COROUTINE_LOOP(){
    if (_0xEA75C0A8 == 2 || _0xEA75C0A8 == 3){
      int _0x879F5CDE = map(analogRead(_0x243BE605), 20, 1000, 0, 100);
      _0xF52BBAFA(_0x879F5CDE);
      int _0x4FB83F7F[2] = { _0x62E95D6C[_0x9472A214][0], _0x62E95D6C[_0x9472A214][1]};
      _0xFCE237AA = _0x4FB83F7F[random(0, 2)];
      int _0x6087C891 = 0;
      int _0xE9F9A12F = 30;
      int _0xA47F5B25 = 2;
      int _0x83DA60AA = 0;

      if (_0xEA75C0A8 == 2) {
        for (_0x3C11ECD6 = 0; _0x3C11ECD6 <= 300; _0x3C11ECD6++) {
          _0x83DA60AA = (_0x6087C891 + _0x3C11ECD6) % 8;
          _0x1DCE59C4(_0x83DA60AA);
          delay(_0xE9F9A12F);
          if (digitalRead(_0x8E1B2EE7) == HIGH) {
            _0xEA75C0A8 = 3;
            _0x6087C891 = _0x83DA60AA;
            break;
          }
        }
      }
      int _0xC208D029 = _0xFCE237AA - _0x6087C891;
      if (_0xC208D029 < 0) {
        _0xC208D029 += 8;
      }
      int _0x646B2E65 = 8 * 8 + _0xC208D029;

      _0xEA75C0A8 = 3;

      for (_0x3C11ECD6 = 0; _0x3C11ECD6 <= _0x646B2E65; _0x3C11ECD6++) {
        _0x83DA60AA = (_0x6087C891 + _0x3C11ECD6) % 8;
        _0x1DCE59C4(_0x83DA60AA);
        delay(_0xE9F9A12F);
        _0xE9F9A12F += _0xA47F5B25;
      }
      _0xEA75C0A8 = 4;
    }
    COROUTINE_YIELD();
  }
}

void setup() {
  _0x96E70466();
}

void loop() {
  _0x00000001.runCoroutine();
  _0x00000002.runCoroutine();
  _0x00000003.runCoroutine();
  _0x00000004.runCoroutine();
  _0x00000005.runCoroutine();
  _0x00000006.runCoroutine();
  _0x00000007.runCoroutine();
  _0x00000008.runCoroutine();
}