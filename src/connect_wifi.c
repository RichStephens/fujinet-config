/**
 * FujiNet for #Adam configuration program
 *
 * Connect to existing WiFi connection
 */
#include "connect_wifi.h"

#ifdef _CMOC_VERSION_
#include <cmoc.h>
#include "coco/stdbool.h"
#include "coco/bar.h"
#include "coco/globals.h"
#include "coco/io.h"
#include "coco/screen.h"
#include "coco/input.h"
#else
#include <string.h>
#endif /* _CMOC_VERSION_ */

#include "pause.h"

#ifdef BUILD_ADAM
#include "adam/io.h"
#include "adam/screen.h"
#include "adam/globals.h"
#include "adam/input.h"
extern bool screen_should_be_cleared;
#endif /* BUILD_ADAM */

#ifdef BUILD_APPLE2
#include "apple2/io.h"
#include "apple2/screen.h"
#include "apple2/globals.h"
#include "apple2/input.h"
#endif /* BUILD_APPLE2 */

#ifdef BUILD_ATARI
#include "atari/io.h"
#include "atari/screen.h"
#include "atari/globals.h"
#include "atari/bar.h"
#include "atari/input.h"
#endif /* BUILD_ATARI */

#ifdef BUILD_C64
#include "c64/io.h"
#include "c64/screen.h"
#include "c64/globals.h"
#include "c64/input.h"
#endif /* BUILD_C64 */

#ifdef BUILD_PC8801
#include "pc8801/io.h"
#include "pc8801/screen.h"
#include "pc8801/globals.h"
#include "pc8801/input.h"
#endif /* BUILD_PC8801 */

#ifdef BUILD_PC6001
#include "pc6001/io.h"
#include "pc6001/screen.h"
#include "pc6001/globals.h"
#include "pc6001/input.h"
#endif /* BUILD_PC6001 */

#ifdef BUILD_PMD85
#include "pmd85/io.h"
#include "pmd85/screen.h"
#include "pmd85/globals.h"
#include "pmd85/input.h"
#endif /* BUILD_PMD85 */

#ifdef BUILD_RC2014
#include "rc2014/io.h"
#include "rc2014/screen.h"
#include "rc2014/globals.h"
#include "rc2014/input.h"
#endif /* BUILD_RC2014 */

void connect_wifi(void)
{
	unsigned char retries = 20;
	NetConfig nc;
	unsigned char s, key;

	memcpy(&nc, io_get_ssid(), sizeof(NetConfig));

	state = SET_WIFI;

	screen_connect_wifi(&nc);

	while (retries > 0)
	{
	  // TODO: Write COCO specific version that just checks for a key, as input() blocks! grrr. -thom
#ifndef _CMOC_VERSION_
	  // check for esc key and abort
		if (input() == KEY_ABORT)
		{
			screen_error("CONNECTION ABORTED");
			pause(150);
			state=SET_WIFI;
			return;
		}
#endif /* _CMOC_VERSION_ */
		
		s = io_get_wifi_status();

		switch (s)
		{
		case 1:
			screen_error("NO SSID AVAILABLE");
			pause(150);
			return;
		case 3:
			screen_error("CONNECTION SUCCESS!");
			state = HOSTS_AND_DEVICES;
#ifdef BUILD_ADAM
                        screen_should_be_cleared=true;
#endif
			pause(60);
			return;
		case 4:
			screen_error("CONNECT FAILED");
			pause(150);
			return;
		case 5:
			screen_error("CONNECTION LOST");
			pause(150);
			return;
		default:
			screen_error("PLEASE WAIT...");
 			pause(150);
			retries--;
			break;
		}
	}
	screen_error("UNABLE TO CONNECT");
	pause(150);
	state = SET_WIFI;
}
