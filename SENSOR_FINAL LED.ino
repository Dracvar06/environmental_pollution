

//PM1.0 PM2.5 PM10



#include  <LiquidCrystal.h>
#include <SPI.h>


// You can use any (4 or) 5 pins 
#define sclk 2
#define mosi 3
#define dc   4
#define cs  5
#define rst  6



// Option 1: use any pins but a little slower
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);






#define HEADER_H  0x42
#define HEADER_L  0x4D
#define FRAME_LENGTH  0x14


unsigned char cur_rx_data;
unsigned char pre_rx_data;
unsigned char main_status;
unsigned char recv_buff[256];
unsigned char recv_buff_index;
unsigned char incomingByte;

unsigned short check_sum;
int i;

int pm1_0_cf_1;
int pm2_5_cf_1;
int pm10_cf_1;

int pm1_0;
int pm2_5;
int pm10;





void setup() {
  Serial.begin(9600);
  
  check_sum = 0;
  for(i=0;i< (sizeof(recv_buff)); i++)
  {
    recv_buff[i] = 0x00;
  }
  recv_buff_index = 0x00;
  cur_rx_data = 0;
  pre_rx_data = 0;
  main_status = 0;
// INDICAMOS QUE TENEMOS CONECTADA UNA PANTALLA DE 16X2
  lcd.begin(16, 2);
  // MOVER EL CURSOR A LA PRIMERA POSICION DE LA PANTALLA (0, 0)
  lcd.setCursor(0, 0);
  // IMPRIMIR "Hola Mundo" EN LA PRIMERA LINEA
  lcd.print("Benvinguts");
  // MOVER EL CURSOR A LA SEGUNDA LINEA (1) PRIMERA COLUMNA (0)
  lcd.setCursor ( 0, 1 );
  // IMPRIMIR OTRA CADENA EN ESTA POSICION
  lcd.print("CIBEREDU");
  // ESPERAR UN SEGUNDO
  delay(2000);
 }

//int n = 0;
char temp_buff[64];
void loop() {
  lcd.clear();
  if (Serial.available() > 0) 
  {
    // read the incoming byte:
     cur_rx_data = Serial.read();
     //Serial.print("recv: "); 
     //Serial.println(  recv_buff[recv_buff_index-1], HEX);

     switch(main_status)
     {
        case 0:
         if( cur_rx_data == HEADER_L )
         {
            if( pre_rx_data ==  HEADER_H )
            {
                 main_status++;
                 
                 check_sum += pre_rx_data;
                 check_sum += cur_rx_data;
                 
                 cur_rx_data = 0;
                 pre_rx_data = 0;

                 
            }
         }else
         {
            pre_rx_data = cur_rx_data;
         }
        break;
        case 1:
        if( cur_rx_data == FRAME_LENGTH )
         {
            if( pre_rx_data ==  0x00 )
            {
                 main_status++;
                 
                 check_sum += pre_rx_data;
                 check_sum += cur_rx_data;
                 
                 cur_rx_data = 0;
                 pre_rx_data = 0;
            }
         }else
         {
            pre_rx_data = cur_rx_data;
         }
        break;
        case 2:
        recv_buff[recv_buff_index++] = cur_rx_data;
        check_sum += cur_rx_data;
        if( recv_buff_index == 18)
        {
             main_status++;
          
             cur_rx_data = 0;
             pre_rx_data = 0;
        }
        break;
        case 3:
        recv_buff[recv_buff_index++] = cur_rx_data;
        if( recv_buff_index == 20)
        { 
            if( check_sum == ( (recv_buff[18]<<8) + recv_buff[19]) )
            {
                recv_buff_index = 0;  
                pm1_0_cf_1 = (recv_buff[recv_buff_index] << 8) + recv_buff[recv_buff_index+1];
                recv_buff_index += 2;
                pm2_5_cf_1 = (recv_buff[recv_buff_index] << 8) + recv_buff[recv_buff_index+1];
                recv_buff_index += 2;
                pm10_cf_1 = (recv_buff[recv_buff_index] << 8) + recv_buff[recv_buff_index+1];
                recv_buff_index += 2;
                
                pm1_0 = (recv_buff[recv_buff_index] << 8) + recv_buff[recv_buff_index+1];
                recv_buff_index += 2;
                pm2_5 = (recv_buff[recv_buff_index] << 8) + recv_buff[recv_buff_index+1];
                recv_buff_index += 2;
                pm10 = (recv_buff[recv_buff_index] << 8) + recv_buff[recv_buff_index+1];
Serial.print(pm1_0_cf_1,HEX);Serial.print("\r\n");
Serial.print(pm2_5_cf_1,HEX);Serial.print("\r\n");
Serial.print(pm10_cf_1,HEX);Serial.print("\r\n");
Serial.print(pm1_0,HEX);Serial.print("\r\n");
Serial.print(pm2_5,HEX);Serial.print("\r\n");
Serial.print(pm10,HEX);Serial.print("\r\n");


         lcd.setCursor(0, 0);
         lcd.print("pm1=");
         lcd.setCursor(4, 0);
         lcd.print(pm1_0);
         lcd.setCursor(8, 0);
         lcd.print("2.5=");
         lcd.setCursor(12, 0);
         lcd.print(pm2_5);
         lcd.setCursor(0, 1);
         lcd.print("PM 10 =");
         lcd.setCursor(8, 1);
         lcd.print(pm10);
         delay(1000);
            }
    
   
    
            main_status = 0;    
            cur_rx_data = 0;
            pre_rx_data = 0; 
            check_sum = 0;
            for(i=0;i< (sizeof(recv_buff)); i++)
            {
              recv_buff[i] = 0x00;
            }
            recv_buff_index = 0x00;  
        }
        
        break;
        case 4:
        break;
        case 5:
        break;
        case 6:
        break;
        case 7:
        break;
        case 8:
        break;
     }
 
  }
  
  
}

