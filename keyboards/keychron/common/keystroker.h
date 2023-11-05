#ifndef _KEYSTROKER_H_
#define _KEYSTROKER_H_

enum keystroker_keycodes {
  KEYSTROKER_WRITE = QK_KB_0,
  KEYSTROKER_RESET
};

bool process_record_keystroke(uint16_t keycode, keyrecord_t *record);

#endif
