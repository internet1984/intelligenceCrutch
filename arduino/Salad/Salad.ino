#include <Ultrasonic.h>
#include <SoftwareSerial.h>   //Software Serial Port

#define VibraLeft_PIN       7
#define VibraMiddle_PIN     6 
#define VibraRight_PIN      4

#define Button_PIN          8
#define ULTRASONIC1_PIN    A0
#define ULTRASONIC2_PIN    A1
#define DISTANCE  100
#define RxD 2
#define TxD 3

Ultrasonic ultrasonicAir1(ULTRASONIC1_PIN);
Ultrasonic ultrasonicAir2(ULTRASONIC2_PIN);
SoftwareSerial BLE(RxD,TxD);
unsigned int button=0;

void setup()
{ 
	pinMode(VibraLeft_PIN, OUTPUT );
        pinMode(VibraMiddle_PIN, OUTPUT );
        pinMode(VibraRight_PIN, OUTPUT );
        pinMode(Button_PIN, INPUT );
        
	Serial.begin(9600);

	pinMode(RxD, INPUT);
	pinMode(TxD, OUTPUT);
	setupBleConnection();
	delay(50);
	Serial.println("Test.\r\n"); 
}

void loop()
{       char recvChar;
        unsigned int ultrasonicAirDistance1 = 0;
	unsigned int ultrasonicAirDistance2 = 0;
        
        
        ultrasonicAirDistance1 = ultrasonicAir1.MeasureInCentimeters();
	ultrasonicAirDistance2 = ultrasonicAir2.MeasureInCentimeters();	
        button=digitalRead(Button_PIN);
         if(button==HIGH )  {BLE.print(3); Serial.println("click");}
         while (BLE.available()){//check if there's any data sent from the remote BLE 
          recvChar = BLE.read();
          if (recvChar ==49) { BuzzLeft();BuzzLeft();}
          if( recvChar ==50) {BuzzRight(); BuzzRight();}
          
                
          Serial.println((int)recvChar);
         }
          
      while (Serial.available()){//check if there's any data sent from the local serial terminal, you can add the other applications here
      recvChar  = Serial.read();    
      BLE.print(recvChar); 
    }
      Serial.print("ultra1:");
      Serial.print(ultrasonicAirDistance1);
      Serial.print(" ultra2:");
      Serial.println(ultrasonicAirDistance2);
                
	if(ultrasonicAirDistance1<DISTANCE||ultrasonicAirDistance2<DISTANCE) {int ultrasonicAirDistance=(ultrasonicAirDistance1+ultrasonicAirDistance2);Buzz(ultrasonicAirDistance); Serial.println("stop");}
        delay(5); 
        
}
 
void Buzz(int Time)
{
    digitalWrite(VibraMiddle_PIN, HIGH);     
    delay(Time);                        
    digitalWrite(VibraMiddle_PIN, LOW);         
    delay(Time); 
} 


void BuzzLeft()
{
    digitalWrite(VibraMiddle_PIN , HIGH);         
    delay(500);                        
    digitalWrite(VibraMiddle_PIN , LOW);
    delay(100) ;
    digitalWrite(VibraLeft_PIN , HIGH);         
    delay(500);                        
    digitalWrite(VibraLeft_PIN , LOW);         
}

void BuzzRight()
{
    digitalWrite(VibraMiddle_PIN , HIGH);         
    delay(500);                        
    digitalWrite(VibraMiddle_PIN , LOW);
    delay(100) ;
    digitalWrite(VibraRight_PIN , HIGH);         
    delay(500);                        
    digitalWrite(VibraRight_PIN , LOW);     
}

void setupBleConnection()
{
  BLE.begin(9600); //Set BLE BaudRate to default baud rate 9600
  BLE.print("AT+ROLE0"); //set the bluetooth name as a slaver
  BLE.print("AT+SAVE1");  //don't save the connect information
  BLE.print("AT+NAME LuYou");//rename as LuYou
  
}
