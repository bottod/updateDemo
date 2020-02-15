#ifndef CRC_H
#define CRC_H

void invert_uint8(uint8_t srcBuf,uint8_t *dBuf);
void invert_uint16(uint16_t srcBuf,uint16_t *dBuf);
uint16_t table_crc16_x25(uint16_t crc,uint8_t  *ptr,uint16_t len); // 字节查表法求CRC

#endif // CRC_H
