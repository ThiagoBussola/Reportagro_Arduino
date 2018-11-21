#include <SoftwareSerial.h>

#include <DHT.h>
#include <LiquidCrystal.h>
//RX pino 2, TX pino 3
SoftwareSerial esp8266(2, 3);
 
#define DEBUG true
int valor_analogico_solo;
int valor_analogico_luz;
int ldr  = A3;
#define sinal_analogico A0
#define sinal_analogico_2 A1
//Define a ligação ao pino de dados do sensor
#define DHTPIN A2
//Define o tipo de sensor DHT utilizado
#define DHTTYPE DHT11
#define led_luz_a 1
#define led_luz_b 2
#define led_luz_c 3
#define led_vermelho_solo 4
#define led_amarelo_solo 5
#define led_verde_solo 6
//Define os pinos que serão ligados ao LCD
LiquidCrystal lcd(7,8,9,10,11,12);
 
//Array simbolo grau
byte grau[8] ={ B00001100,
                B00010010,
                B00010010,
                B00001100,
                B00000000,
                B00000000,
                B00000000,
                B00000000,};

DHT dht(DHTPIN, DHTTYPE);

// --- Protótipo das funções auxiliáres ---
String sendData(String command, const int timeout, boolean debug){
  // Envio dos comandos AT para o modulo
  String response = "";
  esp8266.print(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (esp8266.available())
    {
      // The esp has data so display its output to the serial window
      char c = esp8266.read(); // read the next character.
      response += c;
    }
  }
  if (debug)
  {
    Serial.print(response);
  }
  return response;
}
 
void setup() {

  Serial.begin(9600);
  esp8266.begin(19200);

// ======================== Wi-Fi ===========================//
  sendData("AT+RST\r\n", 2000, DEBUG); // rst
  // Conecta a rede Wi-Fi
  sendData("AT+CWJAP=\"transnatalino\",\"m@thihe09\"\r\n",2000, DEBUG);
  delay(3000);
  // Mostra o endereço IP
  sendData("AT+CWMODE=1\r\n", 1000, DEBUG);
  // Configura para multiplas conexões
  sendData("AT+CIPMUX=1\r\n", 1000, DEBUG);
  // Inicia o web server na porta 80
  sendData("AT+CIPSERVER=1,80\r\n", 1000, DEBUG);

// ============================================================ //

  lcd.begin(16,2); //Inicializa LCD
  lcd.clear(); //Limpa o LCD
  lcd.createChar(0, grau); //Cria o caractere customizado com o simbolo do grau

  pinMode(sinal_analogico, INPUT);
  pinMode(sinal_analogico_2, INPUT);

  // pinMode de Solo
  pinMode(led_vermelho_solo, OUTPUT);
  pinMode(led_amarelo_solo, OUTPUT);
  pinMode(led_verde_solo, OUTPUT);

  // pinMode Luminosidade
  pinMode(led_luz_a, OUTPUT);
  pinMode(led_luz_b, OUTPUT);
  pinMode(led_luz_c, OUTPUT);

  Serial.println("Iniciando....");
  dht.begin();
}
 
void loop() {

// ========================================== Wi-Fi ========================================== //


// ===================================================== Umidade da Terra ======================================================== //
  valor_analogico_solo = analogRead(sinal_analogico);
 
  Serial.print("Porta analogica: ");
  Serial.print(valor_analogico_solo);

  // LED VERDE
   if (valor_analogico_solo > 0 && valor_analogico_solo < 400){
    Serial.println(" Status: Solo umido");
    apagaleds();
    digitalWrite(led_verde_solo, HIGH);
  }

  // LED AMARELO
   if (valor_analogico_solo > 400 && valor_analogico_solo < 800){
    Serial.println(" Status: Umidade moderada");
    apagaleds();
    digitalWrite(led_amarelo_solo, HIGH);
  }

  // LED VERMELHO
  if (valor_analogico_solo > 800 && valor_analogico_solo < 1024){
    Serial.println(" Status: Solo seco");
    apagaleds();
    digitalWrite(led_vermelho_solo, HIGH);
  }
  delay(100);

// =============================================== LCD e DHT ==================================================================== //

  float humidity = dht.readHumidity(); //Le o valor da umidade
  float temperature = dht.readTemperature(); //Le o valor da temperatura
  lcd.setCursor(0,0);
  lcd.print("Temp : ");
  lcd.print(" ");
  lcd.setCursor(7,0);
  lcd.print(temperature,1);
  lcd.setCursor(12,0);
   
  //Mostra o simbolo do grau formado pelo array
  lcd.write((byte)0);
   
  //Mostra o simbolo do grau quadrado
  //lcd.print((char)223);
   
  lcd.setCursor(0,1);
  lcd.print("Umid : ");
  lcd.print(" ");
  lcd.setCursor(7,1);
  lcd.print(humidity,1);
  lcd.setCursor(12,1);
  lcd.print("%");
  //Intervalo recomendado para leitura do sensor
  delay(2000);


  lcd.scrollDisplayLeft();
  delay(400);

  if (valor_analogico_solo > 0 && valor_analogico_solo < 400){
    lcd.setCursor(0,1);
    lcd.print("SOLO UMIDO");
  } else if (valor_analogico_solo > 400 && valor_analogico_solo < 800){
    lcd.setCursor(0,1);
    lcd.print("UMIDADE MODERADA");
  } else {
    lcd.setCursor(0,1);
    lcd.print("SOLO SECO");
  }

  // ========================================= Sensor de Luz ============================================//

  valor_analogico_luz = analogRead(ldr);
  Serial.print("Leitura de 0 a 1023: ");
  Serial.println(valor_analogico_luz);

  if(valor_analogico_luz >= 100 && valor_analogico_luz < 300) {
    Serial.println("Status: Baixa Incidencia de luz");
    apagaleds();
    digitalWrite(led_luz_a, HIGH);
    digitalWrite(led_luz_b, LOW);
    digitalWrite(led_luz_c, LOW);
  }

  if(valor_analogico_luz >= 300 && valor_analogico_luz < 600) {
    Serial.println("Status: Media Incidencia de luz");
    apagaleds();
    digitalWrite(led_luz_a, HIGH);
    digitalWrite(led_luz_b, HIGH);
    digitalWrite(led_luz_c, LOW);
  }

  if(valor_analogico_luz >= 600 && valor_analogico_luz < 1023) {
    Serial.println("Status: Alta Incidencia de luz");
    apagaleds();
    digitalWrite(led_luz_a, HIGH);
    digitalWrite(led_luz_b, HIGH);
    digitalWrite(led_luz_c, HIGH);
  }
  delay(1000);
}
 
void apagaleds(){
  digitalWrite(led_vermelho_solo, LOW);
  digitalWrite(led_amarelo_solo, LOW);
  digitalWrite(led_verde_solo, LOW);
}

// ======================================== Umidade e temperatura ambiente ================================================//
  /*float h = dht.readHumidity();
  float t = dht.readTemperature();
    if (isnan(t) || isnan(h)){
    Serial.println("Failed to read from DHT");
  } 
  else {
    Serial.print("Umidade: ");
    Serial.print(h);
    Serial.print(" %t");
    Serial.print("Temperatura: ");
    Serial.print(t);
    Serial.println(" *C");
  }
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    
    if (isnan(t) || isnan(h)) {
      Serial.println("Failed to read from DHT");
    } 
  else {
    Serial.print("Umidade: ");
    Serial.print(h);
    Serial.print(" %t");
    Serial.print("Temperatura: ");
    Serial.print(t);
    Serial.println(" *C");
  }*/
