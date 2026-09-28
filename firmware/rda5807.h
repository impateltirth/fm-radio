#ifndef RDA5807_H
#define RDA5807_H

#include <stdint.h>

#define RDA5807_I2C_WR  0x20   /* 7-bit addr 0x10 + W */
#define RDA5807_I2C_RD  0x21   /* 7-bit addr 0x10 + R */

/* Register 0x02 bits */
#define RDA5807_DHIZ        0x8000
#define RDA5807_DMUTE       0x4000
#define RDA5807_MONO        0x2000
#define RDA5807_SEEKUP      0x0200
#define RDA5807_SEEK        0x0100
#define RDA5807_SKMODE      0x0080
#define RDA5807_RDS_EN      0x0008
#define RDA5807_NEW_METHOD  0x0004
#define RDA5807_SOFT_RESET  0x0002
#define RDA5807_ENABLE      0x0001

/* Register 0x03 bits */
#define RDA5807_CHAN_MASK   0xFFC0
#define RDA5807_TUNE        0x0010
#define RDA5807_BAND_87_108 0x0000
#define RDA5807_SPACE_100K  0x0000

/* Register 0x0A bits (read) */
#define RDA5807_STC         0x4000  /* seek/tune complete */
#define RDA5807_SF          0x2000  /* seek failed */
#define RDA5807_ST          0x0400  /* stereo indicator */
#define RDA5807_READCHAN_MASK 0x03FF

#define RDA5807_SEEK_DOWN   0
#define RDA5807_SEEK_UP     1

void    rda5807_init(void);
void    rda5807_set_frequency(float mhz);   /* 87.0 – 108.0 */
float   rda5807_get_frequency(void);
uint8_t rda5807_seek(uint8_t direction);    /* 1 if a station was found */
uint8_t rda5807_is_stereo(void);
void    rda5807_set_volume(uint8_t level);  /* 0 – 15 */
void    rda5807_set_mute(uint8_t mute);     /* 1 = muted */

#endif /* RDA5807_H */
