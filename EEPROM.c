#include <mega16.h>
#include <delay.h>
#include <alcd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define bit_set(r, b) r |= (1 << b)
#define bit_clr(r, b) r &= ~(1 << b)

char keypad();
void ChangePasswordUser();
void ChangePasswordAdmin();
void EE_Write(unsigned int add, unsigned char data);
unsigned char EE_Read(unsigned int add);

unsigned int storedPassword = 0;
unsigned int OldPassword = 0;
unsigned int NewPassword = 0;
unsigned int ReenterNewPassword = 0;
unsigned int ChangeAdminPasswords = 0;
unsigned int ChangeAdminPasswordsForUsers = 0;

unsigned int pass1;
unsigned int pass2;
unsigned int pass3;
unsigned int password;
unsigned int id1;
unsigned int id2;
unsigned int id3;
unsigned int enteredID;
unsigned int NewID = 0;
void initializeEEPROM() {

  // Initialize EEPROM with some default values       

  unsigned int defaultPassword1 = 203;
  unsigned int defaultPassword2 = 129;
  unsigned int defaultPassword3 = 325;
  unsigned int defaultPassword4 = 426;
  unsigned int defaultPassword5 = 170;
  EE_Write(111, defaultPassword1 % 255);
  EE_Write(112, defaultPassword1 / 255);
  EE_Write(126, defaultPassword2 % 255);
  EE_Write(127, defaultPassword2 / 255);
  EE_Write(128, defaultPassword3 % 255);
  EE_Write(129, defaultPassword3 / 255);
  EE_Write(130, defaultPassword4 % 255);
  EE_Write(131, defaultPassword4 / 255);
  EE_Write(132, defaultPassword5 % 255);
  EE_Write(133, defaultPassword5 / 255);

  // Add more passwords if needed
}

void main(void) {
  DDRC = 0b00000111; // 1 pin unused, 4 rows (input), 3 columns (output)
  PORTC = 0b11111000; // pull-up  resistance  to avoid floating  for keypad
  DDRD .2 = 0; // INT0 Admin
  PORTD .2 = 1; // the output pin doesnt use
  DDRD .3 = 0; // INT1 Set PC for user
  PORTD .3 = 1; // the output pin doesnt use
  DDRD .1 = 1; //Motor
  PORTD .1 = 0; // the input pin doesnt use
  DDRD .5 = 1; // Alarm
  PORTD .5 = 0;// the input pin doesnt use
  // INT0
  bit_set(MCUCR, 1); //MCUCR |= (1<<1)
  bit_clr(MCUCR, 0); //MCUCR &= ~(1<<0)
  // INT1
  bit_set(MCUCR, 3);
  bit_clr(MCUCR, 2);
  #asm("sei"); // Set Enable Interrupt
  bit_set(GICR, 6); // EXT0 specific enable
  bit_set(GICR, 7); // EXT1
  lcd_init(16); // Important to initialize the LCD, Give it the number of characters per line

  initializeEEPROM(); // Initialize EEPROM with default values

  while (1) {
    // Main application loop
    // Code for handling user inputs and controlling the door access
    lcd_clear();
    lcd_printf("Press * to Enter");
    while (keypad() != 10);
    lcd_clear();
    lcd_printf("Entered ID:");
    id1 = keypad();
    id2 = keypad();
    id3 = keypad();
    enteredID = id3 + (id2 * 10) + (id1 * 100);
    lcd_gotoxy(0, 1);
    lcd_printf("%u", enteredID);
    //while (keypad() != 10);
    //lcd_clear();

    if (enteredID == 111 || enteredID == 126 || enteredID == 128 || enteredID == 130 || enteredID == 132) // enteredID == 111 || enteredID == 222 || enteredID == 333 || enteredID == 444 || enteredID == 555
    {
      lcd_clear();
      lcd_printf("Enter Password:\n");
      lcd_gotoxy(0, 1);
      pass1 = keypad();
      pass2 = keypad();
      pass3 = keypad();
      password = (pass1 * 100) + (pass2 * 10) + (pass3 * 1);

      lcd_printf("%u", password);

      storedPassword = EE_Read(enteredID);
      storedPassword = storedPassword + (EE_Read(enteredID + 1) * 255);

      lcd_printf("%u", storedPassword);

      //while (keypad() != 10);
      if (password == storedPassword && enteredID == 111) {
        lcd_clear();
        lcd_gotoxy(5, 0);
        lcd_printf("Welcome!");
        lcd_gotoxy(7, 1);
        lcd_printf("Prof");
        delay_ms(1000);
        lcd_clear();
        lcd_printf("Door is opening");
        lcd_gotoxy(0, 1);
        lcd_printf("Press * to Enter");
        PORTD .1 = 1;
      } else if (password == storedPassword && enteredID == 126) {
        lcd_clear();
        lcd_gotoxy(5, 0);
        lcd_printf("Welcome!");
        lcd_gotoxy(7, 1);
        lcd_printf("John");
        delay_ms(1000);
        lcd_clear();
        lcd_printf("Door is opening");
        lcd_gotoxy(0, 1);
        lcd_printf("Press * to Enter");
        PORTD .1 = 1;
      } else if (password == storedPassword && enteredID == 128) {
        lcd_clear();
        lcd_gotoxy(5, 0);
        lcd_printf("Welcome!");
        lcd_gotoxy(7, 1);
        lcd_printf("Yacoub");
        delay_ms(1000);
        lcd_clear();
        lcd_printf("Door is opening");
        lcd_gotoxy(0, 1);
        lcd_printf("Press * to Enter");
        PORTD .1 = 1;
      } else if (password == storedPassword && enteredID == 130) {
        lcd_clear();
        lcd_gotoxy(5, 0);
        lcd_printf("Welcome!");
        lcd_gotoxy(7, 1);
        lcd_printf("Mohamed");
        delay_ms(1000);
        lcd_clear();
        lcd_printf("Door is opening");
        lcd_gotoxy(0, 1);
        lcd_printf("Press * to Enter");
        PORTD .1 = 1;
      } else if (password == storedPassword && enteredID == 132) {
        lcd_clear();
        lcd_gotoxy(5, 0);
        lcd_printf("Welcome!");
        lcd_gotoxy(7, 1);
        lcd_puts("Rashad");
        delay_ms(1000);
        lcd_clear();
        lcd_printf("Door is opening");
        lcd_gotoxy(0, 1);
        lcd_printf("Press * to Enter");
        PORTD .1 = 1;
      } else {
        lcd_clear();
        lcd_printf("Wrong Password");
        // alarm
        PORTD .5 = 1;
        delay_ms(1000); // Wait for 1 second
        PORTD .5 = 0;
        continue; // Restart the loop to re-enter a valid password
      }
    } else {
      lcd_clear();
      lcd_printf("Invalid ID");
      // Alarm
      PORTD .5 = 1;
      delay_ms(1000); // Wait for 1 second
      PORTD .5 = 0;
      delay_ms(1000);
      PORTD .5 = 1;
      delay_ms(1000); // Wait for 1 second
      PORTD .5 = 0;
      delay_ms(1000);
      continue; // Restart the loop to re-enter a valid ID
    }

    // Optional: You can add a delay or additional logic here
    while (keypad() != 10);
    //ChangePasswordUser( enteredID);
    // Wait for the '*' key to be released
    if (keypad() == 11)
      PORTD .1 = 0;
  }
}
// Function to get the pressed key from the keypad
char keypad() {
  while (1) {
    PORTC .0 = 0; // column 1 is activated by 0
    PORTC .1 = 1; // column 2 is inactive by 1
    PORTC .2 = 1; // column 3 is inactive by 1
    switch (PINC) {
      // 0bxrrrrccc
    case 0b11110110:
      while (PINC .3 == 0);
      return 1;
      break;
    case 0b11101110:
      while (PINC .4 == 0);
      return 4;
      break;
    case 0b11011110:
      while (PINC .5 == 0);
      return 7;
      break;
    case 0b10111110:
      while (PINC .6 == 0);
      return 10; // '*' corresponds to number 10
      break;
    }
    PORTC .0 = 1; // column 1 is inactive by 1
    PORTC .1 = 0; // column 2 is activated by 0
    PORTC .2 = 1; // column 3 is inactive by 1
    switch (PINC) {
      // 0bxrrrrccc
    case 0b11110101:
      while (PINC .3 == 0);
      return 2;
      break;
    case 0b11101101:
      while (PINC .4 == 0);
      return 5;
      break;
    case 0b11011101:
      while (PINC .5 == 0);
      return 8;
      break;
    case 0b10111101:
      while (PINC .6 == 0);
      return 0;
      break;
    }
    PORTC .0 = 1; // column 1 is inactive by 1
    PORTC .1 = 1; // column 2 is inactive by 1
    PORTC .2 = 0; // column 3 is activated by 0
    switch (PINC) {
      // 0bxrrrrccc
    case 0b11110011:
      while (PINC .3 == 0);
      return 3;
      break;
    case 0b11101011:
      while (PINC .4 == 0);
      return 6;
      break;
    case 0b11011011:
      while (PINC .5 == 0);
      return 9;
      break;
    case 0b10111011:
      while (PINC .6 == 0);
      return 11;
      break;
    }
  }
}
// Function to read a byte from EEPROM
unsigned char EE_Read(unsigned int add) {
  while (EECR .1 == 1); //Wait till EEPROM is ready
  EEAR = add; //Prepare the address you want to read from

  EECR .0 = 1; //Execute read command

  return EEDR;

}
// Function to write a byte to EEPROM
void EE_Write(unsigned int add, unsigned char data) {
  while (EECR .1 == 1); //Wait till EEPROM is ready
  EEAR = add; //Prepare the address you want to read from
  EEDR = data; //Prepare the data you want to write in the address above
  EECR .2 = 1; //Master write enable
  EECR .1 = 1; //Write Enable    

}
// Function to change the password for users
void ChangePasswordUser() {

  lcd_clear();
  lcd_printf("Enter_ID");
  NewID = (keypad() * 100) + (keypad() * 10) + keypad();
  lcd_gotoxy(0, 1);
  lcd_printf("%u", NewID);
  while (keypad() != 10);
  lcd_clear();
  lcd_printf("Enter Old-PC");
  lcd_gotoxy(0, 1);

  OldPassword = 0;

  OldPassword = (keypad() * 100) + (keypad() * 10) + keypad();

  storedPassword = EE_Read(NewID);
  storedPassword = storedPassword + (EE_Read(NewID + 1) * 255);
  lcd_printf("%u", OldPassword);
  lcd_printf("%u", storedPassword);

  while (keypad() != 10);

  if (OldPassword == storedPassword) {
    lcd_clear();
    lcd_printf("Enter New-PC");
    lcd_gotoxy(0, 1);
    //NewPassw1 = keypad();
    // NewPassw2 = keypad();
    //NewPassw3 = keypad();
    NewPassword = (keypad() * 100) + (keypad() * 10) + keypad();
    lcd_printf("%u", NewPassword);
    while (keypad() != 10);
    lcd_clear();
    lcd_printf("Re-enter PC");
    lcd_gotoxy(0, 1);

    ReenterNewPassword = (keypad() * 100) + (keypad() * 10) + keypad();
    lcd_printf("%u", ReenterNewPassword);
    while (keypad() != 10);
    if (ReenterNewPassword == NewPassword) {
      lcd_clear();

      EE_Write(NewID, NewPassword % 255);
      EE_Write(NewID + 1, NewPassword / 255);
      lcd_printf("Change");
      lcd_gotoxy(0, 1);
      lcd_printf("Successfully");
      delay_ms(1000);

      lcd_clear();
    } else {
      PORTD .5 = 1;
      delay_ms(1000);
      PORTD .5 = 0;
      delay_ms(1000);
    }

  } else {
    PORTD .5 = 1;
    delay_ms(1000);
    PORTD .5 = 0;
    delay_ms(1000);
    lcd_clear();
    //   delay_ms(1000);
    ChangePasswordUser();
  }

}
// Function to change the password for admin
void ChangePasswordAdmin() {
  lcd_clear();
  lcd_printf("You are Admin");
  while (keypad() != 10);
  lcd_clear();
  lcd_printf("Enter PC: ");
  lcd_gotoxy(0, 1);
  OldPassword = (keypad() * 100) + (keypad() * 10) + keypad();
  storedPassword = EE_Read(111);
  storedPassword = storedPassword + (EE_Read(112) * 255);

  lcd_printf("%u", OldPassword);
  lcd_printf("%u", storedPassword);
  while (keypad() != 10);

  if (storedPassword == OldPassword) {
    lcd_clear();

    lcd_printf("Entered ID:");
    lcd_gotoxy(0, 1);
    id1 = keypad();
    id2 = keypad();
    id3 = keypad();
    NewID = id3 + (id2 * 10) + (id1 * 100);

    lcd_printf("%u", NewID);
    while (keypad() != 10);
    lcd_clear();

    lcd_printf("Enter-new PC: ");
    lcd_gotoxy(0, 1);
    ChangeAdminPasswordsForUsers = (keypad() * 100) + (keypad() * 10) + keypad();

    lcd_printf("%u", ChangeAdminPasswordsForUsers);

    while (keypad() != 10);
    lcd_clear();
    EE_Write(NewID, ChangeAdminPasswordsForUsers % 255);
    EE_Write(NewID + 1, ChangeAdminPasswordsForUsers / 255);
    lcd_clear();
    lcd_printf("Change");
    lcd_gotoxy(0, 1);
    lcd_printf("Successfully");
    delay_ms(1000);
    lcd_clear();
  } else {
    PORTD .5 = 1;
    delay_ms(1000);
    PORTD .5 = 0;
    delay_ms(1000);
    lcd_clear();
    delay_ms(1000);
    ChangePasswordAdmin();

  }

}
// Admin  High priority

interrupt[2] void ext0(void) {
  ChangePasswordAdmin();

}
// Set PC
interrupt[3] void ext1(void) {
  ChangePasswordUser();
}