#include <Ethernet.h>
#include <EthernetUdp.h>
#define UDP_TX_PACKET_MAX_SIZE 546
#define PAYLOAD_SIZE 518

byte FlexMAC[] = {0x00, 0x1C, 0x2D, 0x05, 0x11, 0x10};    // Frisco 6400 MAC address
IPAddress FlexIP(67, 198, 99, 60);                       // Frisco 6400 IP address
IPAddress FlexRouterIP(67, 198, 99, 60);                       // Frisco Gateway IP Address

byte ArduinoMAC[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ArduinoIP(192,168,1,213);

IPAddress Brodcast (255,255,255,255);
unsigned int DiscoveryPort = 4992;      // local port to listen on

int connectedPin = 10;

// Protocol Definition
byte header[4] ={0x38, 0x57, 0x00, 0x88};
byte stream_id[4] = {0x00, 0x00, 0x08, 0x00};
byte class_id[8] = {0x00, 0x00, 0x1C, 0x2D, 0x53, 0x4C, 0xFF, 0xFF};
byte timestamp_int[4] = {0x65, 0x56, 0x3F,0xBF};
byte timestamp_frac[8] = {0x00, 0x00,0x00,0x00,0x00,0x00,0x00,0x00};
/*
char payload[PAYLOAD_SIZE] = "discovery_protocol_version=3.0.0.2 model=FLEX-6400 serial=2019-2544-6400-6026 version=3.5.9.22963 nickname=Locale callsign=I8NHJ
ip=67.198.99.60 port=4992 status=Available inuse_ip= inuse_host= max_licensed_version=v3 radio_license_id=00-1C-2D-05-11-10 requires_additional_license=0 fpc_mac= wan_connected=1 licensed_clients=2 available_clients=2 max_panadapters=2 available_panadapters=2 max_slices=2 available_slices=2 gui_client_ips= gui_client_hosts= gui_client_programs= gui_client_stations= gui_client_handles= ";
*/
char ST0_payload_protocol_version[] = "discovery_protocol_version=3.0.0.2 ";
char ST0_payload_model[] = "model=FLEX-6400 ";
char ST0_payload_serial[] = "serial=2019-2544-6400-6026 ";
char ST0_payload_version[] = "version=3.5.9.22963 ";
char ST0_payload_nick[] = "nickname=Frisco callsign=N5NHJ ";
char ST0_payload_ip[] = "ip=67.198.99.60 port=4992 ";
char ST0_payload_status[] = "status=Available ";
char ST0_payload_inuse_ip[] = "inuse_ip= ";
char ST0_payload_inuse_host[] = "inuse_host= ";
char ST0_payload_max_license[] = "max_licensed_version=v3 radio_license_id=00-1C-2D-05-11-10 requires_additional_license=0 ";
char ST0_payload_fpc_mac[] = "fpc_mac= ";
char ST0_payload_wan_connected[] = "wan_connected=1 ";
char ST0_payload_licensed_clients[] = "licensed_clients=2 ";
char ST0_payload_available_clients[] = "available_clients=2 ";
char ST0_payload_max_panadapters[] = "max_panadapters=2 ";
char ST0_payload_available_panadapters[] = "available_panadapters=2 ";
char ST0_payload_max_slices[] = "max_slices=2 ";
char ST0_payload_available_slices[] = "available_slices=2 ";
char ST0_payload_gui_client_ips[] = "gui_client_ips= ";
char ST0_payload_gui_client_hosts[] = "gui_client_hosts= ";
char ST0_payload_gui_client_programs[] = "gui_client_programs= ";
char ST0_payload_gui_client_stations[] = "gui_client_stations= ";
char ST0_payload_gui_client_handles[] = "gui_client_handles= ";

char ST1_payload_protocol_version[] = "discovery_protocol_version=3.0.0.2 ";
char ST1_payload_model[] = "model=FLEX-6400 ";
char ST1_payload_serial[] = "serial=2019-2544-6400-6026 ";
char ST1_payload_version[] = "version=3.5.9.22963 ";
char ST1_payload_nick[] = "nickname=Frisco callsign=N5NHJ ";
char ST1_payload_ip[] = "ip=67.198.99.60 port=4992 ";
char ST1_payload_status[] = "status=Available ";
char ST1_payload_inuse_ip[] = "inuse_ip= XXX.YYY.WWW.ZZZ ";
char ST1_payload_inuse_host[] = "inuse_host= XXX.YYY.WWW.ZZZ ";
char ST1_payload_max_license[] = "max_licensed_version=v3 radio_license_id=00-1C-2D-05-11-10 requires_additional_license=0 ";
char ST1_payload_fpc_mac[] = "fpc_mac= ";
char ST1_payload_wan_connected[] = "wan_connected=1 ";
char ST1_payload_licensed_clients[] = "licensed_clients=2 ";
char ST1_payload_available_clients[] = "available_clients=1 ";
char ST1_payload_max_panadapters[] = "max_panadapters=2 ";
char ST1_payload_available_panadapters[] = "available_panadapters=1 ";
char ST1_payload_max_slices[] = "max_slices=2 ";
char ST1_payload_available_slices[] = "available_slices=1 ";
char ST1_payload_gui_client_ips[] = "gui_client_ips=XXX.YYY.WWW.ZZZ ";
char ST1_payload_gui_client_hosts[] = "gui_client_hosts=XXX.YYY.WWW.ZZZ ";
char ST1_payload_gui_client_programs[] = "gui_client_programs=SmartSDR-Win ";
char ST1_payload_gui_client_stations[] = "gui_client_stations=MAC-N5NHJ-WIN ";
char ST1_payload_gui_client_handles[] = "gui_client_handles= 0x3602E5F5";

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

void setup() {
  // You can use Ethernet.init(pin) to configure the CS pin
  Ethernet.init(10);  // Most Arduino shields

  // start the Ethernet
  Ethernet.begin(ArduinoMAC, ArduinoIP);

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }
  // start UDP
  Udp.begin(DiscoveryPort);
  // Connection Switch
  pinMode(connectedPin, INPUT_PULLUP);

}

void loop() {
  Udp.beginPacket(Brodcast, DiscoveryPort);
  Udp.write(header, sizeof(header));
  Udp.write(stream_id, sizeof(stream_id));
  Udp.write(class_id, sizeof(class_id));
  Udp.write(timestamp_int, sizeof(timestamp_int));
  Udp.write(timestamp_frac, sizeof(timestamp_frac));
  // PAYLOAD
  if (digitalRead(connectedPin)) {
    Udp.write(ST1_payload_protocol_version,       sizeof(ST1_payload_protocol_version));
    Udp.write(ST1_payload_model,                  sizeof(ST1_payload_model));
    Udp.write(ST1_payload_serial,                 sizeof(ST1_payload_serial));
    Udp.write(ST1_payload_version,                sizeof(ST1_payload_version));
    Udp.write(ST1_payload_nick,                   sizeof(ST1_payload_nick));
    Udp.write(ST1_payload_ip,                     sizeof(ST1_payload_ip));
    Udp.write(ST1_payload_status,                 sizeof(ST1_payload_status));
    Udp.write(ST1_payload_inuse_ip,               sizeof(ST1_payload_inuse_ip));
    Udp.write(ST1_payload_inuse_host,             sizeof(ST1_payload_inuse_host));
    Udp.write(ST1_payload_max_license,            sizeof(ST1_payload_max_license));
    Udp.write(ST1_payload_fpc_mac,                sizeof(ST1_payload_fpc_mac));
    Udp.write(ST1_payload_wan_connected,          sizeof(ST1_payload_wan_connected));
    Udp.write(ST1_payload_licensed_clients,       sizeof(ST1_payload_licensed_clients));
    Udp.write(ST1_payload_available_clients,      sizeof(ST1_payload_available_clients));
    Udp.write(ST1_payload_max_panadapters,        sizeof(ST1_payload_max_panadapters));
    Udp.write(ST1_payload_available_panadapters,  sizeof(ST1_payload_available_panadapters));
    Udp.write(ST1_payload_max_slices,             sizeof(ST1_payload_max_slices));
    Udp.write(ST1_payload_available_slices,       sizeof(ST1_payload_available_slices));
    Udp.write(ST1_payload_gui_client_ips,         sizeof(ST1_payload_gui_client_ips));
    Udp.write(ST1_payload_gui_client_hosts,       sizeof(ST1_payload_gui_client_hosts));
    Udp.write(ST1_payload_gui_client_programs,    sizeof(ST1_payload_gui_client_programs));
    Udp.write(ST1_payload_gui_client_stations,    sizeof(ST1_payload_gui_client_stations));
    Udp.write(ST1_payload_gui_client_handles,     sizeof(ST1_payload_gui_client_handles));
  }
  else {
    Udp.write(ST0_payload_protocol_version,       sizeof(ST0_payload_protocol_version));
    Udp.write(ST0_payload_model,                  sizeof(ST0_payload_model));
    Udp.write(ST0_payload_serial,                 sizeof(ST0_payload_serial));
    Udp.write(ST0_payload_version,                sizeof(ST0_payload_version));
    Udp.write(ST0_payload_nick,                   sizeof(ST0_payload_nick));
    Udp.write(ST0_payload_ip,                     sizeof(ST0_payload_ip));
    Udp.write(ST0_payload_status,                 sizeof(ST0_payload_status));
    Udp.write(ST0_payload_inuse_ip,               sizeof(ST0_payload_inuse_ip));
    Udp.write(ST0_payload_inuse_host,             sizeof(ST0_payload_inuse_host));
    Udp.write(ST0_payload_max_license,            sizeof(ST0_payload_max_license));
    Udp.write(ST0_payload_fpc_mac,                sizeof(ST0_payload_fpc_mac));
    Udp.write(ST0_payload_wan_connected,          sizeof(ST0_payload_wan_connected));
    Udp.write(ST0_payload_licensed_clients,       sizeof(ST0_payload_licensed_clients));
    Udp.write(ST0_payload_available_clients,      sizeof(ST0_payload_available_clients));
    Udp.write(ST0_payload_max_panadapters,        sizeof(ST0_payload_max_panadapters));
    Udp.write(ST0_payload_available_panadapters,  sizeof(ST0_payload_available_panadapters));
    Udp.write(ST0_payload_max_slices,             sizeof(ST0_payload_max_slices));
    Udp.write(ST0_payload_available_slices,       sizeof(ST0_payload_available_slices));
    Udp.write(ST0_payload_gui_client_ips,         sizeof(ST0_payload_gui_client_ips));
    Udp.write(ST0_payload_gui_client_hosts,       sizeof(ST0_payload_gui_client_hosts));
    Udp.write(ST0_payload_gui_client_programs,    sizeof(ST0_payload_gui_client_programs));
    Udp.write(ST0_payload_gui_client_stations,    sizeof(ST0_payload_gui_client_stations));
    Udp.write(ST0_payload_gui_client_handles,     sizeof(ST0_payload_gui_client_handles));
  }
  Serial.println("Discovery Sent.");
  Udp.endPacket();
  delay(1000);
}

