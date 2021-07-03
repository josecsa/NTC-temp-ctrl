// Include the SPI library:
#include <SPI.h>
#include <math.h>

const int pino10=10;	//ccs 
const int pino13=13;	//led indicadora do ccs
const int pino9=9;		//ventilador
const int pino8=8;		//aquecedor
float media=0; 
float valorleitura=0;
float temp,resistencia,muxValue,sensorValue;
float valorconvertido,tensaoconvertida;
float temperatura=22;  //de inicio o programa inicia com a temperatura de 22º
float buffert=0,bufferr=0,bufferto=0;
float atuador=0;
int ccs=0;

void setup()
{
	// Initialize serial port for com to host
	Serial.begin (9600);
	
	// Initialize SPI
	SPI.begin();
	SPI.setBitOrder (MSBFIRST);
	pinMode(A0, INPUT);  // CCS1
	pinMode(pino10, INPUT);		//Selecionador
	pinMode(pino13, OUTPUT);
	pinMode(pino8, INPUT);		
	pinMode(pino9, OUTPUT);
	delay(1000);
}

float leitura(){
	valorleitura=0;	//resetar cada vez que a função é chamada
	media=0;	//resetar cada vez que a função é chamada
	
	for(int i=0; i<20; i++){
		sensorValue = analogRead(A0);//leitura do pino A0 0-1023
		if(sensorValue>1024){
			i--;		//se obtiver uma leitura errada irá voltar a fazer +1 leitura
		}
		else{	media=media+sensorValue;}
	}
	media=(media/20);	//media
	return media;
}


void loop(){
	if (Serial.available() > 0) {
		
		temperatura=Serial.parseInt();} //le a temperatua a igualar
	
	valorconvertido=leitura();
	muxValue = digitalRead(pino10);
	
	
	
	if(muxValue==0){	//ccs1
		ccs=1;
		digitalWrite (pino13,LOW);//led desligada (ccs1)
		
		tensaoconvertida=sensorValue * (5.0 / 1024.0);
		resistencia=((57.1417/(tensaoconvertida+3.64726))-1);
		temp=(log(resistencia/14.669)/-0.0439);
		atuadores();
		
		if(bufferr!=resistencia || buffert!=temp || bufferto!=temperatura){
			bufferr=resistencia;
			buffert=temp;
			bufferto=temperatura;
			
			printecra();
			
		}
		
	}
	
	
	if(muxValue==1){	//ccs2
		ccs=2;
		digitalWrite (pino13,HIGH);//ccs2
		
		
		tensaoconvertida=sensorValue * (5.0 / 1024.0);
		resistencia=((22.1602/(tensaoconvertida+3.32349))-1);
		temp=(log(resistencia/14.669)/-0.0439);
		atuadores();
		
		if(bufferr!=resistencia || buffert!=temp || bufferto!=temperatura){
			bufferr=resistencia;
			buffert=temp;
			bufferto=temperatura;
			
			printecra();
			
		}
		
	}
	
	
	delay (100);
	
}

void printecra(){
	
	Serial.print("Condicionamento de sinal: ");   
	Serial.print(ccs);
	Serial.print("\n\n");  
	
	Serial.print("Resistencia = ");   
	Serial.print(resistencia);
	Serial.print(" Ohms \n\n");
	
	
	Serial.print("Temperatura atual = ");   
	Serial.print(temp);
	Serial.println(" ºC \n");
	
	Serial.print("Temperatura desejada = ");   
	Serial.print(temperatura);
	Serial.println(" ºC \n");
	
	
	if(atuador==3){
		Serial.print("A Temperatura atual é superior á desejada\n");
		Serial.print("Ventoinha:ON\n");
		Serial.print("Aquecedor:OFF\n");
		
	}
	
	if(atuador==2){
		Serial.print("A Temperatura atual é a desejada\n");
		Serial.print("Ventoinha:OFF\n");
		Serial.print("Aquecedor:OFF\n");
		
		
	}
	
	if(atuador==1){
		Serial.print("A Temperatura atual é inferior á desejada\n");
		Serial.print("Ventoinha:OFF \n");
		Serial.print("Aquecedor:ON \n");
	}
	
	
}
void atuadores(){
	
	if(temp>temperatura){
		digitalWrite (pino8,LOW);	
		digitalWrite (pino9,HIGH);
		atuador=3;
	}
	if(temp<temperatura){
		digitalWrite (pino8,HIGH);	
		digitalWrite (pino9,LOW);
		
		atuador=1;
	}
	if(temp==temperatura){
		digitalWrite (pino8,LOW);	//desliga resistencia termica e ventilador
		digitalWrite (pino9,LOW);
		atuador=2;
	}
	
	
}