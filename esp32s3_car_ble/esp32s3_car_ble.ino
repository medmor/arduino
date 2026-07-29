#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "car.h"

// Built-in RGB LED on ESP32-S3 boards that support RGB_BUILTIN.
// Use red while idle and green when a client is connected.
#ifndef RGB_BUILTIN
#define RGB_BUILTIN 48
#endif

// UUIDs — keep these in sync with the React Native app.
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361f1196"

Car car(3, 4, 5, 6); // ESP32-S3 safe GPIOs for DRV8833

BLEServer *pServer = nullptr;

void setLedState(bool connected)
{
    if (connected) {
        rgbLedWrite(RGB_BUILTIN, 0, 255, 0); // green
    } else {
        rgbLedWrite(RGB_BUILTIN, 255, 0, 0); // red
    }
}

class ServerCallbacks : public BLEServerCallbacks
{
    void onConnect(BLEServer *server) override
    {
        setLedState(true);
    }

    void onDisconnect(BLEServer *server) override
    {
        setLedState(false);
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
    Serial.println("Starting ESP32-S3 BLE Car...");

    // TODO: change this name later to the final board name.
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
    setLedState(false);

    Serial.println("BLE advertising. Connect with the app.");
}

void loop()
{
    delay(10);
}
