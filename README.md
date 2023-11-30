# ESP8266串口透传MQTT

使用视频见：

可以在可视化界面配置`WIFI`、`MQTT`、订阅主题、连接状态，并且**持久化存储**，下一次重启自动连接，长按`flash`按钮清除信息复位

| ![image-20231130162217993](https://sanxiadaba-pic.oss-cn-hangzhou.aliyuncs.com/img/image-20231130162217993.png) | ![image-20231130162242987](https://sanxiadaba-pic.oss-cn-hangzhou.aliyuncs.com/img/image-20231130162242987.png) | ![image-20231130162251171](https://sanxiadaba-pic.oss-cn-hangzhou.aliyuncs.com/img/image-20231130162251171.png) |
| ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |

| ![image-20231130164454869](https://sanxiadaba-pic.oss-cn-hangzhou.aliyuncs.com/img/image-20231130164454869.png) | ![image-20231130164423978](https://sanxiadaba-pic.oss-cn-hangzhou.aliyuncs.com/img/image-20231130164423978.png) |
| ------------------------------------------------------------ | ------------------------------------------------------------ |



**使用步骤**

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









**注：**

1、串口波特率只能设置为`9600`（过高波特在测试下会出现收发数据分包等现象）

2、长按`flash`三秒左右等待模块LED闪烁，说明清楚配置

3、串口向`MQTT`发送数据时，注意要有一个结尾标志，默认是`uart-mqtt`

4、在重启时，模块会通过串口打印自己信息

5、事先启动一个`MQTT`服务器，在配置页面可以配置`user` `password` `clientId`等

6、本仓库、视频都是基于`ESP8266 Node MCU`，有任何问题可以联系我 sanxia19@foxmail.com