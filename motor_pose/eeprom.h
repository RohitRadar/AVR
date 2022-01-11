uint8_t eeprom_read_byte (const uint8_t *__p); 		/* Read one byte from
							EEPROM address */
uint16_t eeprom_read_word (const uint16_t *__p);	/* Read one 16-bit
							word from EEPROM
							address */
uint32_t eeprom_read_dword (const uint32_t *__p);  	/* Read one 32-bit
							double word from 
							EEPROM address*/
float eeprom_read_float (const float *__p);  		/* Read one float
							value from EEPROM 
							address */
void eeprom_read_block (void *__dst, const void *__src, size_t __n);/* Read 
							a block of n bytes 
							from EEPROM address
							to SRAM */
void eeprom_write_byte (uint8_t *__p, uint8_t __value);	/* Write a byte value
							to EEPROM address */
void eeprom_write_word (uint16_t *__p, uint16_t __value);/* Write a word to
							EEPROM address */
void eeprom_write_dword (uint32_t *__p, uint32_t __value);/* Write a 32-bit
							double word to EEPROM
							address */
void eeprom_write_float (float *__p, float __value);	/* Write float value
							to EEPROM address */
void eeprom_write_block (const void *__src, void *__dst, size_t __n);/* Write
							a block of bytes to 
							EEPROM address */
void eeprom_update_byte (uint8_t *__p, uint8_t __value);/* Update byte value
							to EEPROM address */
void eeprom_update_word (uint16_t *__p, uint16_t __value);/* Update word val
							to EEPROM address */
void eeprom_update_dword (uint32_t *__p, uint32_t __value);/* Update a 32-bit
							double word value to
							EEPROM address */
void eeprom_update_float (float *__p, float __value);	/* Update float value
							to EEPROM address */
void eeprom_update_block (const void *__src, void *__dst, size_t __n);/* Up-
							date a block of bytes
							to EEPROM address */