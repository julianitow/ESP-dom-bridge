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

#define mqtt_server "share.alesia-julianitow.ovh"
#define temperature_topic "sensor/temperature"
#define humidity_topic "sensor/humidity"
#define relay_set_on_topic "relay/set/on"
#define relay_get_on_topic "relay/get/on"

#endif