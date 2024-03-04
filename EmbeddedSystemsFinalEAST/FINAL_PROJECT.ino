#include<avr/io.h> 
#include<avr/delay.h>
#include<avr/interrupt.h>

static int duty_cycle = 200; //Duty cycle starting point of 200
static int delta = 20; //Delta is the amount the duty cycle will change every intterupt
int binary_num = 0;//Binary num is a counter for the current number to be converted to binary

//Binary check takes the current binary number and sets the ports accordingly
//(I WANTED TO MAKE THIS BASED ON AN ARRAY WITH MATH LIKE PROJECT 3 BUT I COULDN'T FIGURE IT OUT FOR BARE METAL)
void binary_check(int bin)
{
  int number = bin;

  if(number == 0){
    PORTB = 0b00000000;
  }
  else if(number == 1){
    PORTB = 0b00000001;
  }
  else if(number == 2){
    PORTB = 0b00000010;
  }
  else if(number == 3){
    PORTB = 0b00000011;
  }
  else if(number == 4){
    PORTB = 0b00000100;
  }
  else if(number == 5){
    PORTB = 0b00000101;
  }
  else if(number == 6){
    PORTB = 0b00000110;
  }
  else if(number == 7){
    PORTB = 0b00000111;
  }
  else if(number == 8){
    PORTB = 0b00001000;
  }
  else if(number == 9){
    PORTB = 0b00001001;
  }
  else if(number == 10){
    PORTB = 0b00001010;
  }
  else if(number == 11){
    PORTB = 0b00001011;
  }
  else if(number == 12){
    PORTB = 0b00001100;
  }
  else if(number == 13){
    PORTB = 0b00001101;
  }
  else if(number == 14){
    PORTB = 0b00001110;
  }
  else if(number == 15){
    PORTB = 0b00001111;
  }
}

//ISR for the compare match A interrupt of Timer1.
ISR(TIMER1_COMPA_vect) // Interrupt Service Routine for overflow mode
{
  duty_cycle = duty_cycle + delta; //Every time we hit the interrupt add delta to the duty cycle 

  //If duty cycle is higher than max of OCR0A (LED BLINK)
  if (duty_cycle >= 0xFF ){

    //Increase the binary counter if within range
    if(binary_num<15){
      binary_num = binary_num+1;
    }
    //Reset when it gets too high
    else{
      binary_num = 0;
    }
    delta = -delta; // reverse direction: increasing <--> decreasing duty cycle
  }
  //When duty cycle is zero
  if (duty_cycle == 0){
    delta = -delta; // reverse direction: increasing <--> decreasing duty cycle
  }

  //Once duty cycle ,delta , and binary_num are updated
  binary_check(binary_num);//Call binary_check to set the binary counter LEDs
  OCR0A = duty_cycle; //Set brightness of pulsing LED using PWM duty cycle
  _delay_ms(100);
}

//MAIN FUNCTION
int main(){
  cli(); //Clear interrupt data
  DDRD = (1<<6); //Set port D6 as output
  DDRB = 0b00001111; // Set pins 8-11 as output pins 
  PORTB = 0b00000000; //set the pin logic to 0 // pull up register inactive

  OCR0A = 10; //Controls PWM 
  TCCR0A = 0b10000011;
  TIMSK0 = 0b00000000; //No need for interrupt 
  TCCR0B = 0b00000100;

  //Controls separate timer for interrupt
  OCR1A = 47999; //What point the comparison happens 
  TCCR1A = 0x00; //No pin behavior
  TCCR1B = 0b00001001; //WGM bit compare to OCR1A to timer and set prescale
  TIMSK1 = 0b00000010; // only set up compa 

  sei();
  while(true){
  } 
}
