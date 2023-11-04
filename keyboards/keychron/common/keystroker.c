#include "raw_hid.h"
#include "keychron_common.h"
#include "keystroker.h"

uint64_t keystrokes = 0;

void add_keystroke()
{
  keystrokes++;
}

bool process_record_keystroke(uint16_t keycode, keyrecord_t *record)
{
  if (record->event.pressed) add_keystroke();
  return true;
}

void raw_hid_receive(uint8_t *data, uint8_t length)
{
  if (data[0] == 0xff) {
    for (int i = 0; i < 8; i++) {
      data[i + 1] = (uint8_t)(keystrokes >> (i * 8)) & 0xff;
    }

    raw_hid_send(data, length);
  }
}

