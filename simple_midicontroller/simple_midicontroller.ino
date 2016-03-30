// 
// Simple MIDI controller
// 2016.03.30 D.F.Mac. 
//
// This sketch is targeted for Sparkfun's Pro Micro with depend on rkistner/arcore.
//   Pro Micro:  https://www.sparkfun.com/products/12640
//   arcore:     https://github.com/rkistner/arcore
// And It recommended to comment-out to line '#define HID_ENABLED' in cores/arcore/USBDesc.h to avoid conflict HID with MIDI.
 
#define BUTTON 10
#define LED 9
#define VOLUME A2
#define THRUMAX 10
#define DELAY_MS 5
#define VELOCITY 100

int oButton = LOW;
int oNote = -1;
int thruCnt = 0;
boolean isPlay = false;

void noteOn(byte pitch, byte velocity) {
  thruCnt = THRUMAX;
  MIDIEvent noteOn = {0x09, 0x90, pitch, velocity};
  MIDIUSB.write(noteOn);
  digitalWrite(LED,HIGH);
}

void noteOff(byte pitch) {
  MIDIEvent noteOff = {0x08, 0x80, pitch, 0};
  MIDIUSB.write(noteOff);
}


void setup() {
  pinMode(BUTTON,INPUT);
  pinMode(LED,OUTPUT);
}

void loop() {
  int button = digitalRead(BUTTON);
  if((button == HIGH)&&(oButton == LOW)){
    noteOn(oNote,VELOCITY);
    isPlay = true;
  }else if((button == LOW)&&(oButton == HIGH)){
    noteOff(oNote);
    isPlay = false;
  }
  oButton = button;

  if(isPlay){
    int note = analogRead(VOLUME);
    note >>= 3;
    if(thruCnt <= 0){
      if(oNote != -1){
        if(checkChange(note,oNote,2)){
          noteOff(oNote);
          noteOn(note,VELOCITY);
        }
      }else{
        noteOn(note,VELOCITY);
      }
    }else{
      thruCnt --;
      if(thruCnt == 0){
        digitalWrite(LED,LOW);
      }
    }
    oNote = note;
  }
    
  delay(DELAY_MS);
}

boolean checkChange(int value, int oValue, int range){
  int maxv = oValue + range;
  int minv = oValue - range;
  if((value >= maxv)||(value <= minv)){
    return true;
  }else{
    return false;
  }    
}

