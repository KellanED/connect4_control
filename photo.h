/** @file photo.h
*
* @brief This module provides control functions for reading the photo-interrupters.
*/

#ifndef PHOTO_H_
#define PHOTO_H_

void photo_init(void);

uint8_t photo_wait(void);

#endif /* PHOTO_H_ */
