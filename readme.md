# 基于Lidar的室内三维建模系统

## Final Release

​		本课题采用Lidar传感器，基于测距、三维建模课题展开研究，设计了一整套室内三维数据采集及上位机显示方案。其中包括：设计研究了多种机械云台扫描方案、点云数据处理方案和三维实时显示方案，并详细对比讨论了各个方案的实施效果和优缺点。本课题重点针对Lidar传感器自身的采集缺陷问题展开深入探讨， 提出了多种解决方案，并测试了实现效果。最终对比各方案的适用条件及特点，经过综合调试和运行，得出了有较高可靠性、较好稳定性的三维扫描建模解决方案，实现 了对室内环境较高精度的三维图像数字重建。 

![IMG_20230601_145654-01](https://github.com/shanshili/LiDAR-3D-Reconstructor/blob/1d0d42c7706f8927510bf580b446569e7fa0af27/readme.assets/IMG_20230601_145654-01.jpeg)

## 系统精度测试

![IMG_20230601_145941-01](https://github.com/shanshili/LiDAR-3D-Reconstructor/blob/1d0d42c7706f8927510bf580b446569e7fa0af27/readme.assets/IMG_20230601_145941-01.jpeg)

![1](https://github.com/shanshili/LiDAR-3D-Reconstructor/blob/main/readme.assets/1.jpg)

![4](https://github.com/shanshili/LiDAR-3D-Reconstructor/blob/main/readme.assets/4.jpg)

## 早期版本

![桌子竖起来](https://github.com/shanshili/LiDAR-3D-Reconstructor/blob/main/readme.assets/%E6%A1%8C%E5%AD%90%E7%AB%96%E8%B5%B7%E6%9D%A5.jpeg)

![image16](https://github.com/shanshili/LiDAR-3D-Reconstructor/blob/main/readme.assets/image16.png)

## 拓展测试：环绕扫描合成

<img src="https://github.com/shanshili/LiDAR-3D-Reconstructor/blob/main/readme.assets/IMG_20230611_211254.jpg" alt="IMG_20230611_211254" style="zoom:10%;" />

![mmexport1686489638593](https://github.com/shanshili/LiDAR-3D-Reconstructor/blob/main/readme.assets/mmexport1686489638593.png)





# 方案Ⅰ 双舵机

![QQ图片20220529181839](https://github.com/shanshili/LiDAR-3D-Reconstructor/blob/main/readme.assets/QQ%E5%9B%BE%E7%89%8720220529181839.jpg)

![IMG_20220420_201532-01](https://github.com/shanshili/LiDAR-3D-Reconstructor/blob/main/readme.assets/IMG_20220420_201532-01.jpeg)

**主控：STM32F103ZET6**

**OC2  PB5 **

**OC3  PB0**

**JY901S(USART3):**
	**USB-TTL                   STM32Core              		JY901s 仅接收角度 俯仰x 横滚y 航向z**
	**VCC          -----           VCC                ----        	 VCC**
	**TX             -----           RX1  (PA10)   
	RX             -----           TX1  (PA9)**
	**GND          -----           GND               ----       	GND**
									   **RX2  (PB11)    ----             TX**
										**TX2  (PB10)    ----             RX**
**LIDAR(USART2):**
	**TX PA2** 
	**RX PA3** 



## 供电

单片机由电脑供电，并连接串口，使用MATLAB、Python上位机显示

双舵机由7v电池供电

JY901S和LIDAR由单片机供电

## 问题

1. 角度数据与距离数据匹配问题
2. 姿态传感器欧拉角问题：不能超过90°；轴之间联动效应



# 方案Ⅱ-2：丝杠+单线雷达

![image-20250316125317732](https://github.com/shanshili/LiDAR-3D-Reconstructor/blob/main/readme.assets/image-20250316125317732.png)

**主控：stm32f103rct6**

**PC2 PC3 PC4 PC5 ** **01  03  02  04**电机控制：GPIO

**TX PC10  ** **RX PC11**  **VCC GND**电机雷达：UART4、中断

**SCLK b8**  **SDIN b9** **VCC GND**OLED:电机距离实时显示

**RX1  PA10**   **TX1  PA9 ** **VCC GND**串口打印：USART1 

**OC2  PB5** 舵机：PWM              

**RX2  PB11**   **TX2  PB10**  **VCC GND**jy901s:  USART3、中断                                                                                                                                                                                                    

**TX PA2**  **RX PA3** **VCC GND**舵机雷达：USART2、中断

（电机雷达）（电机复位/不复位直接读取）（舵机复位）→JY901S→舵机雷达→电机雷达→电机→舵机

→JY901S→舵机雷达→电机雷达→电机→舵机

##  供电

单片机由12V转5V模块供电，与电脑只连接串口

舵机由7V电池供电

外设由12V转5V模块供电

滑台由L298N驱动，该模块供电12V

## 开机不复位

## 自检系统

外设分别自检

## 扫描密度

密度可通过set进行调节(每次舵机转角匹配的电机转角个数)

## 系统舵机-jy901s

9%-66%

标定：

42%(168=400*42%) --0°  

19%(76=400*19%) --123°

## 系统丝杠

min  100
max  242 //系统不发热运行上限为242 距离上限370

round125 = 10mm

## JY901S上位机及接线

![image-20230511170115381](https://github.com/shanshili/LiDAR-3D-Reconstructor/blob/main/readme.assets/image-20230511170115381.png)

![image-20230508171606614](https://github.com/shanshili/LiDAR-3D-Reconstructor/blob/main/readme.assets/image-20230508171606614.png)



# 方案Ⅲ-XY双滑台

![image-20250316125327268](https://github.com/shanshili/LiDAR-3D-Reconstructor/blob/main/readme.assets/image-20250316125327268.png)

**PC2 PC3 PC4 PC5**
**01  03  02  04**

**PC6 PC7 PC8 PC9**
**01  03  02  04**

**红  黄  绿  蓝**
**A   B   A-  B-**

**LIDAR(USART2):**
	**TX PA2** 
	**RX PA3** 

**矩阵键盘PA8-PA15**