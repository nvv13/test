// ********************************************************************
// source prg https://github.com/daniel1111/ArduinoCdPlayer.git
// ********************************************************************

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

#include "CShell.hpp"
#include "SystemTime.h"
#include "cplusplus.h"

#include "atapi.h"
#include "ide_io.h"

#define CHAR_STOP 123
#define CHAR_RR 124
#define CHAR_FF 125
#define CHAR_PAUSE 126
#define CHAR_PLAY 127

enum ScanState
{
  RR,
  FF,
  NONE
};

static CShell *PoCShell = NULL;
static atapi *P_atapi = NULL;
static enum ScanState _scan_state;

static unsigned long last_subchannel_read = 0;
static unsigned long last_mode_sense = 0;
static uint8_t current_track = 1;
static atapi::MediaStatus media_status;

static bool button_state = true;

static void
scan (enum ScanState scan)
{
  switch (scan)
    {
    case ScanState::FF:
      P_atapi->scan (0);
      break;

    case ScanState::RR:
      P_atapi->scan (1);
      break;

    case ScanState::NONE:
      P_atapi->play (0xFF);
      break;
    }

  _scan_state = scan;
}

static char
get_state_char (struct atapi::sub_channel_data *sub_ch_data)
{
  switch (sub_ch_data->audio_status)
    {
    case atapi::sub_channel_data::PLAY_PAUSED:
      return CHAR_PAUSE;

    case atapi::sub_channel_data::PLAY_IN_PROGRESS:
      if (_scan_state == ScanState::RR)
        return CHAR_RR;
      else if (_scan_state == ScanState::FF)
        return CHAR_FF;
      else
        return CHAR_PLAY;

    default:
      return CHAR_STOP;
    }

  return ' ';
}

static void
reread_toc_on_disc_load (atapi::MediaStatus media_status)
{
  static atapi::MediaStatus last_media_status = atapi::MediaStatus::DOOR_OPEN;

  if (last_media_status != media_status)
    {
      printf ("MediaStatus change %d -> %d\r\n", last_media_status,
              media_status);

      if (media_status == atapi::MediaStatus::DOOR_CLOSED_VALID_DISC
          || media_status == atapi::MediaStatus::DOOR_CLOSED_UNKNOWN_DISC)
        {
          printf ("(re)reading TOC\r\n");
          //_display->show_message("READING");
          P_atapi->read_toc ();
          printf ("First track=%d, last track=%d\r\n",
                  P_atapi->_toc.first_track, P_atapi->_toc.last_track);
        }

      last_media_status = media_status;
    }
}

#define LED_PIN PB7

static int
cmd_eject (int argc, char **argv)
{
  P_atapi->eject ();
  printf ("=eject\r\n");
  return 0;
};

static int
cmd_stop (int argc, char **argv)
{
  P_atapi->stop ();
  printf ("=stop\r\n");
  return 0;
};

static int
cmd_play (int argc, char **argv)
{
  P_atapi->play (current_track);
  printf ("=play\r\n");
  return 0;
};

static int
cmd_pause (int argc, char **argv)
{
  P_atapi->pause_toggle ();
  printf ("=pause\r\n");
  return 0;
};

static int
cmd_prev (int argc, char **argv)
{
  P_atapi->prev ();
  printf ("=prev\r\n");
  return 0;
};

static int
cmd_next (int argc, char **argv)
{
  P_atapi->next ();
  printf ("=next\r\n");
  return 0;
};

static int
cmd_FF (int argc, char **argv)
{
  if (button_state)
    scan (ScanState::FF);
  else
    scan (ScanState::NONE);
  button_state = !button_state;
  printf ("=FF\r\n");
  return 0;
};

static int
cmd_RR (int argc, char **argv)
{
  if (button_state)
    scan (ScanState::RR);
  else
    scan (ScanState::NONE);
  button_state = !button_state;
  printf ("=RR\r\n");
  return 0;
};

static const shell_command_t shell_commands[] = {
  //{ "init", "Setup a particular SPI configuration", cmd_init },
  { "eject", "eject", cmd_eject },
  { "stop", "stop", cmd_stop },
  { "play", "play", cmd_play },
  { "pause", "pause", cmd_pause },
  { "prev", "prev", cmd_prev },
  { "next", "next", cmd_next },
  { "FF", "FF - Fast Forward", cmd_FF },
  { "RR", "RR - Rewind", cmd_RR },
  //  { "off", "set LED Off", led_off },
  { NULL, NULL, NULL }
};

extern "C" int
main (void)
{

  /* setup */
  DDRB |= (1 << LED_PIN); // set LED pin as OUTPUT
  CUsart::GetInstance ();
  printf ("Start\r\n");
  ide_io _ide = ide_io ();
  atapi _atapi = atapi (&_ide);
  P_atapi = &_atapi;

  /* ATAPI init */
  ide_io::data16 read_val;
  _scan_state = ScanState::NONE;

  P_atapi->reset ();

  if (P_atapi->is_atapi_device ())
    printf ("Found ATAPI Dev.\r\n");
  else
    {
      printf ("No ATAPI Device!\r\n");
      while (1)
        {
          PORTB ^= (1 << LED_PIN); //
          _delay_ms (100);         //
        }; // No need to go ahead.
    }

  // Initialise task file
  printf ("Init task file\r\n");
  P_atapi->init_task_file ();
  printf ("Init task file done\r\n");

  // Run Self Diagnostic
  _delay_ms (3000);
  printf ("Self Diag: ");

  if (P_atapi->run_self_diag ())
    {
      printf ("OK\r\n");
    }
  else
    {
      printf ("Fail\r\n"); // Units failing this may still work fine
    }

  printf ("ATAPI Device:\r\n");

  // Identify Device
  struct atapi::identify_drive_details drive_details;
  P_atapi->identify_drive (&drive_details);
  printf (drive_details.model);

  struct atapi::sense_data sd;
  P_atapi->request_sense (&sd); // Send packet 'Request Sense'

  if (sd.asc == 0x29)               // Req. Sense returns 'HW Reset'
    {                               // (ASC=29h) at first since we had one.
      P_atapi->request_sense (&sd); // New Req. Sense returns if media
    } // is present or not.

  do
    {
      // Wait until drive is ready.
      P_atapi->request_sense (&sd); // Some devices take some time
    }
  while (sd.asc == 0x04); // ASC=04h -> LOGICAL DRIVE NOT READY

  /* ATAPI init End*/

  CShell oCShell = CShell (shell_commands);
  PoCShell = &oCShell;
  printf (" enter help for usage\r\n");
  while (1)
    {
      oCShell.Idle ();

      struct atapi::sub_channel_data sub_ch_data;

      if (sysTime.Millis () - last_mode_sense > 1000)
        {
          last_mode_sense = sysTime.Millis ();
          media_status = P_atapi->mode_sense_media_status ();
        }

      if (media_status == atapi::MediaStatus::DOOR_CLOSED_NO_DISC)
        {
          //_display->show_message ("NO DISC");
        }

      else if (media_status == atapi::MediaStatus::DOOR_OPEN)
        {
          //_display->show_message ("(OPEN)");
        }

      else if (sysTime.Millis () - last_subchannel_read > 100)
        {
          // bool deemph;
          //_wm880x->loop ();
          // deemph = _wm880x->is_deemph ();
          // if (deemph)
          //  digitalWrite (PIN_DEEMPH, HIGH);
          // else
          //  digitalWrite (PIN_DEEMPH, LOW);

          P_atapi->read_subchannel (&sub_ch_data);
          last_subchannel_read = sysTime.Millis ();

          char state_char = get_state_char (&sub_ch_data);

          //_display->show_position (
          //    sub_ch_data.track_number, sub_ch_data.relative_address.M,
          //    sub_ch_data.relative_address.S, deemph, state_char);
        }

      reread_toc_on_disc_load (media_status);
    }
}
