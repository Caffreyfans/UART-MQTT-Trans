# ESP8266串口透传MQTT

### 项目说明

**UART⇄MQTT**

可视化配置`WIFI`、`MQTT`、订阅主题、连接状态，**持久化存储**，下一次重启自动连接，长按`flash`按钮清除连接信息、自定义控制日志输出

以下代码都在 `ESP8266 Node MCU`验证过(`ESP12F`模组)

下面以`ESP8266 Node MCU`为例，视频演示链接：https://www.bilibili.com/video/BV1d94y1c7RA （注：视频演示的是核心功能的使用，`github`仓库会更新一些新功能）

### 配置页面

|                           设备信息                           |                           MQTT配置                           |                           WIFI配置                           |
| :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: |
| ![image-20231130162217993](https://sanxiadaba-pic.oss-cn-hangzhou.aliyuncs.com/img/image-20231130162217993.png) | ![企业微信截图_17037258401379](https://sanxiadaba-pic.oss-cn-hangzhou.aliyuncs.com/img/企业微信截图_17037258401379.png) | ![image-20231130162242987](https://sanxiadaba-pic.oss-cn-hangzhou.aliyuncs.com/img/image-20231130162242987.png) |



### 串口、MQTT发送接收

|               串口发送长文本，MQTT订阅消息接收               |                    MQTT消息推送、串口接收                    |
| :----------------------------------------------------------: | :----------------------------------------------------------: |
| ![image-20231130164454869](https://sanxiadaba-pic.oss-cn-hangzhou.aliyuncs.com/img/image-20231130164454869.png) | ![image-20231130164423978](https://sanxiadaba-pic.oss-cn-hangzhou.aliyuncs.com/img/image-20231130164423978.png) |



### 日志输出

|                     日志输出（默认开启）                     |
| :----------------------------------------------------------: |
| ![image-20231201111801450](https://sanxiadaba-pic.oss-cn-hangzhou.aliyuncs.com/img/image-20231201111801450.png) |





### **固件使用步骤**

1、下载烧录固件并烧录（固件在`build`目录下，烧录工具`ESP8266Flasher.exe`也在仓库里。

*注：使用这个软件烧录需要确保你的`ESP8266`自带自动下载电路（大部分都带，有的不带的需要点击一下`Flash`按键），确保端口、烧录固件选择正确*

| ![image-20231130165151013](https://sanxiadaba-pic.oss-cn-hangzhou.aliyuncs.com/img/image-20231130165151013.png) | ![image-20231130165202264](https://sanxiadaba-pic.oss-cn-hangzhou.aliyuncs.com/img/image-20231130165202264.png) |
| ------------------------------------------------------------ | ------------------------------------------------------------ |

2、`ESP8266`会启动一个热点，连接上热点之后（以`ESP`开头的热点）访问`192.168.4.1`会跳转到配置页面

| ![image-20231130165433039](https://sanxiadaba-pic.oss-cn-hangzhou.aliyuncs.com/img/image-20231130165433039.png) | ![image-20231130165535946](https://sanxiadaba-pic.oss-cn-hangzhou.aliyuncs.com/img/image-20231130165535946.png) |
| ------------------------------------------------------------ | ------------------------------------------------------------ |

3、点击选择`WIFI`，输入`WIFI`密码、配置`MQTT`之后点击保存

*注：事先准备一个`MQTT`服务器，`MQTT`服务器可以是外网，确保此`WIFI`可以访问你的`MQTT`服务器*，*建议先配置好`WIFI`保存后再配置`MQTT`*

| ![image-20231130165615520](https://sanxiadaba-pic.oss-cn-hangzhou.aliyuncs.com/img/image-20231130165615520.png) | ![企业微信截图_17037258401379](https://sanxiadaba-pic.oss-cn-hangzhou.aliyuncs.com/img/企业微信截图_17037258401379.png) | ![image-20231130165649370](https://sanxiadaba-pic.oss-cn-hangzhou.aliyuncs.com/img/image-20231130165649370.png) |
| ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |

4、串口连接使用（确保客户端、`ESP8266`都能够正常访问`MQTT`服务器）

*注：串口默认波特率为`9600`，串口发送`ESP8266`需要结束符，默认为`uart-mqtt`，二者都可以在配置页面修改(修改后重启模块)*

|      | ![image-20231130164423978](https://sanxiadaba-pic.oss-cn-hangzhou.aliyuncs.com/img/image-20231130164423978.png) |
| ---- | ------------------------------------------------------------ |

5、日志输出默认开启，日志输出引脚默认为`D0`（可在代码里自定义修改），波特率与透传串口相同，调试信息默认打印设备信息以及`ESP8266`与`MQTT`透传、接收的信息

![调试口](https://sanxiadaba-pic.oss-cn-hangzhou.aliyuncs.com/img/调试口.png)



### 代码开发（`windows`环境）

1、准备好`ESP8266`的`Arduino`开发环境

- 安装`Arduino`软件（下载官网：[Software | Arduino](https://www.arduino.cc/en/software)）这里是`1.8.19`版本(在我的测试中`Arduino IDE`版本`1.8.19`、`2.1.0`均正常) *注：安装过程中会让你安装一些组件，一路点安装、下一步即可*![image-20231201115618594](https://sanxiadaba-pic.oss-cn-hangzhou.aliyuncs.com/img/image-20231201115618594.png)

- 安装`ESP8266`的`Arduino`开发环境，先添加附加开发板管理网址`http://arduino.esp8266.com/stable/package_esp8266com_index.json`，然后安装开发板扩展包，注意是`3.1.12`版本，其它版本可能出现问题，下载库的时候可能会因为`github`被墙而无法下载，自行在网上搜索解决方案

  ![image-20231201130006353](https://sanxiadaba-pic.oss-cn-hangzhou.aliyuncs.com/img/image-20231201130006353.png)

![image-20231201125507908](https://sanxiadaba-pic.oss-cn-hangzhou.aliyuncs.com/img/image-20231201125507908.png)

![image-20231201130043913](https://sanxiadaba-pic.oss-cn-hangzhou.aliyuncs.com/img/image-20231201130043913.png)

![image-20231201133347768](https://sanxiadaba-pic.oss-cn-hangzhou.aliyuncs.com/img/image-20231201133347768.png)

- 烧录`ESP8266`的`Arduino`固件(已经烧录`Arduino`固件的不用烧录)

  ![image-20231201120003337](https://sanxiadaba-pic.oss-cn-hangzhou.aliyuncs.com/img/image-20231201120003337.png)



2、准备好`MQTT`服务器、串口助手、烧录工具

3、双击打开`Aduino`项目，在**首选项配置项目文件夹位置为D:\\UART-MQTT-Trans-master**(具体看自己路径)

| ![image-20231201114558214](https://sanxiadaba-pic.oss-cn-hangzhou.aliyuncs.com/img/image-20231201114558214.png) |
| ------------------------------------------------------------ |
| ![image-20231201134034984](https://sanxiadaba-pic.oss-cn-hangzhou.aliyuncs.com/img/image-20231201134034984.png) |

3、注意不要更新相关库文件版本（尤其是`WIFI Manager`库）

![image-20231201114231826](https://sanxiadaba-pic.oss-cn-hangzhou.aliyuncs.com/img/image-20231201114231826.png)

4、配置开发板信息

| ![image-20231201114822875](https://sanxiadaba-pic.oss-cn-hangzhou.aliyuncs.com/img/image-20231201114822875.png) |      |
| ------------------------------------------------------------ | ---- |

5、点击编译

![image-20231201122511046](https://sanxiadaba-pic.oss-cn-hangzhou.aliyuncs.com/img/image-20231201122511046.png)

6、数据线连接好后配置好端口，可以点击上传

| ![image-20231201122712940](https://sanxiadaba-pic.oss-cn-hangzhou.aliyuncs.com/img/image-20231201122712940.png) | ![image-20231201122851936](https://sanxiadaba-pic.oss-cn-hangzhou.aliyuncs.com/img/image-20231201122851936.png) |
| ------------------------------------------------------------ | ------------------------------------------------------------ |

7、自定义修改代码后可以选择导出新的固件

| ![image-20231201123000228](https://sanxiadaba-pic.oss-cn-hangzhou.aliyuncs.com/img/image-20231201123000228.png) | ![image-20231201123120176](https://sanxiadaba-pic.oss-cn-hangzhou.aliyuncs.com/img/image-20231201123120176.png) |
| ------------------------------------------------------------ | ------------------------------------------------------------ |

8、代码可自定义，自己实现想要的功能，比如开启、关闭日志串口，修改日志串口的输出引脚，修改串口向`MQTT`的结尾标志字符串

![image-20231201123243421](https://sanxiadaba-pic.oss-cn-hangzhou.aliyuncs.com/img/image-20231201123243421.png)

![image-20231201123713932](https://sanxiadaba-pic.oss-cn-hangzhou.aliyuncs.com/img/image-20231201123713932.png)



**注：**

1、串口波特率默认设置为`9600`（可在配置页面自行更改，修改后需重启模块）

2、长按`flash`三秒左右等待模块LED闪烁，说明清除配置成功

3、串口向`MQTT`发送数据时，注意要有一个结尾标志，默认是`uart-mqtt`（可在配置页面自行更改，修改后需重启模块）

4、在重启时，模块会通过串口打印自己信息（注意串口波特率要匹配）

5、事先启动一个`MQTT`服务器，在配置页面可以配置`user` `password` `clientId`等

6、本固件已在`ESP8266MCU`模块上验证过

7、日志默认在`D1`也就是`GPIO16`输出日志（可在代码里自行配置）