#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <DFMiniMp3.h>

static const uint8_t PIN_MP3_TX = 11; // Connects to module's RX
static const uint8_t PIN_MP3_RX = 10; // Connects to module's TX

SoftwareSerial softwareSerial(PIN_MP3_RX, PIN_MP3_TX);

class Mp3Notify;

typedef DFMiniMp3<SoftwareSerial, Mp3Notify> DfMp3;

DfMp3 dfmp3(softwareSerial);

class Mp3Notify
{
public:
  static void PrintlnSourceAction(DfMp3_PlaySources source, const char *action)
  {
    if (source & DfMp3_PlaySources_Sd)
    {
      Serial.print("SD Card, ");
    }
    if (source & DfMp3_PlaySources_Usb)
    {
      Serial.print("USB Disk, ");
    }
    if (source & DfMp3_PlaySources_Flash)
    {
      Serial.print("Flash, ");
    }
    Serial.println(action);
  }
  static void OnError([[maybe_unused]] DfMp3 &mp3, uint16_t errorCode)
  {
    // see DfMp3_Error for code meaning
    Serial.println();
    Serial.print("Com Error ");
    Serial.println(errorCode);
  }
  static void OnPlayFinished([[maybe_unused]] DfMp3 &mp3, [[maybe_unused]] DfMp3_PlaySources source, uint16_t track)
  {
    Serial.print("Play finished for #");
    Serial.println(track);
  }
  static void OnPlaySourceOnline([[maybe_unused]] DfMp3 &mp3, DfMp3_PlaySources source)
  {
    PrintlnSourceAction(source, "online");
  }
  static void OnPlaySourceInserted([[maybe_unused]] DfMp3 &mp3, DfMp3_PlaySources source)
  {
    PrintlnSourceAction(source, "inserted");
  }
  static void OnPlaySourceRemoved([[maybe_unused]] DfMp3 &mp3, DfMp3_PlaySources source)
  {
    PrintlnSourceAction(source, "removed");
  }
};

int x = 0;
int lastX = 0;
const int volume = 15;

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{

  while (1 < Wire.available()) // loop through all but the last
  {
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);      // print the character
  }
  x = Wire.read(); // receive byte as an integer
  Serial.println(x);
}

void operateSpeaker()
{
  // Set volume to maximum (0 to 30).
  dfmp3.setVolume(volume);
  // Play the first MP3 file on the SD card
  dfmp3.playGlobalTrack(1);
}

void setup()
{
  Wire.begin(4);                // join i2c bus with address #4
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for output
  softwareSerial.begin(9600);
  dfmp3.begin();
}

void loop()
{
  // delay(100);
  if (x == 1 && lastX == 0)
  {
    // Set volume to maximum (0 to 30).
    dfmp3.setVolume(volume);
    // Play the first MP3 file on the SD card (mission impossible theme)
    dfmp3.playGlobalTrack(1);

    Serial.println("playing mission impossible...");
  }
  else if (x == 2 && lastX != 2)
  {
    // Set volume to maximum (0 to 30).
    dfmp3.setVolume(volume);
    // play laser touching sound
    dfmp3.playAdvertisement(1); // sd:/advert/0001.mp3
  }

  lastX = x;
}