/* *****************************************************************
 *
 * Download latest Blinker library here:
 * https://github.com/blinker-iot/blinker-library/archive/master.zip
 * 
 * 
 * Blinker is a cross-hardware, cross-platform solution for the IoT. 
 * It provides APP, device and server support, 
 * and uses public cloud services for data transmission and storage.
 * It can be used in smart home, data monitoring and other fields 
 * to help users build Internet of Things projects better and faster.
 * 
 * Make sure installed 2.7.4 or later ESP8266/Arduino package,
 * if use ESP8266 with Blinker.
 * https://github.com/esp8266/Arduino/releases
 * 
 * Make sure installed 1.0.4 or later ESP32/Arduino package,
 * if use ESP32 with Blinker.
 * https://github.com/espressif/arduino-esp32/releases
 * 
 * Docs: https://diandeng.tech/doc
 *       
 * 
 * *****************************************************************
 * 
 * Blinker 库下载地址:
 * https://github.com/blinker-iot/blinker-library/archive/master.zip
 * 
 * Blinker 是一套跨硬件、跨平台的物联网解决方案，提供APP端、设备端、
 * 服务器端支持，使用公有云服务进行数据传输存储。可用于智能家居、
 * 数据监测等领域，可以帮助用户更好更快地搭建物联网项目。
 * 
 * 如果使用 ESP8266 接入 Blinker,
 * 请确保安装了 2.7.4 或更新的 ESP8266/Arduino 支持包。
 * https://github.com/esp8266/Arduino/releases
 * 
 * 如果使用 ESP32 接入 Blinker,
 * 请确保安装了 1.0.4 或更新的 ESP32/Arduino 支持包。
 * https://github.com/espressif/arduino-esp32/releases
 * 
 * 文档: https://diandeng.tech/doc
 *       
 * 
 * *****************************************************************
 *
 *
 * -这个程序是我本人根据官方例程Blinker_Hello--Hello_Wifi修改得来的-
 * -使用ESP8266控制双路光耦继电器，可接入小米小爱，通过语音助手控制-
 * 
 * by Tracker     
 * My blog:https://baichuan2077.top
 * *****************************************************************/
 
#define BLINKER_WIFI
#define BLINKER_MIOT_MULTI_OUTLET

#include <Blinker.h>

//继电器控制口
int relay1 = D5;
int relay2 = D6;

bool oState[5] = { false };  //检测用

char auth[] = "3b7f4fafab03";
char ssid[] = "HY";
char pswd[] = "hy888888";

// 新建组件对象
BlinkerButton Button1("btn-1");
BlinkerButton Button2("btn-2");


// 按下按键1即会执行该函数
void button1_callback(const String & state)
{
    BLINKER_LOG("get button1 state: ", state);
    digitalWrite(relay1, !digitalRead(relay1));

    //oState[1] = !(oState[1]);

}

// 如果未绑定的组件1被触发，则会执行其中内容
void dataRead1(const String & data)
{
    BLINKER_LOG("Blinker readString: ", data);
}

// 按下按键2即会执行该函数
void button2_callback(const String & state)
{    
    BLINKER_LOG("get button2 state: ", state);
    digitalWrite(relay2, !digitalRead(relay2));

    //oState[2] = !(oState[2]);
    
}

// 如果未绑定的组件2被触发，则会执行其中内容
void dataRead2(const String & data)
{
    BLINKER_LOG("Blinker readString: ", data);
}


void miotPowerState(const String & state, uint8_t num)             //relay继电器电源状态
{
    
    BLINKER_LOG("need set power state: ", state,"num-back", num);
    
    unsigned char i = 0; //初始化
      if(num == 1)
      {
        i = relay1;
      }
      else if(num == 2)
      {
        i = relay2;
      }
      
    
    if (state == BLINKER_CMD_ON) 
    {
        digitalWrite(i, HIGH);

        BlinkerMIOT.powerState("on");
        BlinkerMIOT.print();

        digitalWrite(i, !digitalRead(i));

        oState[num] = true;
    }
    else if (state == BLINKER_CMD_OFF) 
    {
        digitalWrite(i, LOW);
        
        BlinkerMIOT.powerState("off");
        BlinkerMIOT.print();
        digitalWrite(i, !digitalRead(i));

        oState[num] = false;
    }
  
}

void miotQuery(int32_t queryCode, uint8_t num)
{
    BLINKER_LOG("AliGenie Query outlet: ", num,", codes: ", queryCode);

    switch (queryCode)
    {
        case BLINKER_CMD_QUERY_ALL_NUMBER :
            BLINKER_LOG("MIOT Query All");
            BlinkerMIOT.powerState(oState[num] ? "on" : "off", num);
            BlinkerMIOT.print();
            break;
        case BLINKER_CMD_QUERY_POWERSTATE_NUMBER :
            BLINKER_LOG("MIOT Query Power State");
            BlinkerMIOT.powerState(oState[num] ? "on" : "off", num);
            BlinkerMIOT.print();
            break;
        default :
            BlinkerMIOT.powerState(oState[num] ? "on" : "off", num);
            BlinkerMIOT.print();
            break;
    }
}

void setup()
{
    // 初始化串口
    Serial.begin(115200);
    BLINKER_DEBUG.stream(Serial);
    BLINKER_DEBUG.debugAll();
    
    // 初始化IO
    pinMode(relay1, OUTPUT);
    digitalWrite(relay1, HIGH);
    pinMode(relay2, OUTPUT);
    digitalWrite(relay2, HIGH);
    // 初始化blinker
    Blinker.begin(auth, ssid, pswd);
    Blinker.attachData(dataRead1);                 //获取1
    Blinker.attachData(dataRead2);                 //获取2

    Button1.attach(button1_callback);              //按钮1
    Button2.attach(button2_callback);              //按钮2
    
    BlinkerMIOT.attachPowerState(miotPowerState);  //注册电源函数1
    BlinkerMIOT.attachQuery(miotQuery);            //注册回调函数
}

void loop() {
    Blinker.run();
}
