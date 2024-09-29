/*
程序名称：BLE_Blink
主要功能：通过BLE控制LED的闪烁频率
详细描述：实验芯片为ESP32-S3，通过BLE发送特定的数据控制LED闪烁频率。例如："1000"为间隔1s闪烁，"500"为间隔0.5s闪烁
备注：默认沟通的蓝牙设备发送的数据符合相应格式，这里不做类型转换的校验，且数据转化后大于100小于10000（即闪烁最小0.1s最多10s）
*/


#include <BLEDevice.h>
#include <BLEServer.h>
#define LED 10
#define LED_ON HIGH
#define LED_OFF LOW
//服务UUID
#define SERVICE_UUID "dce0ea62-7dc5-c314-574b-f41db85ca3ad"
//特性UUID
#define CHARACTERISTIC_UUID "21e18c04-ba64-5a87-c8d5-eb458b8098cc"


//全局变量，闪烁间隔时间
int BlinkTime = 0;

//全局变量，LED亮灭状态
bool isLEDon = 0;

//全局变量，特性指针
BLECharacteristic *pCharacteristic;


//回调函数
class callBacks : public BLEServerCallbacks {
  void onConnect(BLEServer *pServer) {
  }
  void onDisconnect(BLEServer *pServer) {
    BLEDevice::startAdvertising();
  }
};


void setup() {

  pinMode(LED, OUTPUT);

  //初始化蓝牙，不再过多叙述

  BLEDevice::init("BLE_CONTROL_LED");

  BLEServer *pServer = BLEDevice::createServer();

  pServer->setCallbacks(new callBacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);

  pCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);

  //设定默认是1s闪烁
  pCharacteristic -> setValue("1000");

  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();

  pAdvertising->addServiceUUID(SERVICE_UUID);

  pAdvertising->setScanResponse(false);

  BLEDevice::startAdvertising();

}

void loop() {

  //获取BLE数据
  String valueString = pCharacteristic -> getValue();

  //数据类型转换
  BlinkTime = atoi(valueString.c_str());

  //检测数据范围合规
  if (BlinkTime >= 100 && BlinkTime <= 10000){

    //如果数据范围合规，判断LED亮灭状态，并翻转
    if (isLEDon == 1){
      isLEDon = 0;
      digitalWrite(LED, LED_OFF);
  } else {
      isLEDon = 1;
      digitalWrite(LED, LED_ON);
  }

    //如果数据合规，开始循环闪烁
    delay(BlinkTime);
  
  } else {

    //如果数据不合规，LED熄灭
    digitalWrite(LED, LED_OFF);

  }

}
