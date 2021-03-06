 

#include <stdarg.h> 

#include<LedControl.h> 

#include <Wire.h> // Added library* This is for the LCD SCREEN  

#include <LiquidCrystal_I2C.h> // Added library* This is for the LCD screen  

LiquidCrystal_I2C lcd(0x27,20,4);//I2C address for the LCD display and declaring it //as a 20x4 //display                  

LedControl lc=LedControl(12,11,10,1); // this is declaring the pins for the LED 8x8 matrix 

/**********************************************************/ // this will be used to split up code as it //makes it easier for me to read and write  

int BL1=2; // player 1 Left button on pin 2 

int BR1=3; // player 1 Right button on pin 3 

int BL2=4; // player 2 Left button on pin 4 

int BR2=5; // player 2 Right button on pin 5 

int buzz = 9; // for buzzer pin 9 

//All bat parameters are in arrays, where the first parameter is bat 0 and the second is bat 1 

int batSize[]={3,3}; 

int batPos[]={0,0}; 

int height=8; //Height (and width) of the matrix 

 

//Score for player 0 and 1/ 

int score[]={0,0}; 

int rbx = (int)random(3,6); //random number between 3 and 6 for the x coordinate for the ball 

int rby = (int)random(3,6); //random number between 3 and 6 for the y coordinate for the ball 

// as stated above this generate a random number for the ball position between the 3rd and 6th //LED on the Matrix 

int ballPos[]={rbx,rby}; 

 

const int RandVX [] = {-1,1}; // this is an array that has the ball directions of -1 and 1 

int rvx = RandVX [random (0, 2)]; // this random function uses the array to pick one of the 2 //numbers to generate the random number  

// this is so that it doesn't pick 0 as the wall wouldn't move in a diagonal direction. 

 

const int RandVY [] = {-1,1}; // this is an array that has the ball directions of -1 and 1 

int rvy = RandVY [random (0, 2)]; // this random function uses the array to pick one of the 2 //numbers to generate the random number 

int ballVec[]={rvx,rvy}; //the vector direction where the ball starts 

 

/**********************************************************/ 

byte Pacman[] = { // pacman for the intial screen  

  // pacman might be relevent for pong but it looks nice as a intial display  

  B00111100, 

  B01111110, 

  B01111011, 

  B11111111, 

  B11111111, 

  B11111111, 

  B11101111, 

  B01000110 

  }; 

 

/**********************************************************/ 

int moveBat(int batNum, int dir){ 

   //moves a bat 

   //batNum is 0 or 1, 0 for left bat, 1 for right 

   //dir is a number. -1 or 1 will move the bat up or down. Other numbers will be ignored 

   //Function returns -1 if the bat hit an edge and couldn't move the full distance 

   //   0 on success 

   //   1 when given an incorrect value for dir 

   if(dir==-1 || dir==1){ 

     batPos[batNum]+=dir; 

     if(batPos[batNum]<0){ 

       batPos[batNum]=0; 

       return -1; 

     } 

     if(batPos[batNum]+batSize[batNum]>height){ 

       batPos[batNum]=height-batSize[batNum];   

       return -1; 

     } 

     return 0; 

   } 

   return 1; 

} 

/**********************************************************/ 

//Move the ball one step, bouncing if necessary 

//returns -1 unless the ball hits a left or right edge 

//If it hits the left-hand edge it returns 0 

//If it hits the right-hand edge it returns 1 

//If it hits a bat, it returns -1 

int ballStep(){ 

  int next[]={0,0}; 

  next[0]=ballPos[0]+ballVec[0]; 

  next[1]=ballPos[1]+ballVec[1]; 

  int edgeNum=-1; 

  //Check top/bottom 

  if(next[1]<0 || next[1]>=height){ 

     ballVec[1]*=-1; 

  } 

  //Now check bats... 

  if(next[0]==0 && ballPos[1]>=batPos[0] && ballPos[1]<(batPos[0]+batSize[0])){ 

    ballVec[0]*=-1; 

    lcd.setCursor(0,0); // this sets where the text will appear in the LCD screen 

    lcd.print("Hit bat 0"); // this prints to the LCD screen  

    } 

   else if(next[0] < 0){ 

    lcd.setCursor(0,1);// this sets where the text will appear in the LCD screen below the Bat hitter //text 

    lcd.print("Left edge hit");// this prints to the LCD screen 

    ballVec[0]*=-1; 

    edgeNum=0; 

    digitalWrite(buzz, HIGH); // this turns on the buzzer when the edge has been hit  

    delay(100); 

    digitalWrite(buzz, LOW); //this turns off the buzzer after the delay  

    } 

  if(next[0]==height-1 && ballPos[1]>=batPos[1] && ballPos[1]<batPos[1]+batSize[1]){ 

    ballVec[0]*=-1; 

    lcd.setCursor(0,0); // this sets where the text will appear in the LCD screen 

    lcd.print("Hit bat 1"); // this prints to the LCD screen 

    } 

  else if(next[0] >= height){ 

    lcd.setCursor(0,1);// this sets where the text will appear in the LCD screen below the Bat hitter //text 

    lcd.print("Right edge hit");// this prints to the LCD screen 

    ballVec[0]*=-1; 

    edgeNum=1; 

    digitalWrite(buzz, HIGH); // this turns on the buzzer when the edge has been hit  

    delay(100); 

    digitalWrite(buzz, LOW); //this turns off the buzzer after the delay  

    } 

  ballPos[0]+=ballVec[0]; 

  ballPos[1]+=ballVec[1]; 

  return edgeNum;; 

  } 

/**********************************************************/ 

void drawGame(){ 

 lc.clearDisplay(0); 

 //Bats 

 for(int i=0; i<2; i++) 

   for(int j=0; j<batSize[i];j++) 

     lc.setLed(0,i*(height-1),j+batPos[i],true);  //Ball 

     lc.setLed(0,ballPos[0],ballPos[1],true); 

       } 

/**********************************************************/ 

void setup() { 

  /* 

   The MAX72XX is in power-saving mode on startup, 

   we have to do a wakeup call 

   */ 

  pinMode(BL1,INPUT); //setting the button to an input 

  pinMode(BR1,INPUT); //setting the button to an input 

  pinMode(BL2,INPUT); //setting the button to an input 

  pinMode(BR2,INPUT); //setting the button to an input 

  pinMode(buzz, OUTPUT);//setting the buzzer to an output 

  lc.shutdown(0,false); 

  /* Set the brightness to a medium values */ 

  lc.setIntensity(0,1); 

  /* and clear the display */ 

  lc.clearDisplay(0); 

  lcd.init(); // 20 x 4 LCD module  initialize the lcd  

  lcd.backlight(); // turns the back light on  

  lcd.clear(); // clears the LCD display  

  randomSeed(A1); // initializes the pseudo-random number generator, it is works on an //unconnected pin 

   

  newGame(); // this makes it so that the program starts at newGame() instead of the default of //starting at the loop 

  } 

/**********************************************************/ 

void loop(){ 

  drawGame(); 

  int result; 

  result=ballStep(); 

  if(result==0){ 

    score[1]+=1; 

  } 

  if(result==1){ 

   score[0]+=1; 

  } 

  lcd.setCursor(0,3); // this sets where the text will appear in the LCD screen below the Edge hit  //3rd line down 

  lcd.print("Score is "); // this prints to the LCD screen  

  lcd.setCursor(10,3); // this sets where the text will appear which is 3rd line and 10 character //spaces in 

  lcd.print(score[0]); // this print the score of player 1 to the LCd 

  lcd.setCursor(13,3); // this sets where the text will appear which is 3rd line and 13 character //spaces in 

  lcd.print(score[1]); // this print the score of player 2 to the LCd 

 

  int val1l = digitalRead(BL1); //reads player 1 left button 

   if(val1l == HIGH){ // when button value is high then the if statement is true 

      moveBat(0,1); // moves bat1 up  

      } 

  int val1r = digitalRead(BR1);//reads player 1 right button 

   if (val1r == HIGH){ // when button value is high then the if statement is true 

    moveBat(0,-1); // moves bat1 down  

      } 

  int val2l = digitalRead(BL2); //reads player 2 left button 

   if(val2l == HIGH){ // when button value is high then the if statement is true 

    moveBat(1,1);  // moves bat2 up  

      } 

  int val2r = digitalRead(BR2);//reads player 2 right button 

   if (val2r == HIGH){ // when button value is high then the if statement is true 

    moveBat(1,-1); // moves bat2 down  

      }       

 

  if ( score[0] == 5){ //if statement for when score of player 1 is 5 

    lcd.clear(); // clears the matrix  

    lcd.setCursor(0,0); // sets where the text will appear on the LCD screen  

    lcd.print("Player 1 Wins :)");// Print a message to the LCD. 

    delay(500); // delays by half a second 

    WIN(); // goes to WIN() where the screen will do some pretty lines 

    } 

   

  if (score[1] == 5){ //if statement for when score of player 2 is 5 

    lcd.clear(); // clears the matrix  

    lcd.setCursor(0,0); // sets where the text will appear on the LCD screen  

    lcd.print("Player 2 Wins :)"); // Print a message to the LCD. 

    delay(500); // delays by half a second 

    WIN(); // goes to WIN() where the screen will do some pretty lines 

    } 

 

  delay(170); // this delay is crucial as it controls how fast the ball moves 

   

  } 

/**********************************************************/ 

void WIN(){ 

  // this function runs when it is called which is when a player wins  

  // in this function a screen turns on a column of leds at a time which is 10 milli seconds 

  lc.clearDisplay(0); // clears the led matrix  

  lc.setColumn(0,7,B11111111); 

  delay(10); 

  lc.setColumn(0,6,B11111111); 

  delay(10); 

  lc.setColumn(0,5,B11111111); 

  delay(10); 

  lc.setColumn(0,4,B11111111); 

  delay(10); 

  lc.setColumn(0,3,B11111111); 

  delay(10); 

  lc.setColumn(0,2,B11111111); 

  delay(10); 

  lc.setColumn(0,1,B11111111); 

  delay(10); 

  lc.setColumn(0,0,B11111111); 

  delay(10); 

  lc.setColumn(0,7,B00000000); 

  delay(10); 

  lc.setColumn(0,6,B00000000); 

  delay(10); 

  lc.setColumn(0,5,B00000000); 

  delay(10); 

  lc.setColumn(0,4,B00000000); 

  delay(10); 

  lc.setColumn(0,3,B00000000); 

  delay(10); 

  lc.setColumn(0,2,B00000000); 

  delay(10); 

  lc.setColumn(0,1,B00000000); 

  delay(10); 

  lc.setColumn(0,0,B00000000); 

  delay(10); 

 

  newGame(); // calls the function of newGame() 

} 

/**********************************************************/ 

void snew(){ 

// this for loop turns on a row of LEDs on the matrix at a time  

  for(int i=0;i<8;i++){  

      lc.setRow(0,i,Pacman[i]); //     

      } 

  lcd.setCursor(0,0); //sets where the text will appear on the LCD screen  

  lcd.print("Press left Button   Player 1 to start");   // prints the text to the LCD display    

} 

/**********************************************************/ 

void newGame(){ 

  snew(); // calls the function of snew()  

  while(digitalRead(BL1) == false){ // this while is looks constantly for the BL1 to be pressed it will //allow the program to continue 

    // once the button has been pressed  

  } 

  score[0] = 0; // resets the score to 0 for player 1 

  score[1] = 0; // resets the score to 0 for player 2 

   

  lcd.clear(); // clears the LCD display  

  batPos[0]=0; // this resets the bat position for every new game for Bat 1 

  batPos[1]=0; // this resets the bat position for every new game for Bat 2 

  } 

/**********************************************************/ 
