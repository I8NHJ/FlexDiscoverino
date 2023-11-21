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

// Protocol Definition
byte header[4] ={0x38, 0x58, 0x00, 0x88};
byte stream_id[4] = {0x00, 0x00, 0x08, 0x00};
byte class_id[8] = {0x00, 0x00, 0x1C, 0x2D, 0x53, 0x4C, 0xFF, 0xFF};
byte timestamp_int[4] = {0x65, 0x56, 0x3F,0xBF};
byte timestamp_frac[8] = {0x00, 0x00,0x00,0x00,0x00,0x00,0x00,0x00};
char payload[PAYLOAD_SIZE] = "discovery_protocol_version=3.0.0.2 model=FLEX-6400 serial=2019-2544-6400-6026 version=3.5.9.22963 nickname=Locale callsign=I8NHJ ip=67.198.99.60 port=4992 status=Available inuse_ip= inuse_host= max_licensed_version=v3 radio_license_id=00-1C-2D-05-11-10 requires_additional_license=0 fpc_mac= wan_connected=1 licensed_clients=2 available_clients=2 max_panadapters=2 available_panadapters=2 max_slices=2 available_slices=2 gui_client_ips= gui_client_hosts= gui_client_programs= gui_client_stations= gui_client_handles= ";

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
}

void loop() {
  Udp.beginPacket(Brodcast, DiscoveryPort);
  Udp.write(header, sizeof(header));
  Udp.write(stream_id, sizeof(stream_id));
  Udp.write(class_id, sizeof(class_id));
  Udp.write(timestamp_int, sizeof(timestamp_int));
  Udp.write(timestamp_frac, sizeof(timestamp_frac));
  Udp.write(payload, sizeof(payload));
  Serial.println("Discovery Sent.");
  Udp.endPacket();
  delay(1000);
}

