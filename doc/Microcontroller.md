# Device overview

In this part describes connections with microcontroller and device construction. Device consists of:

- base module - display, keyboard - where is hidden microcontroller and all ports
- custom modules - part connected to base module

&nbsp;

## Communication with custom modules

Custom modules was connected to base using 8P8C (RJ45) port and using i2c protocol.
Cable description:

| PIN number |    Color     | Connection |
|:----------:|:------------:|:----------:|
|     1.     | green-white  |    SDA     |
|     2.     |    green     |    SCL     |
|     3.     | orange-white |    GND     |
|     4.     |    orange    |    GND     |
|     5.     |  blue-white  |   +3.3V    |
|     6.     |     blue     |   +3.3V    |
|     7.     | brown-white  |    +5V     |
|     8.     |    brown     |    +5V     |


&nbsp;

## Pinout

| Connected | Pin description | PIN | &nbsp;&nbsp;&nbsp; | PIN | Pin description | Connected |
|:---------:|:---------------:|:---:|--------------------|:---:|:---------------:|:---------:|
|  Display  |  GP0, I2C0 SDA  |  1  |                    | 40  |      VBUS       |           |
|  Display  |  GP1, I2C0 SCL  |  2  |                    | 39  |      VSYS       |           |
|           |     **GND**     |  3  |                    | 38  |     **GND**     |           |
| KYB_SLV0  |       GP2       |  4  |                    | 37  |     3V3_EN      |           |
| KYB_SLV1  |       GP3       |  5  |                    | 36  |    3V3(OUT)     |           |
| KYB_SLV2  |       GP4       |  6  |                    | 35  |                 |           |
| KYB_SLV3  |       GP5       |  7  |                    | 34  |                 |           |
|           |     **GND**     |  8  |                    | 33  |                 |           |
| KYB_MSTR0 |       GP6       |  9  |                    | 32  |                 |           |
| KYB_MSTR1 |       GP7       | 10  |                    | 31  |                 |           |
|           |       GP8       | 11  |                    | 30  |                 |           |
|           |       GP9       | 12  |                    | 29  |                 |           |
|           |     **GND**     | 13  |                    | 28  |                 |           |
|           |      GP10       | 14  |                    | 27  |                 |           |
|           |      GP11       | 15  |                    | 26  |                 |           |
|           |      GP12       | 16  |                    | 25  |                 |           |
|           |      GP13       | 17  |                    | 24  |                 |           |
|           |     **GND**     | 18  |                    | 23  |                 |           |
|  Modules  | GP14, I2C1, SDA | 19  |                    | 22  |      GP17       |  Buzzer   |
|  Modules  | GP15, I2C1, SCL | 20  |                    | 21  |      GP16       |           |

&nbsp;

## Keyboard

![img_1.png](img/img_1.png)

Blue lines is master.

Orange lines is slave.

Starting from left:

| PIN number | Description |
|:----------:|:-----------:|
|     1      |  KYB_MSTR1  |
|     2      |  KYB_MSTR0  |
|     3      |  KYB_SLV0   |
|     4      |  KYB_SLV1   |
|     5      |  KYB_SLV2   |
|     6      |  KYB_SLV3   |
