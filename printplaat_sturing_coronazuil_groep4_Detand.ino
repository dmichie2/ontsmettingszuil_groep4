///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////                                             ///////
/////// Ingenieursproject: Contactloze coronazuil   ///////
///////                  ONTVANGER                  ///////
///////                   Groep 4                   ///////
///////               David Michiels                ///////
///////                 25/01/2020                  ///////
///////                                             ///////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

          ///// BUZZER /////
          const int buzzer = A4;
          
          ///// LEDSTRIP /////
          #include <Adafruit_NeoPixel.h>
          
          #define LEDPIN     4
          #define NUMPIXELS 60
          Adafruit_NeoPixel pixels(NUMPIXELS, LEDPIN, NEO_GRB + NEO_KHZ800);
          Adafruit_NeoPixel strip(NUMPIXELS, LEDPIN, NEO_GRB + NEO_KHZ800);

          ///// RADIO COMMUNICATION /////
          #include "RF24.h"
          #include <SPI.h>

          ///// 7-SEGMENT 4-DIGIT DISPLAY /////
          #include <TM1637Display.h>
          #define CLK_1 6
          #define CLK_2 5
          #define DIO_1 7
          #define DIO_2 8

          int dagelijks = 37;
          int totaal = 256;

          int interrupt_effect = 0;
           
          TM1637Display display_1 = TM1637Display(CLK_1, DIO_1);
          TM1637Display display_2 = TM1637Display(CLK_2, DIO_2);
                
                // Een array dat alle segmenten inschakelt
                const uint8_t data_display[] = {0xff, 0xff, 0xff, 0xff};
                
                // Een array dat alle segmenten uitschakelt
                const uint8_t blank[] = {0x00, 0x00, 0x00, 0x00};
                
                // De individuele segmenten instellen om een woord te spellen
                const uint8_t HERO[] = {
                  SEG_B | SEG_C | SEG_E | SEG_F | SEG_G,           // H
                  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G,           // E
                  SEG_F | SEG_E | SEG_D,                           // L
                  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G            // d
                };
                
          ///// DISTANCE SENSOR  /////
          #define trigPin 10
          #define echoPin 9
          long  duration;
          float distanceCm;
          int   hand = 0;

          ///// RECEIVER  /////
          RF24 myRadio (3, 2);     //CE, CSN
          struct package
          {
            int voet;
          };          
          typedef struct package Package;
          Package data;
          const byte address[6] = "00001";


void setup() {

          Serial.begin(115200);

          ///// BUZZER  /////
          pinMode(buzzer, OUTPUT); // Pin 5 definiëren als output
          
          ///// LEDSTRIP  /////
          pixels.begin();

          ///// MAGNEETVENTIEL  /////
          pinMode(A2, OUTPUT);
          digitalWrite(A2, HIGH);
  
          ///// DISTANCE SENSOR  /////
          pinMode(echoPin, INPUT);
          pinMode(trigPin, OUTPUT);

          ///// RECEIVER  /////
          myRadio.begin();
          myRadio.setChannel(115);
          myRadio.setPALevel(RF24_PA_MIN);                //power amplifier level
          myRadio.setDataRate(RF24_250KBPS); 
          myRadio.openReadingPipe(0, address);
          myRadio.startListening();

          ///// 7-SEGMENT 4-DIGIT DISPLAY /////
          display_1.clear();
          display_2.clear();

          ///// LEDSTRIP /////
          pixels.clear();
          pixels.show();
          strip.begin();
          strip.show();
          strip.setBrightness(150);    // max = 255


                    
}

void colorWipe(uint32_t color) {
  for(int i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, color);        
    strip.show();                         
    delay(30);         // Tijdsinterval tussen het oplichten van ledjes                 
  }
}

void rainbow(int wait) {  

  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
          
    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
      
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show();
    delay(wait);
  }
}

void loop() {
  
          /////  RECEIVER  /////
            while (myRadio.available())
            {
              myRadio.read( &data, sizeof(data) );
              Serial.print("Voet: "); Serial.println(data.voet);
            }

          //LEDSTRIP EERSTE (RODE) BAND   
          if(data.voet == 1){
              for(int i=6; i<10; i++) {
    
              pixels.setPixelColor(i, pixels.Color(100, 0, 0));
          
              pixels.show();
          
              delay(30); // Tijdsinterval tussen het oplichten van ledjes
              }

              for(int i=22; i<61; i++) {
    
              pixels.setPixelColor(i, pixels.Color(200, 150, 0));
          
              pixels.show();
         
              }
          }
          
          ///// DISTANCE SENSOR /////
          digitalWrite(trigPin, LOW);
          delayMicroseconds(2);
          digitalWrite(trigPin, HIGH);
          delayMicroseconds(10);
          digitalWrite(trigPin, LOW);
          
          duration = pulseIn(echoPin, HIGH);
          
          distanceCm = (duration * 0.0133 /2)* 2.54;
//          Serial.print("Afstand in cm: "); Serial.println(distanceCm);

            if(data.voet == 1 && distanceCm < 8){
                
                data.voet = 0;

                ///// BUZZER /////
                tone(buzzer, 3000); // Send 3KHz sound signal...
                delay(300);        // ...for 300 ms
                noTone(buzzer);     // Stop sound...
  
                //LEDSTRIP TWEEDE (GELE) BAND   
                for(int i=10; i<14; i++) {
      
                pixels.setPixelColor(i, pixels.Color(50, 50, 0));
            
                pixels.show();
            
                delay(30); // Tijdsinterval tussen het oplichten van ledjes
                }
                
                Serial.println("Hand AANWEZIG");  
                hand = 1; 

                ///// MAGNEETVENTIEL /////
                digitalWrite(A2, LOW);
                delay(3000);
                digitalWrite(A2, HIGH);

                
                ///// 7-SEGMENT 4-DIGIT DISPLAYS /////
                // Set the brightness:
                display_1.setBrightness(5);
                display_2.setBrightness(5);
      
                // Aantal dagelijkse gebruikers en totaal aantal gebruikers
                display_1.setSegments(HERO);
                delay(1000);
                display_1.clear();
                display_2.clear();
                display_1.showNumberDec(dagelijks - 1);
                display_2.showNumberDec(totaal - 1);
                delay(500);
                display_1.clear();
                display_2.clear();
                display_1.showNumberDec(dagelijks);
                display_2.showNumberDec(totaal);
      
      
               //LEDSTRIP LAATSTE (GROENE) BAND   
               for(int i=22; i<61; i++) {
              
              pixels.setPixelColor(i, pixels.Color(0, 0, 0));
          
              pixels.show();
         
              }
               
               for(int i=14; i<22; i++) {

                  /////  ALS DE VOET GEDETECTEERD WORDT, DAN STOPT DE LOOP EN BEGINT HET PROGRAMMA OPNIEUW  /////
                  while (myRadio.available())
                  {
                    myRadio.read( &data, sizeof(data) );
                    Serial.print("Voet: "); Serial.println(data.voet);
                  }

                  if(data.voet == 1){
                    interrupt_effect = 1;
                    break;
                    }
      
              pixels.setPixelColor(i, pixels.Color(0, 50, 0));
          
              pixels.show();
          
              delay(2500); // Tijdsinterval tussen het oplichten van ledjes --> totale handwrijftijd (20) gedeeld door het aantal ledjes (8) = 2,5 seconden
              }
              
              
              /// SPECIAL EFFECT EINDE CYCLUS
            /*  colorWipe(strip.Color(100,   0,   0)); // Rood         
              colorWipe(strip.Color(  100, 100,   0)); // Geel           
              colorWipe(strip.Color(  0,   100, 0)); // Groen   */
              if (interrupt_effect != 1){
              rainbow(2);
              }
      
              /// TOESTEL KLAARZETTEN VOOR VOLGENDE GEBRUIKER
              pixels.clear(); // Set all pixel colors to 'off'
              pixels.show();
              dagelijks ++;
              totaal ++;
              interrupt_effect = 0;
      
              //De tellerdisplays uitschakelen (stroom besparen)
              display_1.clear();
              display_2.clear();

              data.voet = 0;              
        
            }

         else{
//          Serial.println("Hand AFWEZIG");
          hand = 0;
              }

}
