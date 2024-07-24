#include <Arduino.h>
#include "config.h"

#define RELAY_PIN 19

bool flag = 0;
bool mc_flag = 1;

struct Manikins_All {
      int pinNo;
      int mode;
      char *name;
};

////////////////// Plural Tap ////////////////////////
Manikins_All input1 = {13, true, "LP_SUBDURAL_SPACE"};
// Manikins_All input2 = {14, false, "LP_IVB_L1"};
Manikins_All input2 = {14, true, "MOTOR_ON"};
// Manikins_All input3 = {15, false, "LP_IVB_L2"};
Manikins_All input3 = {15, false, "MOTOR_OFF"};
Manikins_All input4 = {27, false, "LP_SPINE_LAMINA"};
Manikins_All input5 = {5, false, "LP_IVB_L5"};
Manikins_All input6 = {22, true, "MED_INTAKE"};

Manikins_All inputs[TOTAL_INPUTS] = {input1, input2, input3, input4, input5, input6};//, input6, input7, input8, input9, input10, input11, input12};

void configureGPIOs()
{
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GRN, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(ONBOARD_LED, OUTPUT);

  for (int i = 0; i < TOTAL_INPUTS; i++)
  {
    pinMode(inputs[i].pinNo, INPUT_PULLUP);
  }
}

void indicate(bool mode)
{
  if (mode == true)
  {
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GRN, HIGH);
  }
  else if (mode == false)
  {
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GRN, LOW);
  }
  digitalWrite(BUZZER, HIGH);
}

void setup()
{
  Serial.begin(115200);
  Serial2.begin(115200);
  pinMode(Switch1, INPUT_PULLUP);
  pinMode(Switch2, INPUT_PULLUP);
  pinMode(Switch3, INPUT_PULLUP);
  pinMode(Switch4, INPUT_PULLUP);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);
  configureGPIOs();
  Serial.println(DEVICE_NAME);
}

unsigned long t1 = millis();
bool inputState = false;
int status = true;
bool inputFlag = true;

void loop()
{
    if (Serial2.available()) {
        String inByte = Serial2.readStringUntil('\n');
        Serial.println(inByte);
        delay(20);
    }
    if (digitalRead(inputs[0].pinNo) == LOW)
      { 
      if (inputFlag == true)  {
         inputFlag = false;
        //  indicate(inputs[0].mode);
         Serial.println(inputs[0].name);
         delay(100);
        }
      }
      else if (digitalRead(inputs[1].pinNo) == LOW)
      { 
      if (inputFlag == true)  {
         inputFlag = false;
         digitalWrite(RELAY_PIN, LOW);
        //  indicate(inputs[1].mode);
         Serial.println(inputs[1].name);
         delay(50);
        }
      }
      else if (digitalRead(inputs[2].pinNo) == LOW)
      { 
      if (inputFlag == true)  {
         inputFlag = false;
         digitalWrite(RELAY_PIN, HIGH);
        //  indicate(inputs[2].mode);
         Serial.println(inputs[2].name);
         delay(50);
        }
      }
      else if (digitalRead(inputs[5].pinNo) == LOW)
      { 
      if (inputFlag == true)  {
         inputFlag = false;
         indicate(inputs[5].mode);
         Serial.println(inputs[5].name);
         delay(100);
        }
      }
     else if (inputFlag == false) {
        inputFlag = true;
        delay(20);
      }

  
  Female_catheter();
//  Male_catheter();
 
  if ((millis() - t1) > 500)
  {
    t1 = millis();
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GRN, LOW);
    digitalWrite(BUZZER, LOW);

//    if (deviceConnected)
//    {
//      if (inputState == false)
//      {
//        pCharacteristic->setValue("NO_INPUT");
//        pCharacteristic->notify();
//        Serial.println("NO_INPUT");
//        digitalWrite(ONBOARD_LED, LOW);
//      }
//    }
//    else
//    {
//      digitalWrite(ONBOARD_LED, !digitalRead(ONBOARD_LED));
//    }
  }

}

void Female_catheter() {
    if (digitalRead(Switch1) == LOW)  {
      if(flag == 0) {    
        flag = 1;
        Serial.println("FC_TRUE");
        delay(20);
      }
    }
    else if (digitalRead(Switch2) == LOW) { 
        if(flag == 0) {
          flag = 1;
          Serial.println("FC_VAGINA");
          delay(20);
        } 
     }
    else if (flag == 1) {
        flag = 0; 
        delay(20); 
     }
}

void Male_catheter() {
  if (digitalRead(Switch3) == LOW)
  {
    unsigned long t1 = millis();
    Serial.println("Started");
    status = true;
    while ((millis() - t1) < 5000)
    {
      if (digitalRead(Switch4) == LOW)
      {
        status = false;
        Serial.println("FALSE");
        while (digitalRead(Switch4) == LOW);
        delay(100);
        break;
      }
      delay(50);
    }
    if (status)
    {
      Serial.println("TRUE");
    }
  }
  else if (digitalRead(Switch4) == LOW)
      {
       if(mc_flag == 1) {
          mc_flag = 0;
          Serial.println("FALSE");
          delay(20);
       }
      }
  else if(mc_flag == 0) {
        mc_flag = 1;
        delay(20);
   }
}

