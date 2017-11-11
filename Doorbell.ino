#include "math.h"

int led1 = D0;
int BUZZER = D1;
int PIR = D2;
String melody = "";
int bpm = 95;
int sinceMotion = 0;
bool playBell = true;
bool alert = false;

String rld3 = "F#5eR-0eC#5qR-0eC#5qF#5qF#5sA-5sF#5sR-0sF#5eC#5eF#5eC#5eF#5eE-5qB-4qB-4eE-5qF#5qF#5sR-0sF#5sR-0sF#5eA-5eG#5eF#5eE-5e\
C#5eR-0eA-5qR-0eC#5qA-5qC#5eA-4sR-0sC#5eA-5eC#5eA-4eC#5eB-4qG#4qG#4eB-4qC#5qF#5sR-0sF#5sR-0sF#5eE-5eC#5eB-4eC#5e";
// String rld3 = "F#5eR-0eC#5qR-0eC#5qF#5qF#5sA-5sF#5sR-0sF#5eC#5eF#5eC#5eF#5eE-5qB-4qB-4eE-5qF#5qF#5sR-0sF#5sR-0sF#5eA-5eG#5eF#5eE-5e\
// C#5eR-0eA-5qR-0eC#5qA-5qC#5eA-4sR-0sC#5eA-5eC#5eA-4eC#5eB-4qG#4qG#4eB-4qC#5qF#5sR-0sF#5sR-0sF#5eE-5eC#5eB-4eC#5e\
// F#5eR-0eC#5qR-0eC#5qF#5qF#5sA-5sF#5sR-0sF#5eC#5eF#5eC#5eF#5eE-5qB-4qB-4eE-5qF#5qF#5sR-0sF#5sR-0sF#5eA-5eG#5eF#5eE-5e\
// C#5eR-0eA-5qR-0eC#5qA-5qC#5eA-4sR-0sC#5eA-5eC#5eA-4eC#5eB-4qG#4qG#4eB-4qC#5qF#5sR-0sF#5sF-0sF#5eE-5eC#5eB-4eC#5e";

void setup() {
  pinMode(BUZZER, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(PIR, INPUT);
  Particle.function("play", playCommand);
  Particle.function("set", setMelody);
  Particle.function("bpm", setBPM);
  Particle.function("alert", setAlert);
  Particle.function("bell", setBell);
  melody = rld3;
}

int noteToFrequency(char letter, char intonation, int octave){
  int temp = 0;
  
  switch (letter){
    case 'C' :
      if (intonation == '#'){
        temp = round(17.32 * pow(2, octave));
      }
      else{
        temp = round(16.35 * pow(2, octave));    
      }
      break;
    case 'D' :
      if (intonation == '#'){
        temp = round(19.45 * pow(2, octave));
      }
      else{
        temp = round(18.35 * pow(2, octave));    
      }
      break;
    case 'E' :
      temp = round(20.6 * pow(2, octave));
      break;
    case 'F' :
      if (intonation == '#'){
        temp = round(23.12 * pow(2, octave));
      }
      else{
        temp = round(21.83 * pow(2, octave));    
      }
      break;
    case 'G' :
      if (intonation == '#'){
        temp = round(25.96 * pow(2, octave));
      }
      else{
        temp = round(24.5 * pow(2, octave));    
      }
      break;
    case 'A' :
      if (intonation == '#'){
        temp = round(29.14 * pow(2, octave));
      }
      else{
        temp = round(27.5 * pow(2, octave));    
      }
      break;
    case 'B' :
      temp = round(30.87 * pow(2, octave));
      break;
    case 'R' :
      temp = 0;
      break;
  }
  
  return temp;
}

int subToDelay(char subdivision, int bpm){
  int sub = 0;
  
  switch (subdivision){
    case 'w':
      sub = (60000 / bpm);
      break;
    case 'h':
      sub = (60000 / bpm) / 2;
      break;
    case 'q':
      sub = (60000 / bpm) / 4;
      break;
    case 'e':
      sub = (60000 / bpm) / 8;
      break;
    case 's':
      sub = (60000 / bpm) / 16;
      break;
    case 't':
      sub = (60000 / bpm) / 32;
      break;
  }
  
  return sub * 2;
}

void playNote(char letter, char intonation, int octave, char subdivision, int bpm){
  if(letter != 'R')
    tone(BUZZER, noteToFrequency(letter, intonation, octave));
    
  delay(subToDelay(subdivision, bpm));
  noTone(BUZZER);
}

void holdNote(char letter, char intonation, int octave, char subdivision, int bpm){
  if(letter != 'R')
    tone(BUZZER, noteToFrequency(letter, intonation, octave));
    
  delay(subToDelay(subdivision, bpm));
}

int setMelody(String arg){
    if(arg == "rld3"){
        melody = rld3;
        return 0;
    }
    else{
        if(arg.length() < 64){
            melody = arg;
            return 1;
        }
        else{
            return -2;
        }
    }
    return -1;
}

int playCommand(String arg){
    Particle.publish("Play", "true");
    playMelody(melody, 95);
    return 1;
}

int setBPM(String arg){
    int temp = arg.toInt();
    
    if(temp != 0){
        bpm = temp;
        return 1;
    }
    
    return 0;
}

int setAlert(String arg){
    if(arg == "on"){
        alert = true;
        return 1;
    }
    if(arg == "off"){
        alert = false;
        return 0;
    }
    return -1;
}

int setBell(String arg){
    if(arg == "on"){
        playBell = true;
        return 1;
    }
    if(arg == "off"){
        playBell = false;
        return 0;
    }
    return -1;
}

void playMelody(String melody, int bpm){
  int melodyLength = melody.length() / 4;
  for(int i = 0; i < melodyLength; i++){
    if(i < melodyLength - 1){
      if(melody[i * 4] == melody[(i + 1) * 4])
        holdNote(melody[i*4], melody[i*4 + 1], melody[i*4 + 2] - '0', melody[i*4 + 3], bpm);
      else
        playNote(melody[i*4], melody[i*4 + 1], melody[i*4 + 2] - '0', melody[i*4 + 3], bpm);
    }
    else 
      playNote(melody[i*4], melody[i*4 + 1], melody[i*4 + 2] - '0', melody[i*4 + 3], bpm);
  }
}

void loop() {
    int sensor = digitalRead(PIR);
    if(sensor){
        sinceMotion = 0;
        Particle.publish("motion", "1");
        if(alert){
            Particle.publish("motionAlert", "1");
        }
        if(playBell){
            playMelody(melody, bpm);
        }
    }   
    else{
        if(sinceMotion == 30){
            Particle.publish("motion", "0");
            sinceMotion = 0;
        }
        sinceMotion++;
    }
    delay(1000);
}