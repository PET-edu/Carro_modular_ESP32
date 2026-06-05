#include <WiFi.h>
#include <WebServer.h>

/* ================== PINOS ================== */
// Motor A
const int motor_a_in1 = 32;
const int motor_a_in2 = 18;

// Motor B
const int motor_b_in1 = 27;
const int motor_b_in2 = 26;

/* ================== PWM ================== */
const int pwm_freq = 1000;
const int pwm_resolution = 8; // 0–255

const int pwm_channel_a1 = 0;
const int pwm_channel_a2 = 1;
const int pwm_channel_b1 = 2;
const int pwm_channel_b2 = 3;

/* ================== WIFI ================== */
const char* ap_ssid = "Jimin do BTS";

/* ================== WEB SERVER ================== */
WebServer server(80);

/* ================== HTML ================== */
String html_page()
{
    return R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Controle ESP32</title>
<style>
body { font-family: Arial; text-align: center; }
button { width: 100px; height: 40px; margin: 5px; font-size: 16px; }
input { width: 200px; }
</style>
</head>
<body>

<h2>Motor A</h2>
<button onclick="send('A','UP')">UP</button>
<button onclick="send('A','DOWN')">DOWN</button><br>
<input type="range" min="0" max="255" value="0"
oninput="sendSpeed('A', this.value)">

<h2>Motor B</h2>
<button onclick="send('B','UP')">UP</button>
<button onclick="send('B','DOWN')">DOWN</button><br>
<input type="range" min="0" max="255" value="0"
oninput="sendSpeed('B', this.value)">

<script>
function send(motor, dir)
{
    fetch(`/dir?m=${motor}&d=${dir}`);
}

function sendSpeed(motor, speed)
{
    fetch(`/speed?m=${motor}&s=${speed}`);
}
</script>

</body>
</html>
)rawliteral";
}

/* ================== FUNÇÕES ================== */

void motor_stop()
{
    ledcWrite(pwm_channel_a1, 0);
    ledcWrite(pwm_channel_a2, 0);
    ledcWrite(pwm_channel_b1, 0);
    ledcWrite(pwm_channel_b2, 0);
}

void motor_dir(char motor, char dir)
{
    if (motor == 'A')
    {
        if (dir == 'U') // UP
        {
            ledcWrite(pwm_channel_a2, 0);
        }
        else // DOWN
        {
            ledcWrite(pwm_channel_a1, 0);
        }
    }

    if (motor == 'B')
    {
        if (dir == 'U')
        {
            ledcWrite(pwm_channel_b2, 0);
        }
        else
        {
            ledcWrite(pwm_channel_b1, 0);
        }
    }
}

void motor_speed(char motor, int speed)
{
    speed = constrain(speed, 0, 255);

    if (motor == 'A')
    {
        if (ledcRead(pwm_channel_a1) == 0)
            ledcWrite(pwm_channel_a2, speed);
        else
            ledcWrite(pwm_channel_a1, speed);
    }

    if (motor == 'B')
    {
        if (ledcRead(pwm_channel_b1) == 0)
            ledcWrite(pwm_channel_b2, speed);
        else
            ledcWrite(pwm_channel_b1, speed);
    }
}

/* ================== ROTAS ================== */

void handle_root()
{
    server.send(200, "text/html", html_page());
}

void handle_dir()
{
    char motor = server.arg("m")[0];
    char dir   = server.arg("d")[0];

    motor_dir(motor, dir);
    server.send(200, "text/plain", "OK");
}

void handle_speed()
{
    char motor = server.arg("m")[0];
    int speed  = server.arg("s").toInt();

    motor_speed(motor, speed);
    server.send(200, "text/plain", "OK");
}

/* ================== SETUP ================== */

void setup()
{
    Serial.begin(115200);

    pinMode(motor_a_in1, OUTPUT);
    pinMode(motor_a_in2, OUTPUT);
    pinMode(motor_b_in1, OUTPUT);
    pinMode(motor_b_in2, OUTPUT);

    ledcSetup(pwm_channel_a1, pwm_freq, pwm_resolution);
    ledcSetup(pwm_channel_a2, pwm_freq, pwm_resolution);
    ledcSetup(pwm_channel_b1, pwm_freq, pwm_resolution);
    ledcSetup(pwm_channel_b2, pwm_freq, pwm_resolution);

    ledcAttachPin(motor_a_in1, pwm_channel_a1);
    ledcAttachPin(motor_a_in2, pwm_channel_a2);
    ledcAttachPin(motor_b_in1, pwm_channel_b1);
    ledcAttachPin(motor_b_in2, pwm_channel_b2);

    motor_stop();

    WiFi.softAP(ap_ssid);
    Serial.println(WiFi.softAPIP());

    server.on("/", handle_root);
    server.on("/dir", handle_dir);
    server.on("/speed", handle_speed);

    server.begin();
}

/* ================== LOOP ================== */

void loop()
{
    server.handleClient();
}

