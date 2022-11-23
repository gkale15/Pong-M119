// using skeleton code taken from 
// https://github.com/ucla-hci/m119/blob/main/m2b_peripheral/m2b_peripheral.ino

#include <ArduinoBLE.h>
#include <Arduino_LSM6DS3.h>

#define BLE_UUID_ACCELEROMETER_SERVICE "1102"
#define BLE_UUID_ACCELEROMETER_X "2101"
#define BLE_UUID_ACCELEROMETER_Y "2102"
#define BLE_UUID_ACCELEROMETER_Z "2103"

#define BLE_UUID_GYROSCOPE_SERVICE "1105"
#define BLE_UUID_GYROSCOPE_X "3101"
#define BLE_UUID_GYROSCOPE_Y "3102"
#define BLE_UUID_GYROSCOPE_Z "3103"

#define BLE_DEVICE_NAME "GKNano"

BLEService accelerometerService(BLE_UUID_ACCELEROMETER_SERVICE);
BLEService gyroscopeService(BLE_UUID_GYROSCOPE_SERVICE);

BLEFloatCharacteristic accelerometerCharacteristicX(BLE_UUID_ACCELEROMETER_X, BLERead | BLENotify);
BLEFloatCharacteristic accelerometerCharacteristicY(BLE_UUID_ACCELEROMETER_Y, BLERead | BLENotify);
BLEFloatCharacteristic accelerometerCharacteristicZ(BLE_UUID_ACCELEROMETER_Z, BLERead | BLENotify);

BLEFloatCharacteristic gyroscopeCharacteristicX(BLE_UUID_GYROSCOPE_X, BLERead | BLENotify);
BLEFloatCharacteristic gyroscopeCharacteristicY(BLE_UUID_GYROSCOPE_Y, BLERead | BLENotify);
BLEFloatCharacteristic gyroscopeCharacteristicZ(BLE_UUID_GYROSCOPE_Z, BLERead | BLENotify);

float x, y, z, gx, gy, gz;

void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;

  // initialize IMU
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1)
      ;
  }

//  Serial.print("Accelerometer sample rate = ");
//  Serial.print(IMU.accelerationSampleRate());
//  Serial.println("Hz");

  // initialize BLE
  if (!BLE.begin()) {
    Serial.println("Starting Bluetooth® Low Energy module failed!");
    while (1)
      ;
  }

  BLE.setDeviceName(BLE_DEVICE_NAME);
  BLE.setLocalName(BLE_DEVICE_NAME);

  
  BLE.setAdvertisedService(accelerometerService);
  accelerometerService.addCharacteristic(accelerometerCharacteristicX);
  accelerometerService.addCharacteristic(accelerometerCharacteristicY);
  accelerometerService.addCharacteristic(accelerometerCharacteristicZ);

  BLE.setAdvertisedService(gyroscopeService);
  gyroscopeService.addCharacteristic(gyroscopeCharacteristicX);
  gyroscopeService.addCharacteristic(gyroscopeCharacteristicY);
  gyroscopeService.addCharacteristic(gyroscopeCharacteristicZ);


  
  BLE.addService(accelerometerService);
  accelerometerCharacteristicX.writeValue(0);
  accelerometerCharacteristicY.writeValue(0);
  accelerometerCharacteristicZ.writeValue(0);

  BLE.addService(gyroscopeService);
  gyroscopeCharacteristicX.writeValue(0);
  gyroscopeCharacteristicY.writeValue(0);
  gyroscopeCharacteristicZ.writeValue(0);

  BLE.advertise();

}

void loop() {
  BLEDevice central = BLE.central();
  IMU.readAcceleration(x, y, z);
  IMU.readGyroscope(gx, gy, gz);

  
  accelerometerCharacteristicX.writeValue(x);
  accelerometerCharacteristicY.writeValue(y);
  accelerometerCharacteristicZ.writeValue(z);
  
  gyroscopeCharacteristicX.writeValue(gx);
  gyroscopeCharacteristicY.writeValue(gy);
  gyroscopeCharacteristicZ.writeValue(gz);

  Serial.print(millis());
  Serial.print(",");
  Serial.print(x);
  Serial.print(",");
  Serial.print(y);
  Serial.print(",");
  Serial.print(z);
  Serial.print(",");
  Serial.print(gx);
  Serial.print(",");
  Serial.print(gy);
  Serial.print(",");
  Serial.print(gz);
  Serial.println("");
  
  
}
