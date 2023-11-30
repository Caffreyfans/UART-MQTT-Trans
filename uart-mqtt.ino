#include "stdlib.h"
#include "string.h"
#include "LittleFS.h"
#include "ArduinoJson.h"
#include "OneButton.h"
#include "PubSubClient.h"
#include "WiFiManager.h"


// 存放配置
#define CONFIG_PATH "/config"

// 串口波特率，这里只能9600 其他波特率测试会出现接收不全情况
// 8N1
#define DEFAULT_BAUD_RATE 9600
// 重置引脚号 长按3s会重置 注意串口输出打印信息
#define FLASH_PIN 0

/* MQTT 编译参数 */
// 规定最大长度
#define MQTT_CHECK_INTERVAL 3
#define MQTT_SERVER_MAX_LEN 128
#define MQTT_PORT_MAX_LEN 6
#define MQTT_USER_MAX_LEN 128
#define MQTT_PASSWORD_MAX_LEN 128
#define MQTT_TOPIC_MAX_LEN 128
#define MQTT_CLIENT_ID_LEN 128

// 开辟数组、存储配置
char mqtt_server[MQTT_SERVER_MAX_LEN];
char mqtt_port[MQTT_PORT_MAX_LEN];
char mqtt_user[MQTT_USER_MAX_LEN];
char mqtt_password[MQTT_PASSWORD_MAX_LEN];
char mqtt_send_topic[MQTT_TOPIC_MAX_LEN];
char mqtt_receive_topic[MQTT_TOPIC_MAX_LEN];
char mqtt_client_id[MQTT_TOPIC_MAX_LEN];

// 按键配置
OneButton button(FLASH_PIN, true);

// WIFI MQTT 配置(一般不用管)
static WiFiManager wm;
DynamicJsonDocument config(512);
WiFiClient espClient;
PubSubClient mqttClient(espClient);
bool save_params = false;
unsigned long last_check_mqtt;
unsigned long last_read_uart;

// 串口发送的结束符
String end_flag = "uart-mqtt";
// MQTT最终发送字符串
String uart_buffer = "";

// 配置页面的显示
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
WiFiManagerParameter custom_mqtt_receive_id("mqtt_client_id",
                                               "mqtt client id",
                                               mqtt_client_id,
                                               sizeof(mqtt_client_id));

// 读取配置
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

// 保存配置
bool save_config() {
  File cache = LittleFS.open(CONFIG_PATH, "w");
  if (!cache) {
    return false;
  }
  serializeJson(config, cache);
  cache.close();
  return true;
}


// 读取串口字符串
void read_uart() {
  // 拆分字符串发送(一次发送的话，大文本会出现问题)
  int cut = 128;
  while (Serial.available() > 0)  
  {
      uart_buffer += char(Serial.read());
  }
  if (uart_buffer.length() > 0 && uart_buffer.endsWith(end_flag))
  {
    uart_buffer = uart_buffer.substring(0,uart_buffer.length()-end_flag.length());
    const char *topic = config["send_topic"];
    int json_str_len = uart_buffer.length();
    const char *MQTT_PUB_SELF = topic;
    if (json_str_len > cut)
    {
      //开始发送长文件参数分别为  主题，长度，是否持续
      mqttClient.beginPublish(MQTT_PUB_SELF, json_str_len, true );
      int count = json_str_len / cut;
      for (int i = 0; i < (count-1); i++)
      {
          mqttClient.print(uart_buffer.substring(i * cut, (i * cut + cut)));
      }
      mqttClient.print(uart_buffer.substring(cut * (count - 1)));
      //结束发送文本
      mqttClient.endPublish();
    }
    else
    {
        mqttClient.publish(MQTT_PUB_SELF, uart_buffer.c_str());
    }
    uart_buffer = "";
  }

}

// mqtt接收之后处理
void mqtt_callback(char *topic, byte *payload, unsigned int length) {
  String msg;
  for (unsigned int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }
  Serial.print(msg);
}


// MQTT服务器、端口、回调
void mqtt_set_server() {
  const char *server = config["server"];
  uint16_t port = (int)config["port"];
  mqttClient.setServer(server, port);
  mqttClient.setCallback(mqtt_callback);
}



// 连接MQTT
void mqtt_network_check() {
  if (!mqttClient.connected()) {
    mqttClient.disconnect();
    Serial.println("Attempting MQTT connection...");
    String clientId = config["mqtt_client_id"];
    const char *user = config["user"];
    const char *password = config["password"];
    if (mqttClient.connect(clientId.c_str(), user, password)) {
      Serial.println("connected");
      mqttClient.subscribe(config["receive_topic"]);
    } else {
      Serial.printf("failed, rc=%d\r\n", mqttClient.state());
    }
  }
}


// 初始化
// 初始化信息会通过串口打印
// 长按3s flash按钮清空配置
void setup() {
  LittleFS.begin();
  if (load_config()) {
    Serial.println("load config success\n");
    String server = config["server"];
    String port = config["port"];
    String user = config["user"];
    String password = config["password"];
    String send_topic = config["send_topic"];
    String receive_topic = config["receive_topic"];
    String mqtt_client_id_string = config["mqtt_client_id"];
    if (!server.isEmpty())
    custom_mqtt_server.setValue(server.c_str(), MQTT_SERVER_MAX_LEN );
    if (!port.isEmpty())
    custom_mqtt_port.setValue(port.c_str(), MQTT_PORT_MAX_LEN );
    if (!user.isEmpty())
    custom_mqtt_user.setValue(user.c_str(), MQTT_USER_MAX_LEN );
    if (!password.isEmpty())
    custom_mqtt_password.setValue(password.c_str(),MQTT_PASSWORD_MAX_LEN );
    if (!send_topic.isEmpty())
    custom_mqtt_send_topic.setValue(send_topic.c_str(), MQTT_TOPIC_MAX_LEN );
    if (!receive_topic.isEmpty())
    custom_mqtt_receive_topic.setValue(receive_topic.c_str(), MQTT_TOPIC_MAX_LEN );
    if (!mqtt_client_id_string.isEmpty())
    custom_mqtt_receive_id.setValue(mqtt_client_id_string.c_str(), MQTT_CLIENT_ID_LEN );
  } else {
    Serial.println("Load config failed\n");
  }
    Serial.begin(DEFAULT_BAUD_RATE);
    Serial1.begin(DEFAULT_BAUD_RATE);
  button.attachDuringLongPress([]() {
    Serial.println("erase wifi config");
    delay(2000);
    wm.erase();
    LittleFS.format();
    delay(1000);
    ESP.restart();
  });
  wm.setDebugOutput(false);
  wm.addParameter(&custom_mqtt_server);
  wm.addParameter(&custom_mqtt_port);
  wm.addParameter(&custom_mqtt_user);
  wm.addParameter(&custom_mqtt_password);
  wm.addParameter(&custom_mqtt_send_topic);
  wm.addParameter(&custom_mqtt_receive_topic);
  wm.addParameter(&custom_mqtt_receive_id);
  wm.setSaveParamsCallback([]() { 
    save_params = true; 
    });
  if (!wm.autoConnect()) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    ESP.restart();
  }
  WiFi.mode(WIFI_STA);
  wm.startWebPortal();
  mqtt_set_server();

  // WiFi连接成功后将通过串口监视器输出连接成功信息 
  Serial.println(""); 
  Serial.print("ESP8266 Connected to ");
  Serial.println(WiFi.SSID());              // WiFi名称
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());  
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
    // config["baud_rate"] = custom_baud_rate.getValue();
    config["server"] = custom_mqtt_server.getValue();
    config["port"] = custom_mqtt_port.getValue();
    config["user"] = custom_mqtt_user.getValue();
    config["password"] = custom_mqtt_password.getValue();
    config["send_topic"] = custom_mqtt_send_topic.getValue();
    config["receive_topic"] = custom_mqtt_receive_topic.getValue();
    config["mqtt_client_id"] = custom_mqtt_receive_id.getValue();
    save_config();
    save_params = false;
    ESP.restart();
  }
}