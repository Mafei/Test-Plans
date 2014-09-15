/*
  Gadget Factory
  Papilio Pro QuickStart Example
 
 Pressing any of the 4 pushbuttons on the Button/LED Wing will light the corresponding LED. 
 
 BPW5007 Button/LED Wing Reference:
 http://www.gadgetfactory.net/gf/project/bpw5007-butnled/
 
 Hardware:
 * Connect a Button/LED Wing to any available Wing Slot

 created 2010
 by Jack Gassett from existing Arduino code snippets
 http://www.gadgetfactory.net
 
 11/19/2012 Jack Gassett
   -Added LED1 blinking
 
 This example code is in the public domain.
 */

#define PORTAB GPIODATA(0)
#define PORTC GPIODATA(1)

int ledPins[] = { 
  0, 2, 4, 6, 15, 17, 19, 21, 25, 29, 33, 37, 41, 45, 49, 53, 22, 26, 30, 34, 38, 42, 46, 50  };       // an array of pin numbers to which LEDs are attached
int ledCount = 24;           // the number of pins (i.e. the length of the array)

int buttonPins[] = { 
  1, 3, 5, 7, 14, 16, 18, 20, 23, 27, 31, 35, 39, 43, 47, 51, 24, 28, 32, 36, 40, 44, 48, 52 };       // an array of pin numbers to which Buttons are attached
int buttonCount = 24;           // the number of pins (i.e. the length of the array)

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status
int thisPin;
int ledState = LOW;
int incomingByte = 0;

// first visible ASCIIcharacter '!' is number 33:
int thisByte = 33; 
// you can also write ASCII characters in single quotes.
// for example. '!' is the same as 33, so you could also use this:
//int thisByte = '!'; 

void setup() {
  // initialize the LED pins as an output:
  for (int thisPin = 0; thisPin < ledCount; thisPin++)  {
    pinMode(ledPins[thisPin], OUTPUT);  
  }
  
  // initialize the pushbutton pin as an input:
  for (int thisPin = 0; thisPin < ledCount; thisPin++)  {
    pinMode(buttonPins[thisPin], INPUT);      
  }   
  
  //Delay for 1 seconds to prevent detection as a serial mouse.
  delay(2000);
  
  //Setup Serial port and send out Title
  Serial.begin(9600); 

  //Start with a Mem Test
  memtest();
  ioTest();
  
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  pinMode(21, OUTPUT);
  digitalWrite(21, HIGH);

  // prints title with ending line break 
  Serial.println("ASCII Table ~ Character Map"); 
}

void ioTest()
{
  Serial.println("Starting I/O Test");
  Serial.println("The I/O Test will only be succesful if a stimulus board is connected.");
  Serial.println("Do not be alarmed by a failure if a stimulus board is not connected.");  
  GPIOTRIS(0) = 0xFFFFFFFF;
  GPIOTRIS(1) = 0xFFFFFFFF;
  pinMode(0, OUTPUT); 
  testport(&PORTAB, "AL-BH", 0x55557D54, 0xAAAABAAB);
  testport(&PORTC, "CL-CH", 0x255555, 0x2AAAAA);
  Serial.println();
  GPIOTRIS(0) = 0xAAAAAAAA;
  GPIOTRIS(1) = 0xAAAAAAAA;  
  delay(1000);
    // initialize the LED pins as an output:
  for (int thisPin = 0; thisPin < ledCount; thisPin++)  {
    pinMode(ledPins[thisPin], OUTPUT);  
  }
  
  // initialize the pushbutton pin as an input:
  for (int thisPin = 0; thisPin < ledCount; thisPin++)  {
    pinMode(buttonPins[thisPin], INPUT);      
  } 
}

void testport(volatile uint32_t* port, char pName[2], uint32_t check1, uint32_t check2) {
    int status = 0;
    int temp;
    digitalWrite(0, LOW);
    delay(10);
      //Serial.println(*port, HEX);    
    if (*port == check1) {
      status = 1;
    }
    else
      temp = *port;
    digitalWrite(0, HIGH);
    delay(10);
      //Serial.println(*port, HEX);    
    if (*port == check2 && status==1){
      Serial.print(pName);
      Serial.println(" Passed");}
    else{
      Serial.print(pName);
      Serial.println(" Failed");
      Serial.print("Should be:");
      Serial.print(check1, HEX);
      Serial.print(" ");
      Serial.print(check2, HEX);
      Serial.print(" "); 
      Serial.print(check1, BIN);
      Serial.print(" ");
      Serial.println(check2, BIN);      

      Serial.print("Actual   :");
      Serial.print(temp, HEX);
      Serial.print(" ");
      Serial.print(*port, HEX);
      Serial.print(" "); 
      Serial.print(temp, BIN);
      Serial.print(" ");
      Serial.println(*port, BIN);       
      Serial.println();    
    }
}

void printnibble(unsigned int c)
{
	c&=0xf;
	if (c>9)
		Serial.write(c+'a'-10);
	else
		Serial.write(c+'0');
}

void printhexbyte(unsigned int c)
{
	printnibble(c>>4);
	printnibble(c);
}


void printhex(unsigned int c)
{
	printhexbyte(c>>24);
	printhexbyte(c>>16);
	printhexbyte(c>>8);
	printhexbyte(c);
}

void memfill(volatile unsigned*end)
{
	volatile unsigned *c = (volatile unsigned*)0x2000;
	while (c<end) {
		*c = (unsigned)c ^ 0xffffbeef;
        c++;
	}
}

int memcheck(volatile unsigned *end)
{
	volatile unsigned *c = (volatile unsigned*)0x2000;
	while (c<end) {
		unsigned v = (unsigned)c ^ 0xffffbeef;
		if (v !=*c) {
			Serial.print("Error at address "); printhex((unsigned)c); Serial.println("");
			return -1;
		}
		c++;
	}
    return 0;
}

void memtest()
{
	volatile unsigned *ptr = (volatile unsigned *)0x2000;

//	while (!Serial.available());
//	Serial.read();

	Serial.println("Starting memory test from 0x2000 and beyond");
	unsigned v = *((unsigned*)0);
	Serial.print("check value is 0x"); printhex(v); Serial.println("");

	int i;
	for (;;) {
		if (*ptr == v) {
			Serial.print("Found tag at "); printhex((unsigned)ptr); Serial.println("");
			Serial.println("Testing up to that. Filling...");
			memfill(ptr);
			Serial.println("Checking...");
			if (memcheck(ptr)==0) {
				Serial.println("All OK!");
			}
			break;
		}
		ptr++;
	}
        Serial.println();
}

void loop(){
  delay(1000);
  memtest();
  ioTest();

}