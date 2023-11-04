#ifndef _KEYSTROKER_H_
#define _KEYSTROKER_H_

enum keystroker_keycodes {
  KEYSTROKER_WRITE = QK_KB_0,
  KEYSTROKER_RESET
};

void add_keystroke(void);
void reset_keystroke(void);
void eeprom_write_keystroke(void);
void eeprom_read_keystroke(uint8_t *buf);
bool process_record_keystroke(uint16_t keycode, keyrecord_t *record);

#endif
