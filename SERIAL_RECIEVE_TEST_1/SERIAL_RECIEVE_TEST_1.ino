void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial1.setTimeout(1000);
}

void loop() {
  if (Serial1.available() > 0)
  {
    String incoming = "";
    incoming = Serial1.readString();
    Serial.println("Recieving: " + incoming);
  }
}
