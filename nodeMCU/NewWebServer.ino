/*
 * Copyright (c) 2019 Team Palindrome.
 * All rights reserved.
 *
 * This code is contribted by
 *     - Siddharth M
 *     - Vaishnav Sivadas
 *	   - Aniketh G
 * 	   - Viboothi 
 * 	   - Srijith Krishnan
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY TEAM PALINDROME AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "DHT.h"
#define LIGHTSENSORPIN A0

/* 
 * Pins for used for each of the sensors
 * Sensors used are :
 * DHT - for temperature
 * pir - for Motion
 *
 */

int pir_sensor = D0;
DHT dht;
int ledPin=D2;
boolean d4 =0;
int light1 =D5;
int light2 =D6;
int fan1 =D7;
int fan2 =D8;

const char* ssid = "abc";
const char* password = "qwertyuiop";

WiFiServer server(80);
String header;


WiFiUDP Udp;
unsigned int localUdpPort = 4210;  // local port to listen on
char incomingPacket[1024];  // buffer for incoming packets
String na="nan";
float td1,td2;

// Array containing the temperature counts
float temperature_count[15];
// Average of the temperatures
float temp_avg;
// Counting temperature values
int temp_ctr = 0;

/*
 * Mode value of 0 - normal mode
 * Mode value of 1 - secure mode
 */
int mode = 0;

/*
 * mutex value of 1 - person in room
 * mutex value of 0 - no one in room
 */
int mutex = 0;

/* 
 * Initial setup(FSM State #0) 
 * - connect to the specified wifi 
 * - Open a Udp port for listening 
 * - Take 15 temperature readings and their average
 *
 */
void setup()
{
		Serial.begin(115200);
		pinMode(LIGHTSENSORPIN,  INPUT);  
		dht.setup(D1);   /* D1 is used for data communication */
		pinMode(pir_sensor, INPUT);
		pinMode(ledPin, OUTPUT);
		pinMode(light1, OUTPUT);
		pinMode(light2, OUTPUT);
		pinMode(fan1, OUTPUT);
		pinMode(fan2, OUTPUT);

		Serial.printf("Connecting to %s ", ssid);
		WiFi.begin(ssid, password);
		while (WiFi.status() != WL_CONNECTED)
		{ 
				delay(500);
				Serial.print(".");
		}
		Serial.println(" connected");

		Udp.begin(localUdpPort);
		Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
		server.begin();

		Serial.printf("Taking inital temp values");
		temperature_setup();
}

/* 
 * temperature setup  
 * - collect 15 readings from the DHT sensor 
 * - caluclate the average of the values 
 *
 */
long temperature_setup() {
		float curr_val;
		float avg = 0;
		for(int i = 0; i < 15; i++) {
				curr_val = temperature_detection_state();
				temperature_count[i] = curr_val;
				avg += curr_val;
				Serial.printf("Temperature %d = %f", i, curr_val);
				delay(10);
		}
		temp_avg = avg / 15;
}

/* 
 * motion_detection_state (FSM state #5)  
 * - Check whether motion is detected by the pir sensor
 * - Light up LED if motion is detected
 * - Return back whether motion is detected or not
 *
 */
long motion_detection_state()
{
		long d0 = digitalRead(pir_sensor);
		if(d0 == HIGH){
				Serial.println("Motion detected!");
				digitalWrite(ledPin, HIGH);
		}
		else {
				Serial.println("Motion absent!");
				digitalWrite(ledPin, LOW);
		}
		return d0;
}

/* 
 * temperature_detection_state(FSM state #6)  
 * - Return back the temperature from the DTH sensor 
 *
 */
float temperature_detection_state()
{
		float d2 = dht.getTemperature();/* Get temperature value */
		return d2;
}

/* 
 * mode_select_state(FSM state #2)  
 * - Check if any UDP packets have arrived 
 * - if the UDP packets contain 1 - change mode to secure 
 * - else change the mode to normal 
 *
 */
int mode_select_state()
{
		int packetSize = Udp.parsePacket();
		if (packetSize) {
				Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
				int len = Udp.read(incomingPacket, 255);
				if (len > 0) {
						incomingPacket[len] = 0;
				}
				Serial.printf("UDP packet contents: %s\n", incomingPacket);
				if (incomingPacket[0] == '1') {
						mode = 1;
				}
				else {
						mode = 0;
				}
		}
}

/* 
 * normal_mode(FSM state #3)  
 * This is the basic mode of the Secure System
 * Desgined for a one person enviroment
 * - If we detect motion we either switch on or off the lights 
 * based on the previous state
 *
 */
int normal_mode()
{
		Serial.printf("Now in normal mode\n");
		if (motion_detection_state()) {
				if (mutex) {
						lights_off_state();
						mutex = 0;
				}
				else {
						lights_on_state();
						mutex = 1;
				}
		}
}

/* 
 * update_temp(FSM state #9)  
 * - Updates the latest temperature into the array 
 * - Find the new average and update them
 *
 */
long update_temp(float temp) {
		Serial.printf("Now in update temp\n");
		temperature_count[temp_ctr % 15] = temp;
		float avg = 0;
		for (int i = 0; i < 15; i++) {
				avg += temperature_count[i];
		}	
		temp_avg = avg / 15;
		Serial.printf("Average Temp = %f\n", temp_avg);
		temp_ctr++;
}

/* 
 * alert_intruder_state(FSM state #7)  
 * - alert the website that a intruder was found 
 *
 */
int alert_intruder_state() {
		/* Alert the website here */
		Serial.printf("Alert intruder state\n");
		delay(100);
}

/* 
 * alert_fire_state(FSM state #8)  
 * - alert the website that there might be fire 
 *
 */
int alert_fire_state() {
		/* Alert the website here */
		Serial.printf("Alert fire state\n");

		delay(100);
}

/* 
 * secure_mode(FSM state #4)
 * Mode used to detect intruders and fire 
 * - go to alert_intruder_state if motion present 
 * - if temp has a 10 C spike over average go to temperature_detection_state
 * - update the temperature 
 *
 */
int secure_mode()
{
		Serial.printf("Now in secure mode\n");
		if (motion_detection_state()) {
				alert_intruder_state();
		}
		float temp = temperature_detection_state();
		if (temp > temp_avg + 10) {
				alert_fire_state();
		}
		update_temp(temp);
}

/* 
 * lights_on_state(FSM state #9)
 * Switch lights and fan on 
 *
 */
int lights_on_state() { 
		digitalWrite(light1, HIGH);  
		digitalWrite(light2, HIGH);  
		digitalWrite(fan1, HIGH);  
		Serial.print("Lights on \n");
}

/* 
 * lights_off_state(FSM state #10)
 * Switch lights and fan off 
 *
 */
int lights_off_state() { 
		digitalWrite(light1, LOW);  
		digitalWrite(light2, LOW);  
		digitalWrite(fan1, LOW);  
		Serial.print("Lights off \n");
}


/* 
 * loop state(FSM state #1) 
 * - go to mode_select_state 
 * - go to the corresponding state 
 * - delay for a short period 
 *
 */
void loop()
{
		Serial.printf("Now in loop\n");
		mode_select_state();
		if (mode == 1) {
				secure_mode();
		}
		else {
				normal_mode();
		}
		Serial.printf("Now temp avg = %f\n", temp_avg);
		delay(2500);
}
