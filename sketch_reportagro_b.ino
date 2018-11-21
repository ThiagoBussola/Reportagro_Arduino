#include <DHT.h>
#include <LiquidCrystal.h>
 
int valor_analogico_solo;
int valor_analogico_luz;
int ldr  = A2;
#define sinal_analogico A0
#define sinal_analogico_2 A1
//Define a ligação ao pino de dados do sensor
#define DHTPIN A1
//Define o tipo de sensor DHT utilizado
#define DHTTYPE DHT22
#define DEBUG true
#define led_luz_a 2
#define led_luz_b 3
#define led_luz_c 4
//Define os pinos que serão ligados ao LCD
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);
DHT dht(DHTPIN, DHTTYPE);
void setup() {

  Serial.begin(9600);

  lcd.begin(16,2); //Inicializa LCD
  lcd.clear(); //Limpa o LCD

  pinMode(sinal_analogico, INPUT);
  pinMode(sinal_analogico_2, INPUT);
  pinMode(6, OUTPUT);
  
  Serial.println("Iniciando....");
  dht.begin();

    // pinMode Luminosidade
  pinMode(led_luz_a, OUTPUT);
  pinMode(led_luz_b, OUTPUT);
  pinMode(led_luz_c, OUTPUT);
}
 
void loop() {
  digitalWrite(6, HIGH);
  valor_analogico_solo = analogRead(sinal_analogico);

  // LED VERDE
   if (valor_analogico_solo > 0 && valor_analogico_solo < 400){
    Serial.println("Status: Solo umido");
  }

  // LED AMARELO
   if (valor_analogico_solo > 400 && valor_analogico_solo < 800){
    Serial.println("Status: Umidade moderada");
  }

  // LED VERMELHO
  if (valor_analogico_solo > 800 && valor_analogico_solo < 1024){
    Serial.println("Status: Solo seco");
  }

  float humidity = dht.readHumidity(); //Le o valor da umidade
  float temperature = dht.readTemperature(); //Le o valor da temperatura
  Serial.print("Umidade: ");
  Serial.println(humidity);
  Serial.print("Temperatura: ");
  Serial.print(temperature);
  Serial.println(" graus.");

  lcd.setCursor(0,0);
  lcd.print("Temp: ");
  lcd.setCursor(6,0);
  lcd.print(temperature,0);
  lcd.setCursor(9,0);
  lcd.print("Um: ");
  lcd.setCursor(13,0);
  lcd.print(humidity,0);
  lcd.setCursor(15,0);
  lcd.print("%");

  
  if (valor_analogico_solo > 0 && valor_analogico_solo < 400){
    lcd.setCursor(0,1);
    lcd.print("Terra: Umida");
  } else if (valor_analogico_solo > 400 && valor_analogico_solo < 800){
    lcd.setCursor(0,1);
    lcd.print("Terra: Molhada");
  } else {
    lcd.setCursor(0,1);
    lcd.print("Terra: Seca");
  }

  // ========================================= Sensor de Luz ============================================//

  valor_analogico_luz = analogRead(ldr);
  Serial.print("Leitura de 0 a 1023: ");
  Serial.println(valor_analogico_luz);

  if(valor_analogico_luz >= 160 && valor_analogico_luz < 300) {
    Serial.println("Status: Alta Incidencia de luz");
    apagaleds();
    digitalWrite(led_luz_a, HIGH);
    digitalWrite(led_luz_b, HIGH);
    digitalWrite(led_luz_c, HIGH);
  }

  if(valor_analogico_luz >= 300 && valor_analogico_luz < 500) {
    Serial.println("Status: Media Incidencia de luz");
    apagaleds();
    digitalWrite(led_luz_a, HIGH);
    digitalWrite(led_luz_b, HIGH);
    digitalWrite(led_luz_c, LOW);
  }

  if(valor_analogico_luz >= 500 && valor_analogico_luz < 1024) {
    Serial.println("Status: Baixa Incidencia de luz");
    apagaleds();
    digitalWrite(led_luz_a, HIGH);
    digitalWrite(led_luz_b, LOW);
    digitalWrite(led_luz_c, LOW);
  }

  
  delay(1000);
  lcd.clear();
}
 void apagaleds(){
  digitalWrite(led_luz_a, LOW);
  digitalWrite(led_luz_b, LOW);
  digitalWrite(led_luz_c, LOW);
}



