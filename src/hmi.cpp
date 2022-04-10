#include "hmi.h"
#include "temp_meas.h"
#include <EEPROM.h>
// #include <LiquidCrystal.h>
#include <LCD_I2C.h>
#include <Keypad.h>
// --- Contants -----------------------------------------
#define TEMP_REQ_SMALL_LOOP_ADD     0
#define TEMP_REQ_BIG_LOOP_ADD       4

// --- Local Functions Keypad -----------------------------------------
void hmi_dispSensorTest(void);
float hmi_checkTempRange(float temp);

void hmi_dispTepPrivod(void);
void hmi_dispTepSpiatokcka(void);
void hmi_dispTepPec(void);
void hmi_dispTepTankUp(void);
void hmi_dispTepTankDown(void);
void hmi_dispTepSmall(void);
void hmi_dispTepReqSmall(void);
void hmi_dispTepBig(void);
void hmi_dispTepReqBig(void);

// --- Local Variable - Keypad ----------------------------------------
const byte ROWS = 4; //řádky
const byte COLS = 4; //sloupce

char hexaKeys[ROWS][COLS] = {  //3D pole s klávesami, jak jsou
{'1','2','3','A'},  //první řádek klávesnice
{'4','5','6','B'},  //druhý řádek
{'7','8','9','C'},  //třetí
{'*','0','#','D'}}; //a čtvrtý

byte rowPins[ROWS] = {46,48,50,52}; /* piny řádků, dodržujte pořadí odshora dolu,
 aby 3D pole bylo takové, jaká je klávesnice */
byte colPins[COLS] = {47,49,51,53}; // piny sloupců, pamatujte na zleva doprava

Keypad myKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// --- Local Variable - LCD --------------------------------------------
// const int rs = 34, en = 32, d4 = 28, d5 = 26, d6 = 24, d7 = 22;
// LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
LCD_I2C lcd(0x27, 16, 2); // Default address of most PCF8574 modules, change according
String inStr = "";
float reqBigLoop = 0;
float reqSmallLoop = 0;
char readKey;
HMIstate_T hmiStates[HMI_CNT] = 
{
  {.hmiID = (HMIstates_E)0,
   .fun_ptr = &hmi_dispTepPrivod 
  },
  {.hmiID = (HMIstates_E)1,
   .fun_ptr = &hmi_dispTepSpiatokcka    
  },
  {.hmiID = (HMIstates_E)2,
   .fun_ptr = &hmi_dispTepPec    
  },
  {.hmiID = (HMIstates_E)3,
   .fun_ptr = &hmi_dispTepTankUp    
  },
  {.hmiID = (HMIstates_E)4,
   .fun_ptr = &hmi_dispTepTankDown    
  },
  {.hmiID = (HMIstates_E)5,
   .fun_ptr = &hmi_dispTepSmall    
  },
  {.hmiID = (HMIstates_E)6,
   .fun_ptr = &hmi_dispTepReqSmall    
  },
  {.hmiID = (HMIstates_E)7,
   .fun_ptr = &hmi_dispTepBig    
  },
  {.hmiID = (HMIstates_E)8,
   .fun_ptr = &hmi_dispTepReqBig    
  }
};

HMIstates_E currHmiState;

// --- Public Functions ----------------------------------------------------
void hmi_init(void)
{
  // set up the LCD's number of columns and rows:
  lcd.begin();
  lcd.backlight();
  lcd.print(F(" Heating system "));
  lcd.setCursor(0, 1);
  lcd.print(F("---Made by LT---"));
  delay(2000);
  lcd.clear();
  lcd.print(F("Loading."));
  delay(800);
  lcd.clear();
  
  // while(1)
  // {
  //   readKey = myKeypad.getKey();

    
    
  //   if (readKey){
  //     Serial.println(readKey);
  //     lcd.clear();
  //     lcd.print(readKey);
  //   }
  // }

  currHmiState = (HMIstates_E)0;
  hmiStates[currHmiState].fun_ptr();

  EEPROM.get(TEMP_REQ_SMALL_LOOP_ADD,reqSmallLoop);
  EEPROM.get(TEMP_REQ_BIG_LOOP_ADD,reqBigLoop);
}

void hmi_process(void)
{
  switch(currHmiState)
  {
    case HMI_TEMP_PRIVOD: 
    {
      readKey = myKeypad.getKey();
      switch(readKey)
      {
        case 'A': lcd.write(readKey); break; 
        case 'B': lcd.write(readKey); break; 
        case 'C': lcd.write(readKey); break; 
        case 'D': lcd.write(readKey); break; 
        case '0': break; 
        case '1': currHmiState = HMI_TEMP_PRIVOD;         break; 
        case '2': currHmiState = HMI_TEMP_SPIATOCKA;      break; 
        case '3': currHmiState = HMI_TEMP_PEC;            break; 
        case '4': currHmiState = HMI_TEMP_NADRZ_TOP;      break; 
        case '5': currHmiState = HMI_TEMP_NADRZ_BOT;      break; 
        case '6': currHmiState = HMI_TEMP_SMALL_LOOP;     break; 
        case '7': currHmiState = HMI_TEMP_BIG_LOOP;       break; 
        case '8': break; 
        case '9': break; 
        case '*': break; 
        case '#': break; 
      }
      hmiStates[currHmiState].fun_ptr();
    } break;
    
    case HMI_TEMP_SPIATOCKA: 
    {
      readKey = myKeypad.getKey();
      switch(readKey)
      {
        case 'A': lcd.write("A");break; 
        case 'B': lcd.write("B"); break; 
        case 'C': lcd.write("C"); break; 
        case 'D': lcd.write("D"); break; 
        case '0': break; 
        case '1': currHmiState = HMI_TEMP_PRIVOD;         break; 
        case '2': currHmiState = HMI_TEMP_SPIATOCKA;      break; 
        case '3': currHmiState = HMI_TEMP_PEC;            break; 
        case '4': currHmiState = HMI_TEMP_NADRZ_TOP;      break; 
        case '5': currHmiState = HMI_TEMP_NADRZ_BOT;      break; 
        case '6': currHmiState = HMI_TEMP_SMALL_LOOP;     break; 
        case '7': currHmiState = HMI_TEMP_BIG_LOOP;       break;  
        case '8': break;  
        case '9': break; 
        case '*': break; 
        case '#': break; 
      }
      hmiStates[currHmiState].fun_ptr();
    }break;

    case HMI_TEMP_PEC: 
    {
      readKey = myKeypad.getKey();
      switch(readKey)
      {
        case 'A': lcd.write("A");break; 
        case 'B': lcd.write("B"); break; 
        case 'C': lcd.write("C"); break; 
        case 'D': lcd.write("D"); break; 
        case '0': break; 
        case '1': currHmiState = HMI_TEMP_PRIVOD;         break; 
        case '2': currHmiState = HMI_TEMP_SPIATOCKA;      break; 
        case '3': currHmiState = HMI_TEMP_PEC;            break; 
        case '4': currHmiState = HMI_TEMP_NADRZ_TOP;      break; 
        case '5': currHmiState = HMI_TEMP_NADRZ_BOT;      break; 
        case '6': currHmiState = HMI_TEMP_SMALL_LOOP;     break; 
        case '7': currHmiState = HMI_TEMP_BIG_LOOP;       break; 
        case '8': break; 
        case '9': break; 
        case '*': break; 
        case '#': break; 
      }
      hmiStates[currHmiState].fun_ptr();
    } break;

    case HMI_TEMP_NADRZ_TOP: 
    {
      readKey = myKeypad.getKey();
      switch(readKey)
      {
        case 'A': lcd.write("A");break; 
        case 'B': lcd.write("B"); break; 
        case 'C': lcd.write("C"); break; 
        case 'D': lcd.write("D"); break; 
        case '0': break; 
        case '1': currHmiState = HMI_TEMP_PRIVOD;         break; 
        case '2': currHmiState = HMI_TEMP_SPIATOCKA;      break; 
        case '3': currHmiState = HMI_TEMP_PEC;            break; 
        case '4': currHmiState = HMI_TEMP_NADRZ_TOP;      break; 
        case '5': currHmiState = HMI_TEMP_NADRZ_BOT;      break; 
        case '6': currHmiState = HMI_TEMP_SMALL_LOOP;     break; 
        case '7': currHmiState = HMI_TEMP_BIG_LOOP;       break;  
        case '8': break; 
        case '9': break; 
        case '*': break; 
        case '#': break; 
      }
      hmiStates[currHmiState].fun_ptr();
    } break;

    case HMI_TEMP_NADRZ_BOT: 
    {
      readKey = myKeypad.getKey();
      switch(readKey)
      {
        case 'A': break; 
        case 'B': break; 
        case 'C': break; 
        case 'D': break; 
        case '0': break; 
        case '1': currHmiState = HMI_TEMP_PRIVOD;         break; 
        case '2': currHmiState = HMI_TEMP_SPIATOCKA;      break; 
        case '3': currHmiState = HMI_TEMP_PEC;            break; 
        case '4': currHmiState = HMI_TEMP_NADRZ_TOP;      break; 
        case '5': currHmiState = HMI_TEMP_NADRZ_BOT;      break; 
        case '6': currHmiState = HMI_TEMP_SMALL_LOOP;     break; 
        case '7': currHmiState = HMI_TEMP_BIG_LOOP;       break;  
        case '8': break; 
        case '9': break; 
        case '*': break; 
        case '#': break; 
      }
      hmiStates[currHmiState].fun_ptr();
    } break;

    case HMI_TEMP_SMALL_LOOP: 
    {
      readKey = myKeypad.getKey();
      switch(readKey)
      {
        case 'A': break; 
        case 'B': break; 
        case 'C': currHmiState = HMI_TEMP_REQ_SMALL_LOOP; break; 
        case 'D': break; 
        case '0': break; 
        case '1': currHmiState = HMI_TEMP_PRIVOD;         break; 
        case '2': currHmiState = HMI_TEMP_SPIATOCKA;      break; 
        case '3': currHmiState = HMI_TEMP_PEC;            break; 
        case '4': currHmiState = HMI_TEMP_NADRZ_TOP;      break; 
        case '5': currHmiState = HMI_TEMP_NADRZ_BOT;      break; 
        case '6': currHmiState = HMI_TEMP_SMALL_LOOP;     break; 
        case '7': currHmiState = HMI_TEMP_BIG_LOOP;       break; 
        case '8': break; 
        case '9': break; 
        case '*': break; 
        case '#': break; 
      }
      hmiStates[currHmiState].fun_ptr();
    } break;

    case HMI_TEMP_REQ_SMALL_LOOP: 
    {
      readKey = myKeypad.getKey();
      switch(readKey)
      {
        case 'A': break; 
        case 'B': currHmiState = HMI_TEMP_SMALL_LOOP; inStr = ""; break; 
        case 'C': break; 
        case 'D': reqSmallLoop = hmi_checkTempRange(inStr.toFloat());
                  inStr = ""; 
                  currHmiState = HMI_TEMP_SMALL_LOOP; 
                  EEPROM.put(TEMP_REQ_SMALL_LOOP_ADD,reqSmallLoop);
                  break; 
        case '0': lcd.write(readKey); inStr += readKey; break; 
        case '1': lcd.write(readKey); inStr += readKey; break; 
        case '2': lcd.write(readKey); inStr += readKey; break; 
        case '3': lcd.write(readKey); inStr += readKey; break; 
        case '4': lcd.write(readKey); inStr += readKey; break; 
        case '5': lcd.write(readKey); inStr += readKey; break; 
        case '6': lcd.write(readKey); inStr += readKey; break; 
        case '7': lcd.write(readKey); inStr += readKey; break; 
        case '8': lcd.write(readKey); inStr += readKey; break; 
        case '9': lcd.write(readKey); inStr += readKey; break; 
        case '*': break; 
        case '#': break; 
      }
    } break;

    case HMI_TEMP_BIG_LOOP: 
    {
      readKey = myKeypad.getKey();
      switch(readKey)
      {
        case 'A': break; 
        case 'B': break; 
        case 'C': currHmiState = HMI_TEMP_REQ_BIG_LOOP;   break; 
        case 'D': break; 
        case '0': break; 
        case '1': currHmiState = HMI_TEMP_PRIVOD;         break; 
        case '2': currHmiState = HMI_TEMP_SPIATOCKA;      break; 
        case '3': currHmiState = HMI_TEMP_PEC;            break; 
        case '4': currHmiState = HMI_TEMP_NADRZ_TOP;      break; 
        case '5': currHmiState = HMI_TEMP_NADRZ_BOT;      break; 
        case '6': currHmiState = HMI_TEMP_SMALL_LOOP;     break; 
        case '7': currHmiState = HMI_TEMP_BIG_LOOP;       break; 
        case '8': break; 
        case '9': break; 
        case '*': break; 
        case '#': break; 
      }
      hmiStates[currHmiState].fun_ptr();
    } break;

    case HMI_TEMP_REQ_BIG_LOOP: 
    {
      readKey = myKeypad.getKey();
      switch(readKey)
      {
        case 'A': break; 
        case 'B': currHmiState = HMI_TEMP_BIG_LOOP; inStr = ""; break; 
        case 'C': break; 
        case 'D': reqBigLoop = hmi_checkTempRange(inStr.toFloat());
                  inStr = "";
                  currHmiState = HMI_TEMP_BIG_LOOP; 
                  EEPROM.put(TEMP_REQ_BIG_LOOP_ADD,reqBigLoop);
                  break; 
        case '0': lcd.write(readKey); inStr += readKey; break; 
        case '1': lcd.write(readKey); inStr += readKey; break; 
        case '2': lcd.write(readKey); inStr += readKey; break; 
        case '3': lcd.write(readKey); inStr += readKey; break; 
        case '4': lcd.write(readKey); inStr += readKey; break; 
        case '5': lcd.write(readKey); inStr += readKey; break; 
        case '6': lcd.write(readKey); inStr += readKey; break; 
        case '7': lcd.write(readKey); inStr += readKey; break; 
        case '8': lcd.write(readKey); inStr += readKey; break; 
        case '9': lcd.write(readKey); inStr += readKey; break; 
        case '*': break; 
        case '#': break; 
      }
    } break;
  }
}

float hmi_readReqSmallLoop(void)
{
  return reqSmallLoop;
}

float hmi_readReqBigLoop(void)
{
  return reqBigLoop;
}

// --- Local Functions ----------------------------------------------------

void hmi_dispTepPrivod(void) // --------- PRIVOD ----------------------
{
  lcd.clear();
  lcd.print(F("[1]Tep.privod"));
  lcd.setCursor(0,1);
  lcd.print(F("T="));

  // Diplay if connected
  if (Temp_isConnected(TEMP_STOVE_OUT) == SENS_CONNECTED)
  {
    lcd.print(Temp_ReadVal(TEMP_STOVE_OUT));
    lcd.print(F("C"));  
  }
  else
    lcd.print(F("Not connected"));
}

void hmi_dispTepSpiatokcka(void) // --------- SPIATOCKA ---------------
{
  lcd.clear();
  lcd.print("[2]Tep.spiatoc");
  lcd.setCursor(0,1);
  lcd.print("T=");

  // Diplay if connected
  if (Temp_isConnected(TEMP_STOVE_IN) == SENS_CONNECTED)
  {
    lcd.print(Temp_ReadVal(TEMP_STOVE_IN));
    lcd.print("C");  
  }
  else
    lcd.print("Not connected");
}

void hmi_dispTepPec(void) // --------- PEC ----------------------------
{
  lcd.clear();
  lcd.print("[3]Tep.pec");
  lcd.setCursor(0,1);
  lcd.print("T=");
  
  // Diplay if connected
  if (Temp_isConnected(TEMP_STOVE) == SENS_CONNECTED)
  {
    lcd.print(Temp_ReadVal(TEMP_STOVE));
    lcd.print("C");  
  }
  else
    lcd.print("Not connected");
}

void hmi_dispTepTankUp(void) // --------- TANK UP ---------------------
{
  lcd.clear();
  lcd.print("[4]Tep.tank up");
  lcd.setCursor(0,1);
  lcd.print("T=");
  
  // Diplay if connected
  if (Temp_isConnected(TEMP_TANK_TOP) == SENS_CONNECTED)
  {
    lcd.print(Temp_ReadVal(TEMP_TANK_TOP));
    lcd.print("C");  
  }
  else
    lcd.print("Not connected");
}

void hmi_dispTepTankDown(void) // --------- TANK DOWN -----------------
{
  lcd.clear();
  lcd.print("[5]Tep.tank down");
  lcd.setCursor(0,1);
  lcd.print("T=");
  
  // Diplay if connected
  if (Temp_isConnected(TEMP_TANK_BOTTON) == SENS_CONNECTED)
  {
    lcd.print(Temp_ReadVal(TEMP_TANK_BOTTON));
    lcd.print("C");  
  }
  else
    lcd.print("Not connected");
}

void hmi_dispTepSmall(void) // --------- SMALL LOOP -------------------
{
  lcd.clear();
  lcd.print("[6]Poz.Tep.maly");
  lcd.setCursor(0,1);
  lcd.print("T=");
  lcd.print(reqSmallLoop);
  lcd.print("C");
}

void hmi_dispTepReqSmall(void) // --------- REQ SMALL LOOP ------------
{
  lcd.clear();
  lcd.print("TepMaly[10-100]");
  lcd.setCursor(0,1);
  lcd.print("T=");
}

void hmi_dispTepBig(void) // --------- BIG LOOP -----------------------
{
  lcd.clear();
  lcd.print("[7]Poz.Tep.velky");
  lcd.setCursor(0,1);
  lcd.print("T=");
  lcd.print(reqBigLoop);
  lcd.print("C");  
}

void hmi_dispTepReqBig(void) // --------- REQ BIG LOOP ----------------
{
  lcd.clear();
  lcd.print("TepVelky[10-100]");
  lcd.setCursor(0,1);
  lcd.print("T=");
}

float hmi_checkTempRange(float temp) // ------CHECK RANGE -------------
{
  float tmp;
  if (temp < 10)
    tmp = 10;
  else if (temp > 100)
    tmp = 100;
  else
    tmp = temp;

  return tmp;    
}
