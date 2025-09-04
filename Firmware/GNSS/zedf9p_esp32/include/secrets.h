//Your WiFi credentials
char WIFI_SSID[] = "Snower-2.4GHz";
char WIFI_PASSWORD[] = "";
IPAddress agent_ip(192, 168, 1, 102);
size_t agent_port = 8889;

//RTK2Go works well and is free
const char casterHost[] = "rtk2go.com"; 
const uint16_t casterPort = 2101;
const char casterUser[] = "lapinamk-at-gmail-d-com"; //User must provide their own email address to use RTK2Go
const char casterUserPW[] = "none";
const char mountPoint[] = "Taroniemi"; //The mount point you want to get data from
