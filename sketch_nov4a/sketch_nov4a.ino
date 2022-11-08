#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4); 

//updated

//Sonar Sensor 1
#define Trig1 D0
#define Echo1 D3

//Sonar Sensor 2
#define Trig2 D4
#define Echo2  D5

//float switch
#define Float1  D6 //float switch 1
#define Float2  D7 //float switch 2

//Ph sensor analog pin
#define phSensor A0

//water pump
#define waterPump  D9

//solenoid valve
#define valve  D10


void setup() {
  Serial.begin(9600);
  lcd.init(); 
  lcd.backlight(); 

// Sonar Sensors
  pinMode(Trig1, OUTPUT);
  pinMode(Echo1, INPUT);
  pinMode(Trig2, OUTPUT);
  pinMode(Echo2, INPUT);

//float sensor
  pinMode(Float1, INPUT_PULLUP);
  pinMode(Float2, INPUT_PULLUP);

//water pump
  pinMode(waterPump, OUTPUT);

//solenoid valve  
  pinMode(valve, OUTPUT);
}

void loop() {



//LCD print  
  LCD_PRINT(
    tankLevel(Trig1,Echo1,21,digitalRead(Float1)), //sonar 1
    tankLevel(Trig2,Echo2,20,digitalRead(Float2)), //sonar 2
    phLevel(), //Ph Level
    100, //Humidity
    100,  //Tenperature
    digitalRead(Float1),
    digitalRead(Float2)
    );

}

//call this function to print something in my LCD
void LCD_PRINT(
  int sonar1,
  int sonar2,
  float phLvl,
  int humid,
  int soilM,
  int floatss1,
  int floatss2
  ){
  lcd.clear();

//  Tank Level
  lcd.print("T1: " + String(sonar1) + "%");
  lcd.print(" ");
  lcd.print("T2: " + String(sonar2) + "%");

  lcd.setCursor(0,1);

// PH Level
  lcd.print("pH level :" + String(phLvl) + "%");
  
  lcd.setCursor(0,2);
  

//// Humid
//  
//  lcd.print("Humd: " + String(humid) + "% ");
//  
////Temperature
//  lcd.print("Tmp: " + String(soilM) + "%");


  
// float sensor

//  lcd.setCursor(0,2);
  floatss1? lcd.print("Tank 1 is not full") : lcd.print("Tank 1 is full");
  lcd.setCursor(0,3);
  floatss2? lcd.print("Tank 2 is not full") : lcd.print("Tank 2 is full");

  if (sonar2 <= 50 ){

        digitalWrite(waterPump,LOW);
          digitalWrite(valve,LOW);
               Serial.println("BOMBA");
  }
  else if(sonar2 >= 100){


 digitalWrite(waterPump,HIGH);
  digitalWrite(valve,HIGH);
  Serial.println("Tigil bomba");
  }
   else if(sonar1 >= 100 || sonar2 >= 100){


 digitalWrite(waterPump,HIGH);
  digitalWrite(valve,HIGH);
  Serial.println("Tigil bomba");
  }
  else{

  Serial.println("PARO pARO g");


  }
if (sonar1 <= 50 ){

          digitalWrite(valve,HIGH);
               Serial.println("BOMBA");
  }

  delay(1000);





}

// Group 1: Tank Filteration 

//for tank level
int tankLevel(int Trig, int Echo, int lvel, int floatLevel){
  
  digitalWrite(Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);

//  duration
  long duration = pulseIn(Echo, HIGH);

//return an inches
  int inches = duration / 74 / 2;

  if (floatLevel){
    return inches * 100 / lvel;
  }else{
    return 100;
  }

    // return inches;
}


//for ph sensor
int buffer_arr[10],temp;

float phLevel(){
  float calibration_value = 21.34 + 1.80;
//    float calibration_value = 21.34;
  unsigned long int avgval = 0; 
  
  for(int i=0;i<10;i++) 
    { 
      buffer_arr[i]=analogRead(A0); //push the ph raw data to array
      delay(30);
    }

//ascending the raw data from ph sensor
  for(int i=0;i<9;i++)
    {
      for(int j=i+1;j<10;j++)
        {
          if(buffer_arr[i]>buffer_arr[j])
            {
              temp=buffer_arr[i];
              buffer_arr[i]=buffer_arr[j];
                buffer_arr[j]=temp;
            }
        }
      }    

// pass the raw ascending to avgVal
  for(int i=2;i<8;i++) avgval+=buffer_arr[i];

// compute the raw data into pH level
  float volt=(float)avgval*5.0/1024/6;
  float ph_act = -5.70 * volt + calibration_value;

  return ph_act;
}

//turn off the water tank
void turnOff(int Floats1, int Floats2){
//if(Floats1 == 0) 
 // {
 //   digitalWrite(waterPump,HIGH);
    //  Serial.println("Turn OFF WATER pump");

 // }else{
 // digitalWrite(waterPump,LOW);
    // Serial.println("Turn On WATER pump");
  //}

//if(Floats2 == 0) 
  //{
  //  digitalWrite(valve,HIGH);
    //  Serial.println("Turn OFF WATER pump");

 // }else{
 // digitalWrite(valve,LOW);
    // Serial.println("Turn On WATER pump");
 // }
}
