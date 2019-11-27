

int reed = 3;
//receiver code
#include <IRremote.h>
int RECV_PIN = 11;
// These variables are used in the receiving function - don't change them:
IRrecv irrecv(RECV_PIN);
decode_results results;
unsigned int irLen = 0;                // The length of the code
unsigned int irBuffer[RAWBUF];         // Storage for raw (UNKNOWN) codes
int codeType;                          // The type of code (NEC, SONY, UNKNOWN, etc.)
unsigned int codeValue;                // The value of a known code
// These variables are used in this sample code:
int i;
//alarm codes setup
int alarm = 12;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(reed, INPUT);
   irrecv.enableIRIn(); // Start the receiver
  pinMode(alarm, OUTPUT);
}

void loop()
{
   //revicer
  Serial.println("Waiting to Receive a Code");
  if (irrecv.decode(&results))
  {
    updateIRReceive(); // Sort out the received code
    Serial.print("Code type: ");
    Serial.println(codeType);
    Serial.print("Code length: ");
    Serial.println(irLen);
    Serial.print("Code value: ");
    Serial.print(codeValue);
    Serial.print(" decimal , ");
    Serial.print(codeValue, HEX);
    Serial.println(" hexadecimal");
    irrecv.resume(); // re-enable receive
  }
  if(digitalRead(reed) == HIGH)
  {
    int t;
    for(t = 0; t<4379; t=t+2)
   {
    tone(alarm, t, 10);
   }
     for(t = 4379; t>0; t=t-2)
   {
    tone(alarm, t, 10);
   }
    };
    //armed turns on the alarm
if (codeValue == 20910 && digitalRead(reed) == HIGH)
  {
    int t;
    for (t = 666; t < 5000; t = t + 2)
    {
      tone(alarm, t, 5);
    }
    for (t = 5000; t > 666; t = t - 2)
    {
      tone(alarm, t, 5);
    }
  }
  //armed but turns off alarm
  if (codeValue == 20910 && digitalRead(reed) == LOW)
  {
    
     digitalWrite(alarm, LOW);
    
  }
   //disarm code
  if (codeValue == 51510)
  {
    digitalWrite(alarm, LOW);
  }

}




// This function does the actual receiving and decoding of the IR signal
// You must include it, and don't change anything in it

void updateIRReceive()
{
  codeType = results.decode_type;
  irLen = results.rawlen;
  if (codeType == UNKNOWN)
  {
    irLen--;
    // We need to convert from ticks to microseconds
    for (int i = 1; i <= irLen; i++)
    {
      if (i % 2)
      {
        // Mark
        irBuffer[i - 1] = results.rawbuf[i] * USECPERTICK - MARK_EXCESS;
        Serial.print(" m");
      }
      else
      {
        // Space
        irBuffer[i - 1] = results.rawbuf[i] * USECPERTICK + MARK_EXCESS;
        Serial.print(" s");
      }
      Serial.print(irBuffer[i - 1], DEC);
    }
    Serial.println();
  }
  else
  {
    if (codeType == NEC)
    {
      Serial.print("Received NEC: ");
      if (results.value == REPEAT)
      {
        // Don't record a NEC repeat value as that's useless.
        Serial.println("repeat; ignoring.");
        return;
      }
    }
    else if (codeType == SONY)
    {
      Serial.print("Received SONY: ");
    }
    else if (codeType == RC5)
    {
      Serial.print("Received RC5: ");
    }
    else if (codeType == RC6)
    {
      Serial.print("Received RC6: ");
    }
    else
    {
      Serial.print("Unexpected codeType ");
      Serial.print(codeType, DEC);
      Serial.println("");
    }
    Serial.println(results.value, HEX);
    codeValue = results.value;
    irLen = results.bits;
  }
}

