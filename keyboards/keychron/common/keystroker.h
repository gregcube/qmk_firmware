#ifndef _KEYSTROKER_H_
#define _KEYSTROKER_H_

enum keystroker_keycodes {
  KEYSTROKER_WRITE = QK_KB_0,
  KEYSTROKER_RESET
};

enum hid_commands {
  CMD_KEYSTROKE_WRITE = 0xff,  // Write keystroke count to eeprom
  CMD_KEYSTROKE_READ  = 0xfe,  // Read keystroke count from eeprom
  CMD_KEYSTROKE_COUNT = 0x01   // Read keystroke count from variable
};

void add_keystroke(void);
void reset_keystroke(void);
void eeprom_write_keystroke(void);
void eeprom_read_keystroke(uint8_t *ptr);
void keystroke_count(uint8_t *ptr);
bool process_record_keystroke(uint16_t keycode, keyrecord_t *record);

#endif
