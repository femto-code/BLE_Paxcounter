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