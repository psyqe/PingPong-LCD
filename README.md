## PingPong Game
A basic ping-pong game written for 16x2 LCD displays to be run on the Arduino platform.

### Interface
Left mini-board for Player 1 with Up and Down buttons. Similar setup for the Player 2 on the right mini-board. Refer diagrams.

### Connections
* Player 1 :
 	- Up Button to analog pin A0
 	- Down Button to analog pin A1
    
 * Player 2 :
 	- Up Button to analog pin A2
 	- Down Button to analog pin A3
    
 * LCD :
 	- RS pin to digital pin 12
 	- Enable pin to digital pin 11
 	- D4 pin to digital pin 5
 	- D5 pin to digital pin 4
 	- D6 pin to digital pin 3
 	- D7 pin to digital pin 2
 	- R/W pin to ground
 	- VSS pin to ground
 	- VCC pin to +5V
   - Backlight to +5V across 220Ω resistor
    
 * 10K potentiometer :
 	- ends to +5V and ground
 	- wiper to LCD V0 pin
    
### Motivation
This project was my submission for IITB FOSSEE 2019 Screening Tasks.
Online simulation available at [Autodesk TinkerCAD](https://www.tinkercad.com/things/cL2SogHJobn?sharecode=Nf75LAH15eyD5xEGsSbnSdjyj0kpy9A-UC08TDohg8E)
