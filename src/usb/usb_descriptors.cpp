/* 
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include "tusb.h"
#include "usbMode.hpp"

#ifdef __cplusplus
extern "C" {
#endif

/* A combination of interfaces must have a unique product id, since PC will save device driver after the first plug.
 * Same VID/PID with different interface e.g MSC (first), then CDC (later) will possibly cause system error on PC.
 *
 * Auto ProductID layout's Bitmap:
 *   [MSB]       MIDI | HID | MSC | CDC          [LSB]
 */
#define _PID_MAP(itf, n)  ( (CFG_CDC_TUD_##itf) << (n) )
#define _PID_MAP_GPAD(itf, n)  ( (CFG_GPAD_TUD_##itf) << (n) )
#define USB_PID_CDC           (0x4000 | _PID_MAP(CDC, 0) | _PID_MAP(MSC, 1) | _PID_MAP(HID, 2) | \
                           _PID_MAP(MIDI, 3) | _PID_MAP(VENDOR, 4) )

#define USB_PID_GPAD           (0x4000 | _PID_MAP(CDC, 0) | _PID_MAP(MSC, 1) | _PID_MAP(HID, 2) | \
                           _PID_MAP(MIDI, 3) | _PID_MAP(VENDOR, 4) )

#define USB_PID_DESK_DECK      (0x4000 | _PID_MAP(CDC, 0) | _PID_MAP(MSC, 1) | _PID_MAP(HID, 2) | \
                           _PID_MAP(MIDI, 3) | _PID_MAP(VENDOR, 4) )


//--------------------------------------------------------------------+
// Device Descriptors
//--------------------------------------------------------------------+
tusb_desc_device_t const desc_device_cdc =
        {
                .bLength            = sizeof(tusb_desc_device_t),
                .bDescriptorType    = TUSB_DESC_DEVICE,
                .bcdUSB             = 0x0200,

                // Use Interface Association Descriptor (IAD) for CDC
                // As required by USB Specs IAD's subclass must be common class (2) and protocol must be IAD (1)
                .bDeviceClass       = TUSB_CLASS_MISC,
                .bDeviceSubClass    = MISC_SUBCLASS_COMMON,
                .bDeviceProtocol    = MISC_PROTOCOL_IAD,
                .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,

                .idVendor           = 0xCafe,
                .idProduct          = USB_PID_CDC + 22,
                .bcdDevice          = 0x0100,

                .iManufacturer      = 0x01,
                .iProduct           = 0x02,
                .iSerialNumber      = 0x03,

                .bNumConfigurations = 0x01
        };

tusb_desc_device_t const desc_device_gPad =
        {
                .bLength            = sizeof(tusb_desc_device_t),
                .bDescriptorType    = TUSB_DESC_DEVICE,
                .bcdUSB             = 0x0200,

                // Use Interface Association Descriptor (IAD) for CDC
                // As required by USB Specs IAD's subclass must be common class (2) and protocol must be IAD (1)
                .bDeviceClass       = TUSB_CLASS_HID,
                .bDeviceSubClass    = 0x00,
                .bDeviceProtocol    = 0x00,
                .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,

                .idVendor           = 0xCafe,
                .idProduct          = USB_PID_GPAD + 11,
                .bcdDevice          = 0x0100,

                .iManufacturer      = 0x01,
                .iProduct           = 0x02,
                .iSerialNumber      = 0x03,

                .bNumConfigurations = 0x01
        };

tusb_desc_device_t const desc_device_deskDeck =
        {
                .bLength            = sizeof(tusb_desc_device_t),
                .bDescriptorType    = TUSB_DESC_DEVICE,
                .bcdUSB             = 0x0200,

                // Use Interface Association Descriptor (IAD) for CDC
                // As required by USB Specs IAD's subclass must be common class (2) and protocol must be IAD (1)
                .bDeviceClass       = TUSB_CLASS_HID,
                .bDeviceSubClass    = 0x00,
                .bDeviceProtocol    = 0x00,
                .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,

                .idVendor           = 0xCafe,
                .idProduct          = USB_PID_DESK_DECK,
                .bcdDevice          = 0x0100,

                .iManufacturer      = 0x01,
                .iProduct           = 0x02,
                .iSerialNumber      = 0x03,

                .bNumConfigurations = 0x01
        };

// Invoked when received GET DEVICE DESCRIPTOR
// Application return pointer to descriptor
uint8_t const *tud_descriptor_device_cb(void) {
    if (usbCMode == USB_SERIAL)
        return (uint8_t const *) &desc_device_cdc;
    else if (usbCMode == USB_JOYSTICK)
        return (uint8_t const *) &desc_device_gPad;
    else if (usbCMode == USB_DESK) {
        return (uint8_t const *) &desc_device_deskDeck;
    }
}

//--------------------------------------------------------------------+
// HID Report Descriptor
//--------------------------------------------------------------------+

uint8_t const desc_joystick_report[] = {
        TUD_HID_REPORT_DESC_GAMEPAD_DASHBOARD (HID_REPORT_ID(1))
};

uint8_t const desc_deskDeck_report[] = {
        TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(1))
};

// Invoked when received GET HID REPORT DESCRIPTOR
// Application return pointer to descriptor
// Descriptor contents must exist long enough for transfer to complete
uint8_t const *tud_hid_descriptor_report_cb(uint8_t instance) {
    if (usbCMode == USB_JOYSTICK)
        return desc_joystick_report;
    else if (usbCMode == USB_DESK)
        return desc_deskDeck_report;
}

//--------------------------------------------------------------------+
// Configuration Descriptor
//--------------------------------------------------------------------+
enum {
    ITF_NUM_CDC_0 = 0,
    ITF_NUM_CDC_0_DATA,
    ITF_NUM_CDC_TOTAL
};

enum {
    ITF_NUM_HID,
    ITF_NUM_HID_TOTAL
};

#define CONFIG_TOTAL_SERIAL_LEN    (TUD_CONFIG_DESC_LEN + CFG_TUD_CDC * TUD_CDC_DESC_LEN)

#define CONFIG_TOTAL_JOYSTICK_LEN    (TUD_CONFIG_DESC_LEN + TUD_HID_DESC_LEN)

#if CFG_TUSB_MCU == OPT_MCU_LPC175X_6X || CFG_TUSB_MCU == OPT_MCU_LPC177X_8X || CFG_TUSB_MCU == OPT_MCU_LPC40XX
// LPC 17xx and 40xx endpoint type (bulk/interrupt/iso) are fixed by its number
// 0 control, 1 In, 2 Bulk, 3 Iso, 4 In etc ...
#define EPNUM_CDC_0_NOTIF   0x81
#define EPNUM_CDC_0_OUT     0x02
#define EPNUM_CDC_0_IN      0x82

#define EPNUM_CDC_1_NOTIF   0x84
#define EPNUM_CDC_1_OUT     0x05
#define EPNUM_CDC_1_IN      0x85

#elif CFG_TUSB_MCU == OPT_MCU_SAMG || CFG_TUSB_MCU == OPT_MCU_SAMX7X
// SAMG & SAME70 don't support a same endpoint number with different direction IN and OUT
//    e.g EP1 OUT & EP1 IN cannot exist together
#define EPNUM_CDC_0_NOTIF   0x81
#define EPNUM_CDC_0_OUT     0x02
#define EPNUM_CDC_0_IN      0x83

#define EPNUM_CDC_1_NOTIF   0x84
#define EPNUM_CDC_1_OUT     0x05
#define EPNUM_CDC_1_IN      0x86

#else
#define EPNUM_CDC_0_NOTIF   0x81
#define EPNUM_CDC_0_OUT     0x02
#define EPNUM_CDC_0_IN      0x82
#define EPNUM_GPAD          0x81
#endif

uint8_t const desc_serial_fs_configuration[] =
        {
                // Config number, interface count, string index, total length, attribute, power in mA
                TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_CDC_TOTAL, 0, CONFIG_TOTAL_SERIAL_LEN,
                                      TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),

                // 1st CDC: Interface number, string index, EP notification address and size, EP data address (out, in) and size.
                TUD_CDC_DESCRIPTOR(ITF_NUM_CDC_0, 4, EPNUM_CDC_0_NOTIF, 8, EPNUM_CDC_0_OUT, EPNUM_CDC_0_IN, 64),
        };

uint8_t const desc_joystick_configuration[] =
        {
                // Config number, interface count, string index, total length, attribute, power in mA
                TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_HID_TOTAL, 0, CONFIG_TOTAL_JOYSTICK_LEN,
                                      TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),

                // Interface number, string index, protocol, report descriptor len, EP In address, size & polling interval
                TUD_HID_DESCRIPTOR(ITF_NUM_HID, 0, HID_ITF_PROTOCOL_NONE, sizeof(desc_joystick_report), EPNUM_GPAD,
                                   CFG_TUD_HID_EP_BUFSIZE, 1),
        };

uint8_t const desc_deskDeck_configuration[] =
        {
                // Config number, interface count, string index, total length, attribute, power in mA
                TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_HID_TOTAL, 0, CONFIG_TOTAL_JOYSTICK_LEN,
                                      TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),

                // Interface number, string index, protocol, report descriptor len, EP In address, size & polling interval
                TUD_HID_DESCRIPTOR(ITF_NUM_HID, 0, HID_ITF_PROTOCOL_NONE, sizeof(desc_deskDeck_report), EPNUM_GPAD,
                                   CFG_TUD_HID_EP_BUFSIZE, 1),
        };
/*
//TODO highspeed
#if TUD_OPT_HIGH_SPEED
uint8_t const desc_hs_configuration[] =
{
  // Config number, interface count, string index, total length, attribute, power in mA
  TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, CONFIG_TOTAL_LEN, TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),

  // 1st CDC: Interface number, string index, EP notification address and size, EP data address (out, in) and size.
  TUD_CDC_DESCRIPTOR(ITF_NUM_CDC_0, 4, EPNUM_CDC_0_NOTIF, 8, EPNUM_CDC_0_OUT, EPNUM_CDC_0_IN, 512),

  // 2nd CDC: Interface number, string index, EP notification address and size, EP data address (out, in) and size.
  TUD_CDC_DESCRIPTOR(ITF_NUM_CDC_1, 4, EPNUM_CDC_1_NOTIF, 8, EPNUM_CDC_1_OUT, EPNUM_CDC_1_IN, 512),
};
#endif
*/
// Invoked when received GET CONFIGURATION DESCRIPTOR
// Application return pointer to descriptor
// Descriptor contents must exist long enough for transfer to complete
uint8_t const *tud_descriptor_configuration_cb(uint8_t index) {
    (void) index; // for multiple configurations

#if TUD_OPT_HIGH_SPEED
    // Although we are highspeed, host may be fullspeed.
    return (tud_speed_get() == TUSB_SPEED_HIGH) ?  desc_hs_configuration : desc_fs_configuration;
#else
    if (usbCMode == USB_SERIAL)
        return desc_serial_fs_configuration;
    else if (usbCMode == USB_JOYSTICK)
        return desc_joystick_configuration;
    else if (usbCMode == USB_DESK)
        return desc_deskDeck_configuration;

#endif
}

//--------------------------------------------------------------------+
// String Descriptors
//--------------------------------------------------------------------+

// array of pointer to string descriptors
char const *string_desc_arr[] =
        {
                (const char[]) {0x09, 0x04}, // 0: is supported language is English (0x0409)
                "RaspberryPi",                     // 1: Manufacturer
                "Raspberry Pi Desk",              // 2: Product
                "123456",                      // 3: Serials, should use chip ID
                "TinyUSB CDC",                 // 4: CDC Interface
        };

static uint16_t _desc_str[32];

// Invoked when received GET STRING DESCRIPTOR request
// Application return pointer to descriptor, whose contents must exist long enough for transfer to complete
uint16_t const *tud_descriptor_string_cb(uint8_t index, uint16_t langid) {
    (void) langid;

    uint8_t chr_count;

    if (index == 0) {
        memcpy(&_desc_str[1], string_desc_arr[0], 2);
        chr_count = 1;
    } else {
        // Note: the 0xEE index string is a Microsoft OS 1.0 Descriptors.
        // https://docs.microsoft.com/en-us/windows-hardware/drivers/usbcon/microsoft-defined-usb-descriptors

        if (!(index < sizeof(string_desc_arr) / sizeof(string_desc_arr[0]))) return NULL;

        const char *str = string_desc_arr[index];

        // Cap at max char
        chr_count = strlen(str);
        constexpr size_t max_count = sizeof(_desc_str) / sizeof(_desc_str[0]) - 1; // -1 for string type
        if ( chr_count > max_count ) chr_count = max_count;

        // Convert ASCII string into UTF-16
        for (uint8_t i = 0; i < chr_count; i++) {
            _desc_str[1 + i] = str[i];
        }
    }

    // first byte is length (including header), second byte is string type
    _desc_str[0] = (TUSB_DESC_STRING << 8) | (2 * chr_count + 2);

    return _desc_str;
}

#ifdef __cplusplus
}
#endif
