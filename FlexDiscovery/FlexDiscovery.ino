#include <Ethernet.h>
#include <EthernetUdp.h>
#define UDP_TX_PACKET_MAX_SIZE 546
#define PAYLOAD_SIZE 518

byte FlexMAC[] = {0x00, 0x1C, 0x2D, 0x05, 0x11, 0x10};    // Frisco 6400 MAC address
IPAddress FlexIP(67, 198, 99, 60);                        // Frisco 6400 IP address
IPAddress FlexRouterIP(67, 198, 99, 60);                  // Frisco Gateway IP Address

byte ArduinoMAC[] = {0x00, 0x1C, 0x2D, 0x05, 0x11, 0x11};
IPAddress ArduinoIP(192,168,1,213);
// IPAddress ArduinoIP(10,19,8,73);
IPAddress Brodcast (255,255,255,255);
unsigned int DiscoveryPort = 4992;      // local port to listen on

int connectedPin = 11;

// Protocol Definition
byte header[2] = {0x38, 0x57};
uint8_t packet_size_hi = 0x00;
uint8_t packet_size_di_lo;
uint8_t packet_size_co_lo;
byte stream_id[4] = {0x00, 0x00, 0x08, 0x00};
byte class_id[8] = {0x00, 0x00, 0x1C, 0x2D, 0x53, 0x4C, 0xFF, 0xFF};
byte timestamp_int[4] = {0x65, 0x56, 0x3F,0xBF};
byte timestamp_frac[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // = 7 words

// /*
char payload[] = 
"discovery_protocol_version=3.0.0.2?model=FLEX-6400?serial=2019-2544-6400-6026?version=3.5.9.22963?nickname=Frisco?callsign=N5NHJ?ip=10.19.8.53?port=4992?status=Available?inuse_ip=?inuse_host=?max_licensed_version=v3?radio_license_id=00-1C-2D-05-11-10?requires_additional_license=0?fpc_mac=?wan_connected=1?licensed_clients=2?available_clients=2?max_panadapters=2?available_panadapters=2?max_slices=2?available_slices=2?gui_client_ips=?gui_client_hosts=?gui_client_programs=?gui_client_stations=?gui_client_handles=?";
// */

char ST0_payload_protocol_version[]             = "discovery_protocol_version=3.0.0.2 ";
char ST0_payload_model[]                        = "model=FLEX-6400 ";
char ST0_payload_serial[]                       = "serial=2019-2544-6400-6026 ";
char ST0_payload_version[]                      = "version=3.5.9.22963 ";
char ST0_payload_nick[]                         = "nickname=Frisco ";
char ST0_payload_callsign[]                     = "callsign=N5NHJ ";
char ST0_payload_ip[]                           = "ip=67.198.99.60  ";
char ST0_payload_port[]                         = "port=4992 ";
char ST0_payload_status[]                       = "status=Available ";
char ST0_payload_inuse_ip[]                     = "inuse_ip= ";
char ST0_payload_inuse_host[]                   = "inuse_host= ";
char ST0_payload_max_license[]                  = "max_licensed_version=v3 ";
char ST0_payload_radio_license[]                = "radio_license_id=00-1C-2D-05-11-10 ";
char ST0_payload_requires_additional_license[]  = "requires_additional_license=0 ";
char ST0_payload_fpc_mac[]                      = "fpc_mac= ";
char ST0_payload_wan_connected[]                = "wan_connected=1 ";
char ST0_payload_licensed_clients[]             = "licensed_clients=2 ";
char ST0_payload_available_clients[]            = "available_clients=2 ";
char ST0_payload_max_panadapters[]              = "max_panadapters=2 ";
char ST0_payload_available_panadapters[]        = "available_panadapters=2 ";
char ST0_payload_max_slices[]                   = "max_slices=2 ";
char ST0_payload_available_slices[]             = "available_slices=2 ";
char ST0_payload_gui_client_ips[]               = "gui_client_ips= ";
char ST0_payload_gui_client_hosts[]             = "gui_client_hosts= ";
char ST0_payload_gui_client_programs[]          = "gui_client_programs= ";
char ST0_payload_gui_client_stations[]          = "gui_client_stations= ";
char ST0_payload_gui_client_handles[]           = "gui_client_handles= ";

char ST1_payload_protocol_version[]             = "discovery_protocol_version=3.0.0.2 ";
char ST1_payload_model[]                        = "model=FLEX-6400 ";
char ST1_payload_serial[]                       = "serial=2019-2544-6400-6026 ";
char ST1_payload_version[]                      = "version=3.5.9.22963 ";
char ST1_payload_nick[]                         = "nickname=Frisco ";
char ST1_payload_callsign[]                     = "callsign=N5NHJ ";
char ST1_payload_ip[]                           = "ip=67.198.99.60 ";
char ST1_payload_port[]                         = "port=4992 ";
char ST1_payload_status[]                       = "status=Available ";
char ST1_payload_inuse_ip[]                     = "inuse_ip=99.127.15.2 ";
char ST1_payload_inuse_host[]                   = "inuse_host=99.127.15.2 ";
char ST1_payload_max_license[]                  = "max_licensed_version=v3 ";
char ST1_payload_radio_license[]                = "radio_license_id=00-1C-2D-05-11-10 ";
char ST1_payload_requires_additional_license[]  = "requires_additional_license=0 ";
char ST1_payload_fpc_mac[]                      = "fpc_mac= ";
char ST1_payload_wan_connected[]                = "wan_connected=1 ";
char ST1_payload_licensed_clients[]             = "licensed_clients=2 ";
char ST1_payload_available_clients[]            = "available_clients=1 ";
char ST1_payload_max_panadapters[]              = "max_panadapters=2 ";
char ST1_payload_available_panadapters[]        = "available_panadapters=1 ";
char ST1_payload_max_slices[]                   = "max_slices=2 ";
char ST1_payload_available_slices[]             = "available_slices=1 ";
char ST1_payload_gui_client_ips[]               = "gui_client_ips=192.168.1.139 ";
char ST1_payload_gui_client_hosts[]             = "gui_client_hosts=192.168.1.139 ";
char ST1_payload_gui_client_programs[]          = "gui_client_programs=SmartSDR-Win ";
char ST1_payload_gui_client_stations[]          = "gui_client_stations=PC-N5NHJ ";
char ST1_payload_gui_client_handles[]           = "gui_client_handles=0x3602E5F5 ";

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

  calculate_sizes();
}

void loop() {
  Udp.beginPacket(Brodcast, DiscoveryPort);
  Udp.write(header, sizeof(header));
  Udp.write(packet_size_hi);

  // PAYLOAD
  if (digitalRead(connectedPin)) {
    Udp.write(packet_size_co_lo);
    Udp.write(stream_id, sizeof(stream_id));
    Udp.write(class_id, sizeof(class_id));
    Udp.write(timestamp_int, sizeof(timestamp_int));
    Udp.write(timestamp_frac, sizeof(timestamp_frac));
    Udp.write(ST1_payload_protocol_version,             sizeof(ST1_payload_protocol_version)-1);
    Udp.write(ST1_payload_model,                        sizeof(ST1_payload_model)-1);
    Udp.write(ST1_payload_serial,                       sizeof(ST1_payload_serial)-1);
    Udp.write(ST1_payload_version,                      sizeof(ST1_payload_version)-1);
    Udp.write(ST1_payload_nick,                         sizeof(ST1_payload_nick)-1);
    Udp.write(ST1_payload_callsign,                     sizeof(ST1_payload_callsign)-1);
    Udp.write(ST1_payload_ip,                           sizeof(ST1_payload_ip)-1);
    Udp.write(ST1_payload_port,                         sizeof(ST1_payload_port)-1);
    Udp.write(ST1_payload_status,                       sizeof(ST1_payload_status)-1);
    Udp.write(ST1_payload_inuse_ip,                     sizeof(ST1_payload_inuse_ip)-1);
    Udp.write(ST1_payload_inuse_host,                   sizeof(ST1_payload_inuse_host)-1);
    Udp.write(ST1_payload_max_license,                  sizeof(ST1_payload_max_license)-1);
    Udp.write(ST1_payload_radio_license,                sizeof(ST1_payload_radio_license)-1);
    Udp.write(ST1_payload_requires_additional_license,  sizeof(ST1_payload_requires_additional_license)-1);
    Udp.write(ST1_payload_fpc_mac,                      sizeof(ST1_payload_fpc_mac)-1);
    Udp.write(ST1_payload_wan_connected,                sizeof(ST1_payload_wan_connected)-1);
    Udp.write(ST1_payload_licensed_clients,             sizeof(ST1_payload_licensed_clients)-1);
    Udp.write(ST1_payload_available_clients,            sizeof(ST1_payload_available_clients)-1);
    Udp.write(ST1_payload_max_panadapters,              sizeof(ST1_payload_max_panadapters)-1);
    Udp.write(ST1_payload_available_panadapters,        sizeof(ST1_payload_available_panadapters)-1);
    Udp.write(ST1_payload_max_slices,                   sizeof(ST1_payload_max_slices)-1);
    Udp.write(ST1_payload_available_slices,             sizeof(ST1_payload_available_slices)-1);
    Udp.write(ST1_payload_gui_client_ips,               sizeof(ST1_payload_gui_client_ips)-1);
    Udp.write(ST1_payload_gui_client_hosts,             sizeof(ST1_payload_gui_client_hosts)-1);
    Udp.write(ST1_payload_gui_client_programs,          sizeof(ST1_payload_gui_client_programs)-1);
    Udp.write(ST1_payload_gui_client_stations,          sizeof(ST1_payload_gui_client_stations)-1);
    Udp.write(ST1_payload_gui_client_handles,           sizeof(ST1_payload_gui_client_handles));
    Serial.println("Connected Sent.");
  }
  else {
    Udp.write(packet_size_di_lo);
    Udp.write(stream_id, sizeof(stream_id));
    Udp.write(class_id, sizeof(class_id));
    Udp.write(timestamp_int, sizeof(timestamp_int));
    Udp.write(timestamp_frac, sizeof(timestamp_frac));
    Udp.write(ST0_payload_protocol_version,             sizeof(ST0_payload_protocol_version)-1);
    Udp.write(ST0_payload_model,                        sizeof(ST0_payload_model)-1);
    Udp.write(ST0_payload_serial,                       sizeof(ST0_payload_serial)-1);
    Udp.write(ST0_payload_version,                      sizeof(ST0_payload_version)-1);
    Udp.write(ST0_payload_nick,                         sizeof(ST0_payload_nick)-1);
    Udp.write(ST0_payload_callsign,                     sizeof(ST0_payload_callsign)-1);
    Udp.write(ST0_payload_ip,                           sizeof(ST0_payload_ip)-1);
    Udp.write(ST0_payload_port,                         sizeof(ST0_payload_port)-1);
    Udp.write(ST0_payload_status,                       sizeof(ST0_payload_status)-1);
    Udp.write(ST0_payload_inuse_ip,                     sizeof(ST0_payload_inuse_ip)-1);
    Udp.write(ST0_payload_inuse_host,                   sizeof(ST0_payload_inuse_host)-1);
    Udp.write(ST0_payload_max_license,                  sizeof(ST0_payload_max_license)-1);
    Udp.write(ST0_payload_radio_license,                sizeof(ST0_payload_radio_license)-1);
    Udp.write(ST0_payload_requires_additional_license,  sizeof(ST0_payload_requires_additional_license)-1);
    Udp.write(ST0_payload_fpc_mac,                      sizeof(ST0_payload_fpc_mac)-1);
    Udp.write(ST0_payload_wan_connected,                sizeof(ST0_payload_wan_connected)-1);
    Udp.write(ST0_payload_licensed_clients,             sizeof(ST0_payload_licensed_clients)-1);
    Udp.write(ST0_payload_available_clients,            sizeof(ST0_payload_available_clients)-1);
    Udp.write(ST0_payload_max_panadapters,              sizeof(ST0_payload_max_panadapters)-1);
    Udp.write(ST0_payload_available_panadapters,        sizeof(ST0_payload_available_panadapters)-1);
    Udp.write(ST0_payload_max_slices,                   sizeof(ST0_payload_max_slices)-1);
    Udp.write(ST0_payload_available_slices,             sizeof(ST0_payload_available_slices)-1);
    Udp.write(ST0_payload_gui_client_ips,               sizeof(ST0_payload_gui_client_ips)-1);
    Udp.write(ST0_payload_gui_client_hosts,             sizeof(ST0_payload_gui_client_hosts)-1);
    Udp.write(ST0_payload_gui_client_programs,          sizeof(ST0_payload_gui_client_programs)-1);
    Udp.write(ST0_payload_gui_client_stations,          sizeof(ST0_payload_gui_client_stations)-1);
    Udp.write(ST0_payload_gui_client_handles,           sizeof(ST0_payload_gui_client_handles));
    Serial.println("Discovery Sent.");
  }
  Udp.endPacket();
  delay(1000);
}

void calculate_sizes(){
  uint16_t discovery_size =
    sizeof(header) +
    sizeof(packet_size_hi) +
    sizeof(packet_size_di_lo) +
    sizeof(stream_id) +
    sizeof(class_id) +
    sizeof(timestamp_int) +
    sizeof(timestamp_frac) +
  sizeof(ST0_payload_protocol_version) +
  sizeof(ST0_payload_model) +
  sizeof(ST0_payload_serial) +
  sizeof(ST0_payload_version) +
  sizeof(ST0_payload_nick) +
  sizeof(ST0_payload_callsign) +
  sizeof(ST0_payload_ip) +
  sizeof(ST0_payload_port) +
  sizeof(ST0_payload_status) +
  sizeof(ST0_payload_inuse_ip) +
  sizeof(ST0_payload_inuse_host) +
  sizeof(ST0_payload_max_license) +
  sizeof(ST0_payload_radio_license) +
  sizeof(ST0_payload_requires_additional_license) +
  sizeof(ST0_payload_fpc_mac) +
  sizeof(ST0_payload_wan_connected) +
  sizeof(ST0_payload_licensed_clients) +
  sizeof(ST0_payload_available_clients) +
  sizeof(ST0_payload_max_panadapters) +
  sizeof(ST0_payload_available_panadapters) +
  sizeof(ST0_payload_max_slices) +
  sizeof(ST0_payload_available_slices) +
  sizeof(ST0_payload_gui_client_ips) +
  sizeof(ST0_payload_gui_client_hosts) +
  sizeof(ST0_payload_gui_client_programs) +
  sizeof(ST0_payload_gui_client_stations) +
  sizeof(ST0_payload_gui_client_handles);
  discovery_size = discovery_size-26; // end char overhead for 27 parameters -1
  Serial.println(discovery_size);
  uint8_t discovery_words_size = discovery_size/4;
  packet_size_di_lo = discovery_words_size;
  Serial.println (discovery_words_size);

  uint16_t connected_size =
    sizeof(header) +
    sizeof(packet_size_hi) +
    sizeof(packet_size_co_lo) +
    sizeof(stream_id) +
    sizeof(class_id) +
    sizeof(timestamp_int) +
    sizeof(timestamp_frac) +
  sizeof(ST1_payload_protocol_version) +
  sizeof(ST1_payload_model) +
  sizeof(ST1_payload_serial)+
  sizeof(ST1_payload_version)+
  sizeof(ST1_payload_nick)+
  sizeof(ST1_payload_callsign)+
  sizeof(ST1_payload_ip)+
  sizeof(ST1_payload_port)+
  sizeof(ST1_payload_status)+
  sizeof(ST1_payload_inuse_ip)+
  sizeof(ST1_payload_inuse_host)+
  sizeof(ST1_payload_max_license)+
  sizeof(ST1_payload_radio_license)+
  sizeof(ST1_payload_requires_additional_license)+
  sizeof(ST1_payload_fpc_mac)+
  sizeof(ST1_payload_wan_connected)+
  sizeof(ST1_payload_licensed_clients)+
  sizeof(ST1_payload_available_clients)+
  sizeof(ST1_payload_max_panadapters)+
  sizeof(ST1_payload_available_panadapters)+
  sizeof(ST1_payload_max_slices)+
  sizeof(ST1_payload_available_slices)+
  sizeof(ST1_payload_gui_client_ips)+
  sizeof(ST1_payload_gui_client_hosts)+
  sizeof(ST1_payload_gui_client_programs)+
  sizeof(ST1_payload_gui_client_stations)+
  sizeof(ST1_payload_gui_client_handles);
  connected_size = connected_size-26; // end char overhead for 27 parameters -1
  Serial.println(connected_size);
  uint8_t connected_words_size = connected_size/4;
  packet_size_co_lo = connected_words_size;
  Serial.println (connected_words_size);
}