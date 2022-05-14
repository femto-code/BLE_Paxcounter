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
		//ESP_LOGE(LOG_TAG, "res: %s", res.c_str());
		if (res.find("bc040a") != std::string::npos) {
			ESP_LOGE(LOG_TAG, "PAC found! Address: %s", advertisedDevice.getAddress().toString().c_str());
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