#include <avr/pgmspace.h>
#include "xmega/usb_xmega.h"

// Notes:
// Fill in VID/PID in device_descriptor
// Fill in msft_extended for WCID
// WCID request ID can be changed below
// Other options in usb.h
// Additional compiler flags: -std=gnu99 -fno-strict-aliasing -Wstrict-prototypes -fno-jump-tables

#define WCID_REQUEST_ID			0x22
#define WCID_REQUEST_ID_STR		u"\x22"


const USB_DeviceDescriptor PROGMEM device_descriptor = {
	.bLength = sizeof(USB_DeviceDescriptor),
	.bDescriptorType = USB_DTYPE_Device,

	.bcdUSB                 = 0x0200,
	.bDeviceClass           = USB_CSCP_VendorSpecificClass,
	.bDeviceSubClass        = USB_CSCP_NoDeviceSubclass,
	.bDeviceProtocol        = USB_CSCP_NoDeviceProtocol,

	.bMaxPacketSize0        = 64,
	.idVendor               = 0x9999,
	.idProduct              = 0x0034,
	.bcdDevice              = 0x0100,

	.iManufacturer          = 0x01,
	.iProduct               = 0x02,
	.iSerialNumber          = 0,

	.bNumConfigurations     = 1
};

typedef struct ConfigDesc {
	USB_ConfigurationDescriptor Config;
	USB_InterfaceDescriptor Interface0;
	USB_EndpointDescriptor DataInEndpoint;
	USB_EndpointDescriptor DataOutEndpoint;

} ConfigDesc;

const __flash ConfigDesc configuration_descriptor = {
	.Config = {
		.bLength = sizeof(USB_ConfigurationDescriptor),
		.bDescriptorType = USB_DTYPE_Configuration,
		.wTotalLength  = sizeof(ConfigDesc),
		.bNumInterfaces = 1,
		.bConfigurationValue = 1,
		.iConfiguration = 0,
		.bmAttributes = USB_CONFIG_ATTR_BUSPOWERED,
		.bMaxPower = USB_CONFIG_POWER_MA(500)
	},
	.Interface0 = {
		.bLength = sizeof(USB_InterfaceDescriptor),
		.bDescriptorType = USB_DTYPE_Interface,
		.bInterfaceNumber = 0,
		.bAlternateSetting = 0,
		.bNumEndpoints = 2,
		.bInterfaceClass = USB_CSCP_VendorSpecificClass,
		.bInterfaceSubClass = 0x00,
		.bInterfaceProtocol = 0x00,
		.iInterface = 0
	},
	.DataInEndpoint = {
		.bLength = sizeof(USB_EndpointDescriptor),
		.bDescriptorType = USB_DTYPE_Endpoint,
		.bEndpointAddress = 0x81,
		.bmAttributes = (USB_EP_TYPE_BULK | ENDPOINT_ATTR_NO_SYNC | ENDPOINT_USAGE_DATA),
		.wMaxPacketSize = 64,
		.bInterval = 0x00
	},
	.DataOutEndpoint = {
		.bLength = sizeof(USB_EndpointDescriptor),
		.bDescriptorType = USB_DTYPE_Endpoint,
		.bEndpointAddress = 0x2,
		.bmAttributes = (USB_EP_TYPE_BULK | ENDPOINT_ATTR_NO_SYNC | ENDPOINT_USAGE_DATA),
		.wMaxPacketSize = 64,
		.bInterval = 0x00
	},
};

const __flash USB_StringDescriptor language_string = {
	.bLength = USB_STRING_LEN(1),
	.bDescriptorType = USB_DTYPE_String,
	.bString = {USB_LANGUAGE_EN_US},
};

const __flash USB_StringDescriptor manufacturer_string = {
	.bLength = USB_STRING_LEN(13),
	.bDescriptorType = USB_DTYPE_String,
	.bString = u"Nonolith Labs"
};

const __flash USB_StringDescriptor product_string = {
	.bLength = USB_STRING_LEN(14),
	.bDescriptorType = USB_DTYPE_String,
	.bString = u"Example Device"
};





const __flash USB_StringDescriptor msft_string = {
	.bLength = 18,
	.bDescriptorType = USB_DTYPE_String,
	.bString = u"MSFT100" WCID_REQUEST_ID_STR
};


__attribute__((__aligned__(4))) const USB_MicrosoftCompatibleDescriptor msft_compatible = {
	.dwLength = sizeof(USB_MicrosoftCompatibleDescriptor) +
				1*sizeof(USB_MicrosoftCompatibleDescriptor_Interface),
	.bcdVersion = 0x0100,
	.wIndex = 0x0004,
	.bCount = 1,
	.reserved = {0, 0, 0, 0, 0, 0, 0},
	.interfaces = {
		{
			.bFirstInterfaceNumber = 0,
			.reserved1 = 0x01,
			.compatibleID = "WINUSB\0\0",
			.subCompatibleID = {0, 0, 0, 0, 0, 0, 0, 0},
			.reserved2 = {0, 0, 0, 0, 0, 0},
		},
	}
};

__attribute__((__aligned__(4))) const USB_MicrosoftExtendedPropertiesDescriptor msft_extended = {
	.dwLength = sizeof(USB_MicrosoftExtendedPropertiesDescriptor),
	.bcdVersion = 0x0100,
	.wIndex = 0x0005,
	.wCount = 2,

	.dwPropLength = 132,
	.dwType = 1,
	.wNameLength = 40,
	.name = L"DeviceInterfaceGUID\0",
	.dwDataLength = 78,
	.data = L"{42314231-5A81-49F0-BC3D-A4FF138216D7}\0",

	.dwPropLength2 = 14 + 12 + 26,
	.dwType2 = 1,
	.wNameLength2 = 6*2,
	.name2 = L"Label\0",
	.dwDataLength2 = 13*2,
	.data2 = L"Name56789AB\0",
};


/*
__attribute__((__aligned__(4))) const USB_MicrosoftExtendedPropertiesDescriptor msft_extended = {
	.dwLength = sizeof(USB_MicrosoftExtendedPropertiesDescriptor),
	.dwLength = 142,
	.bcdVersion = 0x0100,
	.wIndex = 0x0005,
	.wCount = 1,
	.dwPropLength = 132,
	.dwType = 1,
	.wNameLength = 40,
	.name = L"DeviceInterfaceGUID\0",
	.dwDataLength = 78,
	.data = L"{42314231-5A81-49F0-BC3D-A4FF138216D7}\0",
};
*/
/*
__attribute__((__aligned__(4))) const USB_MicrosoftExtendedPropertiesDescriptor msft_extended = {
	.dwLength = sizeof(USB_MicrosoftExtendedPropertiesDescriptor),
	.dwLength = 146,
	.bcdVersion = 0x0100,
	.wIndex = 0x0005,
	.wCount = 1,
	.dwPropLength = 136,
	.dwType = 7,
	.wNameLength = 42,
	.name = L"DeviceInterfaceGUIDs\0",
	.dwDataLength = 80,
	.data = L"{42314231-5A81-49F0-BC3D-A4FF138216D7}\0\0",
};
*/



uint16_t usb_cb_get_descriptor(uint8_t type, uint8_t index, const uint8_t** ptr) {
	const void* address = NULL;
	uint16_t size    = 0;

	switch (type) {
		case USB_DTYPE_Device:
			address = &device_descriptor;
			size    = sizeof(USB_DeviceDescriptor);
			break;
		case USB_DTYPE_Configuration:
			address = &configuration_descriptor;
			size    = sizeof(ConfigDesc);
			break;
		case USB_DTYPE_String:
			switch (index) {
				case 0x00:
					address = &language_string;
					break;
				case 0x01:
					address = &manufacturer_string;
					break;
				case 0x02:
					address = &product_string;
					break;
				case 0xEE:
					address = &msft_string;
					break;
			}
			size = pgm_read_byte(&((USB_StringDescriptor*)address)->bLength);
			break;
	}

	*ptr = usb_ep0_from_progmem(address, size);
	return size;
}

void usb_cb_reset(void) {

}

bool usb_cb_set_configuration(uint8_t config) {
	if (config <= 1) {
		return true;
	} else {
		return false;
	}
}

void usb_cb_completion(void) {

}



void handle_msft_compatible(void) {
	const uint8_t *data;
	uint16_t len;
	if (usb_setup.wIndex == 0x0005) {
		len = msft_extended.dwLength;
		data = (const uint8_t *)&msft_extended;
	} else if (usb_setup.wIndex == 0x0004) {
		len = msft_compatible.dwLength;
		data = (const uint8_t *)&msft_compatible;
	} else {
		return usb_ep0_stall();
	}
	if (len > usb_setup.wLength) {
		len = usb_setup.wLength;
	}
	usb_ep_start_in(0x80, data, len, true);
	usb_ep0_out();
}

void usb_cb_control_setup(void) {
	uint8_t recipient = usb_setup.bmRequestType & USB_REQTYPE_RECIPIENT_MASK;
	if (recipient == USB_RECIPIENT_DEVICE) {
		switch(usb_setup.bRequest) {
			case WCID_REQUEST_ID:
				return handle_msft_compatible();
		}
	} else if (recipient == USB_RECIPIENT_INTERFACE) {
		switch(usb_setup.bRequest) {
			case WCID_REQUEST_ID:
				return handle_msft_compatible();
		}
	}
	return usb_ep0_stall();
}

void usb_cb_control_in_completion(void) {

}

void usb_cb_control_out_completion(void) {

}

bool usb_cb_set_interface(uint16_t interface, uint16_t altsetting) {
	return false;
}
