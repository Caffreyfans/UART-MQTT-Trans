# ESP8266串口透传MQTT

### 项目说明

**UART⇄MQTT**

可视化配置`WIFI`、`MQTT`、订阅主题、连接状态，**持久化存储**，下一次重启自动连接，长按`flash`按钮清除连接信息、自定义控制日志输出

以下代码都在 `ESP8266 Node MCU`验证过(`ESP12F`模组)

### 配置页面

|                           设备信息                           |                           MQTT配置                           |                           WIFI配置                           |
| :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: |
| ![image-20231130162217993](https://sanxiadaba-pic.oss-cn-hangzhou.aliyuncs.com/img/image-20231130162217993.png) | ![image-20231130162251171](https://sanxiadaba-pic.oss-cn-hangzhou.aliyuncs.com/img/image-20231130162251171.png) | ![image-20231130162242987](https://sanxiadaba-pic.oss-cn-hangzhou.aliyuncs.com/img/image-20231130162242987.png) |



### 串口、MQTT发送接收

|               串口发送长文本，MQTT订阅消息接收               |                    MQTT消息推送、串口接收                    |
| :----------------------------------------------------------: | :----------------------------------------------------------: |
| ![image-20231130164454869](https://sanxiadaba-pic.oss-cn-hangzhou.aliyuncs.com/img/image-20231130164454869.png) | ![image-20231130164423978](https://sanxiadaba-pic.oss-cn-hangzhou.aliyuncs.com/img/image-20231130164423978.png) |



### 日志输出

|                     日志输出（默认开启）                     |
| :----------------------------------------------------------: |
| ![image-20231201111801450](https://sanxiadaba-pic.oss-cn-hangzhou.aliyuncs.com/img/image-20231201111801450.png) |





### **固件使用步骤**

1、下载烧录固件（固件在`build`目录下）

| ![image-20231130165151013](https://sanxiadaba-pic.oss-cn-hangzhou.aliyuncs.com/img/image-20231130165151013.png) | ![image-20231130165202264](https://sanxiadaba-pic.oss-cn-hangzhou.aliyuncs.com/img/image-20231130165202264.png) |
| ------------------------------------------------------------ | ------------------------------------------------------------ |

2、`ESP8266`会启动一个热点，连接上之后访问（`192.168.4.1`）会跳转到配置页面

| ![image-20231130165433039](https://sanxiadaba-pic.oss-cn-hangzhou.aliyuncs.com/img/image-20231130165433039.png) | ![image-20231130165535946](https://sanxiadaba-pic.oss-cn-hangzhou.aliyuncs.com/img/image-20231130165535946.png) |
| ------------------------------------------------------------ | ------------------------------------------------------------ |

3、配置成功后点击保存

| ![image-20231130165615520](https://sanxiadaba-pic.oss-cn-hangzhou.aliyuncs.com/img/image-20231130165615520.png) | ![image-20231130165633811](https://sanxiadaba-pic.oss-cn-hangzhou.aliyuncs.com/img/image-20231130165633811.png) | ![image-20231130165649370](https://sanxiadaba-pic.oss-cn-hangzhou.aliyuncs.com/img/image-20231130165649370.png) |
| ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |

4、串口连接使用

|      | ![image-20231130164423978](https://sanxiadaba-pic.oss-cn-hangzhou.aliyuncs.com/img/image-20231130164423978.png) |
| ---- | ------------------------------------------------------------ |

5、日志输出

![调试口](https://sanxiadaba-pic.oss-cn-hangzhou.aliyuncs.com/img/调试口.png)



### 代码开发（`windows`环境）

1、准备好`ESP8266`的`Arduino`开发环境

- 安装`Arduino`软件（下载官网：[Software | Arduino](https://www.arduino.cc/en/software)）![image-20231201115618594](https://sanxiadaba-pic.oss-cn-hangzhou.aliyuncs.com/img/image-20231201115618594.png)

- 安装`ESP8266`的`Arduino`开发环境，注意是`3.1.12`版本，其它版本可能出现问题

  ![image-20231201122308180](https://sanxiadaba-pic.oss-cn-hangzhou.aliyuncs.com/img/image-20231201122308180.png)

- 烧录`ESP8266`的`Arduino`固件(已经烧录`Arduino`固件的不用烧录)

  ![image-20231201120003337](https://sanxiadaba-pic.oss-cn-hangzhou.aliyuncs.com/img/image-20231201120003337.png)



2、准备好`MQTT`服务器、串口助手、烧录工具

3、双击打开`Aduino`项目，首选项配置当前路径（重点）

| ![image-20231201114558214](https://sanxiadaba-pic.oss-cn-hangzhou.aliyuncs.com/img/image-20231201114558214.png) |
| ------------------------------------------------------------ |
| ![image-20231201114456697](https://sanxiadaba-pic.oss-cn-hangzhou.aliyuncs.com/img/image-20231201114456697.png) |

3、注意不要更新（尤其是`WIFI Manager`库）

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

1、串口波特率只能设置为`9600`（过高波特在测试下会出现收发数据分包等现象）

2、长按`flash`三秒左右等待模块LED闪烁，说明清除配置成功

3、串口向`MQTT`发送数据时，注意要有一个结尾标志，默认是`uart-mqtt`

4、在重启时，模块会通过串口打印自己信息

5、事先启动一个`MQTT`服务器，在配置页面可以配置`user` `password` `clientId`等

6、本固件已在`ESP8266MCU`模块上验证过

7、日志默认在`D1`也就是`GPIO16`输出日志，可在代码里自定义配置