#ifndef _KEYSTROKER_H_
#define _KEYSTROKER_H_

void add_keystroke(void);
bool process_record_keystroke(uint16_t keycode, keyrecord_t *record);

#endif
