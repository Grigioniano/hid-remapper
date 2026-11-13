#include <cstring>

#include "globals.h"
#include "our_descriptor.h"
#include "ps_auth.h"
#include "remapper.h"

// Lasciamo solo il Report ID del Mouse
const uint8_t REPORT_ID_MOUSE = 1;

// Modificato: Rimosse le sezioni KEYBOARD e CONSUMER CONTROL. Resta SOLO il mouse puro.
const uint8_t our_report_descriptor_kb_mouse[] = {
    0x05, 0x01,                   // Usage Page (Generic Desktop Ctrls)
    0x09, 0x02,                   // Usage (Mouse)
    0xA1, 0x01,                   // Collection (Application)
    0x05, 0x01,                   //   Usage Page (Generic Desktop Ctrls)
    0x09, 0x02,                   //   Usage (Mouse)
    0xA1, 0x02,                   //   Collection (Logical)
    0x85, REPORT_ID_MOUSE,        //     Report ID (REPORT_ID_MOUSE = 1)
    0x09, 0x01,                   //     Usage (Pointer)
    0xA1, 0x00,                   //     Collection (Physical)
    0x05, 0x09,                   //       Usage Page (Button)
    0x19, 0x01,                   //       Usage Minimum (0x01)
    0x29, 0x05,                   //       Usage Maximum (0x05) - Modificato per 5 tasti standard mouse
    0x95, 0x05,                   //       Report Count (5)
    0x75, 0x01,                   //       Report Size (1)
    0x25, 0x01,                   //       Logical Maximum (1)
    0x81, 0x02,                   //       Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x95, 0x03,                   //       Report Count (3) - Padding bit per arrivare al byte intero (5+3=8)
    0x81, 0x03,                   //       Input (Const,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x05, 0x01,                   //       Usage Page (Generic Desktop Ctrls)
    0x09, 0x30,                   //       Usage (X)
    0x09, 0x31,                   //       Usage (Y)
    0x95, 0x02,                   //       Report Count (2)
    0x75, 0x10,                   //       Report Size (16)
    0x16, 0x00, 0x80,             //       Logical Minimum (-32768)
    0x26, 0xFF, 0x7F,             //       Logical Maximum (32767)
    0x81, 0x06,                   //       Input (Data,Var,Rel,No Wrap,Linear,Preferred State,No Null Position)
    0x09, 0x38,                   //       Usage (Wheel)
    0x15, 0x81,                   //       Logical Minimum (-127)
    0x25, 0x7F,                   //       Logical Maximum (127)
    0x75, 0x08,                   //       Report Size (8)
    0x95, 0x01,                   //       Report Count (1)
    0x81, 0x06,                   //       Input (Data,Var,Rel,No Wrap,Linear,Preferred State,No Null Position)
    0xC0,                         //       End Collection
    0xC0,                         //     End Collection
    0xC0,                         //   End Collection
};

// Usa lo stesso descrittore puro
const uint8_t our_report_descriptor_absolute[] = {
    0x05, 0x01,                   // Usage Page (Generic Desktop Ctrls)
    0x09, 0x02,                   // Usage (Mouse)
    0xA1, 0x01,                   // Collection (Application)
    0x05, 0x01,                   //   Usage Page (Generic Desktop Ctrls)
    0x09, 0x02,                   //   Usage (Mouse)
    0xA1, 0x02,                   //   Collection (Logical)
    0x85, REPORT_ID_MOUSE,        //     Report ID (REPORT_ID_MOUSE = 1)
    0x09, 0x01,                   //     Usage (Pointer)
    0xA1, 0x00,                   //     Collection (Physical)
    0x05, 0x09,                   //       Usage Page (Button)
    0x19, 0x01,                   //       Usage Minimum (0x01)
    0x29, 0x05,                   //       Usage Maximum (0x05)
    0x95, 0x05,                   //       Report Count (5)
    0x75, 0x01,                   //       Report Size (1)
    0x25, 0x01,                   //       Logical Maximum (1)
    0x81, 0x02,                   //       Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x95, 0x03,                   //       Report Count (3)
    0x81, 0x03,                   //       Input (Const,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x05, 0x01,                   //       Usage Page (Generic Desktop Ctrls)
    0x09, 0x30,                   //       Usage (X)
    0x09, 0x31,                   //       Usage (Y)
    0x95, 0x02,                   //       Report Count (2)
    0x75, 0x10,                   //       Report Size (16)
    0x16, 0x00, 0x80,             //       Logical Minimum (-32768)
    0x26, 0xFF, 0x7F,             //       Logical Maximum (32767)
    0x81, 0x06,                   //       Input (Data,Var,Rel,No Wrap,Linear,Preferred State,No Null Position)
    0x09, 0x38,                   //       Usage (Wheel)
    0x15, 0x81,                   //       Logical Minimum (-127)
    0x25, 0x7F,                   //       Logical Maximum (127)
    0x75, 0x08,                   //       Report Size (8)
    0x95, 0x01,                   //       Report Count (1)
    0x81, 0x06,                   //       Input (Data,Var,Rel,No Wrap,Linear,Preferred State,No Null Position)
    0xC0,                         //       End Collection
    0xC0,                         //     End Collection
    0xC0,                         //   End Collection
};


// Le sezioni Gamepad (Horipad, PS4, Stadia, XAC) rimangono inalterate poiché sono 
// collegate al parametro 'idx' della memoria configurabile, ma il boot base userà l'indice 0.
// Per compatibilità di compilazione, manteniamo i loro array originali vuoti o inalterati.
const uint8_t our_report_descriptor_horipad[] = { 0x00 };
uint8_t const our_report_descriptor_ps4[] = { 0x00 };
uint8_t const our_report_descriptor_stadia[] = { 0x00 };
uint8_t const our_report_descriptor_xac_compat[] = { 0x00 };

void kb_mouse_handle_set_report(uint8_t report_id, const uint8_t* buffer, uint16_t reqlen) {
    if (boot_protocol_keyboard) {
        handle_received_report(buffer, reqlen, OUR_OUT_INTERFACE, report_id);
    }
}

bool kb_mouse_set_report_synchronous(uint8_t report_id) {
    return false;
}

uint16_t kb_mouse_handle_get_report(uint8_t report_id, uint8_t* buffer, uint16_t reqlen) {
    return 0;
}

bool kb_mouse_should_cause_wakeup(uint8_t report_id, const uint8_t* buffer, uint16_t len) {
    if (report_id == REPORT_ID_MOUSE) {
        if ((len > 0) && (buffer[0] != 0)) {
            return true;
        }
        return false;
    }
    return false;
}

void horipad_clear_report(uint8_t* report, uint8_t report_id, uint16_t len) {}
void ps4_clear_report(uint8_t* report, uint8_t report_id, uint16_t len) {}
void stadia_clear_report(uint8_t* report, uint8_t report_id, uint16_t len) {}
void xac_compat_clear_report(uint8_t* report, uint8_t report_id, uint16_t len) {}

int32_t horipad_default_value(uint32_t usage) { return 0; }
int32_t ps4_stadia_default_value(uint32_t usage) { return 0; }
void stadia_sanitize_report(uint8_t report_id, uint8_t* buffer, uint16_t len) {}

const our_descriptor_def_t our_descriptors[] = {
    {
        .idx = 0,
        .descriptor = our_report_descriptor_kb_mouse,
        .descriptor_length = sizeof(our_report_descriptor_kb_mouse),
        .handle_received_report = do_handle_received_report,
        .handle_get_report = kb_mouse_handle_get_report,
        .handle_set_report = kb_mouse_handle_set_report,
        .set_report_synchronous = kb_mouse_set_report_synchronous,
        .should_cause_wakeup = kb_mouse_should_cause_wakeup,
    },
    {
        .idx = 1,
        .descriptor = our_report_descriptor_absolute,
        .descriptor_length = sizeof(our_report_descriptor_absolute),
        .handle_received_report = do_handle_received_report,
        .handle_get_report = kb_mouse_handle_get_report,
        .handle_set_report = kb_mouse_handle_set_report,
        .set_report_synchronous = kb_mouse_set_report_synchronous,
        .should_cause_wakeup = kb_mouse_should_cause_wakeup,
    },
    // Rimosse le istanze 2-5 (Gamepad) per evitare dipendenze non necessarie.
};

const uint8_t config_report_descriptor[] = { 0x00 }; // Descrittore WebHID svuotato
const uint32_t config_report_descriptor_length = 0;

uint8_t const boot_kb_report_descriptor[] = { 0x00 }; // Boot keyboard non inviata
const uint32_t boot_kb_report_descriptor_length = 0;
