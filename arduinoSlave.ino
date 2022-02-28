//PANTALLA V3.0
//TT2 CARLOS ESQUIVEL AND BRYAN MONTIEL ORTEGA
//Arduino SLAVE
//

// Entrada D5, Buzzer D12, boton D4

#include <Wire.h>
#include <FreqCount.h>
#define PIN_BUZZER 12


bool fe = false; //Material
bool AB=true; //Auto valance
int valor=0;
int buff=0;
int frq=0;
int resta=0; //int
int sens=2; // sensibilidad
int autoBalance=0;

byte entra = 0;
int t; //Tiempo de respuesta
byte CODE;//OBJETO
byte CODEDOS;//Distancia
int A = 0;
int isOn = 0;
int dist = 0;


void setup() {
  pinMode (13,OUTPUT);
  pinMode (PIN_BUZZER,OUTPUT);
  Serial.begin(9600);
  
  FreqCount.begin(200); //nº ciclos de lectura de frecuencia
  delay(100);
  
  frq=FreqCount.read();
   Wire.begin(0x01);//Me identifico como Slave 1  
  Wire.onReceive(EntradaSolicitud); //Declaro evento
  Wire.onRequest(Peticion); //Declaro de evento cuando el maestro le pide la info
 
  // Balance
  buff=frq;
  
  for(int i=0;i<5;i++){
    for(int i=0;i<10;i++){// genera tono
      digitalWrite(PIN_BUZZER,HIGH);
      delay(2);
      digitalWrite(PIN_BUZZER,LOW);
      delay(2);
    }
    delay(20);
    frq= FreqCount.read();

    if(frq != buff){
      i=0; buff=frq;
    }
  }
  for(int i=0;i<20;i++){ // Genera tono
    digitalWrite(PIN_BUZZER,HIGH);
    delay(1);
    digitalWrite(PIN_BUZZER,LOW);
    delay(1);  
  }
}

void EntradaSolicitud(int re){
  while (Wire.available()){
    entra = Wire.read();
  }
  Serial.println(entra);
}

void Peticion(){
  Wire.write(CODE);
  Wire.write(CODEDOS);
  isOn=1;
  //Serial.println(CODE);
}



void loop() {

  //Serial.println(entra);
  if (isOn == 0){
    //Serial.println(CODE);
    //Serial.println("JAJ");
  }
  else if (isOn == 1){
    AB=true;
    frq = FreqCount.read();
    resta=buff-frq;
    valor= -resta;
    fe=true;
    if(resta<0){
      resta=-resta;
      fe=false;
    }
    if(resta>sens && fe){               // Metales ferrosos
      fe = true;
      for(int i=0;i<10;i++){ // Genera tono
      digitalWrite(PIN_BUZZER,HIGH);
      delay(2);
      digitalWrite(PIN_BUZZER,LOW);
      delay(2);
    }
    delay(40-(constrain(resta*5,10,40)));
    AB=false; //para resetear el autobalance
    CODE = 11; //CODIGO PARA EL MAESTRO
    }
    if(resta>sens && !fe){        // Metales no-ferrosos
      fe=false;
      //resta=-resta;
      for(int i=0;i<20;i++){// genera tono
        digitalWrite(PIN_BUZZER,HIGH);
        delay(1);
        digitalWrite(PIN_BUZZER,LOW);
        delay(1);
      }
      delay(40-(constrain(resta*5,10,40))); 
      AB=false; //para resetear el autobalance
      CODE = 10; //CODIGO PARA EL MAESTRO 
      CODEDOS = 10;
    }

    resta = constrain(resta,0,16);
    for(int i=0;i<resta;i++){
      //lcd.print(O);
    }
    if (resta > 1 ){
      if(fe && resta>sens){
        Serial.print("Material Fe ");
        Serial.println(resta);
        dist = 1;
        //if (resta > 2 %% resta < 6){
        //  CODEDOS = 10; 
        //}
        //if ((resta > 5) %% (resta < 11)){
        //  CODEDOS = 11;
        //}
        //if (resta > 10){
        //  CODEDOS == 1;
       // }
      }
      if(!fe && resta>sens){
        Serial.print("Material NO Fe ");
        Serial.println(resta);
        dist = 2;
      }
    }
    //Distancia varilla
    if (dist = 1){
      if ((resta > 2 && resta < 6)){
        CODEDOS = 10;
      }
      if ((resta > 5 && resta < 11)){
        CODEDOS = 11;
      }
      if (resta > 10){
        CODEDOS = 13;
      }
    }
    //Distancia cobre
    if (dist = 2){
      if ((resta > 2 && resta < 6)){
        CODEDOS = 10;
      }
      if ((resta > 5 && resta < 9)){
        CODEDOS = 11;
      }
      if ((resta > 8 && resta < 11)){
        CODEDOS = 12;
      }
      if (resta > 10){
        CODEDOS = 13;
      }
      
    }

    if(!(valor>sens||valor<-sens))delay(20);
    if(!digitalRead(4)){
      while(!digitalRead(4))delay(50);
      sens++;
      if(sens>4)sens=1;
    }
    if(true){                     // auto Balance
      digitalWrite(13,HIGH);
      if(AB && resta !=0){   
        if(autoBalance>50){autoBalance=0; buff=frq;}
        autoBalance++;
      }
      else autoBalance=0;
      delay(1);
    }
    //AQUI RESET PARA I2C
    if (resta < 2){
      isOn=0;
      CODE = 0;
      CODEDOS = 0;
    }
    //Serial.println(resta);
    //isOn=0;
    
  }
}
