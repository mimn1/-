#include <TMRpcm.h>
#include <SD.h>
#include <SPI.h>

TMRpcm audio;

int file_number = 0;
char filePrefixname[50] = "TEST";
char exten[10] = ".wav";

const int mic_pin = A0;
const int sample_rate = 16000;

#define SD_CSPin 4

enum STATES {NOT_READY, WAITING, RECORDING};
volatile STATES current_state = NOT_READY;

void tangenta()
{
  delay(100);
  long int t1 = 0;
  long int t2 = 0;
  char fileSlNum[20] = "";
  char file_name[50] = "";
  if (current_state == WAITING)
  {
    current_state = RECORDING;
    Serial.println("####################################################################################");
    
    itoa(file_number, fileSlNum, 10);

    strcat(file_name, filePrefixname);
    strcat(file_name, fileSlNum);
    strcat(file_name, exten);
    Serial.print("New File Name: ");
    Serial.println(file_name);
    t1 = millis();
    audio.startRecording(file_name, sample_rate, mic_pin);
    Serial.println("START RECORING");

  }
  else if (current_state == RECORDING)
  {
    audio.stopRecording(file_name);
    Serial.println("STOP RECORING");
    file_number++;
    t2 = millis();
    Serial.println("Elapsed time: " + String((t2 - t1)/1000) + " sec.");
    Serial.println("####################################################################################");
    current_state = WAITING;
  }
  else if (current_state == NOT_READY)
  {
    Serial.println("RECORDER IS NOT READY");
  }
  else 
  {
    Serial.println("AN ERROR OCCURRED! TURNING OFF.");
  }

}

// с помощью этой функции мы затем будем отмерять 2-х минутные интервалы
void wait_min(float mins) {
  int count = 0;
  int secs = (int) (mins * 60);
  while (1) {
    Serial.print('.');
    delay(1000);
    count++;
    if (count == secs) {
      count = 0;
      break;
    }
  }
  Serial.println();
  return ;
}

void setup() {
  
  attachInterrupt(0, tangenta, FALLING  );
  // put your setup code here, to run once:
  //initialises the serial connection (инициализируем последовательную связь) between the arduino and any connected serial device(e.g. computer, phone, raspberry pi...)
  Serial.begin(9600);
  //задаем режимы работы используемых контактов
  pinMode(mic_pin, INPUT);

  Serial.println("loading... SD card");
  if (!SD.begin(SD_CSPin)) {
    Serial.println("An Error has occurred while mounting SD");
  }
  while (!SD.begin(SD_CSPin)) {
    Serial.print(".");
    delay(500);
  }
  current_state = WAITING;
  Serial.println("LOADING SD COMPLETED");
  audio.CSPin = SD_CSPin;
}
void loop() {
  
}
