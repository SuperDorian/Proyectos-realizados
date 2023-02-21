#include <SPI.h>  
#include <SD.h>  
#include <LiquidCrystal.h>
#include <SoftwareSerial.h> 

#define SSpin 4 // pin cs del módulo de lector de tarjeta SD
const int rs = 0, en = 1, d4 = 2, d5 =3, d6 = 5, d7 = 6; // Declaración de pines para la pantalla lcd
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); 

SoftwareSerial SIM808(7, 8); // Pines del serial para el módulo SIM808
char mensaje = 0; //Variable para guardar los caracteres

File archivo; // variable para el contenido de la SD

const int Bocina = 9; //Pin de la sirena 


void setup() { 
  inicializaSIM808(); // función para inicializar el módulo 
lcd.begin(16,2);// iniciar lcd 

mensaje = 0;
pinMode(Bocina,OUTPUT); // inicializar el pin de la sirena como salida 
delay (1000);
digitalWrite(Bocina, HIGH); //Estado inicial de la sirena 




 
 




}

void loop() {
 
if (SIM808.available() > 0)
         {
            mensaje = SIM808.read(); //Guardamos el caracter que llega desde el SIM808
            Serial.print(mensaje); //Mostramos el caracter en el monitor serie
            
} 

  
   leer_mensaje(); // función destinada a leer el mensaje entrante
   
   }
         

void inicializaSIM808() { 
     SIM808.begin(19200); //Configura velocidad del puerto serie para el SIM808
    //Serial.begin(19200); //Configura velocidad del puerto serie del Arduino
     SIM808.print("AT+CLIP=1\r"); // Activa la identificación de llamada
     delay(100);
     SIM808.print("AT+CMGF=1\r"); // Configura el modo texto para enviar o recibir SMS
     delay(1000);
     SIM808.print("AT+CNMI=2,2,0,0,0\r");   // Saca el contenido del SMS por el puerto serie 
      delay(1000);
}

void leer_mensaje(){ 
  
 
   switch(mensaje){ 
    case '@':                               // caracter que debe recibir para dar pie al caso
    lcd.setCursor(0,0);                     // impresión del mensaje en la pantalla lcd
    lcd.print("Alarma Medica");     
    mensaje = 0;  
    
    alarma_medica();                       // función que dará las indicaciones de la alarma médica 
    break; 
    case '*':                              // caracter que debe recibir para dar pie al caso
    lcd.setCursor(0,0);                    // impresión del mensaje en la pantalla lcd
    lcd.print("Alarma Intruso");           
    mensaje = 0;  
   
    alarma_intruso();                       // función que dará las indicaciones de la alarma intruso
    break; 
    case '#':                              // caracter que debe recibir para dar pie al caso
    lcd.setCursor(0,0);                    // impresión del mensaje en la pantalla lcd
    lcd.print("Alarma Apagada");          
    mensaje = 0;  
    alarma_apagada();                     // función que dará las indicaciones del apagado de la alarma 
   }
   
   
 } 

 void alarma_medica(){ 
  Serial.println("Alarma medica");           // Imprime por el puerto serial el mensaje  
  digitalWrite(Bocina, LOW);                 // Enciende la sirena
  mensaje_medica();                          // función que enviará el mensaje 
 }


void alarma_intruso(){  
Serial.println("Alarma intruso");            // Imprime por el puerto serial el mensaje
digitalWrite(Bocina, LOW);                   // Enciende la sirena
  mensaje_intruso();                         // función que enviará el mensaje
}

void alarma_apagada(){                       
  Serial.println("Alarma apagada");          // Imprime por el puerto serial el mensaje
  digitalWrite(Bocina, HIGH);                // Apaga la sirena
  mensaje_apagada();                         // función que enviará el mensaje
} 

void mensaje_medica(){ 
  Serial.println("Enviando SMS...");          // Imprime por el puerto serial 
  delay(2000);
  SIM808.print("AT+CMGF=1\r");                //Configura el modo texto nuevamente para mandar el mensaje 
  delay(1000); 
  SIM808.println("AT+CMGS=\"2225545134\"");   //Número al que se le enviará el mensaje 
  delay(1000); 
  SIM808.println("Emergencia Medica");        // Mensaje que se enviará
  delay(100); 
  SIM808.println((char)26);                  
  delay(100); 
  SIM808.println();
  delay(5000); 
  Serial.println("SMS enviado");
} 

void mensaje_intruso(){ 
  Serial.println("Enviando SMS...");            // Imprime por el puerto serial 
  delay(2000);
  SIM808.print("AT+CMGF=1\r");                  //Configura el modo texto nuevamente para mandar el mensaje 
  delay(1000); 
  SIM808.println("AT+CMGS=\"2225545134\"");     //Número al que se le enviará el mensaje
  delay(1000); 
  SIM808.println("Emergencia Intruso");         // Mensaje que se enviará
  delay(100); 
  SIM808.println((char)26); 
  delay(100); 
  SIM808.println();
  delay(5000); 
  Serial.println("SMS enviado");
} 

void mensaje_apagada(){ 
  Serial.println("Enviando SMS...");             // Imprime por el puerto serial
  delay(2000);
  SIM808.print("AT+CMGF=1\r");                   //Configura el modo texto nuevamente para mandar el mensaje 
  delay(1000); 
  SIM808.println("AT+CMGS=\"2225545134\"");      //Número al que se le enviará el mensaje
  delay(1000); 
  SIM808.println("Alarma Apagada");              // Mensaje que se enviará
  delay(100); 
  SIM808.println((char)26); 
  delay(100); 
  SIM808.println();
  delay(5000); 
  Serial.println("SMS enviado");
}
