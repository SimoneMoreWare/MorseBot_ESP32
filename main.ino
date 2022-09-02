#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>   // Universal Telegram Bot Library written by Brian Lough: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
#include <ArduinoJson.h>

// Replace with your network credentials
const char* ssid = "TIM-19609261";
const char* password = "ciccipako";

// Initialize Telegram BOT
#define BOTtoken "5745762381:AAHrj8bzYMrCDG7o_WhmbNE7uRCXG0z9Sco"  // your Bot Token (Get from Botfather)


WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

// Checks for new messages every 1 second.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

const int ledPin = 13;
bool ledState = LOW;
char* letters[] = {
    ".-", "-…", "-.-.", "-..", ".", "..-.", "–.", "….", "..",
    ".—", "-.-", ".-..", "–", "-.", "—", ".–.", "–.-", ".-.",
    "…", "-", "..-", "…-", ".–", "-..-", "-.–", "–.."
};
char* numbers[] = { "—–", ".—-", "..—",
"…–", "….-", "…..", "-….",
"–…", "—..", "—-." };

int dotDelay = 200;
// Handle what happens when you receive new messages
void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    // Chat id of the requester
    
    
    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(String(text));

    for(int i=0;i<text.length();i++){
       if (text[i] >= 'a' && text[i] <= 'z') {
            flashSequence(letters[text[i] - 'a']);
        }
        else if (text[i] >= 'A' && text[i] <= 'Z') {
            flashSequence(letters[text[i] - 'A']);
        }
        else if (text[i] >= '0' && text[i] <= '9') {
            flashSequence(numbers[text[i] - '0']);
        }
        else if (text[i] == ' ') {
            delay(dotDelay * 4);
            // gap between words
        }
    }
    
    
  }
}

void flashSequence(char* sequence)
{
    int i = 0;
    while (sequence[i] != NULL) {
        flashDotOrDash(sequence[i]);
        i++;
    }
    delay(dotDelay * 3);
}
void flashDotOrDash(char dotOrDash)
{
    digitalWrite(ledPin, HIGH);
    if (dotOrDash == '.') {
        delay(dotDelay);
    }
    else // must be a -
    {
        delay(dotDelay * 3);
    }
    digitalWrite(ledPin, LOW);
    delay(dotDelay);
}

void setup() {
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState);
  
  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());
}

void loop() {
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}
