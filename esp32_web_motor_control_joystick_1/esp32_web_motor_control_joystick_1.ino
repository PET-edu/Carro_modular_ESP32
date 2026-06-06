#include <WiFi.h>
#include <WebServer.h>

// ===== CONFIGURAÇÃO WiFi =====
const char* ssid = "ROBO_JOYSTICK";
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

// Velocidades atuais (0-255)
int velA = 0;
int velB = 0;
char dirA = 'U';  // U = frente, D = ré
char dirB = 'U';

// ===== FUNÇÕES DOS MOTORES =====

void atualizarMotorA() {
  if (velA == 0) {
    ledcWrite(motorA_frente, 0);
    ledcWrite(motorA_re, 0);
  } else if (dirA == 'U') {
    // Frente
    ledcWrite(motorA_frente, velA);
    ledcWrite(motorA_re, 0);
  } else {
    // Ré
    ledcWrite(motorA_frente, 0);
    ledcWrite(motorA_re, velA);
  }
}

void atualizarMotorB() {
  if (velB == 0) {
    ledcWrite(motorB_frente, 0);
    ledcWrite(motorB_re, 0);
  } else if (dirB == 'U') {
    // Frente
    ledcWrite(motorB_frente, velB);
    ledcWrite(motorB_re, 0);
  } else {
    // Ré
    ledcWrite(motorB_frente, 0);
    ledcWrite(motorB_re, velB);
  }
}

// ===== PÁGINA HTML - JOYSTICK =====

String paginaHTML() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html><head><meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Joystick ESP32</title>
  <style>
    * { box-sizing: border-box; margin: 0; padding: 0; }

    body {
      background: #1a1a2e;
      display: flex;
      flex-direction: column;
      align-items: center;
      justify-content: center;
      height: 100vh;
      font-family: Arial, sans-serif;
      color: #eee;
      user-select: none;
      touch-action: none;
      overflow: hidden;
    }

    h2 {
      margin-bottom: 30px;
      font-size: 20px;
      letter-spacing: 3px;
      color: #e94560;
    }

    #controllers {
      display: flex;
      gap: 60px;
      align-items: center;
    }

    .joy-wrapper {
      display: flex;
      flex-direction: column;
      align-items: center;
      gap: 12px;
    }

    .joy-label {
      font-size: 13px;
      letter-spacing: 1px;
      color: #aaa;
      text-transform: uppercase;
    }

    .joy-track {
      position: relative;
      width: 70px;
      height: 260px;
      border-radius: 35px;
      background: #16213e;
      border: 3px solid #0f3460;
      box-shadow: 0 0 18px #0f346066;
    }

    .joy-track::after {
      content: '';
      position: absolute;
      left: 50%;
      top: 50%;
      transform: translate(-50%, -50%);
      width: 30px;
      height: 2px;
      background: #0f3460;
      border-radius: 2px;
    }

    .joy-thumb {
      position: absolute;
      width: 54px;
      height: 54px;
      border-radius: 50%;
      background: radial-gradient(circle at 35% 35%, #e94560, #700);
      box-shadow: 0 0 14px #e9456099;
      left: 50%;
      top: 50%;
      transform: translate(-50%, -50%);
      transition: top 0.05s;
    }

    #info {
      margin-top: 28px;
      display: flex;
      gap: 20px;
    }

    .badge {
      padding: 5px 16px;
      border-radius: 10px;
      background: #0f3460;
      font-size: 13px;
      color: #ccc;
    }

    .badge b { color: #e94560; }

    .arrows {
      display: flex;
      flex-direction: column;
      align-items: center;
      gap: 4px;
      color: #0f3460;
      font-size: 18px;
    }
  </style>
</head>
<body>

<h2>🕹️ ESP32 DUAL JOYSTICK</h2>

<div id="controllers">
  <div class="joy-wrapper">
    <div class="arrows">▲</div>
    <div class="joy-label">Motor B</div>
    <div class="joy-track" id="trackB">
      <div class="joy-thumb" id="thumbB" style="top: 130px; transform: translateX(-50%);"></div>
    </div>
    <div class="arrows">▼</div>
  </div>

  <div class="joy-wrapper">
    <div class="arrows">▲</div>
    <div class="joy-label">Motor A</div>
    <div class="joy-track" id="trackA">
      <div class="joy-thumb" id="thumbA" style="top: 130px; transform: translateX(-50%);"></div>
    </div>
    <div class="arrows">▼</div>
  </div>
</div>

<div id="info">
  <div class="badge">Motor A: <b id="valA">+0</b></div>
  <div class="badge">Motor B: <b id="valB">+0</b></div>
</div>

<script>
  const SEND_INTERVAL = 80;
  const THUMB_HALF = 27;

  function makeJoystick(motor, trackId, thumbId, valId) {
    const track = document.getElementById(trackId);
    const thumb = document.getElementById(thumbId);
    const val = document.getElementById(valId);

    let active = false;
    let trackTop = 0;
    let trackH = 0;
    let lastSpeed = -999;
    let lastDir = '';
    let sendTimer = null;
    const TRAVEL = 0;

    function refreshBounds() {
      const r = track.getBoundingClientRect();
      trackTop = r.top;
      trackH = r.height;
    }

    function setThumb(py) {
      const minY = THUMB_HALF;
      const maxY = trackH - THUMB_HALF;
      py = Math.max(minY, Math.min(maxY, py));
      thumb.style.top = py + 'px';
      thumb.style.transform = 'translateX(-50%)';
      return py;
    }

    function centerPx() { return trackH / 2; }

    function fromPy(py) {
      const center = centerPx();
      const maxDelta = center - THUMB_HALF;
      const delta = py - center;
      const clamped = Math.max(-maxDelta, Math.min(maxDelta, delta));
      const speed = Math.round(Math.abs(clamped) / maxDelta * 255);
      const dir = clamped <= 0 ? 'U' : 'D';
      return { speed, dir };
    }

    function send(speed, dir) {
      if (speed === lastSpeed && dir === lastDir) return;
      lastSpeed = speed;
      lastDir = dir;
      val.textContent = (dir === 'U' ? '+' : '-') + speed;

      clearTimeout(sendTimer);
      sendTimer = setTimeout(() => {
        fetch(`/joy?m=${motor}&s=${speed}&d=${dir}`).catch(()=>{});
      }, SEND_INTERVAL);
    }

    function reset() {
      setThumb(centerPx());
      send(0, 'U');
    }

    function onPointerDown(e) {
      if (active) return;
      e.preventDefault();
      active = true;
      refreshBounds();
      track.setPointerCapture && track.setPointerCapture(e.pointerId);
      const py = (e.clientY - trackTop);
      const { speed, dir } = fromPy(setThumb(py));
      send(speed, dir);
    }

    function onPointerMove(e) {
      if (!active) return;
      e.preventDefault();
      const py = (e.clientY - trackTop);
      const { speed, dir } = fromPy(setThumb(py));
      send(speed, dir);
    }

    function onPointerUp(e) {
      if (!active) return;
      active = false;
      reset();
    }

    track.addEventListener('pointerdown', onPointerDown);
    track.addEventListener('pointermove', onPointerMove);
    track.addEventListener('pointerup', onPointerUp);
    track.addEventListener('pointercancel', onPointerUp);

    refreshBounds();
    reset();
  }

  makeJoystick('B', 'trackB', 'thumbB', 'valB');
  makeJoystick('A', 'trackA', 'thumbA', 'valA');
</script>

</body></html>
)rawliteral";
  return html;
}

// ===== ROTAS DO SERVIDOR =====

void handleRoot() {
  server.send(200, "text/html", paginaHTML());
}

void handleJoystick() {
  if (!server.hasArg("m") || !server.hasArg("s") || !server.hasArg("d")) {
    server.send(400, "text/plain", "Parametros faltando");
    return;
  }

  char motor = server.arg("m")[0];
  int speed = server.arg("s").toInt();
  char dir = server.arg("d")[0];

  speed = constrain(speed, 0, 255);

  if (motor == 'A') {
    velA = speed;
    dirA = dir;
    atualizarMotorA();
  } else if (motor == 'B') {
    velB = speed;
    dirB = dir;
    atualizarMotorB();
  }

  String status = "A: " + String(dirA == 'U' ? '+' : '-') + String(velA) + 
                  " | B: " + String(dirB == 'U' ? '+' : '-') + String(velB);
  Serial.println("JOY: " + status);
  server.send(200, "text/plain", status);
}

// ===== SETUP =====

void setup() {
  Serial.begin(115200);
  Serial.println("\n=== Robô ESP32 - Joystick ===\n");

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
  server.on("/joy", handleJoystick);

  server.begin();
  Serial.println("\nServidor iniciado!");
  Serial.println("Acesse: http://192.168.4.1\n");
}

// ===== LOOP =====

void loop() {
  server.handleClient();
}
