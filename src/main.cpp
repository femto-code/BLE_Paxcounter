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


static const char LOG_TAG[] = "SampleScan";

void setup() {}

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
	void onResult(BLEAdvertisedDevice advertisedDevice) {

		std::string res = "";
		char *pHex = BLEUtils::buildHexData(nullptr, (uint8_t*)advertisedDevice.getManufacturerData().data(), advertisedDevice.getManufacturerData().length());
		res += pHex;
		
		if (res.find("bc040a") != std::string::npos) {
			ESP_LOGE(LOG_TAG, "PAC found! Address: %s", advertisedDevice.getAddress().toString().c_str());
			ESP_LOGE(LOG_TAG, "Byte_count: %d", advertisedDevice.getManufacturerData().length());
			
			ESP_LOGE(LOG_TAG, "man_dat: %s", res.data());
			
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


			std::string device_status = res.substr(26, 2);
						
			//ESP_LOGE(LOG_TAG, "part_nmbr: %s", part_nmbr.data());
			
			//ESP_LOGE(LOG_TAG, "part_nmbr_lower5: %s", part_nmbr_lower5.data());
			//ESP_LOGE(LOG_TAG, "part_nmbr_upper3: %s", part_nmbr_upper3.data());
			//ESP_LOGE(LOG_TAG, "part_nmbr_upper3 int: %d", part_nmbr_upper3_int);
			ESP_LOGE(LOG_TAG, "part_nmbr decoded: %s", part_nmbr_decoded.data());
			
			ESP_LOGE(LOG_TAG, " ");
			//ESP_LOGE(LOG_TAG, "serial_nmbr: %s", serial_nmbr.data());
			/* ESP_LOGE(LOG_TAG, "serial_nmbr upper3: %s", serial_nmbr_upper3.data());
			ESP_LOGE(LOG_TAG, "serial_nmbr upper3 int: %d", serial_nmbr_upper3_int);
			ESP_LOGE(LOG_TAG, "serial_nmbr lower2: %s", serial_nmbr_lower2.data()); */
			ESP_LOGE(LOG_TAG, "serial_nmbr upper3 decoded: %s", serial_nmbr_decoded.data());

			ESP_LOGE(LOG_TAG, "device_status: %s", device_status.data());
		}
		return;


		// ESP_LOGE(LOG_TAG, "payload: %x", advertisedDevice.getPayload());
		ESP_LOGE(LOG_TAG, "man_data: %x", advertisedDevice.getManufacturerData().data());
		std::string temp = advertisedDevice.getManufacturerData();
		ESP_LOGE(LOG_TAG, "temp: %d", temp.c_str());
		if (temp.find("bc04") != std::string::npos) {
			ESP_LOGE(LOG_TAG, "Dräger found!");
			
			
		}
		ESP_LOGE(LOG_TAG, "address: %s", advertisedDevice.getAddress().toString().c_str());
		if (advertisedDevice.haveName()) {
			ESP_LOGE(LOG_TAG, "Advertised Device: %s", advertisedDevice.toString().c_str());
		}
	}
};

static void run() {
	ESP_LOGE(LOG_TAG, "Scanning sample starting");
	BLEDevice::init("");
	BLEScan* pBLEScan = BLEDevice::getScan();
	pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
	pBLEScan->setActiveScan(true);
	BLEScanResults scanResults = pBLEScan->start(30);
	ESP_LOGE(LOG_TAG, "We found %d devices", scanResults.getCount());
	ESP_LOGE(LOG_TAG, "Scanning sample ended");
}

void loop(void)
{
	run();
} // app_main