#include "stm32f3xx.h"
#include "flash.h"

flash::flash(uint16_t dat, uint16_t *addr){
	_dat = (uint16_t)dat;
	_addr = (uint16_t *)addr;
}

void flash::flash_unlock(void) {
	FLASH->KEYR = 0x45670123;
	FLASH->KEYR = 0xCDEF89AB;
}

void flash::flash_lock(void) {
	FLASH->CR |= FLASH_CR_LOCK;
}

inline static void wait_for_busy_is_clr(void) {
	while( FLASH->SR & FLASH_SR_BSY );
}

inline static bool check_eop(void) {
	if( FLASH->SR & FLASH_SR_EOP ) {
		FLASH->SR &= ~FLASH_SR_EOP;
		return true;
	}
	return false;
}

bool flash::flash_write() {
	if( *_addr != 0xFFFF ) {
		return false;
	}
	wait_for_busy_is_clr();
	FLASH->CR |= FLASH_CR_PG;

	*_addr = _dat;
	wait_for_busy_is_clr();
	FLASH->CR &= ~FLASH_CR_PG;

	return check_eop();
}


bool flash::flash_erase_at() {
	wait_for_busy_is_clr();
	FLASH->CR |= FLASH_CR_PER;
	FLASH->AR = *_addr;
	FLASH->CR |= FLASH_CR_STRT;

	wait_for_busy_is_clr();
	FLASH->CR &= ~FLASH_CR_PER;

	return check_eop();
}

bool flash::flash_erase_all(void) {
	wait_for_busy_is_clr();
	FLASH->CR |= FLASH_CR_MER;
	FLASH->CR |= FLASH_CR_STRT;

	wait_for_busy_is_clr();
	FLASH->CR &= ~FLASH_CR_MER;
	return check_eop();
}
