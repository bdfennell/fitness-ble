#include "Particle.h"

// This example does not require the cloud so you can run it in manual mode or
// normal cloud-connected mode
// SYSTEM_MODE(MANUAL);


// 0000034990 [app] INFO: 1 devices found
// 0000034991 [app] INFO: MAC: 26:62:EF:8B:22:D4 | RSSI: -80Bm
// 0000034991 [app] INFO: Advertising name: ECHEX-3-145257

// 0000107161 [wiring.ble] TRACE: Start discovering characteristics of service: 1800.
// 0000109605 [wiring.ble] TRACE: Characteristic discovered.
// 0000109606 [wiring.ble] TRACE: Start discovering characteristics of service: 1801.
// 0000110745 [wiring.ble] TRACE: Characteristic discovered.
// 0000110746 [wiring.ble] TRACE: Start discovering characteristics of service: 0BF669F0-45F2-11E7-9598-0800200C9A66.
// 0000111615 [wiring.ble] TRACE: Characteristic discovered.
// 0000111616 [wiring.ble] TRACE: Start discovering characteristics of service: 0BF669F1-45F2-11E7-9598-0800200C9A66.
// 0000117405 [wiring.ble] TRACE: Characteristic discovered.
// 0000117406 [wiring.ble] TRACE: Start discovering characteristics of service: 180A.
// 0000118815 [wiring.ble] TRACE: Characteristic discovered.
// 0000118816 [wiring.ble] TRACE: Start discovering characteristics of service: FE59.

int cadence = 0;
int resistance = 0;

SYSTEM_THREAD(ENABLED);

SerialLogHandler logHandler(LOG_LEVEL_TRACE);

const size_t SCAN_RESULT_MAX = 30;

BleCharacteristic sensorData;
BleCharacteristic writeCharacteristic;

BleScanResult scanResults[SCAN_RESULT_MAX];
BlePeerDevice peer;



uint16_t lastRate = 0;


void onDataReceived(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context);

void setup() {
	(void)logHandler; // Does nothing, just to eliminate the unused variable warning

	Serial.begin();

	BLE.on();

	Particle.variable("cadence", cadence);
	Particle.variable("resistance", resistance);

#if SYSTEM_VERSION == SYSTEM_VERSION_v310
	// This is required with 3.1.0 only
	BLE.setScanPhy(BlePhy::BLE_PHYS_AUTO);
#endif

	sensorData.onDataReceived(onDataReceived, NULL);
}

void loop() {
    delay(5000);






	if (BLE.connected()) {
		// We're currently connected to a sensor
		Log.info("connected");
		
	}
	else {
	    BleScanFilter filter;
	    filter.deviceName("ECHEX-3-145257");
	    Vector<BleScanResult> scanResults = BLE.scanWithFilter(filter);
	    

        if (scanResults.size()) {
            Log.info("%d devices found", scanResults.size());
            
            for (int ii = 0; ii < scanResults.size(); ii++) {
                // For Device OS 2.x and earlier, use scanResults[ii].address[0], etc. without the ()
                Log.info("MAC: %02X:%02X:%02X:%02X:%02X:%02X | RSSI: %dBm",
                        scanResults[ii].address()[0], scanResults[ii].address()[1], scanResults[ii].address()[2],
                        scanResults[ii].address()[3], scanResults[ii].address()[4], scanResults[ii].address()[5], scanResults[ii].rssi());
    
                String name = scanResults[ii].advertisingData().deviceName();
                if (name.length() > 0) {
                    Log.info("Advertising name: %s", name.c_str());
                }
                
                peer = BLE.connect(scanResults[ii].address());
                
                // Vector<BleCharacteristic> characteristics = peer.discoverAllCharacteristics();
                // for (int ii = 0; ii < characteristics.size(); ii++) {
                //     Log.info(characteristics[ii].UUID());
                // }
                
                bool boolSensor = peer.getCharacteristicByUUID(sensorData, BleUuid("0bf669f4-45f2-11e7-9598-0800200c9a66"));
                
                
                //enable call back
                bool boolWrite = peer.getCharacteristicByUUID(writeCharacteristic, BleUuid("0bf669f2-45f2-11e7-9598-0800200c9a66"));
                
                Log.info(String(boolSensor));
                Log.info(String(boolWrite));
                
                
                byte message[] = {0xF0, 0xB0, 0x01, 0x01, 0xA2};
                writeCharacteristic.setValue(message, 5);
                
                
                
                
                
                

            }            
        }


	}
}


void onDataReceived(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context) {



  switch(data[1]) {
    // Cadence notification
    case 0xD1:
      //runtime = int((data[7] << 8) + data[8]); // This runtime has massive drift
      cadence = int((data[9] << 8) + data[10]);
      Particle.publish("cadence", String(cadence));
    //   power = getPower(cadence, resistance);
      break;
    // Resistance notification
    case 0xD2:
      resistance = int(data[3]);
      Particle.publish("resistance", String(resistance));
    //   power = getPower(cadence, resistance);
      break;
  }


    // 
    // Particle.publish("resistance", String(resistance));

    
}
