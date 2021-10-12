

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        7 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 10 // Popular NeoPixel ring size

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 50 // Time (in milliseconds) to pause between pixels

#define R_PIN A1
#define G_PIN A2
#define B_PIN A3
#define BRIGHT_PIN A4
#define MODE_PIN 4

void setup() {
  Serial.begin(115200);
  
  pinMode(R_PIN, INPUT);
  pinMode(G_PIN, INPUT);
  pinMode(B_PIN, INPUT);
  pinMode(BRIGHT_PIN, INPUT);
  pinMode(MODE_PIN, INPUT);

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.clear(); // Set all pixel colors to 'off'
  
}

int mode = 0;
int toggle = 0;

void loop() {
  int temp = digitalRead(MODE_PIN);
  
  if(temp != toggle) {
    toggle = temp;
    mode++;

  }
  Serial.println(mode);
  if (mode%4 == 0) {
    mode = 0;
  }
  
  if(mode == 0) {
    //pixels.clear(); // Set all pixel colors to 'off'
    solid_wipe();
  }
  if(mode == 1) {
    pixels.clear(); // Set all pixel colors to 'off'
    bounce();
  }
  if(mode == 2) {
    pixels.clear(); // Set all pixel colors to 'off'
    pixels.show();   // Send the updated pixel colors to the hardware.

  }
  if(mode == 3) {
    chase_3();

  }

}


void solid_wipe(){
  int r_val = analogRead(R_PIN);
  int g_val = analogRead(G_PIN);
  int b_val = analogRead(B_PIN);
  r_val = map(r_val,0, 800, 0,255);
  g_val = map(g_val,0, 800, 0,255);
  b_val = map(b_val,0, 800, 0,255);
  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...

    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels.setPixelColor(i, pixels.Color(r_val, g_val, b_val));

    pixels.show();   // Send the updated pixel colors to the hardware.

    delay(DELAYVAL); // Pause before next pass through loop
  }
}

void bounce() {
  int r_val = analogRead(R_PIN);
  int g_val = analogRead(G_PIN);
  int b_val = analogRead(B_PIN);
  r_val = map(r_val,0, 800, 0,255);
  g_val = map(g_val,0, 800, 0,255);
  b_val = map(b_val,0, 800, 0,255);
  //pixels.clear(); // Set all pixel colors to 'off'
  
  for(int i = 0; i <NUMPIXELS +1; i++) {
    pixels.setPixelColor(i, pixels.Color(r_val,g_val, b_val));
    pixels.setPixelColor(i-1, pixels.Color(0,0,0));
    pixels.show();   // Send the updated pixel colors to the hardware.
    delay(40);

  }
  for(int i = NUMPIXELS; i > 0; i--) {
    pixels.setPixelColor(i, pixels.Color(r_val, g_val, b_val));
    pixels.setPixelColor(i+1, pixels.Color(0,0,0));
    pixels.show();   // Send the updated pixel colors to the hardware.
    delay(40);
  }
}

void chase_3() {
  int r_val = analogRead(R_PIN);
  int g_val = analogRead(G_PIN);
  int b_val = analogRead(B_PIN);
  r_val = map(r_val,0, 800, 0,255);
  g_val = map(g_val,0, 800, 0,255);
  b_val = map(b_val,0, 800, 0,255);
  //pixels.clear(); // Set all pixel colors to 'off'

  for(int j = 0; j < 3; j++) {
    for(int i = 0; i <NUMPIXELS +1; i++) {
      pixels.setPixelColor(i, pixels.Color(r_val,g_val, b_val));
      pixels.setPixelColor(i-1, pixels.Color(r_val/2,g_val/2, b_val/2));
      pixels.setPixelColor(i-1, pixels.Color(r_val/3,g_val/3, b_val/3));
      pixels.setPixelColor(i-1, pixels.Color(0,0,0));
  
      pixels.show();   // Send the updated pixel colors to the hardware.
      delay(20);
  
    } 
  }
  delay(500); 
}
