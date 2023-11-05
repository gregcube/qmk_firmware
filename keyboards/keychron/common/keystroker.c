/**
 * Implements a super simple keystroke counter.
 */

#include "raw_hid.h"
#include "eeprom.h"
#include "keychron_common.h"
#include "keystroker.h"

uint64_t keystrokes = 0;

void add_keystroke()
{
  keystrokes++;
}

void reset_keystroke()
{
  keystrokes = 0;
}

void keyboard_post_init_kb()
{
  uint8_t buf[sizeof(uint64_t)];
  eeprom_read_keystroke(buf);
  memcpy(&keystrokes, buf, sizeof(uint64_t));
}

bool process_record_keystroke(uint16_t keycode, keyrecord_t *record)
{
  if (record->event.pressed) add_keystroke();

  switch (keycode) {
  case KEYSTROKER_WRITE:
    if (record->event.pressed) eeprom_write_keystroke();
    return false;

  case KEYSTROKER_RESET:
    if (record->event.pressed) reset_keystroke();
    return false;

  default:
    return true;
  }
}

void eeprom_write_keystroke()
{
  uint16_t addr = WEAR_LEVELING_LOGICAL_SIZE - sizeof(uint64_t);
  for (uint8_t i = 0; i < sizeof(uint64_t); i++) {
    eeprom_write_byte((uint8_t *)(addr + i), (uint8_t)(keystrokes >> (i * 8)));
  }
}

void eeprom_read_keystroke(uint8_t *ptr)
{
  uint16_t addr = WEAR_LEVELING_LOGICAL_SIZE - sizeof(uint64_t);
  for (uint8_t i = 0; i < sizeof(uint64_t); i++) {
    ptr[i] = eeprom_read_byte((const uint8_t *)(addr + i));
  }
}

void keystroke_count(uint8_t *ptr)
{
  for (uint8_t i = 0; i < sizeof(uint64_t); i++) {
    ptr[i] = (keystrokes >> (i * 8)) & 0xff;
  }
}

void raw_hid_receive(uint8_t *data, uint8_t length)
{
  switch (data[0]) {
  case CMD_KEYSTROKE_WRITE:
    eeprom_write_keystroke();
    eeprom_read_keystroke(&data[1]);
    break;

  case CMD_KEYSTROKE_READ:
    eeprom_read_keystroke(&data[1]);
    break;

  case CMD_KEYSTROKE_COUNT:
    keystroke_count(&data[1]);
    break;
  }

  raw_hid_send(data, length);
}

