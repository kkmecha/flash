#ifndef _FLASH_H_
#define _FLASH_H_

class flash{
    public:
    flash(uint16_t dat, uint16_t *addr) : _dat(dat), _addr(addr){
        ;
    }
    void flash_unlock(void);
    void flash_lock(void);
    inline static bool check_eop();
    bool flash_write();
    bool flash_erase_at();
    bool flash_erase_all(void);
    private:
    inline static void wait_for_busy_is_clr(void);

    private:
        uint16_t _dat, *_addr;
};
#endif