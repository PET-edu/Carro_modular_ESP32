#include <WiFi.h>
#include <WebServer.h>

// ===== CONFIGURAÇÃO WiFi =====
const char* ssid = "LADY";
const char* password = "12345678";

WebServer server(80);

// ===== MOTOR A =====
const int motorA_frente = 18;
const int motorA_re     = 32;

// ===== MOTOR B =====
const int motorB_frente = 27;
const int motorB_re     = 26;

const int freq = 5000;
const int resolucao = 8;

// Velocidades
const int VEL_ALTA  = 255;
const int VEL_BAIXA = 180;

// Estado atual dos botões
bool A_frente_alta  = false;
bool A_frente_baixa = false;
bool A_re_alta      = false;
bool A_re_baixa     = false;
bool B_frente_alta  = false;
bool B_frente_baixa = false;
bool B_re_alta      = false;
bool B_re_baixa     = false;

// ===== FUNÇÕES DOS MOTORES =====

void atualizarMotorA() {
  if (A_frente_alta) {
    ledcWrite(motorA_frente, VEL_ALTA);
    ledcWrite(motorA_re, 0);
  } else if (A_frente_baixa) {
    ledcWrite(motorA_frente, VEL_BAIXA);
    ledcWrite(motorA_re, 0);
  } else if (A_re_baixa) {
    ledcWrite(motorA_frente, 0);
    ledcWrite(motorA_re, VEL_BAIXA);
  } else if (A_re_alta) {
    ledcWrite(motorA_frente, 0);
    ledcWrite(motorA_re, VEL_ALTA);
  } else {
    ledcWrite(motorA_frente, 0);
    ledcWrite(motorA_re, 0);
  }
}

void atualizarMotorB() {
  if (B_frente_alta) {
    ledcWrite(motorB_frente, VEL_ALTA);
    ledcWrite(motorB_re, 0);
  } else if (B_frente_baixa) {
    ledcWrite(motorB_frente, VEL_BAIXA);
    ledcWrite(motorB_re, 0);
  } else if (B_re_baixa) {
    ledcWrite(motorB_frente, 0);
    ledcWrite(motorB_re, VEL_BAIXA);
  } else if (B_re_alta) {
    ledcWrite(motorB_frente, 0);
    ledcWrite(motorB_re, VEL_ALTA);
  } else {
    ledcWrite(motorB_frente, 0);
    ledcWrite(motorB_re, 0);
  }
}

// ===== PÁGINA HTML =====

String paginaHTML() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="pt-BR">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Controle Robô ESP32</title>
  <style>
    * {
      margin: 0;
      padding: 0;
      box-sizing: border-box;
      user-select: none;
      -webkit-user-select: none;
      touch-action: manipulation;
    }

    body {
      background: #1a1a2e;
      color: white;
      font-family: 'Segoe UI', Arial, sans-serif;
      height: 100vh;
      display: flex;
      flex-direction: column;
      align-items: center;
      justify-content: center;
      overflow: hidden;
    }

    h1 {
      font-size: 1.5em;
      margin-bottom: 10px;
      color: #00d4ff;
      text-shadow: 0 0 10px #00d4ff55;
    }

    .status {
      font-size: 0.9em;
      margin-bottom: 15px;
      color: #aaa;
      min-height: 20px;
    }

    .container {
      display: flex;
      gap: 40px;
      align-items: center;
      justify-content: center;
    }

    .motor-group {
      display: flex;
      flex-direction: column;
      align-items: center;
      gap: 8px;
    }

    .motor-label {
      font-size: 1.1em;
      font-weight: bold;
      color: #ffd700;
      margin-bottom: 5px;
    }

    .btn {
      width: 120px;
      height: 60px;
      border: none;
      border-radius: 12px;
      font-size: 0.85em;
      font-weight: bold;
      cursor: pointer;
      transition: all 0.1s;
      display: flex;
      align-items: center;
      justify-content: center;
      text-align: center;
      line-height: 1.2;
    }

    .btn:active, .btn.pressed {
      transform: scale(0.95);
      filter: brightness(1.3);
    }

    .btn-frente-alta {
      background: linear-gradient(135deg, #00c853, #009624);
      color: white;
      box-shadow: 0 4px 15px #00c85355;
    }

    .btn-frente-baixa {
      background: linear-gradient(135deg, #66bb6a, #43a047);
      color: white;
      box-shadow: 0 4px 15px #66bb6a55;
    }

    .btn-re-baixa {
      background: linear-gradient(135deg, #ef5350, #c62828);
      color: white;
      box-shadow: 0 4px 15px #ef535055;
    }

    .btn-re-alta {
      background: linear-gradient(135deg, #b71c1c, #880e0e);
      color: white;
      box-shadow: 0 4px 15px #b71c1c55;
    }

    .separator {
      width: 3px;
      height: 280px;
      background: linear-gradient(to bottom, transparent, #00d4ff, transparent);
      border-radius: 2px;
    }

    .parar-container {
      margin-top: 20px;
    }

    .btn-parar {
      width: 280px;
      height: 55px;
      background: linear-gradient(135deg, #ff6f00, #e65100);
      color: white;
      border: none;
      border-radius: 12px;
      font-size: 1.1em;
      font-weight: bold;
      cursor: pointer;
      box-shadow: 0 4px 15px #ff6f0055;
    }

    .btn-parar:active {
      transform: scale(0.95);
      filter: brightness(1.3);
    }

    .velocidade-info {
      display: flex;
      gap: 40px;
      margin-top: 15px;
      font-size: 0.85em;
    }

    .vel-box {
      background: #16213e;
      padding: 8px 15px;
      border-radius: 8px;
      text-align: center;
      min-width: 120px;
    }

    .vel-box span {
      color: #00d4ff;
      font-weight: bold;
      font-size: 1.2em;
    }
  </style>
</head>
<body>

  <h1>🤖 Controle Robô ESP32</h1>
  <div class="status" id="status">Conectado!</div>

  <div class="container">

    <!-- MOTOR A (Esquerda) -->
    <div class="motor-group">
      <div class="motor-label">⚙️ MOTOR A</div>

      <button class="btn btn-frente-alta" id="AF_A"
        ontouchstart="pressionar('AF_A')" ontouchend="soltar('AF_A')"
        onmousedown="pressionar('AF_A')" onmouseup="soltar('AF_A')">
        ⬆️ FRENTE<br>RÁPIDO
      </button>

      <button class="btn btn-frente-baixa" id="AF_B"
        ontouchstart="pressionar('AF_B')" ontouchend="soltar('AF_B')"
        onmousedown="pressionar('AF_B')" onmouseup="soltar('AF_B')">
        ⬆️ FRENTE<br>DEVAGAR
      </button>

      <button class="btn btn-re-baixa" id="AR_B"
        ontouchstart="pressionar('AR_B')" ontouchend="soltar('AR_B')"
        onmousedown="pressionar('AR_B')" onmouseup="soltar('AR_B')">
        ⬇️ RÉ<br>DEVAGAR
      </button>

      <button class="btn btn-re-alta" id="AR_A"
        ontouchstart="pressionar('AR_A')" ontouchend="soltar('AR_A')"
        onmousedown="pressionar('AR_A')" onmouseup="soltar('AR_A')">
        ⬇️ RÉ<br>RÁPIDO
      </button>
    </div>

    <div class="separator"></div>

    <!-- MOTOR B (Direita) -->
    <div class="motor-group">
      <div class="motor-label">⚙️ MOTOR B</div>

      <button class="btn btn-frente-alta" id="BF_A"
        ontouchstart="pressionar('BF_A')" ontouchend="soltar('BF_A')"
        onmousedown="pressionar('BF_A')" onmouseup="soltar('BF_A')">
        ⬆️ FRENTE<br>RÁPIDO
      </button>

      <button class="btn btn-frente-baixa" id="BF_B"
        ontouchstart="pressionar('BF_B')" ontouchend="soltar('BF_B')"
        onmousedown="pressionar('BF_B')" onmouseup="soltar('BF_B')">
        ⬆️ FRENTE<br>DEVAGAR
      </button>

      <button class="btn btn-re-baixa" id="BR_B"
        ontouchstart="pressionar('BR_B')" ontouchend="soltar('BR_B')"
        onmousedown="pressionar('BR_B')" onmouseup="soltar('BR_B')">
        ⬇️ RÉ<br>DEVAGAR
      </button>

      <button class="btn btn-re-alta" id="BR_A"
        ontouchstart="pressionar('BR_A')" ontouchend="soltar('BR_A')"
        onmousedown="pressionar('BR_A')" onmouseup="soltar('BR_A')">
        ⬇️ RÉ<br>RÁPIDO
      </button>
    </div>
  </div>

  <div class="parar-container">
    <button class="btn-parar" onclick="pararTudo()">
      🛑 PARAR TUDO
    </button>
  </div>

  <div class="velocidade-info">
    <div class="vel-box">
      Motor A<br><span id="velA">0</span>
    </div>
    <div class="vel-box">
      Motor B<br><span id="velB">0</span>
    </div>
  </div>

  <script>
    function enviar(cmd) {
      fetch('/' + cmd)
        .then(r => r.text())
        .then(t => {
          document.getElementById('status').innerText = t;
          // Atualiza velocidades
          let partes = t.split('|');
          if (partes.length >= 2) {
            document.getElementById('velA').innerText = partes[0].trim();
            document.getElementById('velB').innerText = partes[1].trim();
          }
        })
        .catch(() => {
          document.getElementById('status').innerText = '❌ Sem conexão!';
        });
    }

    function pressionar(id) {
      document.getElementById(id).classList.add('pressed');
      enviar(id + '_ON');
    }

    function soltar(id) {
      document.getElementById(id).classList.remove('pressed');
      enviar(id + '_OFF');
    }

    function pararTudo() {
      enviar('PARAR');
    }

    // Previne menu de contexto no celular
    document.addEventListener('contextmenu', e => e.preventDefault());
  </script>

</body>
</html>
)rawliteral";

  return html;
}

// ===== ROTAS DO SERVIDOR =====

void handleRoot() {
  server.send(200, "text/html", paginaHTML());
}

String getStatusMotores() {
  int velA = 0, velB = 0;

  if (A_frente_alta) velA = VEL_ALTA;
  else if (A_frente_baixa) velA = VEL_BAIXA;
  else if (A_re_baixa) velA = -VEL_BAIXA;
  else if (A_re_alta) velA = -VEL_ALTA;

  if (B_frente_alta) velB = VEL_ALTA;
  else if (B_frente_baixa) velB = VEL_BAIXA;
  else if (B_re_baixa) velB = -VEL_BAIXA;
  else if (B_re_alta) velB = -VEL_ALTA;

  return "A: " + String(velA) + " | B: " + String(velB);
}

void handleComando(String cmd) {
  // ===== MOTOR A =====
  if (cmd == "AF_A_ON")       A_frente_alta = true;
  else if (cmd == "AF_A_OFF") A_frente_alta = false;
  else if (cmd == "AF_B_ON")  A_frente_baixa = true;
  else if (cmd == "AF_B_OFF") A_frente_baixa = false;
  else if (cmd == "AR_B_ON")  A_re_baixa = true;
  else if (cmd == "AR_B_OFF") A_re_baixa = false;
  else if (cmd == "AR_A_ON")  A_re_alta = true;
  else if (cmd == "AR_A_OFF") A_re_alta = false;

  // ===== MOTOR B =====
  else if (cmd == "BF_A_ON")  B_frente_alta = true;
  else if (cmd == "BF_A_OFF") B_frente_alta = false;
  else if (cmd == "BF_B_ON")  B_frente_baixa = true;
  else if (cmd == "BF_B_OFF") B_frente_baixa = false;
  else if (cmd == "BR_B_ON")  B_re_baixa = true;
  else if (cmd == "BR_B_OFF") B_re_baixa = false;
  else if (cmd == "BR_A_ON")  B_re_alta = true;
  else if (cmd == "BR_A_OFF") B_re_alta = false;

  // ===== PARAR =====
  else if (cmd == "PARAR") {
    A_frente_alta = false; A_frente_baixa = false;
    A_re_alta = false;     A_re_baixa = false;
    B_frente_alta = false; B_frente_baixa = false;
    B_re_alta = false;     B_re_baixa = false;
  }

  atualizarMotorA();
  atualizarMotorB();

  String status = getStatusMotores();
  Serial.println("CMD: " + cmd + " → " + status);
  server.send(200, "text/plain", status);
}

// ===== SETUP =====

void setup() {
  Serial.begin(115200);
  Serial.println("\n=== Robô ESP32 WiFi ===\n");

  // PWM
  ledcAttach(motorA_frente, freq, resolucao);
  ledcAttach(motorA_re, freq, resolucao);
  ledcAttach(motorB_frente, freq, resolucao);
  ledcAttach(motorB_re, freq, resolucao);

  // WiFi Access Point
  WiFi.softAP(ssid, password);
  Serial.println("WiFi AP criado!");
  Serial.print("SSID: ");
  Serial.println(ssid);
  Serial.print("Senha: ");
  Serial.println(password);
  Serial.print("IP: ");
  Serial.println(WiFi.softAPIP());

  // Rotas
  server.on("/", handleRoot);

  // Registra todas as rotas de comando
  String comandos[] = {
    "AF_A_ON","AF_A_OFF","AF_B_ON","AF_B_OFF",
    "AR_A_ON","AR_A_OFF","AR_B_ON","AR_B_OFF",
    "BF_A_ON","BF_A_OFF","BF_B_ON","BF_B_OFF",
    "BR_A_ON","BR_A_OFF","BR_B_ON","BR_B_OFF",
    "PARAR"
  };

  for (int i = 0; i < 17; i++) {
    String rota = "/" + comandos[i];
    String cmd = comandos[i];
    server.on(rota.c_str(), [cmd]() {
      handleComando(cmd);
    });
  }

  server.begin();
  Serial.println("\nServidor iniciado!");
  Serial.println("Acesse: http://192.168.4.1\n");
}

// ===== LOOP =====

void loop() {
  server.handleClient();
}