/***********************************************************************
 *  Module Name : LCD
 *
 *	File Name   : char_lcd.c
 *
 * 	Authors     : Mahmoud Gaber
 *
 *	Created on  : MON, 19-JAN-20 10:15:18 PM
 *
 ************************************************************************/
#include "char_lcd.h"
#include "DIO.h"
#include "Sos.h"

#define LCD_HIGHER_NIBBLE 0
#define LCD_LOWER_NIBBLE  1
#define LCD_IDEL          2


uint8 gu8_Lcdstate = LCD_HIGHER_NIBBLE;
uint8  gu8_index=0;
uint8 gu8_DATA = 0;



void Lcd_Update(void)
{
	switch(gu8_Lcdstate)
	{
	case LCD_HIGHER_NIBBLE:

		LCD_send_nibble(gu8_DATA);
		DIO_Write(LCD_EN_PORT, LCD_EN_PIN, HIGH);
		gu8_Lcdstate = LCD_LOWER_NIBBLE;
		break;

	case LCD_LOWER_NIBBLE:
		DIO_Write(LCD_EN_PORT, LCD_EN_PIN, LOW);
		LCD_send_nibble(gu8_DATA<<4);
		DIO_Write(LCD_EN_PORT, LCD_EN_PIN, HIGH);
		gu8_Lcdstate = LCD_IDEL;
		break;

	case LCD_IDEL:
		DIO_Write(LCD_EN_PORT, LCD_EN_PIN, LOW);
		gu8_Lcdstate = LCD_HIGHER_NIBBLE ;
		Sos_Delete_Task(Lcd_Update);
		break;

	default:
		/**error**/
		break;

	}
}

void LCD_config_pins(void){

	/* set LCD pins as outputs */
	DIO_Cfg_s ST_dio = {0};
	ST_dio.GPIO = LCD_RS_PORT;
	ST_dio.dir = OUTPUT;
	ST_dio.pins = LCD_RS_PIN;
	DIO_init(&ST_dio);

	ST_dio.GPIO = LCD_RW_PORT;
	ST_dio.pins = LCD_RW_PIN;
	DIO_init(&ST_dio);

	ST_dio.GPIO = LCD_EN_PORT;
	ST_dio.pins = LCD_EN_PIN;
	DIO_init(&ST_dio);

	ST_dio.GPIO = LCD_D4_PORT;
	ST_dio.pins = LCD_D4_PIN;
	DIO_init(&ST_dio);

	ST_dio.GPIO = LCD_D5_PORT;
	ST_dio.pins = LCD_D5_PIN;
	DIO_init(&ST_dio);

	ST_dio.GPIO = LCD_D6_PORT;
	ST_dio.pins = LCD_D6_PIN;
	DIO_init(&ST_dio);

	ST_dio.GPIO = LCD_D7_PORT;
	ST_dio.pins = LCD_D7_PIN;
	DIO_init(&ST_dio);
}

void LCD_init(void)//Initializes LCD
{
	LCD_config_pins();

	SwDelay_ms(20);

	LCD_send_command_(0x33);
	LCD_send_command_(0x32);
	//DIO_Write(LCD_EN_PORT, LCD_EN_PIN, HIGH);

	LCD_send_command_(Four_bit); // 4-bit
	LCD_send_command_(Cursor_on);
	LCD_send_command_(Lcd_clear);
	SwDelay_ms(10);
	LCD_send_command_(Increment_cursor);
	LCD_send_command_(Cursor_off); // Cursor off
}
void LCD_EN_pulse(void){
	DIO_Write(LCD_EN_PORT, LCD_EN_PIN, HIGH);
	SwDelay_ms(1);
	DIO_Write(LCD_EN_PORT, LCD_EN_PIN, LOW);
	SwDelay_ms(1);
}
void LCD_send_nibble(uint8 data){
	uint8 mask = 1;
	data >>= 4;
	if(data&mask)
		DIO_Write(LCD_D4_PORT, LCD_D4_PIN, HIGH);
	else
		DIO_Write(LCD_D4_PORT, LCD_D4_PIN, LOW);

	mask *= 2;
	if(data&mask)
		DIO_Write(LCD_D5_PORT, LCD_D5_PIN, HIGH);
	else
		DIO_Write(LCD_D5_PORT, LCD_D5_PIN, LOW);

	mask *= 2;
	if(data&mask)
		DIO_Write(LCD_D6_PORT, LCD_D6_PIN, HIGH);
	else
		DIO_Write(LCD_D6_PORT, LCD_D6_PIN, LOW);

	mask *= 2;
	if(data&mask)
		DIO_Write(LCD_D7_PORT, LCD_D7_PIN, HIGH);
	else
		DIO_Write(LCD_D7_PORT, LCD_D7_PIN, LOW);
}

void LCD_send_command_(uint8 data)	//Sends Command to LCD
{
	DIO_Write(LCD_RS_PORT, LCD_RS_PIN, LOW);

	LCD_send_nibble(data); // Higher nibble first
	LCD_EN_pulse();

	LCD_send_nibble(data<<4);
	LCD_EN_pulse();
	SwDelay_ms(1);
}

void LCD_send_command(uint8 data)	//Sends Command to LCD
{
	gu8_DATA = data;

	DIO_Write(LCD_RS_PORT, LCD_RS_PIN, LOW);

	str_SOSfg_t  gstr_Lcd;

	gstr_Lcd.Priority = 1;
	gstr_Lcd.Task_State= IDEL;
	gstr_Lcd.Sos_AppTask_cbk = Lcd_Update;
	gstr_Lcd.counter=0;
	gstr_Lcd.periodicity=5;
	Sos_Creat_Task(&gstr_Lcd);
}

void LCD_send_char(uint8 data)		//Sends Char to LCD
{
	gu8_DATA = data;
	//LCD_goto_xy(0,0);
	DIO_Write(LCD_RS_PORT, LCD_RS_PIN, HIGH);
	str_SOSfg_t  gstr_Lcd;

		gstr_Lcd.Priority = 2;
		gstr_Lcd.Task_State= IDEL;
		gstr_Lcd.Sos_AppTask_cbk = Lcd_Update;
		gstr_Lcd.counter=0;
		gstr_Lcd.periodicity=2;
		Sos_Creat_Task(&gstr_Lcd);
}

void LCD_clear(void)				//Clears LCD
{
	LCD_send_command(Lcd_clear);
}

void LCD_send_string(uint8* data)	//Outputs string to LCD
{

	while(data[gu8_index] != '\0'){
		gu8_DATA = data[gu8_index] ;
		LCD_send_char(data[gu8_index++]);
	}
}



void LCD_goto_xy(uint8 x, uint8 y)	//Cursor to X Y position
{
	uint8 DDRAMAddr;
	// remap lines into proper order
	switch(y)
	{
	case 0:
		DDRAMAddr = LCD_LINE0_DDRAMADDR+x;
		break;
	case 1:
		DDRAMAddr = LCD_LINE1_DDRAMADDR+x;
		break;
	case 2:
		DDRAMAddr = LCD_LINE2_DDRAMADDR+x;
		break;
	case 3:
		DDRAMAddr = LCD_LINE3_DDRAMADDR+x;
		break;
	default:
		DDRAMAddr = LCD_LINE0_DDRAMADDR+x;
		break;
	}
	// set data address
	LCD_send_command(1<<LCD_DDRAM | DDRAMAddr);

}
