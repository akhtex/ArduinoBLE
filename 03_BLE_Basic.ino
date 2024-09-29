/*
程序名称：BLE_Basic
主要功能：最基础/简单的BLE程序
详细描述：实验芯片为ESP32-S3，创建一个只有广播功能，最基础的BLE服务

备注：烧录程序后可发现，我们在代码中未添加服务和特性，但BLE仍显示有两个服务（UUID 1801 和 UUID 1800）
在这其中我们观察UUID 1800，他的名字为Generic Attribute Profile，内含有三个特性Device Name（UUID 2A00）、Appearance（UUID 2A01）、Central Address Resolution（UUID 2A66）
在这三个特性里，我们可以轻松地观察到，蓝牙设备的名称就是由特性Device Name设定的，这些都是BLE的基础部分

关于UUID：蓝牙规范了UUID为0000xxxx-0000-1000-8000-00805F9B34FB，其中xxxx为可替换部分，也是UUID的缩写，其他部分均为固定内容
关于UUID规范，请参考 https://www.bluetooth.com/wp-content/uploads/Files/Specification/HTML/Assigned_Numbers/out/en/Assigned_Numbers.pdf
*/


//蓝牙必要的头文件
#include <BLEDevice.h>
#include <BLEServer.h>


//初始化单片机程序
void setup() {

  //初始化BLE服务，命名为 ESP32_BLE
  BLEDevice::init("ESP32_BLE");

  //创建一个BLE服务器
  BLEServer *pServer = BLEDevice::createServer();

  //获取一个广播器
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();

  //开始广播
  BLEDevice::startAdvertising();

}

void loop() {
  //主循环内无需内容
}