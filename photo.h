/** @file photo.h
*
* @brief This module provides control functions for reading the photo-interrupters.
*/

#ifndef PHOTO_H_
#define PHOTO_H_

void photo_init(void);

uint8_t photo_wait(uint8_t check_timeout);

#endif /* PHOTO_H_ */
