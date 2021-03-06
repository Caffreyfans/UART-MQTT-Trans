/*
 * @Author: Caffreyfans
 * @Date: 2021-01-08 19:51:44
 * @LastEditTime: 2021-01-19 22:57:22
 * @Description: 串口 MQTT 透传
 */
/*
 *                                |~~~~~~~|
 *                                |       |
 *                                |       |
 *                                |       |
 *                                |       |
 *                                |       |
 *     |~.\\\_\~~~~~~~~~~~~~~xx~~~         ~~~~~~~~~~~~~~~~~~~~~/_//;~|
 *     |  \  o \_         ,XXXXX),                         _..-~ o /  |
 *     |    ~~\  ~-.     XXXXX`)))),                 _.--~~   .-~~~   |
 *      ~~~~~~~`\   ~\~~~XXX' _/ ';))     |~~~~~~..-~     _.-~ ~~~~~~~
 *               `\   ~~--`_\~\, ;;;\)__.---.~~~      _.-~
 *                 ~-.       `:;;/;; \          _..-~~
 *                    ~-._      `''        /-~-~
 *                        `\              /  /
 *                          |         ,   | |
 *                           |  '        /  |
 *                            \/;          |
 *                             ;;          |
 *                             `;   .       |
 *                             |~~~-----.....|
 *                            | \             \
 *                           | /\~~--...__    |
 *                           (|  `\       __-\|
 *                           ||    \_   /~    |
 *                           |)     \~-'      |
 *                            |      | \      '
 *                            |      |  \    :
 *                             \     |  |    |
 *                              |    )  (    )
 *                               \  /;  /\  |
 *                               |    |/   |
 *                               |    |   |
 *                                \  .'  ||
 *                                |  |  | |
 *                                (  | |  |
 *                                |   \ \ |
 *                                || o `.)|
 *                                |`\\) |
 *                                |       |
 *                                |       |
 */

#include <ArduinoJson.h>  
#include <LittleFS.h> 
#include <OneButton.h>
#include <PubSubClient.h>
#include <WiFiManager.h>

#define CONFIG_PATH "/config"

#ifdef DEBUG
#define DEBUG_INFO(fmt, ...)   printf_P(PSTR(fmt "\n") , ##__VA_ARGS__)
#else
#define DEBUG_INFO(fmt, ...)   // printf_P(PSTR(fmt "\n") , ##__VA_ARGS__)
#endif

/* 波特率接受缓存区 */
#define BAUD_RATE_MAX_LEN 10
#define DEFAULT_BAUD_RATE 115200
/* 重置引脚 */
#define RESET_PIN 0

/* MQTT 编译参数 */
#define MQTT_CHECK_INTERVAL 3  // 10s
#define MQTT_SERVER_MAX_LEN 40
#define MQTT_PORT_MAX_LEN 6
#define MQTT_USER_MAX_LEN 32
#define MQTT_PASSWORD_MAX_LEN 32
#define MQTT_TOPIC_MAX_LEN 128

char baud_rate[BAUD_RATE_MAX_LEN];
char mqtt_server[MQTT_SERVER_MAX_LEN];
char mqtt_port[MQTT_PORT_MAX_LEN];
char mqtt_user[MQTT_USER_MAX_LEN];
char mqtt_password[MQTT_PASSWORD_MAX_LEN];
char mqtt_send_topic[MQTT_TOPIC_MAX_LEN];
char mqtt_receive_topic[MQTT_TOPIC_MAX_LEN];

OneButton button(RESET_PIN, true);
static WiFiManager wm;
DynamicJsonDocument config(512);
WiFiClient espClient;
PubSubClient mqttClient(espClient);
bool save_params = false;
unsigned long last_check_mqtt;
unsigned long last_read_uart;

WiFiManagerParameter custom_baud_rate("baud_rate", "baud rate", baud_rate,
                                        sizeof(baud_rate));

WiFiManagerParameter custom_mqtt_server("server", "mqtt server", mqtt_server,
                                        sizeof(mqtt_server));
WiFiManagerParameter custom_mqtt_port("port", "mqtt port", mqtt_port,
                                      sizeof(mqtt_port));
WiFiManagerParameter custom_mqtt_user("user", "mqtt user", mqtt_user,
                                      sizeof(mqtt_user));
WiFiManagerParameter custom_mqtt_password("password", "mqtt password",
                                          mqtt_password, sizeof(mqtt_password));
WiFiManagerParameter custom_mqtt_send_topic("send_topic", "mqtt send topic",
                                            mqtt_send_topic,
                                            sizeof(mqtt_send_topic));
WiFiManagerParameter custom_mqtt_receive_topic("receive_topic",
                                               "mqtt receive topic",
                                               mqtt_receive_topic,
                                               sizeof(mqtt_receive_topic));

bool load_config() {
  File cache = LittleFS.open(CONFIG_PATH, "r");
  if (!cache) {
    return false;
  }
  if (cache.size() > 0) {
    DeserializationError error = deserializeJson(config, cache);
    if (error) {
      return false;
    }
    cache.close();
  }
  return true;
}

bool save_config() {
  File cache = LittleFS.open(CONFIG_PATH, "w");
  if (!cache) {
    return false;
  }
  serializeJson(config, cache);
  cache.close();
  return true;
}

void read_uart() {
  char buffer[512];
  int len = 0;
  bool has_data = false;
  while (Serial.available()) {
    int size = Serial.available();
    Serial.readBytes(buffer + len, size);
    len += size;
    has_data = true;
  }

  if (has_data) {
    buffer[len] = '\0';
    const char *topic = config["send_topic"];
    mqttClient.publish(topic, buffer, len);
  }
}

void mqtt_callback(char *topic, byte *payload, unsigned int length) {
  String msg;
  for (unsigned int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }
  Serial.print(msg);
  // Serial1.print(msg);
}

void mqtt_set_server() {
  const char *server = config["server"];
  uint16_t port = (int)config["port"];
  mqttClient.setServer(server, port);
  mqttClient.setCallback(mqtt_callback);
}

void mqtt_network_check() {
  if (!mqttClient.connected()) {
    mqttClient.disconnect();
    DEBUG_INFO("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";
    clientId += String(ESP.getChipId(), HEX);
    const char *user = config["user"];
    const char *password = config["password"];
    if (mqttClient.connect(clientId.c_str(), user, password)) {
      DEBUG_INFO("connected");
      mqttClient.subscribe(config["receive_topic"]);
    } else {
      DEBUG_INFO("failed, rc=%d\n", mqttClient.state());
    }
  }
}

void setup() {
  LittleFS.begin();
  if (load_config()) {
    DEBUG_INFO("load config success\n");
    String baud_rate = config["baud_rate"];
    String server = config["server"];
    String port = config["port"];
    String user = config["user"];
    String password = config["password"];
    String send_topic = config["send_topic"];
    String receive_topic = config["receive_topic"];
    if (!baud_rate.isEmpty())
    custom_baud_rate.setValue(baud_rate.c_str(), baud_rate.length());
    if (!server.isEmpty())
    custom_mqtt_server.setValue(server.c_str(), server.length());
    if (!port.isEmpty())
    custom_mqtt_port.setValue(port.c_str(), port.length());
    if (!user.isEmpty())
    custom_mqtt_user.setValue(user.c_str(), user.length());
    if (!password.isEmpty())
    custom_mqtt_password.setValue(password.c_str(), password.length());
    if (!send_topic.isEmpty())
    custom_mqtt_send_topic.setValue(send_topic.c_str(), send_topic.length());
    if (!receive_topic.isEmpty())
    custom_mqtt_receive_topic.setValue(receive_topic.c_str(), receive_topic.length());
  } else {
    DEBUG_INFO("Load config failed\n");
  }
  int baud_rate = (int)config["baud_rate"];
  if (baud_rate > 0) {
    Serial.begin(baud_rate);
    Serial1.begin(baud_rate);
  } else {
    Serial.begin(115200);
    Serial1.begin(115200);
  }
  DEBUG_INFO();
  button.attachDuringLongPress([]() {
    DEBUG_INFO("erase wifi config");
    wm.erase();
    LittleFS.format();
    delay(1000);
    ESP.restart();
  });

  wm.setDebugOutput(false);
  wm.addParameter(&custom_baud_rate);
  wm.addParameter(&custom_mqtt_server);
  wm.addParameter(&custom_mqtt_port);
  wm.addParameter(&custom_mqtt_user);
  wm.addParameter(&custom_mqtt_password);
  wm.addParameter(&custom_mqtt_send_topic);
  wm.addParameter(&custom_mqtt_receive_topic);
  wm.setSaveParamsCallback([]() { 
    save_params = true; 
    });
  if (!wm.autoConnect()) {
    DEBUG_INFO("failed to connect and hit timeout");
    delay(3000);
    ESP.restart();
  }
  WiFi.mode(WIFI_STA);

  wm.startWebPortal();
  mqtt_set_server();
}

void loop() {
  wm.process();
  button.tick();
  mqttClient.loop();
  if ((millis() - last_check_mqtt) / 1000 > MQTT_CHECK_INTERVAL) {
    mqtt_network_check();
    last_check_mqtt = millis();
  }
  if (mqttClient.connected() && (millis() - last_read_uart) > 200) {
    read_uart();
    last_read_uart = millis();
  }
  if (save_params) {
    config["baud_rate"] = custom_baud_rate.getValue();
    config["server"] = custom_mqtt_server.getValue();
    config["port"] = custom_mqtt_port.getValue();
    config["user"] = custom_mqtt_user.getValue();
    config["password"] = custom_mqtt_password.getValue();
    config["send_topic"] = custom_mqtt_send_topic.getValue();
    config["receive_topic"] = custom_mqtt_receive_topic.getValue();
    save_config();
    save_params = false;
    ESP.restart();
  }
}