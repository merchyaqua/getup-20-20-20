// Implementing the 20-20-20 by detecting wheter my laptop screen is open.
// Ultrasonic sensor is bad at detecting the human so that's just one thing.

// Hardware related
// Ultrasonic sensor readings
const int trigPin = 12;
const int echoPin = 11;
const int fiveVoltsPin = 13;
long timeVal;
// passive buzzer (one tone)
int buzzPin = 6;
int buzzFreq = 220;

// logic timing
long sitTimer = 0;
const long sitTimeLimit = 1200; // 20 minutes = 1200 seconds
long breakDuration = 20; // look outside for 20 seconds
unsigned long sitEndTime, breakEndTime;

const int distanceThreshold = 50; // cm, how far away from the base plate of the sensor is the open screen. 
long dist;

bool present;

void setup() {
  // put your setup code here, to run once:
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(fiveVoltsPin, OUTPUT);
  digitalWrite(fiveVoltsPin, HIGH);

  pinMode(buzzPin, OUTPUT);
  // Serial.begin(9600);
}

float distance(long time){ // conversion for ultrasonic sensor
  float d = (time-62.8)/56.52;
  return (d);
}

void readDist(){ // ultrasonic sensor reading procedure, assigns result to global variable dist
  digitalWrite(trigPin, LOW);
  delayMicroseconds(10);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  timeVal = pulseIn(echoPin, HIGH);
  dist = distance(timeVal);
  // Serial.println(dist);
}

bool checkPresence(){ // handles entrances and exits
  readDist();
  bool nowPresent = dist < distanceThreshold;
  if (!present && nowPresent ) buzzArrive();
  else if (present && !nowPresent) buzzLeave();
  present = nowPresent;
  delay(100);

}

int secondsUntil(unsigned long limit){
  return (limit - millis())/1000;
}
// int countSecond(){
//   int newMillis =millis();
//   int val = int((newMillis - lastMillis)/1000.0);
//   lastMillis = newMillis;
//   return val;
// }

void loop() {
  checkPresence();
  // Serial.print(dist);
  // lastMillis = millis(); // start time if sit PLEASE refactor if millis bs. it's not good when you have multiple timers they don't share the same millis
  if (present){ // an arrival
    // calculate session end time, converting sitTimeLimit to ms
    sitEndTime = millis() + sitTimeLimit*1000; 
  }

  while (present){
    checkPresence();
    Serial.print("Sat down at " + String(dist) + "cm, "); // notif
    Serial.println(String(secondsUntil(sitEndTime)) + " seconds left."); // ms until end, converted to seconds
    if (millis() >= sitEndTime){
      Serial.println("Get up! You've been sitting for" + String(sitTimeLimit + abs(secondsUntil(sitEndTime))) + "seconds."); // overboard
      breakEndTime = millis() + breakDuration*1000;
      unsigned long nextBuzzTime = millis() + 1000;
      unsigned long violationTime = 0;
      while (millis() < breakEndTime) { // not time for return yet
        checkPresence(); // keep watch
        violationTime = millis();
        while (present){ 
          // do not increase the timer - keep pushing back
          buzzWarn();
           // only start countdown when leave
          Serial.println("Get out");
          checkPresence();
        }
        nextBuzzTime += millis()-violationTime;
        breakEndTime +=  millis()-violationTime;
        
        if (millis() >= nextBuzzTime){
          Serial.println("Come back in: "+ String(secondsUntil(breakEndTime)));
          tone(buzzPin, 200, 50);
          nextBuzzTime += 1000;
        }
      }
      // countdown ended
      buzzBack();
      // reset timer
      // delay(100);

    }

  }
  sitTimer = 0;

  
  Serial.println("Not sat down. Probably.");

  // delay(100);
}



// buzzer functions

void buzzWarn(){
  tone(buzzPin, 500, 100);
}
void buzzBack(){
  tone(buzzPin, 150, 800);
  // tone(buzzPin, 200, 1000);
}
void buzzArrive(){
  tone(buzzPin, 100, 200);
  delay(200);
  tone(buzzPin, 300, 400);
}
void buzzLeave(){
  tone(buzzPin, 300, 200);
  delay(200);
  tone(buzzPin, 100, 400);
}
