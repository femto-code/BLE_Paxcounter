#include <Arduino.h>

/**
 * Perform scanning for BLE advertised servers.
 */
#include "BLEUtils.h"
#include "BLEScan.h"
#include <esp_log.h>
#include <string>

#include "BLEDevice.h"
#include "BLEAdvertisedDevice.h"
#include "sdkconfig.h"

#include <ArduinoJson.h>

#define TX 1
#define RX 3

// Create the JSON document
StaticJsonDocument<1000> doc;
char jbuffer[1000];

//HardwareSerial &hSerial = Serial1; // can be Serial2 as well, just use proper pins

static const char LOG_TAG[] = "SampleScan";

void setup() {
	Serial.begin(115200);  // open serial via USB to PC on default port
	//hSerial.begin(115200, SERIAL_8N1, RX, TX); // open the other serial port
}

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
{
	void onResult(BLEAdvertisedDevice advertisedDevice)
	{

		std::string res = "";
		char *pHex = BLEUtils::buildHexData(nullptr, (uint8_t *)advertisedDevice.getManufacturerData().data(), advertisedDevice.getManufacturerData().length());
		res += pHex;
		// ESP_LOGE(LOG_TAG, "res: %s", res.c_str());
		if (res.find("bc040a") != std::string::npos)
		{
			//ESP_LOGE(LOG_TAG, "PAC found! Address: %s", advertisedDevice.getAddress().toString().c_str());
			//ESP_LOGE(LOG_TAG, "Byte_count: %d", advertisedDevice.getManufacturerData().length());
			
			//ESP_LOGE(LOG_TAG, "man_dat: %s", res.data());
			
		//decodierung der Part-Number
			std::string part_nmbr = res.substr(6, 8);
			int number = (int)stoi(part_nmbr, 0, 16); //wandelt string zu int
			char buffer[33]; //nimmt die intzahl wieder als String auf
			char *intstrp = itoa(number, buffer, 10);
			std::string intstr = intstrp;
			std::string part_nmbr_lower5 = intstr.substr(3, 5);
			std::string part_nmbr_upper3 = intstr.substr(0, 3);
			int part_nmbr_upper3_int = (int)stoi(part_nmbr_upper3, 0, 10);
			char buffer2[33]; 
			char *intstrp36 = itoa(part_nmbr_upper3_int, buffer2, 36);//nimmt die intzahl wieder als String auf
			std::string part_nmbr_decoded = intstrp36 + part_nmbr_lower5;
			
		//decodierung der serial_nmbr
			std::string serial_nmbr_upper3 = res.substr(14, 6);
			int serial_nmbr_upper3_int = (int)stoi(serial_nmbr_upper3, 0, 16);
			char buffer3[33]; 
			char *intstrs36 = itoa(serial_nmbr_upper3_int, buffer3, 36);
			std::string serial_nmbr_lower2 = res.substr(20, 4);
			int serial_nmbr_lower2_int = (int)stoi(serial_nmbr_lower2, 0, 16);
			char buffer4[33]; 
			char *intstrs = itoa(serial_nmbr_lower2_int, buffer4, 10);//nimmt die intzahl wieder als String auf
			std::string intstr4 = intstrs;
			std::string zero_padding_str = "";
			int count= intstr4.length();
			int count2 = 4-count;
			if(count2!=0){
			for (int i=0; i<count2; i++){
				zero_padding_str+="0";
			}
			}
			std::string serial_nmbr_decoded = intstrs36 + zero_padding_str + intstr4;

		//decodierung des device status
			std::string device_status = res.substr(24, 2);
			int device_status_int = (int)stoi(device_status, 0, 16);
			
			std::string device_status_output ;
			int hilf = device_status_int&0x3B;
			if((hilf)==0x01){device_status_output ="Gerät einsatzbereit";}
			else{device_status_output ="Gerät nicht einsatzbereit";}
		//Ausgabe der dekodierten Informationen
			//ESP_LOGE(LOG_TAG, "part_nmbr: %s", part_nmbr.data());
			//ESP_LOGE(LOG_TAG, "part_nmbr_lower5: %s", part_nmbr_lower5.data());
			//ESP_LOGE(LOG_TAG, "part_nmbr_upper3: %s", part_nmbr_upper3.data());
			//ESP_LOGE(LOG_TAG, "part_nmbr_upper3 int: %d", part_nmbr_upper3_int);
			//ESP_LOGE(LOG_TAG, "part_nmbr decoded: %s", part_nmbr_decoded.data());
			
			//ESP_LOGE(LOG_TAG, " ");
			//ESP_LOGE(LOG_TAG, "serial_nmbr: %s", serial_nmbr.data());
			/* ESP_LOGE(LOG_TAG, "serial_nmbr upper3: %s", serial_nmbr_upper3.data());
			ESP_LOGE(LOG_TAG, "serial_nmbr upper3 int: %d", serial_nmbr_upper3_int);
			ESP_LOGE(LOG_TAG, "serial_nmbr lower2: %s", serial_nmbr_lower2.data()); */
			//ESP_LOGE(LOG_TAG, "serial_nmbr upper3 decoded: %s", serial_nmbr_decoded.data());

			//ESP_LOGE(LOG_TAG, "device_status: %s", device_status.data());

			JsonObject obj = doc.createNestedObject();
			obj["mac"] = advertisedDevice.getAddress().toString();
			obj["pn"] = part_nmbr_decoded;
			obj["ds"] = device_status;
			obj["sn"] = serial_nmbr_decoded;
			serializeJson(doc, jbuffer);
			
			//ESP_LOGE(LOG_TAG, "serial_nmbr upper3: %s", serial_nmbr_upper3.data());
			//ESP_LOGE(LOG_TAG, "serial_nmbr upper3 int: %d", serial_nmbr_upper3_int);
			//ESP_LOGE(LOG_TAG, "serial_nmbr lower2: %s", serial_nmbr_lower2.data());
			// ESP_LOGE(LOG_TAG, "serial_nmbr upper3 decoded: %s", serial_nmbr_decoded.data());

			// ESP_LOGE(LOG_TAG, "device_status: %s", device_status.data());
			// ESP_LOGE(LOG_TAG, "device_status int: %d hilf: %d", device_status_int, hilf);
			// ESP_LOGE(LOG_TAG, "device_status_test: %s", device_status_output.data());

		}


		// ESP_LOGE(LOG_TAG, "payload: %x", advertisedDevice.getPayload());
		// ESP_LOGE(LOG_TAG, "man_data: %x", advertisedDevice.getManufacturerData().data());
		// std::string temp = advertisedDevice.getManufacturerData();
		// ESP_LOGE(LOG_TAG, "temp: %d", temp.c_str());
		// if (temp.find("bc04") != std::string::npos) {
		// 	ESP_LOGE(LOG_TAG, "Dräger found!");
			
			
		// }
		// ESP_LOGE(LOG_TAG, "address: %s", advertisedDevice.getAddress().toString().c_str());
		// if (advertisedDevice.haveName()) {
		// 	ESP_LOGE(LOG_TAG, "Advertised Device: %s", advertisedDevice.toString().c_str());
		// }
	}
};

static void run() {
	//ESP_LOGE(LOG_TAG, "Scanning sample starting");
	BLEDevice::init("");
	BLEScan *pBLEScan = BLEDevice::getScan();
	pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
	pBLEScan->setActiveScan(true);
	BLEScanResults scanResults = pBLEScan->start(10);
	//ESP_LOGE(LOG_TAG, "We found %d devices", scanResults.getCount());
	//ESP_LOGE(LOG_TAG, "Scanning sample ended");
	doc.clear();
	//Serial.write(jbuffer);
	//ESP_LOGE(LOG_TAG, "Buffer: %s", jbuffer);
}

void loop(void) {
	run();
	// if (Serial.available()) { // check incoming on default serial (USB) from PC
	// 	hSerial.write("From Serial:" + Serial.read()); // read it from UBS and send it to hSerial
	// }

	// if (hSerial.available()) { // check incoming on other serial from the other board
	// 	Serial.write("From hSerial:" + hSerial.read()); // read it from hSerial and send it to  UBS
	// }
	//hSerial.write("1");
	//Serial.write("2");
	//hSerial.print(buffer);
	//Serial.print("blub");
	Serial.print(jbuffer);
} // app_main