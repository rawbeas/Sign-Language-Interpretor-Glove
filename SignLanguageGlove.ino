#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <avr/pgmspace.h> //for PROGMEM

// --- Pin Definitions and Constants ---
const int LCD_I2C_ADDRESS = 0x27; //LCD's I2C address
const int MP3_RX_PIN = 10;       //Arduino RX for MP3 (connect to MP3 TX with 1k resistor)
const int MP3_TX_PIN = 11;       //Arduino TX for MP3 (connect to MP3 RX)

//Flex Sensor Analog Pins
const int flexPin_f1 = A0;
const int flexPin_f2 = A1;
const int flexPin_f3 = A2;
const int flexPin_f4 = A3;
const int flexPin_f5 = A6;

const int BEND_THRESHOLD = 800  ; //Threshold to consider finger bent

// --- MP3 Player and LCD Objects ---
SoftwareSerial mySerial(MP3_RX_PIN, MP3_TX_PIN);
DFRobotDFPlayerMini myPlayer;
LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS, 16, 2); // 16 columns, 2 rows

// --- Language State ---
enum Language { ENGLISH, HINDI };
Language currentLanguage = ENGLISH; // Default language

// --- Word List (Stored in PROGMEM) ---

// English Word Table
const char en_string_0[] PROGMEM = "hello";
const char en_string_1[] PROGMEM = "food";
const char en_string_2[] PROGMEM = "water";
const char en_string_3[] PROGMEM = "washroom";
const char en_string_4[] PROGMEM = "help";
const char en_string_5[] PROGMEM = "please";
const char en_string_6[] PROGMEM = "thank you";
const char en_string_7[] PROGMEM = "come here";
const char en_string_8[] PROGMEM = "bye";
const char en_string_9[] PROGMEM = "sorry";
const char en_string_10[] PROGMEM = "hot";
const char en_string_11[] PROGMEM = "cold";
const char en_string_12[] PROGMEM = "day";
const char en_string_13[] PROGMEM = "night";
const char en_string_14[] PROGMEM = "yesterday";
const char en_string_15[] PROGMEM = "tomorrow";
const char en_string_16[] PROGMEM = "today";
const char en_string_17[] PROGMEM = "time?";
const char en_string_18[] PROGMEM = "date";
const char en_string_19[] PROGMEM = "like";
const char en_string_20[] PROGMEM = "dislike";
const char en_string_21[] PROGMEM = "happy";
const char en_string_22[] PROGMEM = "sad";
const char en_string_23[] PROGMEM = "angry";
const char en_string_24[] PROGMEM = "how";
const char en_string_25[] PROGMEM = "why";
const char en_string_26[] PROGMEM = "pain";
const char en_string_27[] PROGMEM = "forward";
const char en_string_28[] PROGMEM = "right";
const char en_string_29[] PROGMEM = "left";
const char en_string_30[] PROGMEM = "backward";
const char en_string_31[] PROGMEM = ""; // 31 words

const char* const en_word_table[] PROGMEM = {
  en_string_0, en_string_1, en_string_2, en_string_3, en_string_4, en_string_5, en_string_6, en_string_7,
  en_string_8, en_string_9, en_string_10, en_string_11, en_string_12, en_string_13, en_string_14, en_string_15,
  en_string_16, en_string_17, en_string_18, en_string_19, en_string_20, en_string_21, en_string_22, en_string_23,
  en_string_24, en_string_25, en_string_26, en_string_27, en_string_28, en_string_29, en_string_30, en_string_31
};

// Hindi Word Table (Replace with your actual Hindi translations)
const char hi_string_0[] PROGMEM = "namaste"; //32nd
const char hi_string_1[] PROGMEM = "khana";//33
const char hi_string_2[] PROGMEM = "pani";//34
const char hi_string_3[] PROGMEM = "shauchalay";//35
const char hi_string_4[] PROGMEM = "sahayeta kariye";//36
const char hi_string_5[] PROGMEM = "kripya";//37
const char hi_string_6[] PROGMEM = "dhanyawad";//38th - 36
const char hi_string_7[] PROGMEM = "idhar aao";
const char hi_string_8[] PROGMEM = "alvida";
const char hi_string_9[] PROGMEM = "maaf kijiye";
const char hi_string_10[] PROGMEM = "garam";
const char hi_string_11[] PROGMEM = "thanda";
const char hi_string_12[] PROGMEM = "din";
const char hi_string_13[] PROGMEM = "raat";
const char hi_string_14[] PROGMEM = "beeta hua kal";
const char hi_string_15[] PROGMEM = "aane wala kal";
const char hi_string_16[] PROGMEM = "aaj";
const char hi_string_17[] PROGMEM = "samay?";
const char hi_string_18[] PROGMEM = "tareekh";//50th - 48
const char hi_string_19[] PROGMEM = "pasand";//51st- 57
const char hi_string_20[] PROGMEM = "na pasand";//52nd-49
const char hi_string_21[] PROGMEM = "khush";//53rd-50
const char hi_string_22[] PROGMEM = "dukhhi";
const char hi_string_23[] PROGMEM = "gussa";//55th - 52
const char hi_string_24[] PROGMEM = "kaise";//56th - 58
const char hi_string_25[] PROGMEM = "kyun"; //57th-53
const char hi_string_26[] PROGMEM = "dard"; //27th
const char hi_string_27[] PROGMEM = "aage"; //28th
const char hi_string_28[] PROGMEM = "daayein"; //29th
const char hi_string_29[] PROGMEM = "baayein";
const char hi_string_30[] PROGMEM = "peeche";
const char hi_string_31[] PROGMEM = "";

const char* const hi_word_table[] PROGMEM = {
  hi_string_0, hi_string_1, hi_string_2, hi_string_3, hi_string_4, hi_string_5, hi_string_6, hi_string_7,
  hi_string_8, hi_string_9, hi_string_10, hi_string_11, hi_string_12, hi_string_13, hi_string_14, hi_string_15,
  hi_string_16, hi_string_17, hi_string_18, hi_string_19, hi_string_20, hi_string_21, hi_string_22, hi_string_23,
  hi_string_24, hi_string_25, hi_string_26, hi_string_27, hi_string_28, hi_string_29, hi_string_30, hi_string_31
};

// --- Audio File Mapping (Stored in PROGMEM - Single List) ---
// Maps word index to audio file number (1-based).
// English words 0-29 map to audio files 1-30.
// Hindi words 0-29 map to audio files 31-60.
// Power-on = 61, English Select = 62, Hindi Select = 63.
const uint8_t audio_map[] PROGMEM = {
  1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, // English 0-22 -> Audio 1-23
  23, 24, 25, 26, 66, 27, 28, 29, 30,                                               // English 23-29 -> Audio 24-30
  65, 31, 32, 33, 56, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 57, 49, 50, // Hindi 0-19 -> Audio 31-50
  51, 52, 58, 53, 54, 55, 59, 60, 61, 65                                       // Hindi 20-29 -> Audio 51-60
};

const int NUM_WORDS = sizeof(en_word_table) / sizeof(en_word_table[0]); // Number of words/phrases (30)
const int POWER_ON_AUDIO = 64;
const int ENGLISH_SELECT_AUDIO = 63;
const int HINDI_SELECT_AUDIO = 62;


// --- Helper functions to read from PROGMEM ---
char wordBuffer[30]; //Buffer for reading strings from Flash (size fits longest string + 1)

void readWordFromFlash(int wordIndex, Language lang) {
  char* wordAddress;
  if (lang == ENGLISH) {
    wordAddress = (char*)pgm_read_word(&(en_word_table[wordIndex]));
  } else { // HINDI
    wordAddress = (char*)pgm_read_word(&(hi_word_table[wordIndex]));
  }
  strcpy_P(wordBuffer, wordAddress);
}

uint8_t getAudioNumberForWord(int wordIndex, Language lang) {
    // This logic needs to map the word index and language to the correct audio file number
    if (lang == ENGLISH) {
        return pgm_read_byte(&(audio_map[wordIndex])); // English words map to audio_map indices
    } else { // HINDI
        return pgm_read_byte(&(audio_map[wordIndex + NUM_WORDS])); // Hindi words map to audio_map indices
    }
}


// --- Variables for Gesture Detection and Cooldown ---
static int lastDetectedWordIndex = -1;
static unsigned long lastGestureTime = 0;
const unsigned long gestureCooldown = 3000; //Cooldown in ms

// --- Language Selection Gesture Variables ---
static unsigned long lastLanguageGestureTime = 0;
const unsigned long languageGestureCooldown = 2000; //Cooldown for language selection gesture
const int ENGLISH_GESTURE_WORD_INDEX = -2; //special index for gesture types
const int HINDI_GESTURE_WORD_INDEX = -3;


void setup() {
  Serial.begin(115200); // Use a higher baud rate for debugging
  lcd.init();
  lcd.backlight();

  // --- 1) Power On Sequence ---
  lcd.setCursor(0, 0);
  lcd.print(F("  Powering On ")); // Print "Powering On" from Flash

  mySerial.begin(9600);
  Serial.println(F("Initializing DFPlayer Mini...")); // Print from Flash

  if (!myPlayer.begin(mySerial)) {
    Serial.println(F("DFPlayer Mini Error:"));
    Serial.println(F("1. Check connection!"));
    Serial.println(F("2. Insert SD card!"));
    while(true); // Halt if MP3 player fails
  }
  Serial.println(F("DFPlayer Mini online."));

  //myPlayer.volume(30); // Set a default volume

  // Play power-on sound (assuming 0061.mp3)
  myPlayer.play(63);
  delay(4000); // Wait for power-on sound to play

  // Clear LCD after power on message
  lcd.clear();

  // --- 2) Language Selection (Gesture-based) ---

  lcd.setCursor(0, 0);
  lcd.print(F("Select Language:")); // Print from Flash
  lcd.setCursor(0, 1);
  lcd.print(F("English / Hindi")); // Print from Flash

  bool languageSelected = false;
  while (!languageSelected) {
    // Read flex sensors
    int f1_value = readFlexSensor(flexPin_f1);
    int f5_value = readFlexSensor(flexPin_f5);

    unsigned long currentTime = millis();

    if (f5_value >= BEND_THRESHOLD && currentTime - lastLanguageGestureTime > languageGestureCooldown) {
      // English selected
      currentLanguage = ENGLISH;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F(" Language: Eng ")); // Print from Flash
      // Play English selection audio
      myPlayer.play(64);
      delay(2000); // Wait for audio
      languageSelected = true;
      lastLanguageGestureTime = currentTime; // Update cooldown timer
    } else if (f1_value >= BEND_THRESHOLD && currentTime - lastLanguageGestureTime > languageGestureCooldown) {
      // Hindi selected
      currentLanguage = HINDI;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F(" Language: Hindi")); // Print from Flash
       // Play Hindi selection audio
      myPlayer.play(62);
      delay(2000); // Wait for audio
      languageSelected = true;
      lastLanguageGestureTime = currentTime; // Update cooldown timer
    }
    // Add a small delay in the selection loop to avoid reading too fast
     delay(50);
  }

  // Clear LCD after language selection
  lcd.clear();

  // --- Set up main operation display ---
  lcd.setCursor(0, 0);
  lcd.print(F("Gesture Ready!  ")); // Standard top line text from Flash
}


void loop() {
  // --- Main Operation: Read Sensors, Detect Gestures, Output ---

  // Read all flex sensor values (You might want to use averaging here)
  /*
  int f1_value = analogRead(flexPin_f1);
  int f2_value = analogRead(flexPin_f2);
  int f3_value = analogRead(flexPin_f3);
  int f4_value = analogRead(flexPin_f4);
  int f5_value = analogRead(flexPin_f5);
  */
  
  int f1_value = readFlexSensor(flexPin_f1);
  int f2_value = readFlexSensor(flexPin_f2);
  int f3_value = readFlexSensor(flexPin_f3);
  int f4_value = readFlexSensor(flexPin_f4);
  int f5_value = readFlexSensor(flexPin_f5);
  
  // Determine which fingers are bent based on the threshold
  bool is_f1_bent = (f1_value >= BEND_THRESHOLD);
  bool is_f2_bent = (f2_value >= BEND_THRESHOLD);
  bool is_f3_bent = (f3_value >= BEND_THRESHOLD);
  bool is_f4_bent = (f4_value >= BEND_THRESHOLD);
  bool is_f5_bent = (f5_value >= BEND_THRESHOLD);

  // --- Gesture Detection Logic ---
  // Define combinations of bent fingers mapped to word indices (0-29)

  int detectedWordIndex = -1; // -1 means no word gesture detected yet


 if (is_f1_bent && !is_f2_bent && !is_f3_bent && !is_f4_bent && !is_f5_bent) {
    detectedWordIndex = 0;
  } else if (!is_f1_bent && is_f2_bent && !is_f3_bent && !is_f4_bent && !is_f5_bent) {
    detectedWordIndex = 1;
  } else if (!is_f1_bent && !is_f2_bent && is_f3_bent && !is_f4_bent && !is_f5_bent) {
    detectedWordIndex = 2;
  } else if (!is_f1_bent && !is_f2_bent && !is_f3_bent && is_f4_bent && !is_f5_bent) {
    detectedWordIndex = 3;
  } else if (!is_f1_bent && !is_f2_bent && !is_f3_bent && !is_f4_bent && is_f5_bent) {
    detectedWordIndex = 4;
  }

  // Pair of Finger Gestures (Indices 5-14) - 10 Combinations
  else if (is_f1_bent && is_f2_bent && !is_f3_bent && !is_f4_bent && !is_f5_bent) {
    detectedWordIndex = 5;
  } else if (is_f1_bent && !is_f2_bent && is_f3_bent && !is_f4_bent && !is_f5_bent) {
    detectedWordIndex = 6;
  } else if (is_f1_bent && !is_f2_bent && !is_f3_bent && is_f4_bent && !is_f5_bent) {
    detectedWordIndex = 7;
  } else if (is_f1_bent && !is_f2_bent && !is_f3_bent && !is_f4_bent && is_f5_bent) {
    detectedWordIndex = 8;
  } else if (!is_f1_bent && is_f2_bent && is_f3_bent && !is_f4_bent && !is_f5_bent) {
    detectedWordIndex = 9;
  } else if (!is_f1_bent && is_f2_bent && !is_f3_bent && is_f4_bent && !is_f5_bent) {
    detectedWordIndex = 10;
  } else if (!is_f1_bent && is_f2_bent && !is_f3_bent && !is_f4_bent && is_f5_bent) {
    detectedWordIndex = 11;
  } else if (!is_f1_bent && !is_f2_bent && is_f3_bent && is_f4_bent && !is_f5_bent) {
    detectedWordIndex = 12;
  } else if (!is_f1_bent && !is_f2_bent && is_f3_bent && !is_f4_bent && is_f5_bent) {
    detectedWordIndex = 13;
  } else if (!is_f1_bent && !is_f2_bent && !is_f3_bent && is_f4_bent && is_f5_bent) {
    detectedWordIndex = 14;
  }

  // Triplet Finger Gestures (Indices 15-24) - 10 Combinations
  else if (is_f1_bent && is_f2_bent && is_f3_bent && !is_f4_bent && !is_f5_bent) {
    detectedWordIndex = 15;
  } else if (is_f1_bent && is_f2_bent && !is_f3_bent && is_f4_bent && !is_f5_bent) {
    detectedWordIndex = 16;
  } else if (is_f1_bent && is_f2_bent && !is_f3_bent && !is_f4_bent && is_f5_bent) {
    detectedWordIndex = 17;
  } else if (is_f1_bent && !is_f2_bent && is_f3_bent && is_f4_bent && !is_f5_bent) {
    detectedWordIndex = 18;
  } else if (is_f1_bent && !is_f2_bent && is_f3_bent && !is_f4_bent && is_f5_bent) {
    detectedWordIndex = 19;
  } else if (is_f1_bent && !is_f2_bent && !is_f3_bent && is_f4_bent && is_f5_bent) {
    detectedWordIndex = 20;
  } else if (!is_f1_bent && is_f2_bent && is_f3_bent && is_f4_bent && !is_f5_bent) {
    detectedWordIndex = 21;
  } else if (!is_f1_bent && is_f2_bent && is_f3_bent && !is_f4_bent && is_f5_bent) {
    detectedWordIndex = 22;
  } else if (!is_f1_bent && is_f2_bent && !is_f3_bent && is_f4_bent && is_f5_bent) {
    detectedWordIndex = 23;
  } else if (!is_f1_bent && !is_f2_bent && is_f3_bent && is_f4_bent && is_f5_bent) {
    detectedWordIndex = 24;
  }

  // Quadruplet Finger Gestures (Indices 25-29) - 5 Combinations
  else if (is_f1_bent && is_f2_bent && is_f3_bent && is_f4_bent && !is_f5_bent) {
    detectedWordIndex = 25;
  } else if (is_f1_bent && is_f2_bent && is_f3_bent && !is_f4_bent && is_f5_bent) {
    detectedWordIndex = 26;
  } else if (is_f1_bent && is_f2_bent && !is_f3_bent && is_f4_bent && is_f5_bent) {
    detectedWordIndex = 27;
  } else if (is_f1_bent && !is_f2_bent && is_f3_bent && is_f4_bent && is_f5_bent) {
    detectedWordIndex = 28;
  } else if (!is_f1_bent && is_f2_bent && is_f3_bent && is_f4_bent && is_f5_bent) {
    detectedWordIndex = 29;
  }
  else if (is_f1_bent && is_f2_bent && is_f3_bent && is_f4_bent && is_f5_bent) {
    detectedWordIndex = 30;
  }
  // All Five Fingers (Index 30) - 1 Combination
  else if (is_f1_bent && is_f2_bent && !is_f3_bent && is_f4_bent && is_f5_bent) {
     // This is the specific gesture you wanted to keep for index 31, now mapped to 30
    detectedWordIndex = 31;
  }


  // --- Output if a Word Gesture is Detected ---
  unsigned long currentTime = millis();

  // Check if a word gesture is detected AND it's a new gesture OR enough time has passed
  if (detectedWordIndex != -1 && (detectedWordIndex != lastDetectedWordIndex || currentTime - lastGestureTime > gestureCooldown)) {

    // Clear the previous word on the bottom line
    lcd.setCursor(0, 1);
    lcd.print("                "); // Clear the entire second line

    // Read the word from Flash (correct language) and display it on the bottom line
    readWordFromFlash(detectedWordIndex, currentLanguage);
    lcd.setCursor(0, 1);
    lcd.print(wordBuffer);

    // Get the corresponding audio file number (language-dependent)
    uint8_t audioFileNumber = getAudioNumberForWord(detectedWordIndex, currentLanguage);

    // Play the audio file (from the root or folder 01 if you put everything there)
      myPlayer.play(audioFileNumber);


    // --- For Debugging ---
    Serial.print(F("Detected: "));
    Serial.println(wordBuffer);
    Serial.print(F("Playing audio file: "));
    Serial.println(audioFileNumber);
    // --- End Debugging ---


    // Update last detected gesture and time
    lastDetectedWordIndex = detectedWordIndex;
    lastGestureTime = currentTime;

  } else if (detectedWordIndex == -1 && lastDetectedWordIndex != -1 && currentTime - lastGestureTime > gestureCooldown) {
      // No word gesture detected currently, and enough time has passed since the last one
      // Optionally clear the bottom line or show a "Ready" message again
      // lcd.setCursor(0, 1);
      // lcd.print("                ");
      lastDetectedWordIndex = -1; // Reset the last detected gesture state
  }


  // Add a small delay in the loop to avoid reading too fast and reduce CPU load
  delay(1000);
  
}

// --- Averaging function ---

int readFlexSensor(int pin) {
  int sum = 0;
  for (int i = 0; i < 10; i++) { // Take 10 readings
    sum += analogRead(pin);
    // delay(1); // Small delay between readings (can remove if needed)
  }
  return sum / 10; // Return the average
}