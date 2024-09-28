/*
程序名称：BLE_LED
主要功能：通过BLE控制单片机引脚所接LED的亮灭
详细描述：实验芯片为ESP32-S3，两颗LED分别接在IO10和IO11两个引脚上，均为高电平使能（点亮）。程序创建了一个蓝牙服务，内置两个特性。一个为只读状态，用于主机读取LED的亮灭状态；另一个为可读可写，操作内容是一个由两个阿拉伯数字构成的字符串，第一位数字代表LED0，第二位数字代表LED1，1代表点亮，0代表熄灭。
*/

//导入必要的蓝牙包
#include <BLEDevice.h>
#include <BLEServer.h>


//定义LED0的引脚为10
#define LED0 10
//定义LED1的引脚为11
#define LED1 11
//定义LED点亮是高电平
#define LED_ON HIGH
//定义LED熄灭是低电平
#define LED_OFF LOW
//设定服务的UUID，按照BLE规范，1815为IO控制服务
#define SERVICE_UUID "00001815-0000-1000-8000-00805F9B34FB"
//设定特性的UUID，该UUID内容不在BLE规范文件范围内，属于自定义内容
#define CHARACTERISTIC_LED_STATUS_UUID "E30941BB-5F0F-DE69-16CD-7A1C45035886"
//设定特性的UUID，该UUID内容不在BLE规范文件范围内，属于自定义内容
#define CHARACTERISTIC_LED_CONTROL_UUID "1937E50A-D490-7E71-2780-BA0E68B278BB"


//声明全局变量pCharacteristicStatus
BLECharacteristic *pCharacteristicLEDStatus;
//声明全局变量pCharacteristicLED
BLECharacteristic *pCharacteristicLEDControl;


//创建回调函数，使其在蓝牙断开后，还可继续广播
class callBacks : public BLEServerCallbacks{
  //当蓝牙连接时
  void onConnect(BLEServer* pServer){
    //无操作
  }
  //当蓝牙断开时
  void onDisconnect(BLEServer* pServer){
    //重新开始广播
    BLEDevice::startAdvertising();
  }
};


//单片机初始化函数
void setup() {

  //设置LED0引脚为输出模式
  pinMode(LED0, OUTPUT);

  //设置LED1引脚为输出模式
  pinMode(LED1, OUTPUT);

  //初始化设备
  BLEDevice::init("BLE_CONTROL_LED");

  //创建服务器
  BLEServer *pServer = BLEDevice::createServer();

  //添加回调函数
  pServer -> setCallbacks(new callBacks());

  //创建服务
  BLEService *pService = pServer->createService(SERVICE_UUID);

  //创建特性，显示LED亮灭状态
  pCharacteristicLEDStatus = pService->createCharacteristic(CHARACTERISTIC_LED_STATUS_UUID,BLECharacteristic::PROPERTY_READ);

  //创建特性，控制LED亮灭功能
  pCharacteristicLEDControl = pService->createCharacteristic(CHARACTERISTIC_LED_CONTROL_UUID,BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);

  //设定特性的初始值
  pCharacteristicLEDStatus -> setValue("");

  //设定特性的初始值
  pCharacteristicLEDControl -> setValue("");

  //启动服务
  pService->start();

  //获取广播器
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();

  //向广播添加服务
  pAdvertising->addServiceUUID(SERVICE_UUID);

  //不允许扫描回复
  pAdvertising->setScanResponse(false);

  //开始广播
  BLEDevice::startAdvertising();

}


//单片机程序主循环
void loop() {

  //主循环延迟0.1s
  delay(100);

  //获取特性的值
  String LEDControl = pCharacteristicLEDControl -> getValue();

  //判断获取到的特性数据内容，如果是00
  if (LEDControl == "00"){

    //关闭LED0
    digitalWrite(LED0, LED_OFF);

    //关闭LED1
    digitalWrite(LED1, LED_OFF);

    //设定LED状态的特性值
    pCharacteristicLEDStatus -> setValue("LED0 is OFF,LED1 is OFF");

  //如果是01
  } else if (LEDControl == "01"){

    //关闭LED0
    digitalWrite(LED0, LED_OFF);

    //打开LED1
    digitalWrite(LED1, LED_ON);

    //设定LED状态的特性值
    pCharacteristicLEDStatus -> setValue("LED0 is OFF,LED1 is ON");

    //如果是10
  } else if (LEDControl == "10"){

    //打开LED0
    digitalWrite(LED0, LED_ON);

    //关闭LED1
    digitalWrite(LED1, LED_OFF);

    //设定LED状态的特性值
    pCharacteristicLEDStatus -> setValue("LED0 is ON,LED1 is OFF");

  //如果是11
  } else if (LEDControl == "11"){

    //打开LED0
    digitalWrite(LED0, LED_ON);

    //打开LED1
    digitalWrite(LED1, LED_ON);

    //设定LED状态的特性值
    pCharacteristicLEDStatus -> setValue("LED0 is ON,LED1 is ON");

  }

}
