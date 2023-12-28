# Device overview

In this part describes connections with microcontroller and device construction. Device consists of:

- base module - display, keyboard - where is hidden microcontroller and all ports
- custom modules - part connected to base module

&nbsp;

## Communication with custom modules

Custom modules was connected to base using 8P8C (RJ45) port and using i2c protocol.
Cable description:

1. SDA
2. SCL
3. EMPTY
4. +3.3V
5. +5V
6. EMPTY
7. GND
8. GND

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
|           |                 | 11  |                    | 30  |                 |           |
|           |                 | 12  |                    | 29  |                 |           |
|           |                 | 13  |                    | 28  |                 |           |
|           |                 | 14  |                    | 27  |                 |           |
|           |                 | 15  |                    | 26  |                 |           |
|           |                 | 16  |                    | 25  |                 |           |
|           |                 | 17  |                    | 24  |                 |           |
|           |                 | 18  |                    | 23  |                 |           |
|           |                 | 19  |                    | 22  |                 |           |
|           |                 | 20  |                    | 21  |                 |           |

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
