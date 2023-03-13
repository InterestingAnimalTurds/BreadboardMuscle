#include<Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
//32 data
//35 SRCLK
//34 CLK

#define TFT_CS 22
#define TFT_DC 21
#define TFT_MOSI 23
#define TFT_CLK 18
#define TFT_RST 17//17
#define TFT_MISO 19
//Test2_bmp[]
#include "DrillBitHead.h"
#include "DrillBItHeadefense.h"
#include "DrillBItHeadAttack.h"
#include "EyeballMan.h"
#include "EyeballManDefense.h"
#include "EyeballManAttack.h"
// ESP32 pin definitions


#define PLAYER_1_ATTACK_PIN 12
#define PLAYER_1_DEFENSE_PIN 13
#define PLAYER_2_ATTACK_PIN 14
#define PLAYER_2_DEFENSE_PIN 27

// // Shift register pin definitions
// #define SHIFT_REG_DATA_PIN_1 4
// #define SHIFT_REG_LATCH_PIN_1 5
// #define SHIFT_REG_CLOCK_PIN_1 6
// #define SHIFT_REG_DATA_PIN_2 7
// #define SHIFT_REG_LATCH_PIN_2 8
// #define SHIFT_REG_CLOCK_PIN_2 9

// Initial player health
#define INITIAL_HEALTH 5

int player1_health = INITIAL_HEALTH;
int player2_health = INITIAL_HEALTH;


#define DS_PIN 26
#define SH_CP_PIN 25
#define ST_CP_PIN 33
#define REST_PIN 32

#define MUSCLE_IN 15
bool player1_defending = false;
bool player2_defending = false;

//Adafruit_ILI9341 tft = Adafruit_ILI9341(tft16bitbus, TFT_D0, TFT_WR, TFT_DC, TFT_CS, TFT_RST, TFT_RD);
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

byte num = 0b00111110;
byte pattern = 0b00111100;
void setup() {

  Serial.begin(115200);

  pinMode(PLAYER_1_ATTACK_PIN, INPUT_PULLUP);
  pinMode(PLAYER_1_DEFENSE_PIN, INPUT_PULLUP);
  pinMode(PLAYER_2_ATTACK_PIN, INPUT_PULLUP);
  pinMode(PLAYER_2_DEFENSE_PIN, INPUT_PULLUP);
  
  
  pinMode(DS_PIN, OUTPUT);
  pinMode(SH_CP_PIN, OUTPUT);
  pinMode(ST_CP_PIN, OUTPUT);
  pinMode(REST_PIN, OUTPUT);
  pinMode(MUSCLE_IN,INPUT);
  tft.begin();
  tft.fillScreen(ILI9341_BLACK);

  // pinMode(SHIFT_REG_DATA_PIN_1, OUTPUT);
  // pinMode(SHIFT_REG_LATCH_PIN_1, OUTPUT);
  // pinMode(SHIFT_REG_CLOCK_PIN_1, OUTPUT);
  // pinMode(SHIFT_REG_DATA_PIN_2, OUTPUT);
  // pinMode(SHIFT_REG_LATCH_PIN_2, OUTPUT);
  // pinMode(SHIFT_REG_CLOCK_PIN_2, OUTPUT);
  tft.drawRGBBitmap(20, 20, (uint16_t *)DrillBItHead_map, 100, 100); 
  tft.drawRGBBitmap(100, 200, (uint16_t *)EyeballMan_map, 100, 100); 


}

void loop() {


  int analogValue = analogRead(MUSCLE_IN);
  Serial.println(analogValue);

  digitalWrite(REST_PIN,HIGH);
  digitalWrite(ST_CP_PIN,HIGH); 
  shiftOut(DS_PIN, SH_CP_PIN, 1, num);
  digitalWrite(ST_CP_PIN,LOW);

  digitalWrite(ST_CP_PIN,HIGH); 
  shiftOut(DS_PIN, SH_CP_PIN, 1, 0);
  digitalWrite(ST_CP_PIN,LOW);

  

  if (digitalRead(PLAYER_1_ATTACK_PIN) == LOW && !player1_defending)
  {
    tft.fillCircle(20,150,10,ILI9341_CASET);
    delay(100);
    tft.fillCircle(20,150,10,ILI9341_WHITE);
    delay(20);
  }
  
  // if (analogValue >= 2900 && !player1_defending)
  // {
  //   tft.fillCircle(20,150,10,ILI9341_CASET);
  //   delay(100);
  //   tft.fillCircle(20,150,10,ILI9341_WHITE);
  //   delay(20);
  // }

  if (digitalRead(PLAYER_2_ATTACK_PIN) == LOW && !player2_defending)
  {
    tft.drawRGBBitmap(20, 20, (uint16_t *)DrillBItHeadAttack_map, 100, 100); 

    if(!player1_defending)
    {
    player1_health--;
    num = num & pattern;
    pattern = pattern << 1;
    digitalWrite(REST_PIN,LOW);
    delay(100);

    }
  }
  if (digitalRead(PLAYER_1_ATTACK_PIN) == LOW && !player2_defending && !player1_defending ) {   
    tft.drawRGBBitmap(100, 200, (uint16_t *)EyeballManattack_map, 100, 100); 

      if(!player2_defending)
      {
        player2_health--;
        num = num & pattern;
        pattern = pattern << 1;
        digitalWrite(REST_PIN,LOW);
        delay(100);

      }

    // delay to avoid multiple hits with one button press
  }
  //  if (analogValue >= 2900 && !player2_defending && !player1_defending ) {   
  //   player2_health--;
  //   num = num & pattern;
  //   pattern = pattern << 1;
  //   digitalWrite(REST_PIN,LOW);
  //   delay(100);
  //   // delay to avoid multiple hits with one button press
  // }
  

  // Player 1 defense
  if (digitalRead(PLAYER_1_DEFENSE_PIN) == LOW) {
    player1_defending = true;
    tft.fillRect(50,150,10,10,ILI9341_RED);
    tft.drawRGBBitmap(100, 200, (uint16_t *)EyeballMandefense_map, 100, 100); 
  } else {
    tft.drawRGBBitmap(100, 200, (uint16_t *)EyeballMan_map, 100, 100);
    player1_defending = false;
    tft.fillRect(50,150,10,10,ILI9341_WHITE);
    
  }


  // Player 2 attack
  if (digitalRead(PLAYER_2_ATTACK_PIN) == LOW && !player1_defending &&!player2_defending) {
    player1_health--;
    // delay to avoid multiple hits with one button press
  }

  // Player 2 defense
  if (digitalRead(PLAYER_2_DEFENSE_PIN) == LOW) {
    player2_defending = true;
    tft.fillRect(50,150,10,10,ILI9341_RED);
    tft.drawRGBBitmap(20, 20, (uint16_t *)DrillBItHeadenfense_map, 100, 100); 
  } else {
    tft.drawRGBBitmap(20, 20, (uint16_t *)DrillBItHead_map, 100, 100);
    player2_defending = false;
    tft.fillRect(50,150,10,10,ILI9341_WHITE);
    
  }
  if (analogValue >= 2900) {
    player2_defending = true;
    tft.fillRect(50,150,10,10,ILI9341_RED);
    tft.drawRGBBitmap(20, 20, (uint16_t *)DrillBItHeadenfense_map, 100, 100); 
  } else {
    tft.drawRGBBitmap(20, 20, (uint16_t *)DrillBItHead_map, 100, 100);
    player2_defending = false;
    tft.fillRect(50,150,10,10,ILI9341_WHITE);
    
  }

  // Update LED display
  // updateLEDs();

  // Check for game over
  if (player1_health <= 0 || player2_health <= 0) {
    // Game over, reset player health
    player1_health = INITIAL_HEALTH;
    player2_health = INITIAL_HEALTH;
    num = 0b00111110;
    pattern = 0b00111100;
  }
 
}

// void updateLEDs() {
//   // Combine player health into two bytes
//   byte player1_health_byte = player1_health;
//   byte player2_health_byte = player2_health << 1;

//   // Send the player 1 health byte to the first shift register
//   digitalWrite(SHIFT_REG_LATCH_PIN_1, LOW);
//   shiftOut(SHIFT_REG_DATA_PIN_1, SHIFT_REG_CLOCK_PIN_1, MSBFIRST, player1_health_byte);
//   digitalWrite(SHIFT_REG_LATCH_PIN_1, HIGH);

//   // Send the player 2 health byte to the second shift register
//   digitalWrite(SHIFT_REG_LATCH_PIN_2, LOW);
//   shiftOut(SHIFT_REG_DATA_PIN_2, SHIFT_REG_CLOCK_PIN_2, MSBFIRST, player2_health_byte);
//   digitalWrite(SHIFT_REG_LATCH_PIN_2, HIGH);
// }
