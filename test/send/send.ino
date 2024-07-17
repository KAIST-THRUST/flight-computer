// based on code from: Tom Heylen
#define HC12 Serial1

long baud = 9600;
bool state = false;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(baud);
  Serial.println("Serial monitor available... OK");

  Serial.print("Serial link available... ");
  HC12.begin(9600);

  // test HC-12
  Serial.print("HC-12 available... ");
  HC12.write("AT+DEFAULT");
  delay(1000);
  while (HC12.available() > 0) {
    Serial.write(HC12.read());
  }
  Serial.println();
  Serial.println("initialization done.");
}

void loop() {
  digitalWrite(LED_BUILTIN, state);
  Serial.println("1");
  HC12.println("test123.");
  delay(1000);

  state = !state;
}