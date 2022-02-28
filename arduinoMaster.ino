   //PANTALLA V3.0
    //TT2 CARLOS ESQUIVEL AND BRYAN MONTIEL ORTEGA
    //ARDUINO MAESTRO
    
    //LIBRERIAS
    #include <Adafruit_GFX.h>    
    // Libreria Grafics 
    //http://adafruit.github.io/Adafruit-GFX-Library/html/class_adafruit___g_f_x.html
    #include <Adafruit_TFTLCD.h> // Libreria LCD
    #include <TouchScreen.h> //Libreria panel tactil
    //Libreria para modo Sleep
    #include <LowPower.h> //COMING SONG
    //Libreria para sensor de microondas y bobina
    
    //Libreria Maestro comunicación conección I2C
    #include <Wire.h>
    //Para el módulo SD
    #include <SPI.h> //Libreria bus SPI
    #include <SD.h> //Libreria de tarjeta SD
    
    
    //PINES DE CONECCION
    //PINES DE PANTALLA
    // Pines de conexión de la pantalla   LCD para los elementos gráficos
    #define LCD_CS A3 // Chip Select goes to Analog 3
    #define LCD_CD A2 // Command/Data goes to Analog 2
    #define LCD_WR A1 // LCD Write goes to Analog 1
    #define LCD_RD A0 // LCD Read goes to Analog 0
    #define LCD_RESET A4 
    // Can alternately just connect to Arduino's reset pin
    //Pines para el touch
    #define YP A3
    #define XM A2
    #define YM 9
    #define XP 8
    
    //CONFIGURACIONES GENERALES DE LA PANTALLA
    //Colores RGB565 usados
    //http://www.rinkydinkelectronics.com/calc_rgb565.php
    #define  BLACK   0x0000
    #define WHITE   0xFFFF
    #define LOW_BLUE 0x9FFF
    #define ORANGE 0xFE00
    #define BLUE_UNO 0x1514
    #define BLUE_DOS 0x032D
    #define GRAY 0x7BAE
    #define RED 0xF800
    #define LEDUNO 0x7C00
    #define LEDDOS 0xEFE0
    
    //Limites de la pantalla touch que se pueden presionar
    //De acuerdo a la calibración realizada
    #define TS_MAXX 894
    #define TS_MINY 105
    #define TS_MINX 70
    #define TS_MAXY 912
    //el ulti valor referente a resistencia entre A2 Y 6 para mejorar touch
    TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
    //Controlador de la pantalla
    Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
    //presión maxima y minima de la pantalla ha ejercer
    #define MINPRESSURE 200
    #define MAXPRESSURE 1000
    //Los pines del puerto SPI vienen configurados en la libreria
    //El pin correspondiente al chip Select del bus SPI corresponde a la conexión con la tarjeta
    #define SD_CS 10
    
    //Para guardar pantalla
    
    
    //PIN PARA SENSORMICROONDAS
    #define SEN_MICRO 0
    
    
    //VARIABLES
    //Para el sen de microondas
    int analogValor = 0;
    int voltaje = 0;
    
    //Variables de banderas flags para los menus
    byte flagUno = 0;
    byte flagDos = 0;
    byte flagTres = 0;
    byte flagCuatro = 0;
    byte flagCinco = 0;
    
    //Variable para trasmitir datos
    byte CODE;
    byte CODEDOS;
    
    //Variables para sleep
    int S = 0;
    int Sdos = 0;
    int restaUno = 0;
    int acomodo = 0; int segundos = 300;
    int n = 0; //conteo
    #define intervaloUno 300
    #define intervaloDos 240
    #define intervaloTres 180
    #define intervaloCuatro 120
    #define intervaloCinco 60
    #define intervaloSeis 0
    
    //variables de posicion
    
    
    
    //variables touch
    int TSPointZ;
    int TSPointX;
    int TSPointY;
    
    //para guardar
      String CADENA = "";
      int distanciaSlave = 0.0;
      String CADENADOS="";
    
    void setup(void) {
      Wire.begin(); //Comenzamos la conección I2C
      Serial.begin(9600); //Comunicación con la pantalla o SD
      tft.reset();
      tft.begin(0x9341); //Controlador de nuestro LC
      tft.fillScreen(BLACK); //Pantalla   negra desde inicio
      tft.setRotation(2); //Se establece la posición de la
    
      pantallaInicio();
    }
    void loop() {
      //Pantalla voidPantUno
      if (flagUno == 0) {
        S = millis() / 1000; //Empezamos a contar con timer del arduino
        n = S - Sdos; //Sdos empieza en cero en int variables
        restaUno = segundos - n; 
        //Esta linea resetea el contador del modo Sleep al regresar a pantalla principal
        //COnteo para sleep en pantalla
    
        //stringBlancoNegro(95, 275);
        /*tft.setTextColor(WHITE, BLUE_DOS);
        posicionTexto(95, 273);
        tft.setTextSize(2);*/
        tft.setTextColor(WHITE, BLUE_DOS);
        posicionTexto(95, 273);
    
        
        //Serial.print("Resta Uno: ");Serial.print(restaUno);Serial.print("S: ");Serial.println(S); //TES
    
    
        if ((restaUno > intervaloDos) && (restaUno < intervaloUno) ) {
          minutosPantallaInicio(5);
          //tft.print(F("5")); tft.print(F(" min."));
        }
        else if ((restaUno < intervaloDos ) && (restaUno > intervaloTres)) {
          minutosPantallaInicio(4);
          //tft.print(F("4")); tft.print(F(" min."));
        }
        else if ((restaUno < intervaloTres) && (restaUno > intervaloCuatro)) {
          minutosPantallaInicio(3);
          //tft.print(F("3")); tft.print(F(" min."));
        }
        else if ((restaUno < intervaloCuatro) && (restaUno > intervaloCinco)) {
          minutosPantallaInicio(2);
          //tft.print(F("2")); tft.print(F(" min."));
        }
        else if ((restaUno < intervaloCinco) && (restaUno > intervaloSeis)) {
          tft.print(restaUno); tft.print(F("seg "));
        }
    
    
        if (restaUno < intervaloUno) { //Mientras sea menor a 5 minutos....
          
          
          obtenerPuntosDisplay();
          //Condiciones para detectar el touch
          if (TSPointZ  > MINPRESSURE && TSPointZ  < MAXPRESSURE) {
            //condiciones para ver que se está solicitando del menu
            //Seleccionar botón de iniciar (tríangulo)
            if ((TSPointX > 65 && TSPointX < 195) && (TSPointY  > 125 && TSPointY  < 250 )){
              flagUno = 1;
              botonEmpInicioDos();
              botonEmpInicio();
              animacionParpadeo();
    
    
              pantallaDos();
            }
            if ((TSPointX > 85 && TSPointX < 183) && (TSPointY  > 25 && TSPointY  < 90)){
              flagUno = 2;
              botonSleepIniDos();
              //delay(500);
              botonSleepIni();
              //delay(500);
              goto gotoSleep;
    
            }
    
    
    
          }
        }
        if (restaUno < 0) { //Goto sleep
    gotoSleep:
          modoSleep();
          LowPower.idle(SLEEP_8S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF,
                        SPI_OFF, USART0_OFF, TWI_OFF);
          goto gotoSleep;
    
    
    
        }
      }
    
    
      if (flagUno == 1) {
        voidPantallaDos();
      }
    }
    
    void voidPantallaDos() {
      if (flagDos == 0) {
        S = millis() / 1000; //Empezamos a contar con timer del arduino
        n = S - Sdos; //Sdos empieza en cero en int variables
        restaUno = segundos - n; 
        //Esta linea resetea el contador del modo Sleep al regresar a pantalla principal
        //COnteo para sleep en pantalla
    
        
        obtenerPuntosDisplay();
        
        //Condiciones para detectar el touch
        if (TSPointZ  > MINPRESSURE && TSPointZ  < MAXPRESSURE) {
          //BOTON DETECTAR MATERIALES EN GENERAL
          if ((TSPointX > 25 && TSPointX < 235) && (TSPointY  > 125 && TSPointY  < 180 )) {
            flagDos = 1;
            botonSensorMicroDos();
            //delay(500);
            botonSensorMicro();
            animacionParpadeo();
            pantallaTres();
          }
          //BOTON DETECTAR METALES
          if ((TSPointX > 25 && TSPointX < 235) && (TSPointY  > 45 && TSPointY  < 100 )) {
            
            flagDos = 2;
            botonSensorBobinaDos();
            //delay(500);
            botonSensorBobina();
            animacionParpadeo();
            pantallaCuatro();
    
          }
          //BOTON REGRESAR
          if ((TSPointX > 125 && TSPointX < 145) && (TSPointY  > 300 && TSPointY  < 320 )) {
            Sdos = S;
            restaUno = 300;
            flagUno = 0;
    
          
    
    
            animacionBotonRegreso();
            pantallaInicio();
    
          }
        }
      }
      if (flagDos == 1) {
        voidPantallaTres();
      }
      if (flagDos == 2) {
        voidPantallaCuatro();
      }
    }
    
    //VOID PANTALLA TRES  - DETECTAR MATERIALES EN GENERAL
    void voidPantallaTres() {
      if (flagTres == 0) {
        //PARA EL LED
        voltaje = digitalRead(0);
        //Serial.println(voltaje);
        
        if (voltaje == HIGH) {
              //Serial.println("APAGADO");
              tft.fillCircle(125,185,60,LEDUNO);
               
          }
          else {
              //Serial.println("ENCENDIDO");
              tft.fillCircle(125,185,60,LEDDOS);
          }
          //delay(500);
        /*analogValor = analogRead(1); //SEN_MICRO
        voltaje = 0.0048*analogValor;
        Serial.println(voltaje);
        if (voltaje < 4.88){
          tft.fillCircle(125,185,60,LEDDOS);      
        }
        else{
          ledPantallaTres();
        }*/
        
        
        obtenerPuntosDisplay();
      
         //Condiciones para detectar el touch
        if (TSPointZ  > MINPRESSURE && TSPointZ  < MAXPRESSURE) {
          
          //Boton regreso
          if ((TSPointX > 125 && TSPointX < 145) && (TSPointY  > 300 && TSPointY  < 320 )) {
            flagDos = 0;
            animacionBotonRegreso();
            pantallaDos();
          }
        }
          
    
      }
    }
    
    //VOID PANTALLA CUATRO  - DETECTAR METALES
    void voidPantallaCuatro() {
      gotToChoose:
      if (flagCuatro == 0) {
        //inicializarBobina();
          datosBobina();
          obtenerPuntosDisplay();
    
        //Condiciones para detectar el touch
        if (TSPointZ  > MINPRESSURE && TSPointZ  < MAXPRESSURE) {
          //BOTON Captura de pantalla
          if ((TSPointX > 30 && TSPointX < 125) && (TSPointY  > 40 && TSPointY  < 80 )) {
            //EXISTE UNA MEMORIA?
            if (!SD.begin(SD_CS)) {
              flagCuatro = 1;
              //Wire.requestFrom(0x02,1);//Checar
              
              animacionParpadeo();
              pantallaGuardadoErroneo();
              textoPantallaGuardadoErroneo();
            }
            //ELSE
            if (SD.begin(SD_CS)) {
              flagCuatro = 1;
              //Wire.requestFrom(0x02,1);//Checar          
              animacionParpadeo();
              pantallaGuardadoErroneo();
              guardarArchivos();          
            }
          }
          if ((TSPointX > 140 && TSPointX < 235) && (TSPointY  > 40 && TSPointY  < 80 )) {
                    
            goto gotToChoose;
            //Serial.println("hola"); 95 GOTO CHOOSE
          }
          //BOTON AJUSTAR
          
          //Boton regresar a la pantalla Dos
          if ((TSPointX > 125 && TSPointX < 145) && (TSPointY  > 300 && TSPointY  < 320 )) {
            flagDos = 0;
            //Wire.requestFrom(0x02,1);//Checar
           
            animacionBotonRegreso();
            pantallaDos();
          }
        }

    
      }
      if (flagCuatro == 1) {
        voidPantallaErrorGuardadoCuatro();
      }
    
    }
    //  VOID PARA PANTALLAS DE GUARDADO
    void voidPantallaErrorGuardadoCuatro() {
      if (flagCinco == 0) {
    
        obtenerPuntosDisplay();
    
        if (TSPointZ  > MINPRESSURE && TSPointZ  < MAXPRESSURE) {
          if ((TSPointX > 125 && TSPointX < 145) && (TSPointY  > 300 && TSPointY  < 320 )) {
            flagCuatro = 0;
            animacionBotonRegreso();
            pantallaCuatro();
          }
    
        }
      }
    
    }
    
    
    //FUNCIONES
    //Funciones de proposito general
    
    void stringBlancoNegro(int posSBNX, int posSBNY){
      tft.setTextColor(WHITE, BLACK);
      posicionTexto(95, 273);
      tft.setTextSize(2);
    }
    
    void animacionParpadeo() {
      //delay(300);
      tft.fillScreen(BLACK);
      //delay(500);
    }
    void obtenerPuntosDisplay() {
      TSPoint p = ts.getPoint();
      pinMode(XM, OUTPUT);
      pinMode(YP, OUTPUT);
      p.x = map(p.x, TS_MAXX, TS_MINX, tft.width(), 0);
      p.y = map(p.y, TS_MINY, TS_MAXY, tft.height(), 0);
      TSPointX = p.x;
      TSPointY = p.y;
      TSPointZ = p.z;
    
    
    }
    
    void posicionTexto(int posEquisText, int posYeText) {
      tft.setCursor(posEquisText, posYeText);
    }
    
    void posicionTextoDos(int posEquisText, int posYeText){
      posicionTexto(posEquisText, posYeText); 
      textoDosBlanco();
    }
    
    void textoDosBlanco() {
      tft.setTextSize(2);
      tft.setTextColor(WHITE);
    }
    
    void guardarArchivos () {
      File myFile;
      int randomNumber = random(1,999);
      String FM;
      FM = String(randomNumber)+".txt";
      while (SD.exists(FM)){
        randomNumber = randomNumber + 1;
        FM = String(randomNumber)+".txt";
      }
      myFile = SD.open(FM, FILE_WRITE); 
      // O_TRUNC borra/vacía/limpia el archivo si es que ya existía
      if (myFile) {
        //Serial.println(FM);
        posicionTextoDos(27, 55);
        tft.print(F("GUARDADO EXITOSO"));
        posicionTexto(27, 150);
        tft.print(F("CON EL SIGUIENTE        NOMBRE"));
        posicionTexto(77, 190);
        tft.print(FM);
        //myFile.println(F("Probando...")); //CAMBIAR CADENADOS
        myFile.print("Material: ");myFile.print(CADENADOS);
        myFile.print(" -- Tipo de material: ");myFile.print(CADENA);
        myFile.print(" -- Distancia Aproximada en cm: ");myFile.print(distanciaSlave);
        myFile.close();
    
      }
      
      else {
        //Serial.println("NoWe");
    
      }
      
    
    
    }
    
    void botonRegreso() {
      tft.fillRoundRect(110, 10, 30, 30, 5, ORANGE);
      tft.fillRoundRect(112, 12, 26, 26, 5, GRAY);
      posicionTextoDos(117, 17);
      tft.print(F("<"));
    }
    
    void animacionBotonRegreso() {
      botonRegreso();
      animacionParpadeo();
      
    }
    void pantallaGuardadoErroneo() {
      botonRegreso();
      cuadroPantallaGuardadoErroneo();
      //textoPantallaGuardadoErroneo();
    }
    
    
    
    
    void textoPantallaGuardadoOk() {
      posicionTextoDos(27, 55);
      tft.print(F("EL ARCHIVO SE HA GUARDADO CORRECTAMENTE"));
      posicionTexto(27, 100);
      tft.print(F("  Con el siguiente nombre: "));
    
      posicionTexto(20, 175);
      tft.print(F(" Cierre esta ventana con el boton       para regresar al menu anterior"));
    
    }
    
    
    void cuadroPantallaGuardadoErroneo() {
    
      tft.fillRoundRect(20, 43, 200, 240, 5, ORANGE);
      tft.fillRoundRect(22, 45, 196, 236, 5, GRAY);
      tft.drawLine(20, 300, 220, 300, LOW_BLUE);
    
    }
    void textoPantallaGuardadoErroneo() {
      posicionTextoDos(27, 95);
      tft.print(F("ERROR AL GUARDAR"));
      posicionTexto(27, 180);
      tft.print(F("INTENTE DE NUEVO"));
    }                
    
    
    //Funciones de la pantalla uno
    
    void minutosPantallaInicio(int minutosFuncion){
      tft.print(minutosFuncion); tft.print(F(" min."));
    }
    
    void pantallaInicio () {
      textoInicio();
      botonEmpInicio();
      botonSleepIni();
    }
    void textoInicio() {
      //recuadro de la pila definidas por lineas
      //x,y,x,y
      posicionTextoDos(40,10);
      tft.print(F("Detector para"));
      posicionTexto(30, 30);
      tft.print(F("ubicar material"));
    
      tft.drawLine(20, 60, 220, 60, WHITE);
    
    }
    void botonEmpInicio() {
      tft.fillRect(65, 80, 123, 123, GRAY);
      //Circunferencia sin relleno
      tft.fillCircle(125, 140, 60, ORANGE);
      tft.fillCircle(125, 140, 40, BLACK);
      //Triangulo de enmedio
      tft.fillTriangle
      (160, 140,
       107, 110,
       107, 170, ORANGE);
    }
    void botonEmpInicioDos() {
      tft.fillRect(65, 80, 123, 123, GRAY);
    }
    void botonSleepIni() {
      //Boton Salir
      tft.fillRect(80, 240, 95, 55, BLUE_DOS); //btnUno
      //TEXTO SALIR
      posicionTextoDos(100, 240);
      tft.print(F("Sleep                en:"));
    }
    void botonSleepIniDos() {
      tft.fillRect(80, 240, 95, 55, BLUE_UNO); //btnUno
    
    }
    void modoSleep() {
      tft.fillScreen(BLACK);
      posicionTexto(30, 20);
      tft.setTextSize(3);
      tft.setTextColor(WHITE);
      tft.print(F("Modo Sleep"));
      posicionTexto(10, 140);
      tft.setTextSize(2);
      tft.print(F("Apague manualmente    el dispositivo"));
    
    }
    //Funciones pantalla Dos
    void pantallaDos() {
      
      lineasPantallaCuatro();
      botonSensorMicro();
      botonSensorBobina();
      botonRegreso();
      textoPantDos();
    }
    void textoPantDos() {
      posicionTextoDos(40, 48);
      tft.println(F("SELECCIONA UNA"));  
      tft.setCursor(80, 68);
      tft.println(F("OPCION")); //Escribimos texto en la LCD
      tft.setCursor(80, 155);
      //tft.println(F("DETECTAR         MATERIALES EN          GENERAL.")); 
      tft.println(F("DETECTAR         MATERIALES EN          GENERAL.")); 
      
    }
    
    void botonSensorMicro() {
      tft.fillRect(20, 150, 210, 55, ORANGE); //btnUno
      
      
    }
    void botonSensorMicroDos() {
      tft.fillRect(20, 150, 210, 55, GRAY); //btnUno
      posicionTextoDos(20, 155);
      tft.println(F("     DETECTAR         MATERIALES EN          GENERAL."));
    }
    
    
    void botonSensorBobina() {
      tft.fillRect(20, 230, 210, 55, ORANGE); //btnUno
      posicionTextoDos(30, 240);
      tft.println(F("    DETECTAR            METALES.")); 
    }
    void botonSensorBobinaDos() {
      tft.fillRect(20, 230, 210, 55, GRAY); //btnUno
      posicionTextoDos(30, 240);
      tft.println(F("    DETECTAR            METALES."));
    }
    //Funciones pantalla tres
    
    void pantallaTres(){
      botonRegreso(); 
      encabezadoPantTres();
      lineasPantallaCuatro();
      ledPantallaTres();
    }
    
    void encabezadoPantTres() {
      posicionTextoDos(30, 52); 
      tft.println(F("    DETECTAR          MATERIALES.")); 
    }
    
    void ledPantallaTres(){//0x7C00
      tft.fillCircle(125,185,70,GRAY);  
      //tft.fillCircle(125,185,60,LEDUNO);  
    }
    
    
    //Funciones pantalla Cuatro
    void pantallaCuatro() {
      botonRegreso();
      encabezadoPantCuatro();
      lineasPantallaCuatro();
      textosPantallaCuatro();
      botonCapturaPantCuatro();
      botonAjustar();
    }
    void encabezadoPantCuatro() {
      posicionTextoDos(30, 52);
      tft.println(F("    DETECTAR            METALES.")); 
      
    }
    void lineasPantallaCuatro() {
      tft.drawLine(20, 45, 220, 45, LOW_BLUE); 
      //Pasos siguientes
      tft.drawLine(20, 85, 220, 85, LOW_BLUE);
    }
    void textosPantallaCuatro() {
      posicionTextoDos(80, 100);
      tft.println(F("Material"));
      posicionTexto(0, 150);
      tft.println(F("Distancia Aproximada"));
      posicionTexto(105, 200);
      tft.println(F("Tipo"));
    }
    void botonCapturaPantCuatro() {
      tft.fillRoundRect(20, 250, 100, 40, 5, ORANGE);
      tft.fillRoundRect(22, 252, 96, 36, 5, GRAY);
      posicionTextoDos(30, 252);
      tft.print(F("Guardar"));
      posicionTexto(42, 270);
      tft.print(F("Datos"));
    
    }
    void botonCapturaPantCuatroDos() {
      tft.fillRoundRect(20, 250, 100, 40, 5, ORANGE);
      tft.fillRoundRect(22, 252, 96, 36, 5, ORANGE);
    }
    void botonAjustar() {
      tft.fillRoundRect(130, 250, 100, 40, 5, ORANGE);
      tft.fillRoundRect(132, 252, 96, 36, 5, GRAY);
      posicionTextoDos(142, 260);
      tft.print(F("Ajustar"));
    }
    
    
    
    //PARA I2C    
    void datosBobina(){
    
      //float distanciaSlave = 0.0;
      
      Wire.requestFrom(0x01,1);//Checar
      while(Wire.available()){
        CODE=Wire.read();
        //CODEDOS=Wire.read();  
      }
      Wire.requestFrom(0x01,2);//Checar
      while(Wire.available()){
        //CODE=Wire.read();
        CODEDOS=Wire.read();  
      }
      tft.setCursor(11, 220);
      tft.setTextSize(2);
      tft.setTextColor(WHITE, BLACK); 
      
      if(CODE==10){
        CADENA = "NO FE";
        CADENADOS="COBRE";
        tft.print(F("       "));tft.print(CADENA);
        tft.setCursor(30, 120); 
        tft.print(F("     COBRE "));
        tft.setCursor(95, 165);
        
        if(CODEDOS == 10){
          distanciaSlave = 3.0;
          tft.print(F("3.0cm"));
        }
        if(CODEDOS == 11){
          distanciaSlave = 2.5;
          tft.print(F("2.5cm"));
        }
        if(CODEDOS == 12){
          distanciaSlave = 2.0;
         tft.print(F("2.0cm"));
        }
        if(CODEDOS == 13){
          distanciaSlave = 1.0;
          tft.print(F("1.0cm"));
        }
        
        
    
      }
      else if (CODE==11){
        CADENA = "FE   ";
        CADENADOS="VARILLA";
        tft.print(F("         "));tft.print(CADENA);
        tft.setCursor(30, 120); 
        tft.print(F("    VARILLA"));
        tft.setCursor(95, 165);
        tft.setTextColor(WHITE, BLACK); 
        if(CODEDOS == 10){
          distanciaSlave = 2.0;
          tft.print(F("2.0cm"));
        }
        if(CODEDOS == 11){
          distanciaSlave = 1.5;
          tft.print(F("1.5cm"));
        }
        if(CODEDOS == 13){
          distanciaSlave = 1.0;
          tft.print(F("1.0cm"));
        }
    
      }
      
    
    }
    
