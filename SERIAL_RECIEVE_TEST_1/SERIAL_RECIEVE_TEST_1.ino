void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial1.setTimeout(1000);
}

void loop() {
  if (Serial1.available() > 0)
  {
    int incoming = Serial1.read();
    Serial.print("Recieving: ");
    Serial.print(incoming, DEC);
    Serial.print(", ");
    Serial.print(incoming, HEX);
    Serial.print(", ");
    Serial.print(char(incoming));
    Serial.println(".");
  }
}
