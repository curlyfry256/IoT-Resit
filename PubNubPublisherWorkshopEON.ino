

#include <SPI.h> //serial peripheral interface
#include <Ethernet.h>
#include <PubNub.h>


byte mac[] = { 0x00, 0x0E, 0xEF, 0x00, 0x00, 0x34 }; //mac address 
// do not change the 0x00 0x0E 0xEF 0x00 0x00 prefix

const int pubLedPin = 11;
const int analogIn = A0;

char pubkey[] = "pub-c-9f573d22-ee55-4c45-aca6-5e4a4d659a2d";
char subkey[] = "sub-c-b8076c32-a4ad-11e9-bd55-12369b68aa39";
char channel[] = "iotchannel";

void setup()
{
	pinMode(pubLedPin, OUTPUT);
	digitalWrite(pubLedPin, LOW);

	Serial.begin(9600);
	Serial.println("Serial set up");

	while (!Ethernet.begin(mac)) {
		Serial.println("Ethernet setup error");
		delay(1000);
	}
	Serial.println("Ethernet set up");

	PubNub.begin(pubkey, subkey);
	Serial.println("PubNub set up");
}

void flash(int ledPin)
{
	/* Flash LED three times. */
	for (int i = 0; i < 3; i++) {
		digitalWrite(ledPin, HIGH);
		delay(100);
		digitalWrite(ledPin, LOW);
		delay(100);
	}
}

void loop()
{
	Ethernet.maintain();

	EthernetClient *client;

	char msg[64] = "{\"eon\":{\"sensor\":";

		sprintf(msg + strlen(msg), "%d", analogRead(analogIn));
	
	strcat(msg, "}}");

	Serial.print("publishing message: ");
	Serial.println(msg);
	client = PubNub.publish(channel, msg);
	if (!client) {
		Serial.println("publishing error");
	} else {
		flash(pubLedPin);
		client->stop();
	}

	delay(500);
}
