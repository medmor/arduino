#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "car.h"

// UUIDs — keep these in sync with the React Native app.
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361f1196"

Car car(2, 3, 4, 5, 8); // ESP32-C3 Super Mini safe GPIOs for DRV8833 + LED

BLEServer *pServer = nullptr;

class ServerCallbacks : public BLEServerCallbacks
{
    void onConnect(BLEServer *server) override { }

    void onDisconnect(BLEServer *server) override
    {
        // Restart advertising so the phone can reconnect without a reboot.
        BLEDevice::startAdvertising();
    }
};

class CommandCallback : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *pChar) override
    {
        String cmd = pChar->getValue();
        cmd.trim();

        Serial.print("Received: ");
        Serial.println(cmd);

        if (cmd == "forward")
            car.forward();
        else if (cmd == "backward")
            car.backward();
        else if (cmd == "left")
            car.left();
        else if (cmd == "right")
            car.right();
        else if (cmd == "stop")
            car.stop();
        else
            Serial.println("Unknown command");
    }
};

void setup()
{
    Serial.begin(115200);
    delay(200);
    Serial.println("Starting ESP32-C3 BLE Car...");

    BLEDevice::init("ESP32C3-Car-BLE");
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new ServerCallbacks());
    BLEService *pService = pServer->createService(SERVICE_UUID);

    BLECharacteristic *pChar = pService->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_WRITE_NR);

    pChar->addDescriptor(new BLE2902());
    pChar->setCallbacks(new CommandCallback());
    pChar->setValue("stop");

    pService->start();

    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x0);
    BLEDevice::startAdvertising();

    Serial.println("BLE advertising. Connect with the app.");
}

void loop()
{
    car.update(millis());
    delay(10);
}