#include <IRremote.h>

int RECV_PIN = 15,led=5;
int value;
String irsig="";

//const char* irsig="0xa9016899";


IRrecv irrecv(RECV_PIN);
IRsend irsend;

decode_results results;

void setup()
{
  
  Serial.begin(115200);
  Serial.println("Enabling IRin");
  irrecv.enableIRIn(); // Start the receiver
  Serial.println("Enabled IRin");
  pinMode(led,OUTPUT);
  digitalWrite(led,HIGH);
}

void loop() {
  if (irrecv.decode(&results)) {
    irsig=String(results.value,HEX);
    Serial.println(results.value,HEX);
    if(irsig=="8c7351ae"){
      digitalWrite(led,LOW);
    }
    else if(irsig=="8c73b14e"){
      digitalWrite(led,HIGH);
    }
    irrecv.resume(); // Receive the next value
  }
  delay(80);
}
