//Inclui as funções de Wifi do ESP
#include <ESP8266WiFi.h>

//Cria um server na porta 80 (porta padrão para onde os navegadores enviam as requisições http)
WiFiServer server(80);

String SISTEM_STATE = "off";

void setup()
{
  //Inicializa a Serial apenas para efeito de log
  Serial.begin(115200);

  //Configura o GPIO0 e GPIO2 como output, ou seja, como saída para podermos alterar o seu valor
  pinMode(0, OUTPUT);
  //pinMode(2, OUTPUT);
  //Deixa o GPIO0 e o GPIO2 com saída LOW
  digitalWrite(0, LOW);
  //digitalWrite(2, LOW);

  Serial.print("Conectando");
  //Faz o ESP se conectar à rede WiFi. No nosso exemplo o ssid da rede é TesteESP e a senha é 87654321
  WiFi.begin("LABMET", "****");

  //Enquanto o ESP não se conectar à rede
  while (WiFi.status() != WL_CONNECTED)
  {
    //Esperamos 100 milisegundos
    delay(100);
    Serial.print(".");
  }

  //Se chegou aqui é porque conectou à rede, então mostramos no monitor serial para termos um feedback
  Serial.println("");
  Serial.println("Conectou");

  //Configurações do IP fixo. Você pode alterar conforme a sua rede
  IPAddress ip(192, 168, 255, 30);
  IPAddress gateway(192, 168, 255, 3);
  IPAddress subnet(255, 255, 255, 0);
  Serial.print("Configurando IP fixo para : ");
  Serial.println(ip);

  //Envia a configuração
  WiFi.config(ip, gateway, subnet);

  //Inicializa o server que criamos na porta 80
  server.begin();

  //Mostramos no monitor serial o IP que o ESP possui para verificarmos se é o mesmo que configuramos
  Serial.print("Server em: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  //Verifica se algum cliente está tentando se conectar
  WiFiClient client = server.available();
  if (!client)
  {
    //Se não houver nenhum cliente podemos retornar pois não há nada a fazer
    return;
  }

  Serial.println("Novo cliente conectou");

  //Fazemos a leitura da requisição
  String req = client.readStringUntil('\r');
  Serial.print("Requisição: ");
  Serial.println(req);

  //Este é o html que iremos retornar para o cliente
  //É composto basicamente de dois botões (ON e OFF) para o GPIO0 e dois botões (ON e OFF) para o GPIO2
  //A parte que nos interessa é o <a href=' com a ação vinculada a cada botão
  //Quando clicamos em um destes botões essa informação chegará até o ESP para que ele verifique qual ação deve executar
  //A parte dentro de '<style>' é apenas para modificarmos o visual da página que será exibida, você pode alterá-la como queira
  String html = 
  "<!DOCTYPE html><html>"
    "<head>"
      "<meta name='viewport' content='width=device-width, initial-scale=1, user-scalable=no'/>"
      "<title>""Acionamento Remoto Uirapuru""</title>"
      "<style>"
        "body{"
          "text-align: center;"
          "font-family: sans-serif;"
          "font-size:14px;"
          "padding: 25px;"
        "}"

        "p{"
          "color:#444;"
        "}"

        "button{"
          "outline: none;"
          "border: 2px solid #1fa3ec;"
          "border-radius:18px;"
          "background-color:#FFF;"
          "color: #1fa3ec;"
          "padding: 10px 50px;"
        "}"

        "button:active{"
          "color: #fff;"
          "background-color:#1fa3ec;"
        "}"
      "</style>"
    "</head>"
    
    "<body><img src='https://bingotelescope.org/wp-content/uploads/2021/04/logo-bingo-e1625425603871-100x88.png'></body>" 
    "<body><center><h1>Acionamento Remoto Uirapuru</h1></center>"
    "<center><h2>Precione ON para ligar o sistema e OFF para desligar</h2></center>"
    "<body>"
    "<p> Status do Sistema:" + SISTEM_STATE + "</p>"
    "<p><a href='?acao=gpio0On'><button>ON</button></a></p>"
    "<p><a href='?acao=gpio0Off'><button>OFF</button></a></p>"
//    "<p>GPIO2</p>"
//    "<p><a href='?acao=gpio2On'><button>ON</button></a></p>"
//    "<p><a href='?acao=gpio2Off'><button>OFF</button></a></p>"
    "</body>"
  "</html>";

  //Escreve o html no buffer que será enviado para o cliente
  client.print(html);
  //Envia os dados do buffer para o cliente
  client.flush();

  //Verifica se a requisição possui a ação gpio0On
  if (req.indexOf("acao=gpio0On") != -1)
  {
      //Se possui a ação gpio0On colocamos a saída do GPIO0 como alta
      digitalWrite(0, HIGH);
      SISTEM_STATE = "ON";
  }
  //Senão, verifica se a requisição possui a ação gpio0Off
  else if (req.indexOf("acao=gpio0Off") != -1)
  {
    //Se possui a ação gpio0Off colocamos a saída do GPIO0 como baixa
    digitalWrite(0, LOW);
    SISTEM_STATE = "OFF";
  }
  //Senão, verifica se a requisição possui a ação gpio2On
//  else if (req.indexOf("acao=gpio2On") != -1)
//  {
//    //Se possui a ação gpio2On colocamos a saída do GPIO2 como alta
//    digitalWrite(2, HIGH);
//  }
//  //Senão, verifica se a requisição possui a ação gpio2Off
//  else if (req.indexOf("acao=gpio2Off") != -1)
//  {
//    //Se possui a ação gpio0Off colocamos a saída do GPIO2 como baixa
//    digitalWrite(2, LOW);
//  }

  //Fecha a conexão com o cliente
  client.stop();
  Serial.println("Cliente desconectado");
}