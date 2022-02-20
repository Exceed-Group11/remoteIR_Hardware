#include <IRremote.h>

int RECV_PIN = 15;
int value;
String irsig="";


IRrecv irrecv(RECV_PIN);
IRsend irsend;

decode_results results;

void setup()
{
  Serial.begin(115200);
  // In case the interrupt driver crashes on setup, give a clue
  // to the user what's going on.
  Serial.println("Enabling IRin");
  irrecv.enableIRIn(); // Start the receiver
  Serial.println("Enabled IRin");
}

void loop() {
  if (irrecv.decode(&results)) {
    irsig=String(results.value,HEX);
    //value=results.value;
    Serial.print(results.value,HEX);
    Serial.print("  ");
    Serial.println(irsig);
    irrecv.resume(); // Receive the next value
  }
  delay(100);
}
