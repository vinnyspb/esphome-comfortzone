#include "comfortzone_config.h"
#include "comfortzone_frame.h"
#include "comfortzone_decoder.h"
#include "comfortzone_tools.h"

namespace esphome::comfortzone {

static const char* TAG = "ComfortzoneDecoderBasic";

// this file contains basic decoding function for commands and replies.
// Unless debug mode is set, nothing is done here

void czdec::cmd_r_generic(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_CMD *p)
{
	ESP_LOGD(TAG, "Wanted reply size: %u, CRC: %X", p->wanted_reply_size, p->crc);
}

void czdec::cmd_w_generic_2byte(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, W_CMD *p)
{
	ESP_LOGD(TAG, "%X %X crc: %X", p->reg_value[0], p->reg_value[1], p->crc);
}

void czdec::cmd_w_temp(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, W_CMD *p)
{
	int reg_v;
	float reg_v_f;

	reg_v = get_uint16(p->reg_value);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "%f °C, crc: %X", reg_v_f, p->crc);
}

void czdec::cmd_w_temp_1byte(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, W_CMD *p)
{
	int reg_v;
	float reg_v_f;

	reg_v = ((W_SMALL_CMD*)p)->reg_value;

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "%f °C, crc: %X", reg_v_f, p->crc);
}

void czdec::cmd_w_freq(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, W_CMD *p)
{
	int reg_v;
	float reg_v_f;

	reg_v = get_uint16(p->reg_value);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "%f Hz, crc: %X", reg_v_f, p->crc);
}

void czdec::cmd_w_time(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, W_CMD *p)
{
	int reg_v;
	float reg_v_f;

	reg_v = get_uint16(p->reg_value);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "%f min, crc: %X", reg_v_f, p->crc);
}

void czdec::cmd_w_percentage(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, W_CMD *p)
{
	int reg_v;
	float reg_v_f;

	reg_v = get_int16(p->reg_value);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "%f %%, crc: %X", reg_v_f, p->crc);
}

void czdec::cmd_w_extra_hot_water(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, W_CMD *p)
{
	int reg_v;

	reg_v = get_uint16(p->reg_value);

	if(reg_v == 0x0001) {
		ESP_LOGD(TAG, "on");
	} else if(reg_v == 0xFFFE) {	// -2 (???)
		ESP_LOGD(TAG, "off");
	} else {
		ESP_LOGD(TAG, "%X", reg_v);
	}

	ESP_LOGD(TAG, "crc: %X", p->crc);
}

void czdec::cmd_w_clr_alarm(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, W_CMD *p)
{
	int reg_v;

	reg_v = get_uint16(p->reg_value);

	if(reg_v & 0x0002) {
		ESP_LOGD(TAG, "filter alarm");
	}

	if(reg_v & ~0x0002) {
		ESP_LOGD(TAG, "%X", reg_v);
	}

	ESP_LOGD(TAG, "crc: %X", p->crc);
}

void czdec::cmd_w_daylight_saving(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, W_CMD *p)
{
	int reg_v;

	reg_v = get_uint16(p->reg_value);

	if(reg_v == 0xFFBF)	{	// == ~0x0040
		ESP_LOGD(TAG, "on");
	} else if(reg_v == 0x0040) {
		ESP_LOGD(TAG, "off");
	} else {
		ESP_LOGD(TAG, "%X", reg_v);
	}

	ESP_LOGD(TAG, "crc: %X", p->crc);
}

void czdec::cmd_w_sanitary_priority(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, W_CMD *p)
{
	int reg_v;

	reg_v = get_uint16(p->reg_value);

	if(reg_v == 0x0001) {
		ESP_LOGD(TAG, "low");
	} else if(reg_v == 0x0002) {
		ESP_LOGD(TAG, "normal");
	} else if(reg_v == 0x0003) {
		ESP_LOGD(TAG, "high");
	} else {
		ESP_LOGD(TAG, "%X", reg_v);
	}

	ESP_LOGD(TAG, "crc: %X", p->crc);
}

void czdec::cmd_w_day_delay(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, W_CMD *p)
{
	int reg_v;

	reg_v = get_uint16(p->reg_value);

	if(reg_v == 0) {
		ESP_LOGD(TAG, "off");
	} else {
		ESP_LOGD(TAG, "%d day(s)", reg_v);
	}

	ESP_LOGD(TAG, "crc: %X", p->crc);
}

void czdec::cmd_w_fan_speed(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, W_CMD *p)
{
	int reg_v;

	reg_v = ((W_SMALL_CMD*)p)->reg_value;

	if(reg_v == 0x01) {
		ESP_LOGD(TAG, "low");
	} else if(reg_v == 0x02) {
		ESP_LOGD(TAG, "normal");
	} else if(reg_v == 0x03) {
		ESP_LOGD(TAG, "high");
	} else {
		ESP_LOGD(TAG, "%X", reg_v);
	}

	ESP_LOGD(TAG, "crc: %X", p->crc);
}

void czdec::cmd_w_language(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, W_CMD *p)
{
	int reg_v;

	reg_v = ((W_SMALL_CMD*)p)->reg_value;

	if(reg_v == 0x01) {
		ESP_LOGD(TAG, "English");
	} else if(reg_v == 0x04) {
		ESP_LOGD(TAG, "Français");
	} else {
		ESP_LOGD(TAG, "%X", reg_v);
	}

	ESP_LOGD(TAG, "crc: %X", p->crc);
}

void czdec::cmd_w_digit(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, W_CMD *p)
{
	int reg_v;

	reg_v = ((W_SMALL_CMD*)p)->reg_value;

	ESP_LOGD(TAG, "Value: %d = 0x%X, crc: %X", reg_v, reg_v, p->crc);
}

void czdec::reply_r_generic_2byte(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
	int reg_v;

	reg_v = p->reg_value[0];

	ESP_LOGD(TAG, "%X %X crc: %X", p->reg_value[0], p->reg_value[1], p->crc);
}

void czdec::reply_r_temp(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
	int reg_v;
	float reg_v_f;

	reg_v = get_uint16(p->reg_value);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "%f °C, crc: %X", reg_v_f, p->crc);
}

void czdec::reply_r_temp_1byte(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
	int reg_v;
	float reg_v_f;

	reg_v = ((R_SMALL_REPLY*)p)->reg_value;

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "%f °C, crc: %X", reg_v_f, p->crc);
}

void czdec::reply_r_freq(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
	int reg_v;
	float reg_v_f;

	reg_v = get_uint16(p->reg_value);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "%f Hz, crc: %X", reg_v_f, p->crc);
}

void czdec::reply_r_time(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
	int reg_v;
	float reg_v_f;

	reg_v = get_uint16(p->reg_value);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "%f min, crc: %X", reg_v_f, p->crc);
}

void czdec::reply_r_percentage(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
	int reg_v;
	float reg_v_f;

	reg_v = get_int16(p->reg_value);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "%f %%, crc: %X", reg_v_f, p->crc);
}

void czdec::reply_r_extra_hot_water(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
	int reg_v;

	reg_v = get_uint16(p->reg_value);

	if(reg_v == 0x0C8B) {
		ESP_LOGD(TAG, "on");
	} else if(reg_v == 0x0C8A) {	// -1 in complement 1's
		ESP_LOGD(TAG, "off");
	} else {
		ESP_LOGD(TAG, "%X", reg_v);
	}

	ESP_LOGD(TAG, "crc: %X", p->crc);
}

void czdec::reply_r_daylight_saving(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
	// not tested
	int reg_v;

	reg_v = get_uint16(p->reg_value);

	if(reg_v == 0xFFBF) {		// == ~0x0040
		ESP_LOGD(TAG, "on");
	} else if(reg_v == 0x0040) {
		ESP_LOGD(TAG, "off");
	} else {
		ESP_LOGD(TAG, "%X", reg_v);
	}

	ESP_LOGD(TAG, "crc: %X", p->crc);
}

void czdec::reply_r_sanitary_priority(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
	int reg_v;

	reg_v = get_uint16(p->reg_value);

	if(reg_v == 0x4151) {
		ESP_LOGD(TAG, "low");
	} else if(reg_v == 0x4152) {
		ESP_LOGD(TAG, "normal");
	} else if(reg_v == 0x4153) {
		ESP_LOGD(TAG, "high");
	} else {
		ESP_LOGD(TAG, "%X", reg_v);
	}

	ESP_LOGD(TAG, "crc: %X", p->crc);
}

void czdec::reply_r_clr_alarm(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
	int reg_v;

	reg_v = get_uint16(p->reg_value);

	if(reg_v & 0x0002) {
		ESP_LOGD(TAG, "filter alarm");
	}

	if(reg_v & ~0x0002) {
		ESP_LOGD(TAG, "%X", reg_v);
	}

	ESP_LOGD(TAG, "crc: %X", p->crc);
}

void czdec::reply_r_day_delay(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
	int reg_v;

	reg_v = get_uint16(p->reg_value);

	if(reg_v == 0) {
		ESP_LOGD(TAG, "off");
	} else {
		ESP_LOGD(TAG, "%d day(s)", reg_v);
	}

	ESP_LOGD(TAG, "crc: %X", p->crc);
}

void czdec::reply_r_fan_speed(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
	int reg_v;

	reg_v = ((R_SMALL_REPLY*)p)->reg_value;

	if(reg_v == 0x01) {
		ESP_LOGD(TAG, "low");
	} else if(reg_v == 0x02) {
		ESP_LOGD(TAG, "normal");
	} else if(reg_v == 0x03) {
		ESP_LOGD(TAG, "high");
	} else {
		ESP_LOGD(TAG, "%X", reg_v);
	}

	ESP_LOGD(TAG, "crc: %X", p->crc);
}

void czdec::reply_r_language(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
	int reg_v;

	reg_v = ((R_SMALL_REPLY*)p)->reg_value;

	if(reg_v == 0x01) {
		ESP_LOGD(TAG, "English");
	} else if(reg_v == 0x04) {
		ESP_LOGD(TAG, "Français");
	} else {
		ESP_LOGD(TAG, "%X", reg_v);
	}

	ESP_LOGD(TAG, "crc: %X", p->crc);
}


void czdec::reply_r_digit(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
	int reg_v;

	reg_v = ((R_SMALL_REPLY*)p)->reg_value;

	ESP_LOGD(TAG, "%X crc: %X", reg_v, p->crc);
}

void czdec::reply_w_generic(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, W_REPLY *p)
{
	ESP_LOGD(TAG, "Value: %d = 0x%X crc: %X", p->return_code, p->return_code, p->crc);
}

void czdec::empty(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, W_CMD *p)
{
}

void czdec::empty(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, W_SMALL_CMD *p)
{
}

void czdec::empty(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_CMD *p)
{
}

void czdec::empty(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
}

void czdec::empty(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, W_REPLY *p)
{
}

}
