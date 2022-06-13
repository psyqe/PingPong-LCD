/*
  LCD Ping-Pong Game
  
  Author :
  	Shaunak Guha
    Dept. of Physics,
    St. Xavier's College, Kolkata
    College ID : 18-500-5-05-0113
    
  Contact :
  	Email : mr.shaunakguha@gmail.com
    Phone : +91 9674384899
  
  Circuit Connections :
  
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
    - Backlight to +5V across 220ohm resistor
    
 * 10K potentiometer :
 	- ends to +5V and ground
 	- wiper to LCD V0 pin
    
 * Further Instructions :
 	- README.txt
    
 */

// LCD Library
#include <LiquidCrystal.h>

//Variables
int frate=200;
byte posL=7, posR=6, paddleSize=5;
byte maxPaddlePos = 16-paddleSize;

//Direction of Movement of Ping-Pong Ball
byte direction;
/*
0 = NW
1 = SW
2 = SE
3 = NE
*/

//Player Scores
unsigned short scoreL=0, scoreR=0;

//Timekeeper
unsigned long x;

//Ball Block Position
short posX, posY, ballPos;

//Ball Pixel Position inside BLock
short ballX, ballY;

//Paddle Pixels -
byte leftUp[8],leftDwn[8],rightUp[8],rightDwn[8];

//Ping-Pong Ball Pixels -
byte ball[8];

// Initializing the LCD with interface pin connections
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//Reading Button Input on demand
//Left Player - Up Button
bool checkUp1() {
	return digitalRead(A0);
}

//Left Player - Down Button
bool checkDwn1() {
	return digitalRead(A1);
}

//Right Player - Up Button
bool checkUp2() {
	return digitalRead(A2);
}

//Right Player - Down Button
bool checkDwn2() {
	return digitalRead(A3);
}

//Setup Function
void setup() {
	// Setting up the LCD's number of columns and rows
	lcd.begin(16, 2);
  
	// Configuring input pins
	pinMode(A0, INPUT);
	pinMode(A1, INPUT);
	pinMode(A2, INPUT);
	pinMode(A3, INPUT);
  
  	//Initial Default Direction
  	direction = random(4);
  
  	//Ball Block Position
	posX = random(3,13);
  	posY = random(2);

	//Ball Pixel Position inside BLock
	ballX = random(5);
  	ballY = random(8);
  
	//Start Warning
	for(int i = 5; i>0; i--){
		lcd.write("Starting in : ");
		lcd.print(i);
		x = millis();
      	while(millis() - x < 300);
		lcd.clear();
	}
  
  	//Get Custom Direction from User
  	x = millis();
	lcd.home();
	lcd.write("Press Button for");
  	lcd.setCursor(0,1);
  	lcd.write("Custom Direction");
  	do{
  		setDirection();
    }while(millis() - x < 2000);
	lcd.clear();
}

//Looping Function
void loop() {
	
	printScores();
   	paintBall();
  	detectInput();
	paintLeftPaddle();
	paintRightPaddle();
  	progress();
  
	//FrameRate delay
  	x = millis();
	while(millis()- x < frate);
	lcd.clear();
}

//Accesory Functions with Self-Explanatory Names
void paintLeftPaddle() {
	int i,j = posL + paddleSize;
	for(i=0; i<8; i++) {

		//Upper Section
		if(i>=posL && i<j)
			leftUp[i]=0x03;
		else
			leftUp[i]=0x00;
		//Lower Section
		if(j>=8 && i>=posL-8 && i<j-8)
			leftDwn[i]=0x03;
		else
			leftDwn[i]=0x00;
	  
  }
  
	//Drawing Left Paddle
	lcd.createChar(0, leftUp);
	lcd.createChar(1, leftDwn);

	lcd.setCursor(1,0);
	lcd.write(byte(0));
	lcd.setCursor(1,1);
	lcd.write(byte(1));
}

void paintRightPaddle() {
	int i,j = posR + paddleSize;
	for(i=0; i<8; i++) {
		//Upper Section
		if(i>=posR && i<j)
			rightUp[i]=0x18;
		else
			rightUp[i]=0x00;
		//Lower Section
		if(j>=8 && i>=posR-8 && i<j-8)
			rightDwn[i]=0x18;
		else
			rightDwn[i]=0x00;
  }
	
	//Drawing Right Paddle
	lcd.createChar(2, rightUp);
	lcd.createChar(3, rightDwn);	
  
	lcd.setCursor(14,0);
	lcd.write(byte(2));
	lcd.setCursor(14,1);
	lcd.write(byte(3));
}

void detectInput() {
	
	//Left Up Button
	if(checkUp1()){
	  if(posL!=0)
		  posL -= 1;
    }
	//Left Down Button
	if(checkDwn1()){
	  posL += 1;
	  if(posL > maxPaddlePos)
		  posL = maxPaddlePos;
    }
	//Right Up Button
	if(checkUp2()){
	  if(posR != 0)
		  posR -= 1;
    }
	//Right Down Button
	if(checkDwn2()){	
	  posR += 1;
	  if(posR > maxPaddlePos)
		  posR = maxPaddlePos;
    }
}

//Returns Pixel Position of Ball
byte getBallPixel(byte x) {
  	switch(x){
    	case 0: return 0x10;
      	case 1: return 0x08;
      	case 2: return 0x04;
      	case 3: return 0x02;
      	case 4: return 0x01;
      	default: return 0x00;
    }
}

void paintBall() {
	for(short i=0; i<8; i++){
      	if(i==ballY)
          	ball[i] = getBallPixel(ballX);
       	else
          	ball[i] = 0x00;
     }
  
	//Drawing Ping-Pong Ball
  	lcd.createChar(4,ball);
  	lcd.setCursor(posX,posY);
  	lcd.write(byte(4));
}

//Calculate Movement of the Ping-Pong Ball
void progress() { 
  	ballPos = ballY + 8*posY;
  	switch(direction){
    	case 0: goNW();
  			break;
  		case 1: goSW();
  			break;
  		case 2: goSE();
  			break;
  		case 3: goNE();
  			break;
  		default: return;
	}
}

//North-West Movement
void goNW(){
  
	ballX -= 1;
  	ballY -= 1;

	//Check Paddle Contact
	if(posX==2 && ballX==-1){
		if(hitLeftPaddle()){
			ballX = 1;
			direction = 3;
			if(ballY==-1){
				if(posY==0){
					direction = 2;
					ballY = 1;
				}
				else if(posY==1){
					ballY = 7;
					posY = 0;
				}
			}
		}
		else
			declareWinner(1);   //Right Wins!
	}
	
	//Behavior at Block Corners and Walls
	if(ballX==-1 && posX>2){
		posX -= 1;
		ballX = 4;
		if(ballY==-1){
			if(posY==0){
				direction = 1;
				ballY = 1;
			}
			else if(posY==1){
				posY = 0;
				ballY = 7;
			}
		}
	}
	
	//Behavior at Block Ceilings
	if(ballY==-1 && ballX>=0){
		if(posY==0){
			direction = 1;
			ballY = 1;
		}
		else if(posY==1){
			ballY = 7;
			posY = 0;
		}
	}
}

//North-East Movement
void goNE(){
  
	ballX += 1;
  	ballY -= 1;

	//Check Paddle Contact
	if(posX==13 && ballX==5){
		if(hitRightPaddle()){
			ballX = 3;
			direction = 0;
			if(ballY==-1){
				if(posY==0){
					direction = 1;
					ballY = 1;
				}
				else if(posY==1){
					ballY = 7;
					posY = 0;
				}
			}
		}
		else
			declareWinner(0);   //Left Wins!
	}
	
	//Behavior at Block Corners and Walls
	if(ballX==5 && posX<13){
		posX += 1;
		ballX = 0;
		if(ballY==-1){
			if(posY==0){
				direction = 2;
				ballY = 1;
			}
			else if(posY==1){
				ballY = 7;
				posY = 0;
			}
		}
	}
	
	//Behavior at Block Ceilings
	if(ballY==-1 && ballX<=7){
		if(posY==0){
			direction = 2;
			ballY = 1;
		}
		else if(posY==1){
			ballY = 7;
			posY = 0;
		}
	}
}

//South-West Movement
void goSW(){
  
	ballX -= 1;
  	ballY += 1;

	//Check Paddle Contact
	if(posX==2 && ballX==-1){
		if(hitLeftPaddle()){
			ballX = 1;
			direction = 2;
			if(ballY==8){
				if(posY==1){
					direction = 3;
					ballY = 6;
				}
				else if(posY==0){
					ballY = 0;
					posY = 1;
				}
			}
		}
		else
			declareWinner(1);   //Right Wins!
	}
	
	//Behavior at Block Corners and Walls
	if(ballX==-1 && posX>2){
		posX -= 1;
		ballX = 4;
		if(ballY==8){
			if(posY==1){
				direction = 0;
				ballY = 6;
			}
			else if(posY==0){
				posY = 1;
				ballY = 0;
			}
		}
	}
	
	//Behavior at Block Floors
	if(ballY==8 && ballX>=0){
		if(posY==1){
			direction = 0;
			ballY = 6;
		}
		else if(posY==0){
			ballY = 0;
			posY = 1;
		}
	}
}

//South-East Movement
void goSE(){
  
	ballX += 1;
  	ballY += 1;

	//Check Paddle Contact
	if(posX==13 && ballX==5){
		if(hitRightPaddle()){
			ballX = 3;
			direction = 1;
			if(ballY==8){
				if(posY==1){
					direction = 0;
					ballY = 6;
				}
				else if(posY==0){
					ballY = 0;
					posY = 1;
				}
			}
		}
		else
			declareWinner(0);   //Left Wins!
	}
	
	//Behavior at Block Corners and Walls
	if(ballX==5 && posX<13){
		posX += 1;
		ballX = 0;
		if(ballY==8){
			if(posY==1){
				direction = 3;
				ballY = 6;
			}
			else if(posY==0){
				posY = 1;
				ballY = 0;
			}
		}
	}
	
	//Behavior at Block Floors
	if(ballY==8 && ballX<=4){
		if(posY==1){
			direction = 3;
			ballY = 6;
		}
		else if(posY==0){
			ballY = 0;
			posY = 1;
		}
	}
}

//User Input for Custom Direction
void setDirection(){
  	if(checkUp1())
      direction = 0;	//NW
  	else if(checkDwn1())
      direction = 1;	//SW
    else if(checkDwn2())
      direction = 2;	//SE
    else if(checkUp2())
      direction = 3;	//NE
}

//Check Collision with Left Paddle
bool hitLeftPaddle(){
 	return (posL<=ballPos && ballPos<(posL+paddleSize));
}

//Check Collision with Right Paddle
bool hitRightPaddle(){
  	return (posR<=ballPos && ballPos<(posR+paddleSize));
}

//Display Scores
void printScores(){
	
	lcd.setCursor(0,0);
	lcd.print(scoreL/10);
	lcd.setCursor(0,1);
	lcd.print(scoreL%10);
	lcd.setCursor(15,0);
	lcd.print(scoreR/10);
	lcd.setCursor(15,1);
	lcd.print(scoreR%10);
  
}

//Display the Winner
void declareWinner(byte w){
	lcd.clear();
  	if(w==0){
		lcd.print("Left WINS!");
		scoreL += 1;
    }
  	else if(w==1){
    	scoreR += 1;
		lcd.print("Right WINS!");
    }
	delay(2000);
	lcd.setCursor(0,1);
	lcd.print("Next game -->");
  	delay(2000);
  
  	//Restarting Game
  	setup();
}