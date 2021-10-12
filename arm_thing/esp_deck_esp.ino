#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>
#include "DHT.h"
#include "ESP8266WiFi.h"


#define DHTPIN D1     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11

#define TFT_CS         D2
#define TFT_RST        D4
#define TFT_DC         D3

#define SELECT_PIN D8
#define NEO_PIN D0

DHT dht(DHTPIN, DHTTYPE);

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

float p = 3.1415926;

void setup(void) {
  pinMode(A0, INPUT);
  pinMode(SELECT_PIN, INPUT);
  pinMode(NEO_PIN, OUTPUT);
  digitalWrite(NEO_PIN, LOW);
  
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  Serial.print(F("Hello! ST77xx TFT Test"));
  dht.begin();
  // Use this initializer if using a 1.8" TFT screen:
  tft.initR(INITR_BLACKTAB);      // Init ST7735S chip, black tab
  tft.setRotation(2);

  Serial.println(F("Initialized"));

  tft.fillScreen(ST77XX_BLACK);
  tft.setTextSize(2);
  drawtext("WELCOME...", ST77XX_WHITE, 5, 30);
  delay(400);
  tft.setTextSize(1);
  drawtext("to", ST77XX_WHITE, 40, 70);
  delay(1500);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextSize(2);
  drawtext("ARM.", ST77XX_MAGENTA, 5, 40);
  drawtext("STATION", ST77XX_YELLOW, 15, 60);
  tft.setTextSize(1);
  drawtext("v.6", ST77XX_YELLOW, 40, 100);
  delay(2000);
  tft.fillScreen(ST77XX_BLACK);

}

int menu_space = 20;

float f_temp ;
int dial;
int select = 1;
int selection;
bool is_main_menu = true;

void loop() {
  if(is_main_menu == true) {  
    
    draw_menu();
    Serial.println(dial);
    
    if(digitalRead(SELECT_PIN) == HIGH) {
      if (dial == 0) {
        tft.fillScreen(ST77XX_BLACK);
        updateTemp();
        is_main_menu = false;
      }
      if(dial == 3) {
        tft.fillScreen(ST77XX_BLACK);
        scan_wifi();
        is_main_menu = false;        
      }
      if(dial == 2) {
        digitalWrite(NEO_PIN, !digitalRead(NEO_PIN));
        Serial.println(digitalRead(NEO_PIN));
        delay(100);
      }
    }
  }

  
  else { //in sub-menu
    if(digitalRead(SELECT_PIN) == HIGH) {
      tft.fillScreen(ST77XX_BLACK);
      is_main_menu = true;
    }
  }
//  Serial.println(digitalRead(SELECT_PIN));
//  
//  if(digitalRead(SELECT_PIN) == HIGH) {
//    select = -select;
//  }
//  if (select == 1) {
//    tft.fillScreen(ST77XX_RED);
//  }
//  if (select == -1) {
//    tft.fillScreen(ST77XX_BLUE);
//  }
//  tft.setCursor(50, 50);
//  //Serial.println(select);
//  tft.print(select);
//  //scan_wifi();
//  //delay(2000);
//  //draw_menu();
  delay(100);  //allow screen to catch up
//  //tft.fillScreen(ST77XX_BLACK);

}

void draw_menu() {
  tft.setTextSize(2);
  //  drawtext("WEATHER", ST77XX_BLACK, 20, menu_space * 0);
  //  drawtext("NEO ON/OFF", ST77XX_BLACK, 20, menu_space * 1);
  //  drawtext("NEO MODE", ST77XX_BLACK, 20, menu_space * 2);
  //  drawtext("WIFI SCAN", ST77XX_BLACK, 20, menu_space * 3);
  //  drawtext("LASER", ST77XX_BLACK, 20, menu_space * 4);
  //  drawtext("SCREENSAVER", ST77XX_BLACK, 20, menu_space * 5);
  //  drawtext("7.", ST77XX_BLACK, 20, menu_space * 6);
  //  drawtext("8.", ST77XX_BLACK, 20, menu_space * 7);
  //

  drawtext("WEATHER", ST77XX_YELLOW, 15, menu_space * 0);
  drawtext("NEO ON/OF", ST77XX_CYAN, 15, menu_space * 1);
  drawtext("NEO MODE", ST77XX_GREEN, 15, menu_space * 2);
  drawtext("WIFI SCAN", ST77XX_BLUE, 15, menu_space * 3);
  drawtext("LASER", ST77XX_RED, 15, menu_space * 4);
  drawtext("STANDBY", ST77XX_WHITE, 15, menu_space * 5);
  drawtext("7.", ST77XX_WHITE, 15, menu_space * 6);
  drawtext("8.", ST77XX_WHITE, 15, menu_space * 7);
  
  drawtext(">", ST77XX_BLACK, 5, menu_space * dial);
  dial = analogRead(A0);
  dial = map(dial, 0, 1024, 0, 7);
  drawtext(">", ST77XX_WHITE, 5, menu_space * dial);

}

void scan_wifi() {
  tft.fillScreen(ST77XX_BLACK);
  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  int top10[10];

  if (n == 0) {
    tft.setCursor(0, 0);
    Serial.println("no networks found");
    tft.setTextColor(ST77XX_RED);
    tft.setTextSize(4);
    tft.println("no networks found");
  }

  else {
    tft.setTextSize(1);
    tft.setTextColor(ST77XX_WHITE);
    tft.setCursor(0, 0);  //reset cursor
    tft.print(n);
    tft.println(" networks found.");
    tft.setTextSize(1);

    for (int i = 0; i < n; ++i) {
      tft.setTextColor(ST77XX_YELLOW);
      //tft.print("");
      int r = map(WiFi.RSSI(i), 0, -150, 10, 1);
      
      tft.setTextColor(ST77XX_GREEN); 
      tft.print(WiFi.RSSI(i));
      
      tft.setTextColor(ST77XX_MAGENTA);

      if (WiFi.encryptionType(i) == ENC_TYPE_NONE) {
        tft.setTextColor(ST77XX_CYAN);
      }

      tft.print(WiFi.SSID(i));
      tft.setTextColor(ST77XX_WHITE);
      tft.print(" .. ");
      //top10[i] = WIFIRSSI(i);

      //      for(int j = 0; j <= r; j++) {
      //      tft.setTextColor(ST77XX_GREEN);
      //
      //        tft.print("*");
      //      }
      //tft.println();
    }
  }
}
void updateTemp() {
  tft.setTextSize(3);
  float temp_temp = f_temp;
  f_temp = get_f_temp();
  tft.fillScreen(ST77XX_BLACK);

  //drawtext(temp_temp, ST77XX_BLACK,10,0);
  drawtext(f_temp, ST77XX_GREEN, 5, 50);
}

float get_f_temp() {
  float temp = 0;
  delay(2000);
  temp = dht.readTemperature(true);
  Serial.println(temp);
  return temp;
}

void testlines(uint16_t color) {
  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x = 0; x < tft.width(); x += 6) {
    tft.drawLine(0, 0, x, tft.height() - 1, color);
    delay(0);
  }
  for (int16_t y = 0; y < tft.height(); y += 6) {
    tft.drawLine(0, 0, tft.width() - 1, y, color);
    delay(0);
  }

  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x = 0; x < tft.width(); x += 6) {
    tft.drawLine(tft.width() - 1, 0, x, tft.height() - 1, color);
    delay(0);
  }
  for (int16_t y = 0; y < tft.height(); y += 6) {
    tft.drawLine(tft.width() - 1, 0, 0, y, color);
    delay(0);
  }

  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x = 0; x < tft.width(); x += 6) {
    tft.drawLine(0, tft.height() - 1, x, 0, color);
    delay(0);
  }
  for (int16_t y = 0; y < tft.height(); y += 6) {
    tft.drawLine(0, tft.height() - 1, tft.width() - 1, y, color);
    delay(0);
  }

  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x = 0; x < tft.width(); x += 6) {
    tft.drawLine(tft.width() - 1, tft.height() - 1, x, 0, color);
    delay(0);
  }
  for (int16_t y = 0; y < tft.height(); y += 6) {
    tft.drawLine(tft.width() - 1, tft.height() - 1, 0, y, color);
    delay(0);
  }
}

void print_temp() {
  tft.setCursor(100, 0);
  tft.setTextColor(ST77XX_GREEN);
  tft.print(f_temp);
}


void drawtext(char *text, uint16_t color, int x, int y) {
  tft.setCursor(x, y);
  tft.setTextColor(color);
  //tft.setTextWrap(true);
  tft.print(text);
}

void drawtext(float text, uint16_t color, int x, int y) {
  tft.setCursor(x, y);
  tft.setTextColor(color);
  //tft.setTextWrap(true);
  tft.print(text);
}

void testfastlines(uint16_t color1, uint16_t color2) {
  tft.fillScreen(ST77XX_BLACK);
  for (int16_t y = 0; y < tft.height(); y += 5) {
    tft.drawFastHLine(0, y, tft.width(), color1);
  }
  for (int16_t x = 0; x < tft.width(); x += 5) {
    tft.drawFastVLine(x, 0, tft.height(), color2);
  }
}

void testdrawrects(uint16_t color) {
  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x = 0; x < tft.width(); x += 6) {
    tft.drawRect(tft.width() / 2 - x / 2, tft.height() / 2 - x / 2 , x, x, color);
  }
}

void testfillrects(uint16_t color1, uint16_t color2) {
  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x = tft.width() - 1; x > 6; x -= 6) {
    tft.fillRect(tft.width() / 2 - x / 2, tft.height() / 2 - x / 2 , x, x, color1);
    tft.drawRect(tft.width() / 2 - x / 2, tft.height() / 2 - x / 2 , x, x, color2);
  }
}

void testfillcircles(uint8_t radius, uint16_t color) {
  for (int16_t x = radius; x < tft.width(); x += radius * 2) {
    for (int16_t y = radius; y < tft.height(); y += radius * 2) {
      tft.fillCircle(x, y, radius, color);
    }
  }
}

void testdrawcircles(uint8_t radius, uint16_t color) {
  for (int16_t x = 0; x < tft.width() + radius; x += radius * 2) {
    for (int16_t y = 0; y < tft.height() + radius; y += radius * 2) {
      tft.drawCircle(x, y, radius, color);
    }
  }
}

void testtriangles() {
  tft.fillScreen(ST77XX_BLACK);
  uint16_t color = 0xF800;
  int t;
  int w = tft.width() / 2;
  int x = tft.height() - 1;
  int y = 0;
  int z = tft.width();
  for (t = 0 ; t <= 15; t++) {
    tft.drawTriangle(w, y, y, x, z, x, color);
    x -= 4;
    y += 4;
    z -= 4;
    color += 100;
  }
}

void testroundrects() {
  tft.fillScreen(ST77XX_BLACK);
  uint16_t color = 100;
  int i;
  int t;
  for (t = 0 ; t <= 4; t += 1) {
    int x = 0;
    int y = 0;
    int w = tft.width() - 2;
    int h = tft.height() - 2;
    for (i = 0 ; i <= 16; i += 1) {
      tft.drawRoundRect(x, y, w, h, 5, color);
      x += 2;
      y += 3;
      w -= 4;
      h -= 6;
      color += 1100;
    }
    color += 100;
  }
}

void tftPrintTest() {
  tft.setTextWrap(false);
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 30);
  tft.setTextColor(ST77XX_RED);
  tft.setTextSize(1);
  tft.println("Hello World!");
  tft.setTextColor(ST77XX_YELLOW);
  tft.setTextSize(2);
  tft.println("Hello World!");
  tft.setTextColor(ST77XX_GREEN);
  tft.setTextSize(3);
  tft.println("Hello World!");
  tft.setTextColor(ST77XX_BLUE);
  tft.setTextSize(4);
  tft.print(1234.567);
  delay(1500);
  tft.setCursor(0, 0);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(0);
  tft.println("Hello World!");
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_GREEN);
  tft.print(p, 6);
  tft.println(" Want pi?");
  tft.println(" ");
  tft.print(8675309, HEX); // print 8,675,309 out in HEX!
  tft.println(" Print HEX!");
  tft.println(" ");
  tft.setTextColor(ST77XX_WHITE);
  tft.println("Sketch has been");
  tft.println("running for: ");
  tft.setTextColor(ST77XX_MAGENTA);
  tft.print(millis() / 1000);
  tft.setTextColor(ST77XX_WHITE);
  tft.print(" seconds.");
}

void mediabuttons() {
  // play
  tft.fillScreen(ST77XX_BLACK);
  tft.fillRoundRect(25, 10, 78, 60, 8, ST77XX_WHITE);
  tft.fillTriangle(42, 20, 42, 60, 90, 40, ST77XX_RED);
  delay(500);
  // pause
  tft.fillRoundRect(25, 90, 78, 60, 8, ST77XX_WHITE);
  tft.fillRoundRect(39, 98, 20, 45, 5, ST77XX_GREEN);
  tft.fillRoundRect(69, 98, 20, 45, 5, ST77XX_GREEN);
  delay(500);
  // play color
  tft.fillTriangle(42, 20, 42, 60, 90, 40, ST77XX_BLUE);
  delay(50);
  // pause color
  tft.fillRoundRect(39, 98, 20, 45, 5, ST77XX_RED);
  tft.fillRoundRect(69, 98, 20, 45, 5, ST77XX_RED);
  // play color
  tft.fillTriangle(42, 20, 42, 60, 90, 40, ST77XX_GREEN);
}
