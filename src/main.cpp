#include "./config.hpp"
#include "./struct/packet.hpp"
#include "./struct/translate.hpp"
#include "./utils/broadcast.hpp"
#include <Arduino.h>

#if LED_BOARD == 1
#include "./handler/led/led_handler.hpp"
#include "./handler/led/motor_handler.hpp"
#include "./translate/led_translate.hpp"

LedHanlder led;
MotorHanlder motor;
LedTranslate translate;
#elif CONTROL_BOARD == 1
#include "./service/alarm.hpp"
#include "./service/lcd.hpp"
#include "./service/motor_interval.hpp"
#include "./service/water_level.hpp"
#include "./translate/control_translate.hpp"
#include <ArduinoJson.h>
#include <ESPmDNS.h>
#include <WebServer.h>
#include <WebSocketsClient.h>
#include <WiFi.h>
#include <WiFiClient.h>

WebServer server(HTTP_SERVER_PORT);
ControlTranslate translate;
Alarm ledAlarm;
LCD lcd;
MotorInterval motorInterval;
WaterLevel waterLevel;
void notFound() {
	server.send(404, "text/html", "not found");
}
#endif
void setup() {
	translate.begin();
#if LED_BOARD == 1
	auto instance = Broadcast<device_communication_message_t>::getInstance();
	instance->add(&led);
	instance->add(&motor);
	ESP_LOGI(typename(this), "Start");
#elif CONTROL_BOARD == 1
	WiFi.mode(WIFI_STA);
	WiFi.begin(SSID, PASSWORD);
	while (WiFi.status() != WL_CONNECTED) {
		delay(100);
	}
	ESP_LOGI(typename(this), "IP Address : %s ", WiFi.localIP().toString().c_str());
	ESP_LOGI(typename(this), "Mac Address : %s", WiFi.macAddress().c_str());

	MDNS.begin("ineco");

	server.onNotFound(notFound);

	server.on("/led/{}/{}/{}", HTTP_GET, []() {
		ESP_LOGI("http", "%s", server.uri().c_str());
		const String yStr = server.pathArg(0);
		const String xStr = server.pathArg(1);
		const String colorStr = server.pathArg(2);

		const uint8_t y = static_cast<uint8_t>(atoi(yStr.c_str()));
		const uint8_t x = static_cast<uint8_t>(atoi(xStr.c_str()));
		const uint32_t color = static_cast<uint32_t>(atoll(colorStr.c_str()));


		service_signal_t signal;
		communcation_service_signal_t com;
		led_message_t *ledMessage = new led_message_t;
		memset(ledMessage->message.bytes, 0x00, sizeof(led_message_t));
		auto broadcast = Broadcast<service_signal_t>::getInstance();

		ledMessage->row = y;
		ledMessage->col = x;

		auto bright = static_cast<uint8_t>((color >> 24) & 0xff);
		ledMessage->color.bright = map(bright, 0, UINT8_MAX, 0, 100);
		ledMessage->color.r = static_cast<uint8_t>((color >> 16) & 0xff);
		ledMessage->color.g = static_cast<uint8_t>((color >> 8) & 0xff);
		ledMessage->color.b = static_cast<uint8_t>((color & 0xff));

		com.dir = MESSAGE_DIRECTION::TO_SLAVE;
		com.type = MESSAGE_TYPE::SET_COLOR;
		com.message = &ledMessage->message;

		signal.value = com.value;
		signal.type = SERVICE_SIGNAL_TYPE::PACKET_SEND;

		broadcast->broadcast(signal);

		DynamicJsonDocument json(256);
		json["x"] = x;
		json["y"] = y;
    json["color"]["r"] = ledMessage->color.r;
    json["color"]["g"] = ledMessage->color.g;
    json["color"]["b"] = ledMessage->color.b;
    json["color"]["bright"] = ledMessage->color.bright;
		delete ledMessage;

		size_t len = measureJson(json);
		char *buffer = new char[len + 32];
		serializeJson(json, buffer, len + 1);
		server.send(200, "text/html", buffer);
		json.clear();
    json.shrinkToFit();
    json.garbageCollect();
		delete[] buffer;
	});

	server.on("/mac", HTTP_GET, []() {
		server.send(200, "text/html", WiFi.macAddress());
	});

	server.on("/air", HTTP_GET, []() {
		server.send(200, "text/html", server.uri());
		motorInterval.removeAir();
	});

	server.on("/init", HTTP_GET, []() {
		service_signal_t signal;
		communcation_service_signal_t com;
		motor_message_t *motorMessage = new motor_message_t;
		memset(motorMessage->message.bytes, 0x00, sizeof(motor_message_t));
		auto broadcast = Broadcast<service_signal_t>::getInstance();
		com.dir = MESSAGE_DIRECTION::TO_SLAVE;
		com.type = MESSAGE_TYPE::MODULE_RESTART;
		com.message = &motorMessage->message;
		signal.value = com.value;
		signal.type = SERVICE_SIGNAL_TYPE::PACKET_SEND;
		broadcast->broadcast(signal);
		delete motorMessage;
		server.send(200, "text/html", server.uri());
	});

	server.on("/restart", HTTP_GET, []() {
		server.send(200, "text/html", server.uri());
		ESP.restart();
	});

	server.on("/motor", HTTP_GET, []() {
		server.send(200, "text/html", MOTOR_SET_HTML);
	});

	server.on("/motor/power/{}", HTTP_GET, []() {
		const String onOff = server.pathArg(0);
		service_signal_t signal;
		motor_interval_service_signal_t message;
		message.isIntervalSet = false;
		message.onOff = onOff.equalsIgnoreCase("on");
		signal.type = SERVICE_SIGNAL_TYPE::MOTOR_INTERVAL_SET;
		signal.value = message.value;
		Broadcast<service_signal_t>::getInstance()->broadcast(signal);
		server.send(200, "text/html", String(message.onOff));
	});

	server.on("/motor/set/enable/{}/time/{}/span/{}", HTTP_GET, []() {
		const String enable = server.pathArg(0);
		const String time = server.pathArg(1);
		const String span = server.pathArg(2);
		service_signal_t signal;
		motor_interval_service_signal_t message;
		message.isIntervalSet = true;
		message.intervalEnable = enable.equalsIgnoreCase("on");
		message.intervalSpan = static_cast<uint16_t>(atoi(span.c_str()));
		message.intervalTime = static_cast<uint16_t>(atoi(time.c_str()));
		signal.type = SERVICE_SIGNAL_TYPE::MOTOR_INTERVAL_SET;
		signal.value = message.value;
		Broadcast<service_signal_t>::getInstance()->broadcast(signal);
		server.send(200, "text/plain", enable);
	});

	server.begin();
	lcd.execute();

	auto instance = Broadcast<service_signal_t>::getInstance();
	instance->add(&ledAlarm);
	instance->add(&waterLevel);
	instance->add(&translate);
	instance->add(&motorInterval);
	instance->add(&lcd);
#endif
}

void loop() {
	translate.recv();
#if CONTROL_BOARD == 1
	server.handleClient();
	waterLevel.execute();
	motorInterval.execute();
#endif
}