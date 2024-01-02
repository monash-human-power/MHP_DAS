
// Messages to be sent to python script
const String START_MESSAGE = "start timer";
const String LAP_MESSAGE = "lap";

const int TRIGGER_DELAY = 1000; // Delay to avoid unwanted tiggers (e.g. second wheel of bike)
const int buttonPin = 2; // Pin connected to bump strip


int buttonState = 0; 
bool timerStarted = false;

// Arduino setup
void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() 
{
  int buttonState = digitalRead(buttonPin);

  if (buttonState == LOW) {
        if (!timerStarted){
            Serial.println(START_MESSAGE);
            timerStarted = true;
        }
        else{
            Serial.println(LAP_MESSAGE);
        }

    delay(TRIGGER_DELAY);
  }
  
  
  
}