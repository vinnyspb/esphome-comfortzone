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
#ifdef DEBUG
	ESP_LOGV(TAG, "Wanted reply size: ");
	if(p->wanted_reply_size < 0x10)
		ESP_LOGV(TAG, "0");
	ESP_LOGV(TAG, p->wanted_reply_size, HEX);

	ESP_LOGV(TAG, "crc: ");
	if(p->crc < 0x10)
		ESP_LOGV(TAG, "0");
	ESP_LOGV(TAG, p->crc, HEX);
	return;
#endif
}

void czdec::cmd_w_generic_2byte(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, W_CMD *p)
{
#ifdef DEBUG
	int reg_v;

	reg_v = p->reg_value[0];

	if(reg_v < 0x10)
		ESP_LOGV(TAG, "0");
	ESP_LOGV(TAG, reg_v, HEX);

	ESP_LOGV(TAG, " ");

	reg_v = p->reg_value[1];

	if(reg_v < 0x10)
		ESP_LOGV(TAG, "0");
	ESP_LOGV(TAG, reg_v, HEX);

	ESP_LOGV(TAG, "crc: ");
	if(p->crc < 0x10)
		ESP_LOGV(TAG, "0");
	ESP_LOGV(TAG, p->crc, HEX);
#endif
}

void czdec::cmd_w_temp(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, W_CMD *p)
{
#ifdef DEBUG
	int reg_v;
	float reg_v_f;

	reg_v = get_uint16(p->reg_value);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGV(TAG, reg_v_f);
	ESP_LOGV(TAG, "°C");

	ESP_LOGV(TAG, "crc: ");
	if(p->crc < 0x10)
		ESP_LOGV(TAG, "0");
	ESP_LOGV(TAG, p->crc, HEX);
	return;
#endif
}

void czdec::cmd_w_temp_1byte(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, W_CMD *p)
{
#ifdef DEBUG
	int reg_v;
	float reg_v_f;

	reg_v = ((W_SMALL_CMD*)p)->reg_value;

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGV(TAG, reg_v_f);
	ESP_LOGV(TAG, "°C");

	ESP_LOGV(TAG, "crc: ");
	if(p->crc < 0x10)
		ESP_LOGV(TAG, "0");
	ESP_LOGV(TAG, p->crc, HEX);
	return;
#endif
}

void czdec::cmd_w_freq(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, W_CMD *p)
{
#ifdef DEBUG
	int reg_v;
	float reg_v_f;

	reg_v = get_uint16(p->reg_value);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGV(TAG, reg_v_f);
	ESP_LOGV(TAG, "Hz");

	ESP_LOGV(TAG, "crc: ");
	if(p->crc < 0x10)
		ESP_LOGV(TAG, "0");
	ESP_LOGV(TAG, p->crc, HEX);
	return;
#endif
}

void czdec::cmd_w_time(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, W_CMD *p)
{
#ifdef DEBUG
	int reg_v;
	float reg_v_f;

	reg_v = get_uint16(p->reg_value);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGV(TAG, reg_v_f);
	ESP_LOGV(TAG, "min");

	ESP_LOGV(TAG, "crc: ");
	if(p->crc < 0x10)
		ESP_LOGV(TAG, "0");
	ESP_LOGV(TAG, p->crc, HEX);
	return;
#endif
}

void czdec::cmd_w_time_minutes(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, W_CMD *p)
{
#ifdef DEBUG
	int reg_v;

	reg_v = get_uint16(p->reg_value);

	ESP_LOGV(TAG, reg_v);
	ESP_LOGV(TAG, "min");

	ESP_LOGV(TAG, "crc: ");
	if(p->crc < 0x10)
		ESP_LOGV(TAG, "0");
	ESP_LOGV(TAG, p->crc, HEX);
	return;
#endif
}

void czdec::cmd_w_time_days(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, W_CMD *p)
{
#ifdef DEBUG
	int reg_v;

	reg_v = get_uint16(p->reg_value);

	ESP_LOGV(TAG, reg_v);
	ESP_LOGV(TAG, "days");

	ESP_LOGV(TAG, "crc: ");
	if(p->crc < 0x10)
		ESP_LOGV(TAG, "0");
	ESP_LOGV(TAG, p->crc, HEX);
	return;
#endif
}

void czdec::cmd_w_percentage(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, W_CMD *p)
{
#ifdef DEBUG
	int reg_v;
	float reg_v_f;

	reg_v = get_int16(p->reg_value);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGV(TAG, reg_v_f);
	ESP_LOGV(TAG, "%");

	ESP_LOGV(TAG, "crc: ");
	if(p->crc < 0x10)
		ESP_LOGV(TAG, "0");
	ESP_LOGV(TAG, p->crc, HEX);
	return;
#endif
}

void czdec::cmd_w_extra_hot_water(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, W_CMD *p)
{
#ifdef DEBUG
	int reg_v;

	reg_v = get_uint16(p->reg_value);

	if(reg_v == 0x0001)
		ESP_LOGV(TAG, "on");
	else if(reg_v == 0xFFFE)		// -2 (???)
		ESP_LOGV(TAG, "off");
	else
		ESP_LOGV(TAG, reg_v, HEX);

	ESP_LOGV(TAG, "crc: ");
	if(p->crc < 0x10)
		ESP_LOGV(TAG, "0");
	ESP_LOGV(TAG, p->crc, HEX);
	return;
#endif
}

void czdec::cmd_w_clr_alarm(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, W_CMD *p)
{
#ifdef DEBUG
	int reg_v;

	reg_v = get_uint16(p->reg_value);

	if(reg_v & 0x0002)
		ESP_LOGV(TAG, "filter alarm");

	if(reg_v & ~0x0002)
		ESP_LOGV(TAG, reg_v, HEX);

	ESP_LOGV(TAG, "crc: ");
	if(p->crc < 0x10)
		ESP_LOGV(TAG, "0");
	ESP_LOGV(TAG, p->crc, HEX);
	return;
#endif
}

void czdec::cmd_w_daylight_saving(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, W_CMD *p)
{
#ifdef DEBUG
	int reg_v;

	reg_v = get_uint16(p->reg_value);

	if(reg_v == 0xFFBF)		// == ~0x0040
		ESP_LOGV(TAG, "on");
	else if(reg_v == 0x0040)
		ESP_LOGV(TAG, "off");
	else
		ESP_LOGV(TAG, reg_v, HEX);

	ESP_LOGV(TAG, "crc: ");
	if(p->crc < 0x10)
		ESP_LOGV(TAG, "0");
	ESP_LOGV(TAG, p->crc, HEX);
	return;
#endif
}

void czdec::cmd_w_sanitary_priority(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, W_CMD *p)
{
#ifdef DEBUG
	int reg_v;

	reg_v = get_uint16(p->reg_value);

	if(reg_v == 0x0001)
		ESP_LOGV(TAG, "low");
	else if(reg_v == 0x0002)
		ESP_LOGV(TAG, "normal");
	else if(reg_v == 0x0003)
		ESP_LOGV(TAG, "high");
	else
		ESP_LOGV(TAG, reg_v, HEX);

	ESP_LOGV(TAG, "crc: ");
	if(p->crc < 0x10)
		ESP_LOGV(TAG, "0");
	ESP_LOGV(TAG, p->crc, HEX);
	return;
#endif
}

void czdec::cmd_w_day_delay(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, W_CMD *p)
{
#ifdef DEBUG
	int reg_v;

	reg_v = get_uint16(p->reg_value);

	if(reg_v == 0)
		ESP_LOGV(TAG, "off");
	else
	{
		ESP_LOGV(TAG, reg_v);
		ESP_LOGV(TAG, " day(s)");
	}

	ESP_LOGV(TAG, "crc: ");
	if(p->crc < 0x10)
		ESP_LOGV(TAG, "0");
	ESP_LOGV(TAG, p->crc, HEX);
	return;
#endif
}

void czdec::cmd_w_fan_speed(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, W_CMD *p)
{
#ifdef DEBUG
	int reg_v;

	reg_v = ((W_SMALL_CMD*)p)->reg_value;

	if(reg_v == 0x01)
		ESP_LOGV(TAG, "low");
	else if(reg_v == 0x02)
		ESP_LOGV(TAG, "normal");
	else if(reg_v == 0x03)
		ESP_LOGV(TAG, "high");
#if HP_PROTOCOL == HP_PROTOCOL_1_8
	else if(reg_v == 0x04)
		ESP_LOGV(TAG, "on timer");
#endif
	else
		ESP_LOGV(TAG, reg_v, HEX);

	ESP_LOGV(TAG, "crc: ");
	if(p->crc < 0x10)
		ESP_LOGV(TAG, "0");
	ESP_LOGV(TAG, p->crc, HEX);
	return;
#endif
}

void czdec::cmd_w_language(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, W_CMD *p)
{
#ifdef DEBUG
	int reg_v;

	reg_v = ((W_SMALL_CMD*)p)->reg_value;

	if(reg_v == 0x01)
		ESP_LOGV(TAG, "English");
	else if(reg_v == 0x04)
		ESP_LOGV(TAG, "Français");
	else
		ESP_LOGV(TAG, reg_v, HEX);

	ESP_LOGV(TAG, "crc: ");
	if(p->crc < 0x10)
		ESP_LOGV(TAG, "0");
	ESP_LOGV(TAG, p->crc, HEX);
	return;
#endif
}

void czdec::cmd_w_digit(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, W_CMD *p)
{
#ifdef DEBUG
	int reg_v;

	reg_v = ((W_SMALL_CMD*)p)->reg_value;

	ESP_LOGV(TAG, "Value: ");
	ESP_LOGV(TAG, reg_v);
	ESP_LOGV(TAG, " = 0x");

	if(reg_v < 0x10)
		ESP_LOGV(TAG, "0");
	ESP_LOGV(TAG, reg_v, HEX);

	ESP_LOGV(TAG, "crc: ");
	if(p->crc < 0x10)
		ESP_LOGV(TAG, "0");
	ESP_LOGV(TAG, p->crc, HEX);
	return;
#endif
}

void czdec::reply_r_generic_2byte(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
#ifdef DEBUG
	int reg_v;

	reg_v = p->reg_value[0];

	if(reg_v < 0x10)
		ESP_LOGV(TAG, "0");
	ESP_LOGV(TAG, reg_v, HEX);

	ESP_LOGV(TAG, " ");

	reg_v = p->reg_value[1];

	if(reg_v < 0x10)
		ESP_LOGV(TAG, "0");
	ESP_LOGV(TAG, reg_v, HEX);


	ESP_LOGV(TAG, "crc: ");
	if(p->crc < 0x10)
		ESP_LOGV(TAG, "0");
	ESP_LOGV(TAG, p->crc, HEX);
	return;
#endif
}

void czdec::reply_r_temp(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
#ifdef DEBUG
	int reg_v;
	float reg_v_f;

	reg_v = get_uint16(p->reg_value);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGV(TAG, reg_v_f);
	ESP_LOGV(TAG, "°C");

	ESP_LOGV(TAG, "crc: ");
	if(p->crc < 0x10)
		ESP_LOGV(TAG, "0");
	ESP_LOGV(TAG, p->crc, HEX);
	return;
#endif
}

void czdec::reply_r_temp_1byte(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
#ifdef DEBUG
	int reg_v;
	float reg_v_f;

	reg_v = ((R_SMALL_REPLY*)p)->reg_value;

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGV(TAG, reg_v_f);
	ESP_LOGV(TAG, "°C");

	ESP_LOGV(TAG, "crc: ");
	if(p->crc < 0x10)
		ESP_LOGV(TAG, "0");
	ESP_LOGV(TAG, p->crc, HEX);
	return;
#endif
}

void czdec::reply_r_freq(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
#ifdef DEBUG
	int reg_v;
	float reg_v_f;

	reg_v = get_uint16(p->reg_value);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGV(TAG, reg_v_f);
	ESP_LOGV(TAG, "Hz");

	ESP_LOGV(TAG, "crc: ");
	if(p->crc < 0x10)
		ESP_LOGV(TAG, "0");
	ESP_LOGV(TAG, p->crc, HEX);
	return;
#endif
}

void czdec::reply_r_time(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
#ifdef DEBUG
	int reg_v;
	float reg_v_f;

	reg_v = get_uint16(p->reg_value);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGV(TAG, reg_v_f);
	ESP_LOGV(TAG, "min");

	ESP_LOGV(TAG, "crc: ");
	if(p->crc < 0x10)
		ESP_LOGV(TAG, "0");
	ESP_LOGV(TAG, p->crc, HEX);
	return;
#endif
}

void czdec::reply_r_time_minutes(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
#ifdef DEBUG
	int reg_v;

	reg_v = get_uint16(p->reg_value);

	ESP_LOGV(TAG, reg_v);
	ESP_LOGV(TAG, "min");

	ESP_LOGV(TAG, "crc: ");
	if(p->crc < 0x10)
		ESP_LOGV(TAG, "0");
	ESP_LOGV(TAG, p->crc, HEX);
	return;
#endif
}

void czdec::reply_r_time_days(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
#ifdef DEBUG
	int reg_v;

	reg_v = get_uint16(p->reg_value);

	ESP_LOGV(TAG, reg_v);
	ESP_LOGV(TAG, "days");

	ESP_LOGV(TAG, "crc: ");
	if(p->crc < 0x10)
		ESP_LOGV(TAG, "0");
	ESP_LOGV(TAG, p->crc, HEX);
	return;
#endif
}

void czdec::reply_r_percentage(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
#ifdef DEBUG
	int reg_v;
	float reg_v_f;

	reg_v = get_int16(p->reg_value);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGV(TAG, reg_v_f);
	ESP_LOGV(TAG, "%");

	ESP_LOGV(TAG, "crc: ");
	if(p->crc < 0x10)
		ESP_LOGV(TAG, "0");
	ESP_LOGV(TAG, p->crc, HEX);
	return;
#endif
}

void czdec::reply_r_extra_hot_water(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
#ifdef DEBUG
	int reg_v;

	reg_v = get_uint16(p->reg_value);

	if(reg_v == 0x0C8B)
		ESP_LOGV(TAG, "on");
	else if(reg_v == 0x0C8A)		// -1 in complement 1's
		ESP_LOGV(TAG, "off");
	else
		ESP_LOGV(TAG, reg_v, HEX);

	ESP_LOGV(TAG, "crc: ");
	if(p->crc < 0x10)
		ESP_LOGV(TAG, "0");
	ESP_LOGV(TAG, p->crc, HEX);
	return;
#endif
}

void czdec::reply_r_daylight_saving(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
#ifdef DEBUG
	// not tested
	int reg_v;

	reg_v = get_uint16(p->reg_value);

	if(reg_v == 0xFFBF)		// == ~0x0040
		ESP_LOGV(TAG, "on");
	else if(reg_v == 0x0040)
		ESP_LOGV(TAG, "off");
	else
		ESP_LOGV(TAG, reg_v, HEX);

	ESP_LOGV(TAG, "crc: ");
	if(p->crc < 0x10)
		ESP_LOGV(TAG, "0");
	ESP_LOGV(TAG, p->crc, HEX);
	return;
#endif
}

void czdec::reply_r_sanitary_priority(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
#ifdef DEBUG
	int reg_v;

	reg_v = get_uint16(p->reg_value);

	if(reg_v == 0x4151)
		ESP_LOGV(TAG, "low");
	else if(reg_v == 0x4152)
		ESP_LOGV(TAG, "normal");
	else if(reg_v == 0x4153)
		ESP_LOGV(TAG, "high");
	else
		ESP_LOGV(TAG, reg_v, HEX);

	ESP_LOGV(TAG, "crc: ");
	if(p->crc < 0x10)
		ESP_LOGV(TAG, "0");
	ESP_LOGV(TAG, p->crc, HEX);
	return;
#endif
}

void czdec::reply_r_clr_alarm(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
#ifdef DEBUG
	int reg_v;

	reg_v = get_uint16(p->reg_value);

	if(reg_v & 0x0002)
		ESP_LOGV(TAG, "filter alarm");

	if(reg_v & ~0x0002)
		ESP_LOGV(TAG, reg_v, HEX);

	ESP_LOGV(TAG, "crc: ");
	if(p->crc < 0x10)
		ESP_LOGV(TAG, "0");
	ESP_LOGV(TAG, p->crc, HEX);
	return;
#endif
}

void czdec::reply_r_day_delay(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
#ifdef DEBUG
	int reg_v;

	reg_v = get_uint16(p->reg_value);

	if(reg_v == 0)
		ESP_LOGV(TAG, "off");
	else
	{
		ESP_LOGV(TAG, reg_v);
		ESP_LOGV(TAG, " day(s)");
	}

	ESP_LOGV(TAG, "crc: ");
	if(p->crc < 0x10)
		ESP_LOGV(TAG, "0");
	ESP_LOGV(TAG, p->crc, HEX);
	return;
#endif
}

void czdec::reply_r_fan_speed(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
#ifdef DEBUG
	int reg_v;

	reg_v = ((R_SMALL_REPLY*)p)->reg_value;

	if(reg_v == 0x01)
		ESP_LOGV(TAG, "low");
	else if(reg_v == 0x02)
		ESP_LOGV(TAG, "normal");
	else if(reg_v == 0x03)
		ESP_LOGV(TAG, "high");
	else
		ESP_LOGV(TAG, reg_v, HEX);

	ESP_LOGV(TAG, "crc: ");
	if(p->crc < 0x10)
		ESP_LOGV(TAG, "0");
	ESP_LOGV(TAG, p->crc, HEX);
	return;
#endif
}

void czdec::reply_r_language(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
#ifdef DEBUG
	int reg_v;

	reg_v = ((R_SMALL_REPLY*)p)->reg_value;

	if(reg_v == 0x01)
		ESP_LOGV(TAG, "English");
	else if(reg_v == 0x04)
		ESP_LOGV(TAG, "Français");
	else
		ESP_LOGV(TAG, reg_v, HEX);

	ESP_LOGV(TAG, "crc: ");
	if(p->crc < 0x10)
		ESP_LOGV(TAG, "0");
	ESP_LOGV(TAG, p->crc, HEX);
	return;
#endif
}


void czdec::reply_r_digit(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
#ifdef DEBUG
	int reg_v;

	reg_v = ((R_SMALL_REPLY*)p)->reg_value;

	if(reg_v < 0x10)
		ESP_LOGV(TAG, "0");
	ESP_LOGV(TAG, reg_v, HEX);

	ESP_LOGV(TAG, "crc: ");
	if(p->crc < 0x10)
		ESP_LOGV(TAG, "0");
	ESP_LOGV(TAG, p->crc, HEX);
	return;
#endif
}

void czdec::reply_w_generic(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, W_REPLY *p)
{
#ifdef DEBUG
	ESP_LOGV(TAG, "Value: ");

	ESP_LOGV(TAG, p->return_code);
	ESP_LOGV(TAG, " = 0x");

	if(p->return_code < 0x10)
		ESP_LOGV(TAG, "0");
	ESP_LOGV(TAG, p->return_code, HEX);

	ESP_LOGV(TAG, "crc: ");
	if(p->crc < 0x10)
		ESP_LOGV(TAG, "0");
	ESP_LOGV(TAG, p->crc, HEX);
	return;
#endif
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
