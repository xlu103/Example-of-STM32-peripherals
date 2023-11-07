1. 引脚 PA11，或者其他引脚都可以，单总线协议不挑引脚。
2. 在main函数中定义全局变量如下

```c
// 全局变量
double humi,temp;

// 以下两行在main函数中调用
	DHT11_Init();
	DHT11_Read_Data();

```
