#include <ESP8266WiFi.h>

const char* ssid = "Milka"; // Nome da Rede

const char* password = "SenhaDaRede"; //Password da rede

int ledPin = 5; // GPIO5 do ESP8266 e D1 do NodeMCU

//DEFINIÇÃO DE IP FIXO PARA O NODEMCU
IPAddress ip(192,168,0,175); //COLOQUE UMA FAIXA DE IP DISPONÍVEL DO SEU ROTEADOR. EX: 192.168.1.110 **** ISSO VARIA, NO MEU CASO É: 192.168.0.175
IPAddress gateway(192,168,0,1); //GATEWAY DE CONEXÃO (ALTERE PARA O GATEWAY DO SEU ROTEADOR)
IPAddress subnet(255,255,255,0); //MASCARA DE REDE

WiFiServer server(80); // Porta 80

void setup() {

  Serial.begin(115200);

  delay(10);

  pinMode(ledPin, OUTPUT); // Define o D7 como saída

  digitalWrite(ledPin, LOW); // O LED começa desligado
     
  // Comunicação com a rede WiFi

  WiFi.begin(ssid, password); // Inicia a ligação a rede

  WiFi.config(ip, gateway, subnet); //PASSA OS PARÂMETROS PARA A FUNÇÃO QUE VAI SETAR O IP FIXO NO NODEMCU
  
  while (WiFi.status() != WL_CONNECTED) {

    delay(500);

    Serial.print("."); // Enquanto a ligação não for efectuada com sucesso é apresentado no monitor série uma sucessão de “.”
  }

  Serial.println("");

  Serial.println("WiFi connected"); // Se a ligação é efectuada com sucesso apresenta esta mensagem no monitor série

  // Servidor

  server.begin(); // Comunicação com o servidor

  Serial.println("Servidor iniciado"); //é apresentado no monitor série que o  servidor foi iniciado
  
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
  
  if (!client) { // Verifica se o cliente está conectado ao servidor, executa este ciclo até estar conectado
  
    return;
  
  }
  
  // Espera até o cliente enviar dados
  
  Serial.println("novo cliente"); //Apresenta esta mensagem quando o cliente se liga ao servidor
  
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

    digitalWrite(ledPin, HIGH); // Se o pedido no LedPin for LED=ON, acende o LED

    value = HIGH;

  }

  if (request.indexOf("/LED=OFF") != -1)  {

    digitalWrite(ledPin, LOW); // Se o pedido no LedPin for LED=OFF, apaga o LED

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

  if(value == HIGH) { // Se está ligado apresenta “on”

    client.print("On");

  } else {

    client.print("Off");// Se está desligado apresenta “Off”

  }

  client.println("<br><br>");

  client.println("<a href=\"/LED=ON\"\"><button>Turn On </button></a>");// Ligar o LED corresponde Turn On
  
  client.println("<a href=\"/LED=OFF\"\"><button>Turn Off </button></a><br />");  // Desligar o LED corresponde Turn Off
  
  client.println("</html>");
  
  delay(1);
  
  Serial.println("Cliente desconectado"); // Depois do cliente efectuar o pedido apresenta esta mensagem no monitor série
  
  Serial.println("");
  
}
