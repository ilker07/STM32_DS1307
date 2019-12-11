#include "stm32f10x.h"                  // Device header
#include "lcd.h"
#include "delay.h"
#include "stdio.h"




char ds1307Tarih[5];

uint8_t degerAl(uint8_t gelen_veri);
void i2c_init(void);
uint8_t I2C1_Oku (uint8_t Adr);
void I2C1_Yaz (uint8_t Adr, uint8_t Dat);

int main()
{
	delay_init();
	portAyarlari();
  lcd_basla();
	i2c_init();
	I2C1_Yaz (0x00,0x00); //CH Bit 0 yapmak için.
	I2C1_Yaz (0x01,0x36); //Dakika ayarlandi.
	I2C1_Yaz (0x02,0x23); //Saat ayarlandi.
	I2C1_Yaz (0x03,0x02); //Gün ayarlandi.
	I2C1_Yaz (0x04,0x10); //Ayin kaci oldugu ayarlandi.
	I2C1_Yaz (0x05,0x12); //Ay ayarlandi.
	I2C1_Yaz (0x06,0x19); //Yil ayarlandi.
	
	lcdHucreyeGit(1,3);
	lcd_veri_yaz(':');
	lcdHucreyeGit(1,6);
	lcd_veri_yaz(':');
	lcdHucreyeGit(2,3);
	lcd_veri_yaz('/');
	lcdHucreyeGit(2,6);
	lcd_veri_yaz('/');
	while(1)
	{
	
		////Saat
	lcdHucreyeGit(1,1);
  sprintf(ds1307Tarih, "%d", degerAl(I2C1_Oku(0x02)));
  lcd_yaz(ds1307Tarih);	
	
	///Saat 
	
	
	
	///Dakika
	
	lcdHucreyeGit(1,4);
	sprintf(ds1307Tarih, "%d", degerAl(I2C1_Oku(0x01))); 
  lcd_yaz(ds1307Tarih); 	

	
	///Dakika
	

	 
	///Saniye
	 lcdHucreyeGit(1,7);
  
	if(degerAl(I2C1_Oku(0x00))<10)
	
		sprintf(ds1307Tarih, "0%d", degerAl(I2C1_Oku(0x00)));
 else
 
  sprintf(ds1307Tarih, "%d", degerAl(I2C1_Oku(0x00)));
 
 lcd_yaz(ds1307Tarih); 	
 
	
	///Saniye
	
	
	
	
	
	///Gün
 lcdHucreyeGit(1,10);	
 
	switch(I2C1_Oku(0x03))
	{
		case 0:lcd_yaz("Pazar");break;
		case 1:lcd_yaz("Ptesi");break;
		case 2:lcd_yaz("Sali ");break;
		case 3:lcd_yaz("Crs  ");break;
		case 4:lcd_yaz("Prs  ");break;
		case 5:lcd_yaz("Cuma ");break;
		case 6:lcd_yaz("Ctesi");break;
		default:lcd_yaz("Hata ");break;
	
	}
  ////Gün
	
	

 ///AyinKaci
 
  lcdHucreyeGit(2,1);
	sprintf(ds1307Tarih, "%d", degerAl(I2C1_Oku(0x04))); 
  lcd_yaz(ds1307Tarih); 
 
 
 ///AyinKaci
	
	
	///Ay
	lcdHucreyeGit(2,4);
	sprintf(ds1307Tarih, "%d", degerAl(I2C1_Oku(0x05))); 
  lcd_yaz(ds1307Tarih); 
 
	///Ay
	
	
	
	
	///Yil
	lcdHucreyeGit(2,7);
	sprintf(ds1307Tarih, "20%d", degerAl(I2C1_Oku(0x06))); 
  lcd_yaz(ds1307Tarih); 
 
	///Yil
	
	
	
	}
	
}




uint8_t degerAl(uint8_t gelen_veri)
{
	uint8_t toplam;
	
 toplam=(gelen_veri & (0b00001111));
 toplam=toplam+(((gelen_veri & (1<<4))>>4)+2*(((gelen_veri & (1<<5))>>5))+4*(((gelen_veri & (1<<6))>>6))+8*(((gelen_veri & (1<<7))>>7)))*10;
	
	return toplam;

}
void I2C1_Yaz (uint8_t adres, uint8_t veri) {
	
 I2C1->CR1 |= 0x0100; // START biti
 while (!(I2C1->SR1 & 0x0001)) {}; // SB=1 olmasini bekle.
 I2C1->DR = 0xD0; // Slave adresi.
 while (!(I2C1->SR1 & 0x0002)) {}; // ADDR=1 olmasini bekle.
 int Status2 = I2C1->SR2; // Bayrak temizle.
 I2C1->DR = adres; // Register adresi.
 while (!(I2C1->SR1 & 0x0080)) {}; // TXE=1 olmasini bekle.
 I2C1->DR = veri; //Veriyi gonder.
 while (!(I2C1->SR1 & 0x0080)) {}; // TXE=1 olmasini bekle.
 while (!(I2C1->SR1 & 0x0004)) {}; // BTF=1 olmasini bekle.
 I2C1->CR1 |= 0x0200; // STOP biti.
 }

void i2c_init()
{
	
	RCC->APB1ENR   |= 1<<21; //I2C aktif.
	RCC->APB2ENR   |=1<<0;//AFIO portu aktif.
	
	
	//GPIOB->CRL |= 0xFF000000; //SCL ve SDA  AF Open Drain  SCL => PB6  SDA =>PB7
	
	
	I2C1->CR2    =36;
	
	
	//t=1/36mhz=27.7ns  t=1/100khz/2 =5us
	//CCR=5US/27.7NS=180
	
	
	I2C1->CCR    =180;
	
	//1000ns/1/36mhz=36  36+1=37
	
	
	I2C1->TRISE  =37;
	//I2C1->CR1 |=1<<10;//ACK
	
	
	I2C1->CR1     |=(1<<0);//Peripheral aktif. 	


}

/*
void i2c_oku(uint8_t adres,uint8_t boyut)
{
	uint32_t temp=0;

	RCC->AHBENR |=1<<0;//DMA hatti aktif.
	I2C1->CR2   |=1<<11;//DMA Aktif.
	I2C1->CR1 |=1<<10;//ACK.
	
	DMA1_Channel1->CPAR =(uint32_t) &I2C1->DR; //Buradan okunacak.
 //	DMA1_Channel1->CMAR =(uint32_t)buffer;//Buraya yollayacak.
	DMA1_Channel1->CNDTR =boyut;//Transfer edilecek data sayisi.
	DMA1_Channel1->CCR   |=0x00000083;//TCIE,MINC,EN.
	
	I2C1->CR1 |=1<<8;//START .
	while(!(I2C1->SR1 & 1));//START Bayraginin 1 olmasini bekle.
	I2C1->DR = adres;// Slave adres.
	while(!(I2C1->SR1 & 2));// ADDR Bayraginin 1 olmasini bekle.
	temp = I2C1->SR2;	//ADDR Bayragini Temizle.
	
	while(!(DMA1->ISR & (1<<1)));
	
	I2C1->CR1 |=1<<9;//STOP.
	
}*/

uint8_t I2C1_Oku (uint8_t adres) { 

	
 I2C1->CR1 |= 0x0100; // START biti.
 while (!(I2C1->SR1 & 0x0001)) {}; // SB=1 olmasini bekle.
 I2C1->DR = 0xD0; // Slave Adresi.(Yazma)
 while (!(I2C1->SR1 & 0x0002)) {}; // ADDR=1 olmasini bekle.
 int temp = I2C1->SR2; // Bayrak temizle.
 I2C1->DR = adres; // Register adresi.
 while (!(I2C1->SR1 & 0x0080)) {}; //TXE=1 olmasini bekle.
 while (!(I2C1->SR1 & 0x0004)) {}; // BTF=1 olmasini bekle.

 I2C1->CR1 |= 0x0100; // START biti.
 while (!(I2C1->SR1 & 0x0001)) {}; // SB=1 olmasini bekle.
 I2C1->DR = 0xD1; // Slave Adresi.(Okuma)
 while (!(I2C1->SR1 & 0x0002)) {}; // ADDR=1 olmasini bekle.
 temp= I2C1->SR2; // Bayrak temizle.
 	 
 while (!(I2C1->SR1 & 0x0040)) {}; // RxNE=1 olmasini bekle. 
 I2C1->CR1 |= 0x0200; // STOP biti.

 return I2C1->DR; // gelen veriyi al. 
	 
}





