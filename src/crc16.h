#ifndef CRC16_H_INCLUDED
#define CRC16_H_INCLUDED
//#include "stm32f0xx.h"
#include <stdint.h>


/** \brief Compute CRC16
 *
 * \param data - message
 * \param dataLen - length of message to compute crc
 * \return uint16_t crc value
 *
 */

uint16_t crcCompute(const uint8_t* data, uint16_t dataLen);

/** \brief Compute and add CRC16 (BE)
 *
 * \param data - message
 * \param dataLen - length of message to compute crc
 * \return void
 *
 * CRC16 will be added to data[dataLen] = HIGH_BYTE(crc) and dataLen[dataLen+1] = LOW_BYTE(crc)
 */
void crcAdd(uint8_t* data, uint16_t dataLen);


/** \brief Compute and compare CRC16 (BE)
 *
 * \param data - message
 * \param dataLen - length of message to compute crc
 * \return 0 if crc match
 *
 * CRC16 value for compare will be read from data[dataLen] = HIGH_BYTE(crc) and dataLen[dataLen+1] = LOW_BYTE(crc)
 */

uint8_t crcCompare(const uint8_t *data, uint16_t dataLen);

#endif /* CRC16_H_INCLUDED */
