#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9          // Configurable, but generally connected to Arduino D9
#define SS_PIN          10         // Configurable, but generally connected to Arduino D10

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(9600);
  SPI.begin();        // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // Initialize with the I2C addr 0x3C (for the 128x64)
  display.clearDisplay();
  
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
}

void loop() {
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent() || ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Read card UID
  String content = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();

  // Display access status on OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  if (content == "0014153635") { // Replace "YOUR_ALLOWED_CARD_UID" with the UID of the allowed card
    display.println("Access Granted");
    // Here you can add actions when access is granted
  } else {
    display.println("Access Denied");
    // Here you can add actions when access is denied
  }
  display.display();

  // Halt PICC
  mfrc522.PICC_HaltA();
}
