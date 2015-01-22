// LCD module connections
sbit LCD_RS at RC4_bit;
sbit LCD_EN at RC5_bit;
sbit LCD_D4 at RC0_bit;
sbit LCD_D5 at RC1_bit;
sbit LCD_D6 at RC2_bit;
sbit LCD_D7 at RC3_bit;

sbit LCD_RS_Direction at TRISC4_bit;
sbit LCD_EN_Direction at TRISC5_bit;
sbit LCD_D4_Direction at TRISC0_bit;
sbit LCD_D5_Direction at TRISC1_bit;
sbit LCD_D6_Direction at TRISC2_bit;
sbit LCD_D7_Direction at TRISC3_bit;
// End LCD module connections


void main()
{

  int a,b;
  double u;
  char txt[7];
  Lcd_Init();
  Lcd_Cmd(_LCD_CLEAR);          // Clear display
  Lcd_Cmd(_LCD_CURSOR_OFF);     // Cursor off

  TRISB = 0b00010010;           //RB4,RB1 as Input PIN (ECHO)
  TRISC= 0x00;
  Lcd_Out(1,1,"Developed By");
  Lcd_Out(2,1,"CrazyGeo");

  Delay_ms(1000);
  Lcd_Cmd(_LCD_CLEAR);
  T1CON = 0x10;                 //Initialize Timer Module
 for(;;)
 {
  if(PORTB.F1)
  {
    TMR1H = 0;                  //Sets the Initial Value of Timer
    TMR1L = 0;                  //Sets the Initial Value of Timer

    PORTB.F0 = 1;               //TRIGGER HIGH
    Delay_us(10);               //10mS Delay
    PORTB.F0 = 0;               //TRIGGER LOW

    while(!PORTB.F4);           //Waiting for Echo
    T1CON.F0 = 1;               //Timer Starts
    while(PORTB.F4);            //Waiting for Echo goes LOW
    T1CON.F0 = 0;               //Timer Stops

    a = (TMR1L | (TMR1H<<8));   //Reads Timer Value
    a = a/(58.82/2);                //Converts Time to Distance
    a = a + 1;                  //Distance Calibration

    Delay_ms(10);
    
    TMR1H = 0;                  //Sets the Initial Value of Timer
    TMR1L = 0;                  //Sets the Initial Value of Timer
        
    PORTB.F0 = 1;               //TRIGGER HIGH
    Delay_us(10);               //10mS Delay
    PORTB.F0 = 0;               //TRIGGER LOW

    while(!PORTB.F4);           //Waiting for Echo
    T1CON.F0 = 1;               //Timer Starts
    while(PORTB.F4);            //Waiting for Echo goes LOW
    T1CON.F0 = 0;               //Timer Stops

    b = (TMR1L | (TMR1H<<8));   //Reads Timer Value
    b = b/(58.82/2);                //Converts Time to Distance
    b = b + 1;                  //Distance Calibration

    //u=((a-b)*36)/(10);
    if(a>b)
    {
        u= (a-b)*6;
    }
    else
    {
        u= (b-a)*6;
    }

    if(a>=2 && a<=400 && b>=2 && b<=400)          //Check whether the result is valid or not
    {
      IntToStr(b,txt);
      Ltrim(txt);
      Lcd_Cmd(_LCD_CLEAR);
      Lcd_Out(2,1,"Distance = ");
      Lcd_Out_Cp(txt);
      Lcd_Out(2,15,"cm");

      IntToStr(u,txt);
      Ltrim(txt);
      Lcd_Out(1,1,"Speed = ");
      Lcd_Out_Cp(txt);
      Lcd_Out(1,11," m/min");
    }
    else
    {
      Lcd_Cmd(_LCD_CLEAR);
      Lcd_Out(1,1,"Out of Range");
    }
    Delay_ms(100);
  }
  else
  {
      IntToStr(a,txt);
      Lcd_Cmd(_LCD_CLEAR);
      Lcd_Out(1,1,"Press the button");
      Delay_ms(20);
  }
}
}