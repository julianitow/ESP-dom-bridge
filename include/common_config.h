#ifndef COMMON_CONFIG_H
#define COMMON_CONFIG_H

#define NTP_SERVERS "pool.ntp.org", "time.nist.gov"
#define UNIX_TIME_NOV_13_2017 1510592825

#define PST_TIME_ZONE -8
#define PST_TIME_ZONE_DAYLIGHT_SAVINGS_DIFF   1

#define GMT_OFFSET_SECS (PST_TIME_ZONE * 3600)
#define GMT_OFFSET_SECS_DST ((PST_TIME_ZONE + PST_TIME_ZONE_DAYLIGHT_SAVINGS_DIFF) * 3600)

#define SERIAL_LOGGER_BAUD_RATE 115200

#define ESP_RECV_PIN D2
#define ESP_EMIT_PIN D6

#define HOSTNAME "ESP_BRIDGE"

#define MQTT_HOST "share.alesia-julianitow.ovh"
#define MQTT_USERNAME "soulpot"
#define MQTT_PASSWORD "soulpot"
#define MQTT_CLIENT_ID "ESP_BRIDGE"
#define TEMP_TOPIC "sensor/temperature"
#define HUM_TOPIC "sensor/humidity"
#define RELAY_TOPIC "relay/get/on"
#define RELAY_SET_TOPIC "relay/set/on"

#endif