#pragma once

#include <WiFi.h>
#include <DNSServer.h>

class WifiPortal {
public:
    WifiPortal();
    void begin(const char* ssid = "GB-PokeTrader", const char* password = NULL);
    void process();
    IPAddress getIP() const { return _ap_ip; }

private:
    DNSServer _dns_server;
    IPAddress _ap_ip;
    const byte DNS_PORT = 53;
};

extern WifiPortal Portal;
