///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////                                             ///////
/////// Ingenieursproject: Contactloze coronazuil   ///////
///////                    ZENDER                   ///////
///////                   Groep 4                   ///////
///////               David Michiels                ///////
///////                 25/01/2020                  ///////
///////                                             ///////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////          
          
          ///// TRANSMITTER  /////
          #include <SPI.h>  
          #include "RF24.h" 

          RF24 myRadio (3, 2); //CE, CSN
          const byte address[6] = "00001";
          
          int voet = 0;
          

          ///// DISTANCE SENSOR  /////
          #define trigPin 10
          #define echoPin 9
          long  duration;
          float distanceCm;

          ///// BUZZER /////
          const int buzzer = 5; //buzzer to arduino pin 5

void setup() 
{
          Serial.begin(115200);
          
          ///// TRANSMITTER  /////
          delay(100);
          myRadio.begin();  
          myRadio.setChannel(115);
          myRadio.setPALevel(RF24_PA_MIN);              //power amplifier level
          myRadio.setDataRate(RF24_250KBPS) ; 
          myRadio.openWritingPipe(address);
          myRadio.stopListening();
          delay(100);

          ///// DISTANCE SENSOR  /////
          pinMode(echoPin, INPUT);
          pinMode(trigPin, OUTPUT);
          
          ///// BUZZER  /////
          pinMode(buzzer, OUTPUT); // Pin 5 definiëren als output
}

void loop()
{

          ///// TRANSMITTER /////
          if(voet == 1){
          voet = 0;
          delay(4000); // Even wachten nadat signaal 'voet' is verzonden. Anders beep beep beep...
          }
          
          ///// DISTANCE SENSOR /////
          digitalWrite(trigPin, LOW);
          delayMicroseconds(2);
          digitalWrite(trigPin, HIGH);
          delayMicroseconds(10);
          digitalWrite(trigPin, LOW);
          
          duration = pulseIn(echoPin, HIGH);
          
          distanceCm = (duration * 0.0133 /2)* 2.54;
          Serial.print("Afstand in cm: "); Serial.println(distanceCm);
          delay(100); // adempauze om de microprocessor niet onnodig te belasten
          
          if(distanceCm > 8){
            voet = 1;

          ///// BUZZER /////
          tone(buzzer, 3000); // Send 3KHz sound signal...
          delay(300);        // ...for 300 ms
          noTone(buzzer);     // Stop sound...

                          struct package
                          {
                          int voet = 1;
                           };
 
    typedef struct package Package;
    Package data;

         myRadio.write(&data, sizeof(data));
         delay(100);
          }

}
