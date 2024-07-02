void setup()
{
  Serial.begin(9600);
}

void dec2bin(int n)
{
  int c, k;

  for (c = 15; c >= 0; c--)
  {
    k = n >> c;

    if (k & 1)
      Serial.print("1");
    else
      Serial.print("0");
  }
}

void dec2bin8(int n)
{
  int c, k;

  for (c = 7; c >= 0; c--)
  {
    k = n >> c;

    if (k & 1)
      Serial.print("1");
    else
      Serial.print("0");
  }
}

void wait_for_dht11()
{
  delay(2000);
}

void start_signal(uint8_t dht11_pin)
{
  pinMode(dht11_pin, OUTPUT);
  digitalWrite(dht11_pin, LOW);
  delay(18);
  digitalWrite(dht11_pin, HIGH);
  pinMode(dht11_pin, INPUT);
  digitalWrite(dht11_pin, HIGH);
}

void read_dht11(uint8_t dht11_pin)
{
  uint16_t rawHumidity = 0;
  uint16_t rawTemperature = 0;
  uint8_t checkSum = 0;
  uint16_t data = 0;

  uint8_t humi;
  uint8_t humd;
  uint8_t tempi;
  uint8_t tempd;

  unsigned long startTime;

  for (int8_t i = -3; i < 80; i++)
  {
    byte live;
    startTime = micros();

    do
    {
      live = (unsigned long)(micros() - startTime);
      if (live > 90)
      {
        Serial.println("ERROR_TIMEOUT");
        return;
      }
    } while (digitalRead(dht11_pin) == (i & 1) ? HIGH : LOW);

    if (i >= 0 && (i & 1))
    {
      data <<= 1;

      // TON of bit 0 is maximum 30 usecs and of bit 1 is at least 68 usecs.
      if (live > 30)
      {
        data |= 1; // we got a one
      }
    }

    switch (i)
    {
    case 31:
      rawHumidity = data;
      break;
    case 63:
      rawTemperature = data;
    case 79:
      checkSum = data;
      data = 0;
      break;
    }
  }

  Serial.println("Temperature Raw Data: ");
  dec2bin(rawTemperature);
  Serial.print("\t");
  tempi = rawTemperature >> 8;
  dec2bin8(tempi);
  Serial.print("\t");
  rawTemperature = rawTemperature << 8;
  tempd = rawTemperature >> 8;
  dec2bin8(tempd);
  Serial.println("");
  Serial.print("Temperature Degree Celcius: ");
  Serial.print(tempi);
  Serial.print(".");
  Serial.print(tempd);
  Serial.print("C");
  Serial.println("");
  float x = tempi + 0.1 * tempd;
  float temp_f = x * (9.0 / 5.0) + 32.0;
  Serial.print("Temperature Degree Farenheit: ");
  Serial.print(temp_f);

  Serial.println("");
  Serial.println("");
  Serial.println("");
}

void loop()
{
  for (unsigned int x = 0; x < 1000; x++)
  {
    wait_for_dht11();
    start_signal(2);
    read_dht11(2);
  }
  Serial.end();
}