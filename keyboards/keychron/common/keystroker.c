/**
 * Implements a super simple keystroke counter.
 */

#include "raw_hid.h"
#include "eeprom.h"
#include "keychron_common.h"
#include "keystroker.h"

enum hid_commands {
  CMD_KEYSTROKER_WRITE = 0xff,  // Write keystroke count to eeprom
  CMD_KEYSTROKER_READ  = 0xfe,  // Read keystroke count from eeprom
  CMD_KEYSTROKER_COUNT = 0x01,  // Read keystroke count variable
  CMD_KEYSTROKER_SET   = 0x02   // Set keystroke count variable
};

static void add_keystroke(void);
static void reset_keystroke_count(void);
static void set_keystroke_count(uint8_t *ptr);
static void eeprom_write_keystroke_count(void);
static void eeprom_read_keystroke_count(uint8_t *ptr);
static void set_keystroke_hid_data(uint8_t *ptr);

static uint64_t keystrokes = 0;

static void add_keystroke()
{
  keystrokes++;
}

static void reset_keystroke_count()
{
  keystrokes = 0;
}

static void set_keystroke_count(uint8_t *ptr)
{
  memcpy(&keystrokes, ptr, sizeof(uint64_t));
}

static void eeprom_write_keystroke_count()
{
  uint16_t addr = WEAR_LEVELING_LOGICAL_SIZE - sizeof(uint64_t);
  for (uint8_t i = 0; i < sizeof(uint64_t); i++) {
    eeprom_write_byte((uint8_t *)(addr + i), (uint8_t)(keystrokes >> (i * 8)));
  }
}

static void eeprom_read_keystroke_count(uint8_t *ptr)
{
  uint16_t addr = WEAR_LEVELING_LOGICAL_SIZE - sizeof(uint64_t);
  for (uint8_t i = 0; i < sizeof(uint64_t); i++) {
    ptr[i] = eeprom_read_byte((const uint8_t *)(addr + i));
  }
}

static void set_keystroke_hid_data(uint8_t *ptr)
{
  for (uint8_t i = 0; i < sizeof(uint64_t); i++) {
    ptr[i] = (keystrokes >> (i * 8)) & 0xff;
  }
}

bool process_record_keystroke(uint16_t keycode, keyrecord_t *record)
{
  if (record->event.pressed) add_keystroke();

  switch (keycode) {
  case KEYSTROKER_WRITE:
    if (record->event.pressed) eeprom_write_keystroke_count();
    return false;

  case KEYSTROKER_RESET:
    if (record->event.pressed) reset_keystroke_count();
    return false;

  default:
    return true;
  }
}

void keyboard_post_init_kb()
{
  uint8_t buf[sizeof(uint64_t)];
  eeprom_read_keystroke_count(buf);
  set_keystroke_count(buf);
}

void raw_hid_receive(uint8_t *data, uint8_t length)
{
  switch (data[0]) {
  case CMD_KEYSTROKER_WRITE:
    eeprom_write_keystroke_count();
    eeprom_read_keystroke_count(&data[1]);
    break;

  case CMD_KEYSTROKER_READ:
    eeprom_read_keystroke_count(&data[1]);
    break;

  case CMD_KEYSTROKER_COUNT:
    set_keystroke_hid_data(&data[1]);
    break;

  case CMD_KEYSTROKER_SET:
    set_keystroke_count(&data[1]);
    break;
  }

  raw_hid_send(data, length);
}

// vi: expandtab shiftwidth=2 softtabstop=2 tabstop=2
