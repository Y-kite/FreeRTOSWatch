/* u8g2_d_setup.c */
/* generated code, codebuild, u8g2 project */

#include "u8g2.h"
#include "main.h"

/* ssd1306 f */
void u8g2_Setup_ssd1306_i2c_128x64_noname_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
{
  uint8_t tile_buf_height;
  uint8_t *buf;
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1306_128x64_noname, u8x8_cad_ssd13xx_fast_i2c, byte_cb, gpio_and_delay_cb);
  buf = u8g2_m_16_8_f(&tile_buf_height);
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, rotation);
}

void u8g2_Init(u8g2_t *u8g2)
{
	u8g2_Setup_ssd1306_i2c_128x64_noname_f(u8g2,U8G2_R0,u8x8_byte_hw_i2c ,u8g2_stm32_delay);
	u8g2_InitDisplay(u8g2); // send init sequence to the display, display is in sleep mode after this,
	u8g2_SetPowerSave(u8g2, 0); // wake up display
	u8g2_ClearDisplay(u8g2);
}
