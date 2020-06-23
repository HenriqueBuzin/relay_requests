#include <ESP8266WiFi.h>

// Nome da Rede
const char* ssid = "Milka"; 

//Password da rede
const char* password = "SenhaDaRede";

// GPIO5 do ESP8266 e D1 do NodeMCU
int ledPin = 5;

//DEFINIÇÃO DE IP FIXO PARA O NODEMCU
//COLOQUE UMA FAIXA DE IP DISPONÍVEL DO SEU ROTEADOR. EX: 192.168.1.110 **** ISSO VARIA, NO MEU CASO É: 192.168.0.175
IPAddress ip(192,168,0,175); 
//GATEWAY DE CONEXÃO (ALTERE PARA O GATEWAY DO SEU ROTEADOR)
IPAddress gateway(192,168,0,1);
//MASCARA DE REDE
IPAddress subnet(255,255,255,0);

// Porta 80
WiFiServer server(80);

void setup() {

  Serial.begin(115200);

  delay(10);

  // Define o D7 como saída
  pinMode(ledPin, OUTPUT);

  // O LED começa desligado
  digitalWrite(ledPin, LOW);
     
  // Comunicação com a rede WiFi
  
  // Mode de estação 
  WiFi.mode(WIFI_STA);

  // Inicia a ligação a rede
  WiFi.begin(ssid, password);

  //PASSA OS PARÂMETROS PARA A FUNÇÃO QUE VAI SETAR O IP FIXO NO NODEMCU
  WiFi.config(ip, gateway, subnet);
  
  while (WiFi.status() != WL_CONNECTED) {

    delay(500);
    
    // Enquanto a ligação não for efectuada com sucesso é apresentado no monitor série uma sucessão de “.”
    Serial.print(".");
  }

  Serial.println("");

  // Se a ligação é efectuada com sucesso apresenta esta mensagem no monitor série
  Serial.println("WiFi connected");
  
  // Servidor
  // Comunicação com o servidor
  server.begin();
  
  //é apresentado no monitor série que o  servidor foi iniciado
  Serial.println("Servidor iniciado");
  
}

void loop() {

  if(WiFi.status() != WL_CONNECTED){

    Serial.println("Reconectando...");
    // Função que reconectará ao WIFI caso esteja disponível novamente.
    
    WiFi.reconnect();
    // Se o status de WiFi.status() continuar diferente de WL_CONNECTED
    
    // vai aguardar 1 segundo até a proxima verificação.
    delay(1000);
  }
  
  // Verificação se o cliente está conectado
  
  WiFiClient client = server.available();

  // Verifica se o cliente está conectado ao servidor, executa este ciclo até estar conectado
  if (!client) {
  
    return;
  
  }
  
  // Espera até o cliente enviar dados

  //Apresenta esta mensagem quando o cliente se liga ao servidor
  Serial.println("novo cliente");
  
  while(!client.available()){
  
    delay(1);
  
  }
  
  // Ler a primeira linha do pedido

  String request = client.readStringUntil('\r');

  Serial.println(request);

  client.flush();

  // Operação do pedido

  int value = LOW;

  if (request.indexOf("/LED=ON") != -1)  {

    // Se o pedido no LedPin for LED=ON, acende o LED
    digitalWrite(ledPin, HIGH);

    value = HIGH;

  }

  if (request.indexOf("/LED=OFF") != -1)  {

    // Se o pedido no LedPin for LED=OFF, apaga o LED
    digitalWrite(ledPin, LOW);

    value = LOW;

  }

  // Inicialização da página HTML

  // Retorno do resposta

  client.println("HTTP/1.1 200 OK");

  client.println("Content-Type: text/html");

  client.println(""); //  do not forget this one

  client.println("<!DOCTYPE HTML>");

  client.println("<html>");

  client.print("Estado do LED: ");

  // Se está ligado apresenta “on”
  if(value == HIGH) {

    client.print("On");

  } else {

    // Se está desligado apresenta “Off”
    client.print("Off");

  }

  client.println("<br><br>");

  // Ligar o LED corresponde Turn On
  client.println("<a href=\"/LED=ON\"\"><button>Turn On </button></a>");

  // Desligar o LED corresponde Turn Off
  client.println("<a href=\"/LED=OFF\"\"><button>Turn Off </button></a><br />");
  
  client.println("</html>");
  
  delay(1);

  // Depois do cliente efectuar o pedido apresenta esta mensagem no monitor série
  Serial.println("Cliente desconectado");
  
  Serial.println("");
  
}
