#include "wifi_portal.h"

WifiPortal Portal;

WifiPortal::WifiPortal()
    : _ap_ip(192, 168, 4, 1)
{
}

void WifiPortal::begin(const char* ssid, const char* password) {
    WiFi.mode(WIFI_AP);
    IPAddress subnet(255, 255, 255, 0);

    WiFi.softAPConfig(_ap_ip, _ap_ip, subnet);
    if (password && strlen(password) >= 8) {
        WiFi.softAP(ssid, password);
    } else {
        WiFi.softAP(ssid);
    }

    // Capture all DNS requests ('*') and route to local Web Server IP (Captive Portal)
    _dns_server.setErrorReplyCode(DNSReplyCode::NoError);
    _dns_server.start(DNS_PORT, "*", _ap_ip);
}

void WifiPortal::process() {
    _dns_server.processNextRequest();
}
