#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "car.h"
#include "soc/rtc_cntl_reg.h"
// Brownout workaround kept — your USB cable can't supply the BLE radio inrush.

// UUIDs — keep these in sync with the React Native app.
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361f1196"

// NodeMCU-32S safe GPIOs (avoid GPIO3/1 which are tied to USB-UART).
// 32/33 are adjacent on the header; 25/26 are adjacent. Easy wiring.
Car car(32, 33, 25, 26);

BLEServer *pServer = nullptr;

class ServerCallbacks : public BLEServerCallbacks
{
    void onConnect(BLEServer *server) override {}

    void onDisconnect(BLEServer *server) override
    {
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
    Serial.println("Starting ESP32 BLE Car...");

    // Disable brownout detector — the USB cable/dock can't supply the
    // BLE radio inrush spike and the chip resets before the stack comes up.
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

    delay(200);

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

    Serial.println("BLE advertising.");
}

void loop()
{
    delay(10);
}