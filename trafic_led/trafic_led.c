#include<reg51.h>


sbit led_red = P1 ^ 0;
sbit led_yellow = P1 ^ 1;
sbit led_green = P1 ^ 2;

sbit led_red1 = P1 ^ 3;
sbit led_yellow1 = P1 ^ 4;
sbit led_green1 = P1 ^ 5;

unsigned int i = 0;
unsigned int j = 0;

unsigned char code LedChar[] ={0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90 };  //0 1 2 3 4 5 6 7 8 9

static unsigned int sec = 5;
static unsigned char togle_flag = 0;
static unsigned char rg_flag = 0;
static unsigned char skip_yellow = 0;
static unsigned char fs = 0;

void timer0(){
	TMOD = 0x01;
	TH0 = (65536-50000)/256;	 
	TL0 = (65536-50000)%256;
	EA = 1;
	ET0 = 1;
	TR0 = 1;
}

void tcount() interrupt 1 {
	TH0 = (65536-50000)/256;
	TL0 = (65536-50000)%256;
	i++;
}

void display(){
	//P0 = 0x01;
	P0 = 0x05;
	P2 = LedChar[sec % 10];  // 
	P3 = LedChar[(sec + 5) % 10];

	
	// enhance the refresh rate
	P0 = 0x00;
	P2 = 0xFF;
	P3 = 0xFF;
	
	//P0 = 0x02;
	P0 = 0x10;
	P2 = LedChar[sec / 10];  // 
	P3 = LedChar[(sec + 5) / 10];
	
	// enhance the refresh rate
	P0 = 0x00;
	P2 = 0xFF;
	P3 = 0xFF;
}

void first() {
	while(!togle_flag){
			display();
			if(i == 20) { //
				
				if (!rg_flag) {
					led_green = 0;
					led_green1 = !led_green;
				} else {
					led_red = 0;
					led_red1 = !led_red;
				}
				
				i = 0;
				
				if (sec > 0) sec--;
				else {				
					if (rg_flag) {
						togle_flag = 0;
						sec = 5;
						skip_yellow = 1;
					} else if (!rg_flag){
						togle_flag = 1;
						sec = 3;
						skip_yellow = 0;
					}
					rg_flag = !rg_flag;
					led_green = 1;
					led_red = 1;
				}
			}
		}
		
		while (togle_flag) {
			if (skip_yellow) {
				togle_flag = !togle_flag;
				sec = 5;
			} else {
				display();
				if(i == 20) { //
					led_yellow = 0;
					led_yellow1 = 0;
					i = 0;
				// 
					if (sec > 0) sec--;
					else {
						sec = 5;
						togle_flag = !togle_flag;
						led_yellow = 1;
						led_yellow1 = 1;
					}
				}
			}
		}
}

void main(){
	timer0();
	while (1) {
		first();
	}
}
