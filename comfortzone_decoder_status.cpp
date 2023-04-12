#include "comfortzone_config.h"
#include "comfortzone_frame.h"
#include "comfortzone_decoder.h"
#include "comfortzone_tools.h"
#include "comfortzone_status.h"

namespace esphome::comfortzone {

static const char* TAG = "ComfortzoneDecoderStatus";

static void dump_unknown(const char *prefix, byte *start, int length)
{
	ESP_LOGD(TAG, "%s : ", prefix);
	ESP_LOG_BUFFER_HEXDUMP(TAG, start, length, ESP_LOG_DEBUG);
}

void czdec::reply_r_status_01(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
	R_REPLY_STATUS_01 *q = (R_REPLY_STATUS_01 *)p;

	czhp->comfortzone_status.fan_time_to_filter_change = get_uint16(q->fan_time_to_filter_change);

	czhp->comfortzone_status.hot_water_setting = get_uint16(q->hot_water_user_setting);

	int reg_v;
	float reg_v_f;
	int i;

	// ===
	dump_unknown("unknown_s01", q->unknown, sizeof(q->unknown));

	// ===
	reg_v = q->extra_hot_water;

	ESP_LOGD(TAG, "Extra hot water: %s (%0X)", (reg_v == 0xFF) ? "off" : "on", reg_v);
	
	// ===
	reg_v = get_uint16(q->hot_water_user_setting);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "Hot water User setting: %f °C", reg_v_f);

	// ===
	reg_v = get_uint16(q->hot_water_hysteresis);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "Hot water - hysteresis: %f °C", reg_v_f);

	// ===
	for(i = 0; i < STATUS_01_NB_HW_NORMAL_STEPS; i++)
	{
		reg_v = get_int16(q->hot_water_normal_steps[i]);

		reg_v_f = reg_v;
		reg_v_f /= 10.0;

		ESP_LOGD(TAG, "Hot water - Normal step %d: %f °C", i, reg_v_f);
	}

	// ===
	for(i = 0; i < STATUS_01_NB_HW_HIGH_STEPS; i++)
	{
		reg_v = get_int16(q->hot_water_high_steps[i]);

		reg_v_f = reg_v;
		reg_v_f /= 10.0;

		ESP_LOGD(TAG, "Hot water - High step %d: %f °C", i, reg_v_f);
	}
	
	// ===
	for(i = 0; i < STATUS_01_NB_HW_EXTRA_STEPS; i++)
	{
		reg_v = get_int16(q->hot_water_extra_steps[i]);

		reg_v_f = reg_v;
		reg_v_f /= 10.0;

		ESP_LOGD(TAG, "Hot water - Additional step %d: %f °C", i, reg_v_f);
	}
	
	// ===
	dump_unknown("unknown_s01_5a", q->unknown5a, sizeof(q->unknown5a));

	// ===
	reg_v = q->hot_water_max_runtime;

	ESP_LOGD(TAG, "Hot water max runtime: %d min", reg_v);

	// ===
	reg_v = q->hot_water_pause_time;

	ESP_LOGD(TAG, "Hot water pause duration: %d min", reg_v);
	
	// ===
	for(i = 0; i < 8; i++)
	{
		reg_v = get_int16(q->unknown5b[i]);

		reg_v_f = reg_v;

		ESP_LOGD(TAG, "unknown_s01_5b[%d]: %f?", i, reg_v_f);
	}


	// ===
	reg_v = get_uint16(q->hot_water_compressor_min_frequency);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "Hot water - compressor min freq.: %f Hz", reg_v_f);

	// ===
	reg_v = get_uint16(q->hot_water_compressor_max_frequency);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "Hot water - compressor max freq.: %f Hz", reg_v_f);
	
	// ===
	dump_unknown("unknown_s01_6", q->unknown6, sizeof(q->unknown6));

	// ===
	reg_v = get_uint16(q->hot_water_extra_setting);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "Hot water - extra setting: %f °C", reg_v_f);

	// ===
	reg_v = get_uint16(q->hot_water_extra_time);

	ESP_LOGD(TAG, "Hot water - extra time: %d min", reg_v);

	// ===
	dump_unknown("unknown_s01_7", q->unknown7, sizeof(q->unknown7));

	// ===
	reg_v = get_uint16(q->normal_fan_speed);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "Normal fan speed: %f %%", reg_v_f);

	// ===
	reg_v = get_int16(q->reduce_fan_speed);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "Reduce fan speed: %f %%", reg_v_f);

	// ===
	reg_v = get_int16(q->fan_boost_increase);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "Fan boost increase: %f %%", reg_v_f);

	// ===
	dump_unknown("unknown_s01_8", q->unknown8, sizeof(q->unknown8));

	// ===
	reg_v = get_int16(q->supply_fan_t12_adjust);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "Supply fan T12 adjust: %f %%", reg_v_f);

	// ===
	dump_unknown("unknown_s01_8a", q->unknown8a, sizeof(q->unknown8a));

	// ===
	reg_v = get_uint16(q->fan_time_to_filter_change);

	ESP_LOGD(TAG, "Fan - Time to filter change: %d days", reg_v);

	// ===
	dump_unknown("unknown_s01_9", q->unknown9, sizeof(q->unknown9));

	// ===
	reg_v = get_uint16(q->heating_compressor_min_frequency);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "Heating compressor min freq.: %f Hz", reg_v_f);

	// ===
	reg_v = get_uint16(q->heating_compressor_max_frequency);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "Heating compressor max freq.: %f Hz", reg_v_f);

	// ===
	dump_unknown("unknown_s01_9a", q->unknown9a, sizeof(q->unknown9a));

	ESP_LOGD(TAG, "crc: %0X", q->crc);
}

void czdec::reply_r_status_02(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
	R_REPLY_STATUS_02 *q = (R_REPLY_STATUS_02 *)p;
	uint16_t active_alarm;

	czhp->comfortzone_status.sensors_te0_outdoor_temp = get_int16(q->sensors[0]);
	czhp->comfortzone_status.sensors_te1_flow_water = get_int16(q->sensors[1]);
	czhp->comfortzone_status.sensors_te1_flow_water_changed = true;

	czhp->comfortzone_status.sensors_te2_return_water = get_int16(q->sensors[2]);
	czhp->comfortzone_status.sensors_te3_indoor_temp= get_int16(q->sensors[3]);
	czhp->comfortzone_status.sensors_te4_hot_gas_temp = get_int16(q->sensors[4]);
	czhp->comfortzone_status.sensors_te5_exchanger_out = get_int16(q->sensors[5]);
	czhp->comfortzone_status.sensors_te6_evaporator_in = get_int16(q->sensors[6]);
	czhp->comfortzone_status.sensors_te7_exhaust_air = get_int16(q->sensors[7]);
	czhp->comfortzone_status.sensors_te24_hot_water_temp = get_int16(q->sensors[24]);

	czhp->comfortzone_status.additional_power_enabled = (q->general_status[0] & 0x20) ? true : false;
	czhp->comfortzone_status.defrost_enabled = (q->general_status[4] & 0x04) ? true : false;

	switch((q->general_status[1]>>4) & 0x3)
	{
		case 0:	czhp->comfortzone_status.compressor_activity = CZCMP_UNKNOWN;
					break;

		case 1:	czhp->comfortzone_status.compressor_activity = CZCMP_STOPPED;
					break;

		case 2:	czhp->comfortzone_status.compressor_activity = CZCMP_RUNNING;
					break;

		case 3:	czhp->comfortzone_status.compressor_activity = CZCMP_STOPPING;
					break;
	}

	switch((q->general_status[1]>>1) & 0x3)
	{
		case 0:	czhp->comfortzone_status.mode = CZMD_IDLE;
					break;

		case 1:	czhp->comfortzone_status.mode = CZMD_ROOM_HEATING;
					break;

		case 2:	czhp->comfortzone_status.mode = CZMD_UNKNOWN;
					break;

		case 3:	czhp->comfortzone_status.mode = CZMD_HOT_WATER;
					break;
	}

	active_alarm = get_uint16(q->pending_alarm) ^ get_uint16(q->acknowledged_alarm);

	czhp->comfortzone_status.filter_alarm = (active_alarm & 0x0002) ? true : false ;

	czhp->comfortzone_status.hour = q->hour1;
	czhp->comfortzone_status.minute = q->minute1;
	czhp->comfortzone_status.second = q->second1;

	czhp->comfortzone_status.day = q->day;
	czhp->comfortzone_status.month = q->month;
	czhp->comfortzone_status.year = 2000 + q->year;
	czhp->comfortzone_status.day_of_week = q->day_of_week;

#ifdef DEBUG
	int reg_v;
	float reg_v_f;
	int i;

	static const char *sensor_names[STATUS_02_NB_SENSORS] =
						{	"TE0 Outdoor temp",
							"TE1 Flow water",
							"TE2 Return water",
							"TE3 Indoor temp. = Heating - Room temperature",
							"TE4 Hot gas temp.",
							"TE5 Exchanger out",
							"TE6 Evaporator in",
							"TE7 Exhaust air",
							"TE8 ?",			// no value
							"TE9 ?",			// no value
							"TE10 ?",
							"TE11 ?",		// no value
							"TE12 ?",		// no value
							"TE13 ?",		// no value
							"TE14 ?",		// no value
							"TE15 ?",		// no value
							"TE16 ?",		// no value
							"TE17 ?",		// no value
							"TE18 ?",		// no value
							"TE19 ?",		// no value
							"TE20 ?",		// no value
							"TE21 ?",		// no value
							"TE22 ?",		// no value
							"TE23 ?",		// no value
							"TE24 Hot water = Hot water Measured temperature",	
							"TE25 ?",
							"TE26 ?",
							"TE27 ?",
							"TE28 ?",
							"TE29 ?",		// always 0.0°C
							"TE30 ?",		// always 100.0°C
							"TE31 ?",		// always 100.0°C
							"TE32 ?",		// always 0.0°C
							"TE33 ?",		// seems to always be close to TE25
							"TE34 ?",
							"TE35 ?",
							"TE36 ?",
							"TE37 ?",		// always 0.0°C
							"TE38 ?",		// always 0.0°C
							"TE39 ?",		// always 0.0°C
							"TE40 ?",
							"TE41 ?",		// seems to always be close to TE1
							"TE42 ?",		// seems to always be close to TE2
							"TE43 ?",		// seems to always be close to TE3
							"TE44 ?",		// seems to always be close to TE4
							"TE45 ?",		// seems to always be close to TE5
							"TE46 ?",		// seems to always be close to TE6
							"TE47 ?",
							"TE48 ?",		// seems to always be close to TE7
							"TE49 ?",		// no value
							"TE50 ?",		// no value
							"TE51 ?",
							"TE52 ?",		// no value
							"TE53 ?",		// no value
							"TE54 ?",		// no value
							"TE55 ?",		// no value
							"TE56 ?",		// no value
							"TE57 ?",		// no value
							"TE58 ?",		// no value
							"TE59 ?",		// no value
							"TE60 ?",		// no value
							"TE61 ?",		// no value
							"TE62 ?"			// no value
						};

	// ===
	dump_unknown("unknown_s02", q->unknown, sizeof(q->unknown));

	// ===
	ESP_LOGD(TAG, "Time1: ");
	ESP_LOGD(TAG, q->hour1);
	ESP_LOGD(TAG, ":");
	ESP_LOGD(TAG, q->minute1);
	ESP_LOGD(TAG, ":");
	ESP_LOGD(TAG, q->second1);
	ESP_LOGD(TAG, );

	// ===
	ESP_LOGD(TAG, "Day: ");
	ESP_LOGD(TAG, q->day);
	ESP_LOGD(TAG, "/");
	ESP_LOGD(TAG, q->month);
	ESP_LOGD(TAG, "/");
	ESP_LOGD(TAG, q->year + 2000);
	ESP_LOGD(TAG, );

	// ===
	ESP_LOGD(TAG, "Day of week: ");
	ESP_LOGD(TAG, q->day_of_week);

	// ===
	ESP_LOGD(TAG, "Time2: ");
	ESP_LOGD(TAG, q->hour2);
	ESP_LOGD(TAG, ":");
	ESP_LOGD(TAG, q->minute2);
	ESP_LOGD(TAG, ":");
	ESP_LOGD(TAG, q->second2);
	ESP_LOGD(TAG, );

	// ===
	dump_unknown("unknown_s02_3", q->unknown3, sizeof(q->unknown3));

	// ===
	dump_unknown("unknown_general_status", q->general_status, sizeof(q->general_status));

	// ===
	ESP_LOGD(TAG, "Add energy: ");
	ESP_LOGD(TAG,  (q->general_status[0] & 0x20) ? "on" : "off");

	ESP_LOGD(TAG, "Mode (1): ");
	switch(q->general_status[0] & 0x3)
	{
		case 0:	ESP_LOGD(TAG, "Heating");
					break;
		case 1:	ESP_LOGD(TAG, "1?");
					break;
		case 2:	ESP_LOGD(TAG, "2?");
					break;
		case 3:	ESP_LOGD(TAG, "Hot water");
					break;
	}

	ESP_LOGD(TAG, "Heatpump activity: ");
	switch((q->general_status[1]>>4) & 0x3)
	{
		case 0:	ESP_LOGD(TAG, "Stopped ?");
					break;
		case 1:	ESP_LOGD(TAG, "Stopped");
					break;
		case 2:	ESP_LOGD(TAG, "Running");
					break;
		case 3:	ESP_LOGD(TAG, "Stopping");
					break;
	}

	ESP_LOGD(TAG, "Mode (2): ");
	switch((q->general_status[1]>>1) & 0x3)
	{
		case 0:	ESP_LOGD(TAG, "Idle");
					break;
		case 1:	ESP_LOGD(TAG, "Heating");
					break;
		case 2:	ESP_LOGD(TAG, "2?");
					break;
		case 3:	ESP_LOGD(TAG, "Hot water");
					break;
	}

	ESP_LOGD(TAG, "Defrost: ");
	ESP_LOGD(TAG,  (q->general_status[4] & 0x04) ? "on" : "off");

	// ===
	dump_unknown("unknown_s02_3b", q->unknown3b, sizeof(q->unknown3b));

	// ===
	reg_v = get_uint16(q->pending_alarm);
	ESP_LOGD(TAG, "Pending alarm: ");

	if(reg_v & 0x0002)
	{
		ESP_LOGD(TAG, "filter ");
	}

	if(reg_v & ~0x0002)
	{
		ESP_LOGD(TAG, "(0x");
		ESP_LOGD(TAG, reg_v);
		ESP_LOGD(TAG, ")");
	}
	ESP_LOGD(TAG, "");

	// ===
	dump_unknown("unknown_s02_3c", q->unknown3c, sizeof(q->unknown3c));

	// ===
	reg_v = get_uint16(q->acknowledged_alarm);
	ESP_LOGD(TAG, "Acknowledged alarm: ");

	if(reg_v & 0x0002)
	{
		ESP_LOGD(TAG, "filter ");
	}

	if(reg_v & ~0x0002)
	{
		ESP_LOGD(TAG, "(0x");
		ESP_LOGD(TAG, reg_v);
		ESP_LOGD(TAG, ")");
	}
	ESP_LOGD(TAG, "");

	// ===
	dump_unknown("unknown_s02_3d", q->unknown3d, sizeof(q->unknown3d));

	// ===
	for(i = 0 ; i < STATUS_02_NB_SENSORS; i++)
	{
		reg_v = get_int16(q->sensors[i]);

		if(reg_v == -990)		// pas de valeur pour le sensor ? (= -99.0°=
			continue;

		reg_v_f = reg_v;
		reg_v_f /= 10.0;

		ESP_LOGD(TAG, "Sensor - ");
		ESP_LOGD(TAG, sensor_names[i]);
		ESP_LOGD(TAG, ": ");

		ESP_LOGD(TAG, reg_v_f);
		ESP_LOGD(TAG, "°C (0x");
		ESP_LOGD(TAG, reg_v, HEX);
		ESP_LOGD(TAG, " 0x");

		if(q->sensors[i][0] < 0x10)
			ESP_LOGD(TAG, "0");
		ESP_LOGD(TAG, q->sensors[i][0], HEX);

		ESP_LOGD(TAG, " ");
		if(q->sensors[i][1] < 0x10)
			ESP_LOGD(TAG, "0");
		ESP_LOGD(TAG, q->sensors[i][1], HEX);

		ESP_LOGD(TAG, ")");
	}

	// ===
	ESP_LOGD(TAG, "crc: ");
	if(q->crc < 0x10)
		ESP_LOGD(TAG, "0");
	ESP_LOGD(TAG, q->crc, HEX);
#endif
}

void czdec::reply_r_status_03(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
#ifdef DEBUG
	R_REPLY_STATUS_03 *q = (R_REPLY_STATUS_03 *)p;

	//int reg_v;
	//float reg_v_f;

	// ===
	dump_unknown("unknown_s03", q->unknown, sizeof(q->unknown));

	ESP_LOGD(TAG, "crc: ");
	if(q->crc < 0x10)
		ESP_LOGD(TAG, "0");
	ESP_LOGD(TAG, q->crc, HEX);
#endif
}

void czdec::reply_r_status_04(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
#ifdef DEBUG
	R_REPLY_STATUS_04 *q = (R_REPLY_STATUS_04 *)p;

	int reg_v;
	//float reg_v_f;

	// ===
	dump_unknown("unknown_s04", q->unknown, sizeof(q->unknown));

	// ===
	reg_v = get_uint16(q->chauffage_puissance_consommee1);

	ESP_LOGD(TAG, "Chauffage - Puissance consommée 1: ");
	ESP_LOGD(TAG, reg_v);
	ESP_LOGD(TAG, "W");

	// ===
	dump_unknown("unknown_s04_8", q->unknown8, sizeof(q->unknown8));

	// ===
	reg_v = get_uint16(q->chauffage_puissance_consommee2);

	ESP_LOGD(TAG, "Chauffage - Puissance consommée 2: ");
	ESP_LOGD(TAG, reg_v);
	ESP_LOGD(TAG, "W");

	// ===
	reg_v = q->hot_water_production;

	ESP_LOGD(TAG, "Hot water in progress (s4): ");
	if(reg_v == 0x00)
		ESP_LOGD(TAG, "no");
	else if(reg_v == 0x77)
		ESP_LOGD(TAG, "yes (0x77)");
	else if(reg_v == 0x78)
		ESP_LOGD(TAG, "yes (0x78)");
	else
		ESP_LOGD(TAG, reg_v, HEX);

	// ===
	dump_unknown("unknown_s04_9", q->unknown9, sizeof(q->unknown9));

	ESP_LOGD(TAG, "crc: ");
	if(q->crc < 0x10)
		ESP_LOGD(TAG, "0");
	ESP_LOGD(TAG, q->crc, HEX);
#endif
}

void czdec::reply_r_status_05(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
	int reg_v;

	R_REPLY_STATUS_05 *q = (R_REPLY_STATUS_05 *)p;

	if(q->hot_water_production == 0x00)
		czhp->comfortzone_status.hot_water_production = false;
	else
		czhp->comfortzone_status.hot_water_production = true;

	reg_v = get_uint16(q->room_heating_in_progress);
	if(reg_v == 0x012C)
		czhp->comfortzone_status.room_heating_in_progress = false;
	else
		czhp->comfortzone_status.room_heating_in_progress = true;

	czhp->comfortzone_status.fan_speed = q->fan_speed;
	czhp->comfortzone_status.fan_speed_duty = get_uint16(q->fan_speed_duty);

	czhp->comfortzone_status.room_heating_setting = get_uint16(q->heating_calculated_setting);
	czhp->comfortzone_status.hot_water_calculated_setting = get_uint16(q->hot_water_calculated_setting);

	czhp->comfortzone_status.extra_hot_water_setting = ((q->extra_hot_water == 0x0F)? true : false);

#ifdef DEBUG
	float reg_v_f;
	int i;

	// ===
	reg_v = q->hot_water_production;

	ESP_LOGD(TAG, "Hot water in progress (s5): ");
	if(reg_v == 0x00)
		ESP_LOGD(TAG, "no");
	else if(reg_v == 0x77)
		ESP_LOGD(TAG, "yes (0x77)");
	else if(reg_v == 0x78)
		ESP_LOGD(TAG, "yes (0x78)");
	else
		ESP_LOGD(TAG, reg_v, HEX);

	// ===
	dump_unknown("unknown_s05a", q->unknown, sizeof(q->unknown));

	// ===
	reg_v = get_uint16(q->heating_calculated_setting);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "Heating - Calculated setting: ");
	ESP_LOGD(TAG, reg_v_f);
	ESP_LOGD(TAG, "°C");
	
	// ===
	dump_unknown("unknown_s05_02", q->unknown0, sizeof(q->unknown0));

	ESP_LOGD(TAG, "TE3 Indoor temp history (new values first, 1 value = 10seconds): ");
	// ===
	for(i = 0; i < STATUS_05_TE3_INDOOR_TEMP_HISTORY_NB; i++)
	{
		reg_v = get_int16(q->te3_indoor_temp_history[i]);

		reg_v_f = reg_v;
		reg_v_f /= 10.0;

		ESP_LOGD(TAG, reg_v_f);
		ESP_LOGD(TAG, "°C ");
	}
	ESP_LOGD(TAG, "");

	// ===
	ESP_LOGD(TAG, "TE2 Return water history (new values first, 1 value = 10seconds): ");
	for(i = 0; i < STATUS_05_TE2_RETURN_WATER_HISTORY_NB; i++)
	{
		reg_v = get_int16(q->te2_return_water_history[i]);

		reg_v_f = reg_v;
		reg_v_f /= 10.0;

		ESP_LOGD(TAG, reg_v_f);
		ESP_LOGD(TAG, "°C ");
	}
	ESP_LOGD(TAG, "");

	// ===
	reg_v = get_uint16(q->room_heating_in_progress);

	ESP_LOGD(TAG, "Room heating in progress: ");
	if(reg_v == 0x012C)
	{
		ESP_LOGD(TAG, "no (0x012C)");
	}
	else
	{
		ESP_LOGD(TAG, "yes (0x");
		ESP_LOGD(TAG, reg_v, HEX);
		ESP_LOGD(TAG, ")");
	}

	// ===
	dump_unknown("unknown_s05_1", q->unknown1, sizeof(q->unknown1));

	// ===
	reg_v = get_uint16(q->hot_water_calculated_setting);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "Hot water Calculated setting (s05): ");
	ESP_LOGD(TAG, reg_v_f);
	ESP_LOGD(TAG, "°C");

	// ===
	dump_unknown("unknown_s05_2", q->unknown2, sizeof(q->unknown2));

	// ===
	reg_v = get_uint16(q->unknown_count_down);

	ESP_LOGD(TAG, "s05 - unknown count down (seems to be hot water remaining runtime. 0x00 00 when not running else count down from hotwater max runtime): ");
	ESP_LOGD(TAG, reg_v);
	ESP_LOGD(TAG, "seconds");
	
	// ===
	dump_unknown("unknown_s05_2b", q->unknown2b, sizeof(q->unknown2b));

	// ===
	reg_v = q->extra_hot_water;

	ESP_LOGD(TAG, "Extra hot water (s05): ");
	if(reg_v == 0x00)
		ESP_LOGD(TAG, "off");
	else if(reg_v == 0x0F)
		ESP_LOGD(TAG, "on");
	else
		ESP_LOGD(TAG, reg_v, HEX);

	// ===
	dump_unknown("unknown_s05_2c", q->unknown2c, sizeof(q->unknown2c));

	// ===
	reg_v = q->fan_speed;

	ESP_LOGD(TAG, "Fan speed: ");
	if(reg_v == 0x01)
		ESP_LOGD(TAG, "low");
	else if(reg_v == 0x02)
		ESP_LOGD(TAG, "normal");
	else if(reg_v == 0x03)
		ESP_LOGD(TAG, "high");
	else
		ESP_LOGD(TAG, reg_v, HEX);

	// ===
	dump_unknown("unknown_s05_3", q->unknown3, sizeof(q->unknown3));

	// ===
	reg_v = get_uint16(q->condensing_temperature);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "Condensing temperature: ");
	ESP_LOGD(TAG, reg_v_f);
	ESP_LOGD(TAG, "°C");

	// ===
	reg_v = get_uint16(q->condensing_pressure);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "Condensing pressure: ");
	ESP_LOGD(TAG, reg_v_f);
	ESP_LOGD(TAG, "bar");

	// ===

	ESP_LOGD(TAG, "crc: ");
	if(q->crc < 0x10)
		ESP_LOGD(TAG, "0");
	ESP_LOGD(TAG, q->crc, HEX);
#endif
}

void czdec::reply_r_status_06(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
	R_REPLY_STATUS_06 *q = (R_REPLY_STATUS_06 *)p;

	czhp->comfortzone_status.heatpump_current_compressor_frequency = get_uint16(q->heatpump_current_compressor_frequency);
	czhp->comfortzone_status.heatpump_current_compressor_frequency_changed = true;

	czhp->comfortzone_status.heatpump_current_compressor_power = get_uint16(q->heatpump_current_compressor_power);
	czhp->comfortzone_status.heatpump_current_compressor_power_changed = true;
	czhp->comfortzone_status.heatpump_current_add_power = get_uint16(q->heatpump_current_add_power);
	czhp->comfortzone_status.heatpump_current_add_power_changed = true;
	czhp->comfortzone_status.heatpump_current_total_power = get_uint16(q->heatpump_current_total_power1);
	czhp->comfortzone_status.heatpump_current_total_power_changed = true;
	czhp->comfortzone_status.heatpump_current_compressor_input_power = get_uint16(q->heatpump_compressor_input_power);
	czhp->comfortzone_status.heatpump_current_compressor_input_power_changed = true;

	int reg_v;
	float reg_v_f;

	// ===
	reg_v = get_uint16(q->evaporator_pressure);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "Evaporator pressure: %f bar", reg_v_f);

	// ===
	reg_v = get_uint16(q->pressure_ratio);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "Pressure ratio: %f", reg_v_f);

	// ===
	dump_unknown("unknown_s06_0a", q->unknown0a, sizeof(q->unknown0a));

	// ===
	reg_v = get_uint16(q->heatpump_compressor_max_frequency1);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	// not real max as it changes autonomously
	ESP_LOGD(TAG, "Heatpump - Compressor max frequency (1) (erroneous): %f Hz", reg_v_f);

	// ===
	reg_v = get_uint16(q->hot_water_active_max_frequency);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	// not real max as it changes autonomously
	ESP_LOGD(TAG, "Hot water - Compressor active max frequency: %f Hz", reg_v_f);

	// ===
	// During defrost, forced to 0Hz else set to heating compressor max frequency
	reg_v = get_uint16(q->heatpump_active_max_frequency1);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "Heatpump - Compressor active max frequency (during defrost, set to 0Hz else real compressor max frequency) (1): %f Hz", reg_v_f);

	// ===
	dump_unknown("unknown_s06_0c", q->unknown0c, sizeof(q->unknown0c));

	// ===
	reg_v = get_uint16(q->heatpump_active_max_frequency2);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "Heatpump - Compressor active max frequency (during defrost, set to 0Hz else real compressor max frequency) (2): %f Hz", reg_v_f);

	// ===
	reg_v = get_uint16(q->heatpump_active_max_frequency3);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "Heatpump - Compressor active max frequency (during defrost, set to 0Hz else real compressor max frequency) (3): %f Hz", reg_v_f);

	// ===
	reg_v = get_uint16(q->heatpump_current_compressor_frequency);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "Heatpump - current compressor frequency: %f Hz", reg_v_f);

	// ===
	reg_v = get_uint16(q->chauffage_compressor_max_frequency3);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "Chauffage - Compressor max frequency (3): %f Hz", reg_v_f);

	// ===
	dump_unknown("unknown_s06_0d", q->unknown0d, sizeof(q->unknown0d));

	// ===
	reg_v = get_uint16(q->heating_compressor_min_frequency);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "Heating - Compressor min frequency: %f Hz", reg_v_f);

	// ===
	reg_v = get_uint16(q->heating_compressor_max_frequency);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "Heating - Compressor max frequency: %f Hz", reg_v_f);

	// ===
	dump_unknown("unknown_s06_0", q->unknown0, sizeof(q->unknown0));

	// ===
	reg_v = get_uint16(q->heatpump_current_compressor_power);

	ESP_LOGD(TAG, "Heatpump - current compressor power: %d W", reg_v);

	// ===
	reg_v = get_uint16(q->heatpump_current_add_power);

	ESP_LOGD(TAG, "Heatpump - current add power: %d W", reg_v);

	// ===
	reg_v = get_uint16(q->heatpump_current_total_power1);

	ESP_LOGD(TAG, "Heatpump - current total power 1: %d W", reg_v);

	// ===
	reg_v = get_uint16(q->heatpump_current_total_power2);

	ESP_LOGD(TAG, "Heatpump - current total power 2: %d W", reg_v);

	// ===
	reg_v = get_uint16(q->heatpump_compressor_input_power);

	ESP_LOGD(TAG, "Heatpump - Compressor input power: %d W", reg_v);

	// ===
	dump_unknown("unknown_s06_1a", q->unknown1a, sizeof(q->unknown1a));

	// ===
	reg_v = get_uint16(q->unknown_count_down);

	ESP_LOGD(TAG, "Heatpump - remaining min runtime (?): %d seconds", reg_v);

	// ===
	dump_unknown("unknown_s06_1b", q->unknown1b, sizeof(q->unknown1b));

	// ===
	reg_v = get_uint16(q->heatpump_defrost_delay);

	ESP_LOGD(TAG, "Heatpump - remaining time to next defrost (=remaining max runtime): %d seconds", reg_v);

	// ===
	dump_unknown("unknown_s06_2", q->unknown2, sizeof(q->unknown2));

	// ===
	reg_v = get_uint16(q->expansion_valve_calculated_setting);
	reg_v_f = reg_v / 10;

	ESP_LOGD(TAG, "Expansion valve - Calculated setting: %f K", reg_v_f);

	// ===
	reg_v = get_int16(q->vanne_expansion_xxx);
	reg_v_f = reg_v / 10;

	ESP_LOGD(TAG, "Vanne expansion - xxx?: %f K", reg_v_f);

	// ===
	reg_v = get_int16(q->expansion_valve_temperature_difference1);
	reg_v_f = reg_v / 10;

	ESP_LOGD(TAG, "Expansion valve - Temperature difference 1: %f K", reg_v_f);

	// ===
	reg_v = get_int16(q->expansion_valve_temperature_difference2);
	reg_v_f = reg_v / 10;

	ESP_LOGD(TAG, "Expansion valve - Temperature difference 2: %f K", reg_v_f);

	// ===
	dump_unknown("unknown_s06_2a", q->unknown2a, sizeof(q->unknown2a));

	// ===
	reg_v = get_uint16(q->expansion_valve_valve_position1);
	reg_v_f = reg_v / 10;

	ESP_LOGD(TAG, "Expansion valve - Valve position 1: %f %%", reg_v_f);

	// ===
	reg_v = get_uint16(q->expansion_valve_valve_position1);
	reg_v_f = reg_v / 10;

	ESP_LOGD(TAG, "Expansion valve - Valve position 2: %f %%", reg_v_f);

	// ===
	dump_unknown("unknown_s06_2a", q->unknown2a, sizeof(q->unknown2a));

	ESP_LOGD(TAG, "crc: %0X", q->crc);
}

void czdec::reply_r_status_07(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
#ifdef DEBUG
	R_REPLY_STATUS_07 *q = (R_REPLY_STATUS_07 *)p;

	int reg_v;
	//float reg_v_f;

	// ===
	dump_unknown("unknown_s07", q->unknown, sizeof(q->unknown));

	// ===
	reg_v = get_uint16(q->input_power_limit);

	ESP_LOGD(TAG, "Input power limit: ");
	ESP_LOGD(TAG, reg_v);
	ESP_LOGD(TAG, "W");

	// ===
	dump_unknown("unknown_s07_2", q->unknown2, sizeof(q->unknown2));

	ESP_LOGD(TAG, "crc: ");
	if(q->crc < 0x10)
		ESP_LOGD(TAG, "0");
	ESP_LOGD(TAG, q->crc, HEX);
#endif
}

void czdec::reply_r_status_08(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
	R_REPLY_STATUS_08 *q = (R_REPLY_STATUS_08 *)p;

	czhp->comfortzone_status.compressor_energy = get_uint32(q->compressor_energy);
	czhp->comfortzone_status.add_energy = get_uint32(q->add_energy);
	czhp->comfortzone_status.hot_water_energy = get_uint32(q->hot_water_energy);

	czhp->comfortzone_status.compressor_runtime = get_uint32(q->compressor_runtime);
	czhp->comfortzone_status.total_runtime = get_uint32(q->total_runtime);

#ifdef DEBUG
	int reg_v;
	float reg_v_f;

	// ===
	reg_v = q->bcd_second;

	ESP_LOGD(TAG, "Second (BCD): ");
	ESP_LOGD(TAG, reg_v, HEX);

	// ===
	reg_v = q->bcd_minute;

	ESP_LOGD(TAG, "Minute (BCD): ");
	ESP_LOGD(TAG, reg_v, HEX);

	// ===
	reg_v = q->bcd_hour;

	ESP_LOGD(TAG, "Hour (BCD): ");
	ESP_LOGD(TAG, reg_v, HEX);

	// ===
	dump_unknown("unknown_s08_0 (increase by 1 every day but it is not day of week)", &(q->unknown0), sizeof(q->unknown0));

	// ===
	reg_v = q->bcd_day;

	ESP_LOGD(TAG, "Day (BCD): ");
	ESP_LOGD(TAG, reg_v, HEX);

	// ===
	reg_v = q->bcd_month;

	ESP_LOGD(TAG, "Month (BCD): ");
	ESP_LOGD(TAG, reg_v, HEX);

	// ===
	reg_v = q->bcd_year;

	ESP_LOGD(TAG, "Year (BCD)(20xx): ");
	ESP_LOGD(TAG, reg_v, HEX);

	// ===
	dump_unknown("unknown_s08_0a", q->unknown0a, sizeof(q->unknown0a));

	// ===
	dump_unknown("unknown_s08_", q->unknown, sizeof(q->unknown));

	// ===
	reg_v = get_uint32(q->compressor_energy);

	reg_v_f = reg_v;
	reg_v_f /= 100.0;

	ESP_LOGD(TAG, "Compressor energy: ");
	ESP_LOGD(TAG, reg_v_f);
	ESP_LOGD(TAG, "kWh");

	// ===
	reg_v = get_uint32(q->add_energy);

	reg_v_f = reg_v;
	reg_v_f /= 100.0;

	ESP_LOGD(TAG, "Add energy: ");
	ESP_LOGD(TAG, reg_v_f);
	ESP_LOGD(TAG, "kWh");

	// ===
	reg_v = get_uint32(q->hot_water_energy);

	reg_v_f = reg_v;
	reg_v_f /= 100.0;

	ESP_LOGD(TAG, "Hot water energy: ");
	ESP_LOGD(TAG, reg_v_f);
	ESP_LOGD(TAG, "kWh");

	// ===
	reg_v = get_uint32(q->compressor_runtime);

	ESP_LOGD(TAG, "Compressor runtime: ");
	ESP_LOGD(TAG, reg_v / 60);
	ESP_LOGD(TAG, ":");
	reg_v = reg_v % 60;
	if(reg_v < 10)
		ESP_LOGD(TAG, "0");
	ESP_LOGD(TAG, reg_v);
	ESP_LOGD(TAG, "");

	// ===
	reg_v = get_uint32(q->total_runtime);

	ESP_LOGD(TAG, "Total runtime: ");
	ESP_LOGD(TAG, reg_v / 60);
	ESP_LOGD(TAG, ":");
	reg_v = reg_v % 60;
	if(reg_v < 10)
		ESP_LOGD(TAG, "0");
	ESP_LOGD(TAG, reg_v);
	ESP_LOGD(TAG, "");

	// ===
	ESP_LOGD(TAG, "crc: ");
	if(q->crc < 0x10)
		ESP_LOGD(TAG, "0");
	ESP_LOGD(TAG, q->crc, HEX);
#endif
}

void czdec::reply_r_status_09(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
	int reg_v;

	R_REPLY_STATUS_09 *q = (R_REPLY_STATUS_09 *)p;

	reg_v = get_uint16(q->hotwater_priority);

	if(reg_v == 0x4151)
		czhp->comfortzone_status.hot_water_priority_setting = 1;
	else if(reg_v == 0x4152)
		czhp->comfortzone_status.hot_water_priority_setting = 2;
	else if(reg_v == 0x4153)
		czhp->comfortzone_status.hot_water_priority_setting = 3;

#ifdef DEBUG
	float reg_v_f;
	int i;

	// ===
	dump_unknown("unknown_s09", q->unknown, sizeof(q->unknown));

	// ===
	reg_v = get_uint16(q->hotwater_priority);

	ESP_LOGD(TAG, "Hot water priority: ");
	if(reg_v == 0x4151)
		ESP_LOGD(TAG, "low");
	else if(reg_v == 0x4152)
		ESP_LOGD(TAG, "normal");
	else if(reg_v == 0x4153)
		ESP_LOGD(TAG, "high");
	else
		ESP_LOGD(TAG, reg_v, HEX);

	// ===
	dump_unknown("unknown_s09_2", q->unknown2, sizeof(q->unknown2));

	// ===
	for(i = 0; i < STATUS_09_NB_AJUSTMENT; i++)
	{
		reg_v = q->hardware_settings_adjustments_teX_adjust[i];

		reg_v_f = reg_v;
		reg_v_f /= 10.0;

		ESP_LOGD(TAG, "Hardware settings - Adjustments - TE");
		ESP_LOGD(TAG, i);
		ESP_LOGD(TAG, " Adjust: ");

		ESP_LOGD(TAG, reg_v_f);
		ESP_LOGD(TAG, "°C (0x");

		if(reg_v < 0x10)
			ESP_LOGD(TAG, "0");
		ESP_LOGD(TAG, reg_v, HEX);

		ESP_LOGD(TAG, ")");
	}

	// ===
	dump_unknown("unknown_s09_6", q->unknown6, sizeof(q->unknown6));

	ESP_LOGD(TAG, "crc: ");
	if(q->crc < 0x10)
		ESP_LOGD(TAG, "0");
	ESP_LOGD(TAG, q->crc, HEX);
#endif
}

void czdec::reply_r_status_10(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
#ifdef DEBUG
	R_REPLY_STATUS_10 *q = (R_REPLY_STATUS_10 *)p;

	int reg_v;
	float reg_v_f;
	int i;

	// ===
	// seems to never change
	dump_unknown("unknown_s10_", q->unknown, sizeof(q->unknown));

	// ===
	reg_v = get_uint16(q->holiday_temperature_reduction);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "Holiday temperature reduction: ");
	ESP_LOGD(TAG, reg_v_f);
	ESP_LOGD(TAG, "°C");

	// ===
	reg_v = get_uint16(q->holiday_minimal_room_temperature);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "CW Minimal room temperature: ");
	ESP_LOGD(TAG, reg_v_f);
	ESP_LOGD(TAG, "°C");

	// ===
	reg_v = get_uint16(q->cw_minimal_temperature);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "CW Minimal temperature: ");
	ESP_LOGD(TAG, reg_v_f);
	ESP_LOGD(TAG, "°C");

	// ===
	reg_v = get_uint16(q->cw_maximal_temperature);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "CW Maximal temperature: ");
	ESP_LOGD(TAG, reg_v_f);
	ESP_LOGD(TAG, "°C");

	// ===
	reg_v = get_uint16(q->cw_overheat_hysteresis);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "CW overheat hysteresis: ");
	ESP_LOGD(TAG, reg_v_f);
	ESP_LOGD(TAG, "°C");

	// ===
	// seems to never change
	dump_unknown("unknown_s10_2a", q->unknown2a, sizeof(q->unknown2a));

	// ===
	reg_v = get_uint16(q->cw_return_maximal_temperature);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "CW return maximal temperature: ");
	ESP_LOGD(TAG, reg_v_f);
	ESP_LOGD(TAG, "°C");

	// ===
	// seems to never change
	dump_unknown("unknown_s10_2b", q->unknown2b, sizeof(q->unknown2b));

	// ===
	reg_v = get_uint16(q->cw_pump);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "CW pump: ");
	ESP_LOGD(TAG, reg_v_f);
	ESP_LOGD(TAG, "%");

	// ===
	dump_unknown("unknown_s10_2", q->unknown2, sizeof(q->unknown2));

	// ===
	for(i = 0; i < STATUS_10_STEP_WISE_HEATING_NB; i++)
	{
		reg_v = get_uint16(q->step_wise_heating_temperature[i]);

		reg_v_f = reg_v;
		reg_v_f /= 10.0;

		ESP_LOGD(TAG, "Step-wise heating temperature ");
		ESP_LOGD(TAG, i);
		ESP_LOGD(TAG, ": ");
		ESP_LOGD(TAG, reg_v_f);
		ESP_LOGD(TAG, "°C");

		reg_v_f = q->step_wise_heating_days[i];
		reg_v_f /= 10.0;

		ESP_LOGD(TAG, "Step-wise heating days ");
		ESP_LOGD(TAG, i);
		ESP_LOGD(TAG, ": ");
		ESP_LOGD(TAG, reg_v_f);
		ESP_LOGD(TAG, "day");
	}

	// ===
	dump_unknown("unknown_s10_3", q->unknown3, sizeof(q->unknown3));

	ESP_LOGD(TAG, "crc: ");
	if(q->crc < 0x10)
		ESP_LOGD(TAG, "0");
	ESP_LOGD(TAG, q->crc, HEX);
#endif
}

void czdec::reply_r_status_11(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
	R_REPLY_STATUS_11 *q = (R_REPLY_STATUS_11 *)p;

	czhp->comfortzone_status.led_luminosity_setting = q->led_luminosity;

#ifdef DEBUG
	int reg_v;
	//float reg_v_f;

	// ===
	// seems to never change
	dump_unknown("unknown_s11_", q->unknown, sizeof(q->unknown));

	// ===
	reg_v = q->led_luminosity;

	ESP_LOGD(TAG, "Led luminosity: ");
	ESP_LOGD(TAG, reg_v);

	// ===
	// seems to never change
	dump_unknown("unknown_s11_2", q->unknown2, sizeof(q->unknown2));

	// ===
	reg_v = get_uint16(q->holiday_reduction);

	ESP_LOGD(TAG, "Holiday reduction: ");
	if(reg_v == 0)
		ESP_LOGD(TAG, "off");
	else
	{
		ESP_LOGD(TAG, reg_v);
		ESP_LOGD(TAG, " day(s)");
	}

	// ===
	// seems to never change
	dump_unknown("unknown_s11_3", q->unknown3, sizeof(q->unknown3));

	ESP_LOGD(TAG, "crc: ");
	if(q->crc < 0x10)
		ESP_LOGD(TAG, "0");
	ESP_LOGD(TAG, q->crc, HEX);
#endif
}

void czdec::reply_r_status_12(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
#ifdef DEBUG
	R_REPLY_STATUS_12 *q = (R_REPLY_STATUS_12 *)p;

	int reg_v;
	float reg_v_f;

	// ===
	// seems to never change
	dump_unknown("unknown_s12_", q->unknown, sizeof(q->unknown));

	// ===
	reg_v = get_uint16(q->heatpump_compressor_blocked_frequency1);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "Heatpump - compressor - blocked frequency 1: ");
	ESP_LOGD(TAG, reg_v_f);
	ESP_LOGD(TAG, "Hz");

	// ===
	reg_v = get_uint16(q->heatpump_compressor_blocked_frequency2);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "Heatpump - compressor - blocked frequency 2: ");
	ESP_LOGD(TAG, reg_v_f);
	ESP_LOGD(TAG, "Hz");

	// ===
	reg_v = get_uint16(q->heatpump_compressor_blocked_frequency3);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "Heatpump - compressor - blocked frequency 3: ");
	ESP_LOGD(TAG, reg_v_f);
	ESP_LOGD(TAG, "Hz");

	// ===
	dump_unknown("unknown_s12_3", q->unknown3, sizeof(q->unknown3));

	// ===
	reg_v = get_uint16(q->heatpump_defrost_delay);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "Heatpump - defrost delay: ");
	ESP_LOGD(TAG, reg_v_f);
	ESP_LOGD(TAG, "min");

	// ===
	// seems to never change
	dump_unknown("unknown_s12_8", q->unknown8, sizeof(q->unknown8));

	ESP_LOGD(TAG, "crc: ");
	if(q->crc < 0x10)
		ESP_LOGD(TAG, "0");
	ESP_LOGD(TAG, q->crc, HEX);
#endif
}

void czdec::reply_r_status_13(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
#ifdef DEBUG
	R_REPLY_STATUS_13 *q = (R_REPLY_STATUS_13 *)p;

	//int reg_v;
	//float reg_v_f;

	// ===
	// seems to never change
	dump_unknown("unknown_s13", q->unknown, sizeof(q->unknown));

	ESP_LOGD(TAG, "crc: ");
	if(q->crc < 0x10)
		ESP_LOGD(TAG, "0");
	ESP_LOGD(TAG, q->crc, HEX);
#endif
}

void czdec::reply_r_status_14(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
#ifdef DEBUG
	R_REPLY_STATUS_14 *q = (R_REPLY_STATUS_14 *)p;

	int reg_v;
	//float reg_v_f;

	// ===
	// seems to never change
	dump_unknown("unknown_s14", q->unknown, sizeof(q->unknown));

	// ===
	reg_v = q->language;

	ESP_LOGD(TAG, "Language: ");
	if(reg_v == 0x01)
		ESP_LOGD(TAG, "English");
	else if(reg_v == 0x04)
		ESP_LOGD(TAG, "Français");
	else
		ESP_LOGD(TAG, reg_v, HEX);

	// ===
	// seems to never change
	dump_unknown("unknown_s14d", q->unknownd, sizeof(q->unknownd));

	ESP_LOGD(TAG, "crc: ");
	if(q->crc < 0x10)
		ESP_LOGD(TAG, "0");
	ESP_LOGD(TAG, q->crc, HEX);
#endif
}

void czdec::reply_r_status_15(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
#ifdef DEBUG
	R_REPLY_STATUS_15 *q = (R_REPLY_STATUS_15 *)p;

	//int reg_v;
	//float reg_v_f;

	// ===
	// seems to never change
	dump_unknown("unknown_s15", q->unknown, sizeof(q->unknown));

	ESP_LOGD(TAG, "crc: ");
	if(q->crc < 0x10)
		ESP_LOGD(TAG, "0");
	ESP_LOGD(TAG, q->crc, HEX);
#endif
}

void czdec::reply_r_status_16(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
#ifdef DEBUG
	R_REPLY_STATUS_16 *q = (R_REPLY_STATUS_16 *)p;

	//int reg_v;
	//float reg_v_f;

	// ===
	// seems to never change
	dump_unknown("unknown_s16", q->unknown, sizeof(q->unknown));

	ESP_LOGD(TAG, "crc: ");
	if(q->crc < 0x10)
		ESP_LOGD(TAG, "0");
	ESP_LOGD(TAG, q->crc, HEX);
#endif
}

void czdec::reply_r_status_17(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
#ifdef DEBUG
	R_REPLY_STATUS_17 *q = (R_REPLY_STATUS_17 *)p;

	//int reg_v;
	//float reg_v_f;

	// ===
	// seems to never change
	dump_unknown("unknown_s17", q->unknown, sizeof(q->unknown));

	ESP_LOGD(TAG, "crc: ");
	if(q->crc < 0x10)
		ESP_LOGD(TAG, "0");
	ESP_LOGD(TAG, q->crc, HEX);
#endif
}

void czdec::reply_r_status_18(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
#ifdef DEBUG
	R_REPLY_STATUS_18 *q = (R_REPLY_STATUS_18 *)p;

	//int reg_v;
	//float reg_v_f;

	// ===
	// seems to never change
	dump_unknown("unknown_s18", q->unknown, sizeof(q->unknown));

	ESP_LOGD(TAG, "crc: ");
	if(q->crc < 0x10)
		ESP_LOGD(TAG, "0");
	ESP_LOGD(TAG, q->crc, HEX);
#endif
}

void czdec::reply_r_status_19(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
#ifdef DEBUG
	R_REPLY_STATUS_19 *q = (R_REPLY_STATUS_19 *)p;

	//int reg_v;
	//float reg_v_f;

	// ===
	// seems to always contain '\0'
	dump_unknown("unknown_s19", q->unknown, sizeof(q->unknown));

	ESP_LOGD(TAG, "crc: ");
	if(q->crc < 0x10)
		ESP_LOGD(TAG, "0");
	ESP_LOGD(TAG, q->crc, HEX);
#endif
}

void czdec::reply_r_status_20(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
#ifdef DEBUG
	R_REPLY_STATUS_20 *q = (R_REPLY_STATUS_20 *)p;

	//int reg_v;
	//float reg_v_f;

	// ===
	// seems to always contain '\0'
	dump_unknown("unknown_s20", q->unknown, sizeof(q->unknown));

	ESP_LOGD(TAG, "crc: ");
	if(q->crc < 0x10)
		ESP_LOGD(TAG, "0");
	ESP_LOGD(TAG, q->crc, HEX);
#endif
}

void czdec::reply_r_status_22(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
#ifdef DEBUG
	R_REPLY_STATUS_22 *q = (R_REPLY_STATUS_22 *)p;

	//int reg_v;
	//float reg_v_f;

	// ===
	dump_unknown("unknown_s22", q->unknown, sizeof(q->unknown));

	ESP_LOGD(TAG, "crc: ");
	if(q->crc < 0x10)
		ESP_LOGD(TAG, "0");
	ESP_LOGD(TAG, q->crc, HEX);
#endif
}

void czdec::reply_r_status_23(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
#ifdef DEBUG
	R_REPLY_STATUS_23 *q = (R_REPLY_STATUS_23 *)p;

	int reg_v;
	//float reg_v_f;
	int i;

	// ===
	// pas de rapport avec le fonctionnement du chauffage à priori car pas de changement entre off=>start=>stop=>off
	for(i = 0; i < STATUS_23_UNKNOWN1_NB; i++)
	{
		reg_v = get_uint16(q->unknown1[i]);

		ESP_LOGD(TAG, "S23 Unknown1 [");
		ESP_LOGD(TAG, i);
		ESP_LOGD(TAG, "]: ");
		ESP_LOGD(TAG, reg_v);
		ESP_LOGD(TAG, " (0x");

		if(((reg_v>>8)&0xFF) < 0x10)
			ESP_LOGD(TAG, "0");
		ESP_LOGD(TAG, (reg_v>>8)&0xFF), HEX);

		ESP_LOGD(TAG, " ");
		if((reg_v & 0xFF) < 0x10)
			ESP_LOGD(TAG, "0");
		ESP_LOGD(TAG, reg_v & 0xFF), HEX);

		ESP_LOGD(TAG, ")");
	}

	// ===
	// seems to never change
	dump_unknown("unknown_s23_", q->unknown, sizeof(q->unknown));

	// ===
	// pas de rapport avec le fonctionnement du chauffage à priori car pas de changement entre off=>start=>stop=>off
	for(i = 0; i < STATUS_23_UNKNOWN2_NB; i++)
	{
		reg_v = get_uint16(q->unknown2[i]);

		ESP_LOGD(TAG, "S23 Unknown2 [");
		ESP_LOGD(TAG, i);
		ESP_LOGD(TAG, "]: ");
		ESP_LOGD(TAG, reg_v);
		ESP_LOGD(TAG, " (0x");

		if(((reg_v>>8)&0xFF) < 0x10)
			ESP_LOGD(TAG, "0");
		ESP_LOGD(TAG, (reg_v>>8)&0xFF), HEX);

		ESP_LOGD(TAG, " ");
		if((reg_v & 0xFF) < 0x10)
			ESP_LOGD(TAG, "0");
		ESP_LOGD(TAG, reg_v & 0xFF), HEX);

		ESP_LOGD(TAG, ")");
	}

	// ===
	// seems to never change
	dump_unknown("unknown_s23_3", q->unknown3, sizeof(q->unknown3));

	ESP_LOGD(TAG, "crc: ");
	if(q->crc < 0x10)
		ESP_LOGD(TAG, "0");
	ESP_LOGD(TAG, q->crc, HEX);
#endif
}

void czdec::reply_r_status_24(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
#ifdef DEBUG
	R_REPLY_STATUS_24 *q = (R_REPLY_STATUS_24 *)p;

	//int reg_v;
	//float reg_v_f;

	// ===
	dump_unknown("unknown_s24", q->unknown, sizeof(q->unknown));

	ESP_LOGD(TAG, "crc: ");
	if(q->crc < 0x10)
		ESP_LOGD(TAG, "0");
	ESP_LOGD(TAG, q->crc, HEX);
#endif
}

void czdec::reply_r_status_25(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
#ifdef DEBUG
	R_REPLY_STATUS_25 *q = (R_REPLY_STATUS_25 *)p;

	//int reg_v;
	//float reg_v_f;

	// ===
	// seems to never change
	dump_unknown("unknown_s25", q->unknown, sizeof(q->unknown));

	ESP_LOGD(TAG, "crc: ");
	if(q->crc < 0x10)
		ESP_LOGD(TAG, "0");
	ESP_LOGD(TAG, q->crc, HEX);
#endif
}

void czdec::reply_r_status_v180_x58(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
#ifdef DEBUG
	R_REPLY_STATUS_V180_STATUS_x58 *q = (R_REPLY_STATUS_V180_STATUS_x58 *)p;

	dump_unknown("RAW R_REPLY_STATUS_V180_STATUS_x58", (byte *)q, sizeof(*q));
	ESP_LOGD(TAG, "");

	//int reg_v;
	//float reg_v_f;

	// ===
	// seems to never change
	dump_unknown("unknown_v180_x58", q->unknown, sizeof(q->unknown));

	ESP_LOGD(TAG, "crc: ");
	if(q->crc < 0x10)
		ESP_LOGD(TAG, "0");
	ESP_LOGD(TAG, q->crc, HEX);
#endif
}

void czdec::reply_r_status_v180_x68(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
#ifdef DEBUG
	R_REPLY_STATUS_V180_STATUS_x68 *q = (R_REPLY_STATUS_V180_STATUS_x68 *)p;

	dump_unknown("RAW R_REPLY_STATUS_V180_STATUS_x68", (byte *)q, sizeof(*q));
	ESP_LOGD(TAG, "");

	//int reg_v;
	//float reg_v_f;

	// ===
	// seems to never change
	dump_unknown("unknown_v180_x68", q->unknown, sizeof(q->unknown));
	dump_unknown("unknown1_v180_x68", q->unknown1, sizeof(q->unknown1));
	dump_unknown("unknown2_v180_x68", q->unknown2, sizeof(q->unknown2));
	dump_unknown("unknown3_v180_x68", q->unknown3, sizeof(q->unknown3));
	dump_unknown("unknown4_v180_x68", q->unknown4, sizeof(q->unknown4));
	dump_unknown("unknown5_v180_x68", q->unknown5, sizeof(q->unknown5));
	dump_unknown("unknown6_v180_x68", q->unknown6, sizeof(q->unknown6));
	dump_unknown("unknown7_v180_x68", q->unknown7, sizeof(q->unknown7));
	dump_unknown("unknown8_v180_x68", q->unknown8, sizeof(q->unknown8));
	dump_unknown("unknown9_v180_x68", q->unknown9, sizeof(q->unknown9));
	dump_unknown("unknown10_v180_x68", q->unknown10, sizeof(q->unknown10));
	dump_unknown("unknown11_v180_x68", q->unknown11, sizeof(q->unknown11));
	dump_unknown("unknown12_v180_x68", q->unknown12, sizeof(q->unknown12));
	dump_unknown("unknown13_v180_x68", q->unknown13, sizeof(q->unknown13));
	dump_unknown("unknown14_v180_x68", q->unknown14, sizeof(q->unknown14));
	dump_unknown("unknown15_v180_x68", q->unknown15, sizeof(q->unknown15));
	dump_unknown("unknown16_v180_x68", q->unknown16, sizeof(q->unknown16));

	ESP_LOGD(TAG, "crc: ");
	if(q->crc < 0x10)
		ESP_LOGD(TAG, "0");
	ESP_LOGD(TAG, q->crc, HEX);
#endif
}

void czdec::reply_r_status_v180_x40(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
	R_REPLY_STATUS_V180_STATUS_x40 *q = (R_REPLY_STATUS_V180_STATUS_x40 *)p;

	czhp->comfortzone_status.room_heating_setting = get_uint16(q->heating_calculated_setting);

#ifdef DEBUG
	int reg_v;
	float reg_v_f;
	int i;

	dump_unknown("RAW R_REPLY_STATUS_V180_STATUS_x40", (byte *)q, sizeof(*q));
	ESP_LOGD(TAG, "");

	// ===
	reg_v = get_uint16(q->calculated_flow_set);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "Calculated flow set: ");
	ESP_LOGD(TAG, reg_v_f);
	ESP_LOGD(TAG, "°C");

	for(i = 0; i < STATUS_V180_x40_NB_TEMP; i++)
	{
		reg_v = get_int16(q->temp[i]);

		reg_v_f = reg_v;
		reg_v_f /= 10.0;

		ESP_LOGD(TAG, "x40 ?Temp #");
		ESP_LOGD(TAG, i);
		ESP_LOGD(TAG, ": ");
		ESP_LOGD(TAG, reg_v_f);
		ESP_LOGD(TAG, "°C");
		ESP_LOGD(TAG, " ");
		dump_unknown("", q->temp[i], 2);
	}

	// ===
	reg_v = get_uint16(q->heating_calculated_setting);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "Heating - Calculated setting: ");
	ESP_LOGD(TAG, reg_v_f);
	ESP_LOGD(TAG, "°C");

	ESP_LOGD(TAG, "crc: ");
	if(q->crc < 0x10)
		ESP_LOGD(TAG, "0");
	ESP_LOGD(TAG, q->crc, HEX);
#endif
}

void czdec::reply_r_status_v180_x26(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
#ifdef DEBUG
	R_REPLY_STATUS_V180_STATUS_x26 *q = (R_REPLY_STATUS_V180_STATUS_x26 *)p;

	//int reg_v;
	//float reg_v_f;

	dump_unknown("RAW R_REPLY_STATUS_V180_STATUS_x26", (byte *)q, sizeof(*q));
	ESP_LOGD(TAG, "");

	// ===
	// seems to never change
	dump_unknown("unknown_v180_x26", q->unknown, sizeof(q->unknown));

	ESP_LOGD(TAG, "crc: ");
	if(q->crc < 0x10)
		ESP_LOGD(TAG, "0");
	ESP_LOGD(TAG, q->crc, HEX);
#endif
}

void czdec::reply_r_status_v180_x8d(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
	R_REPLY_STATUS_V180_STATUS_x8d *q = (R_REPLY_STATUS_V180_STATUS_x8d *)p;

	czhp->comfortzone_status.fan_time_to_filter_change = get_uint16(q->fan_time_to_filter_change);

#ifdef DEBUG
	int reg_v;
	float reg_v_f;
	int i;

	dump_unknown("RAW R_REPLY_STATUS_V180_STATUS_x8d", (byte *)q, sizeof(*q));
	ESP_LOGD(TAG, "");

	for(i = 0; i < STATUS_V180_x8d_NB_TEMP; i++)
	{
		reg_v = get_int16(q->temp[i]);

		reg_v_f = reg_v;
		reg_v_f /= 10.0;

		ESP_LOGD(TAG, "x8d_a ?Temp #");
		ESP_LOGD(TAG, i);
		ESP_LOGD(TAG, ": ");
		ESP_LOGD(TAG, reg_v_f);
		ESP_LOGD(TAG, "°C");
		ESP_LOGD(TAG, " ");
		dump_unknown("", q->temp[i], 2);
	}

	// ===
	// seems to never change
	dump_unknown("unknown_v180_x8d", q->unknown, sizeof(q->unknown));

	for(i = 0; i < STATUS_V180_x8d_NB_TEMP1; i++)
	{
		reg_v = get_int16(q->temp1[i]);

		reg_v_f = reg_v;
		reg_v_f /= 10.0;

		ESP_LOGD(TAG, "x8d_b ?Temp1 #");
		ESP_LOGD(TAG, i);
		ESP_LOGD(TAG, ": ");
		ESP_LOGD(TAG, reg_v_f);
		ESP_LOGD(TAG, "°C");
		ESP_LOGD(TAG, " ");
		dump_unknown("", q->temp1[i], 2);
	}

	// ===
	reg_v = get_uint16(q->normal_fan_speed);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "Normal fan speed: ");
	ESP_LOGD(TAG, reg_v_f);
	ESP_LOGD(TAG, "%");

	// ===
	reg_v = get_int16(q->reduce_fan_speed);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "Reduce fan speed: ");
	ESP_LOGD(TAG, reg_v_f);
	ESP_LOGD(TAG, "%");

	// ===
	reg_v = get_int16(q->fan_boost_increase);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "Fan boost increase: ");
	ESP_LOGD(TAG, reg_v_f);
	ESP_LOGD(TAG, "%");

	// ===
	for(i = 0; i < STATUS_V180_x8d_NB_TEMP1a; i++)
	{
		reg_v = get_int16(q->temp1a[i]);

		reg_v_f = reg_v;
		reg_v_f /= 10.0;

		ESP_LOGD(TAG, "x8d_b ?Temp1a #");
		ESP_LOGD(TAG, i);
		ESP_LOGD(TAG, ": ");
		ESP_LOGD(TAG, reg_v_f);
		ESP_LOGD(TAG, "°C");
		ESP_LOGD(TAG, " ");
		dump_unknown("", q->temp1a[i], 2);
	}

	// ===
	reg_v = get_uint16(q->fan_time_to_filter_change);

	ESP_LOGD(TAG, "Fan - Time to filter change: ");
	ESP_LOGD(TAG, reg_v);
	ESP_LOGD(TAG, "d");

	// ===
	for(i = 0; i < STATUS_V180_x8d_NB_TEMP2; i++)
	{
		reg_v = get_int16(q->temp2[i]);

		reg_v_f = reg_v;
		reg_v_f /= 10.0;

		ESP_LOGD(TAG, "x8d_b ?Temp2 #");
		ESP_LOGD(TAG, i);
		ESP_LOGD(TAG, ": ");
		ESP_LOGD(TAG, reg_v_f);
		ESP_LOGD(TAG, "°C");
		ESP_LOGD(TAG, " ");
		dump_unknown("", q->temp2[i], 2);
	}


	ESP_LOGD(TAG, "crc: ");
	if(q->crc < 0x10)
		ESP_LOGD(TAG, "0");
	ESP_LOGD(TAG, q->crc, HEX);
#endif
}

// 202301113 - proto v1.8 ok
void czdec::reply_r_status_v180_runtime_and_energy(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
	R_REPLY_STATUS_V180_STATUS_runtime_and_energy *q = (R_REPLY_STATUS_V180_STATUS_runtime_and_energy *)p;

	// update same informations as "Status 08" but it may be not here
	czhp->comfortzone_status.compressor_energy = get_uint32(q->compressor_energy);
	czhp->comfortzone_status.add_energy = get_uint32(q->add_energy);
	czhp->comfortzone_status.hot_water_energy = get_uint32(q->hot_water_energy);

	czhp->comfortzone_status.compressor_runtime = get_uint32(q->compressor_runtime);
	czhp->comfortzone_status.total_runtime = get_uint32(q->total_runtime);

#ifdef DEBUG

	int reg_v;
	float reg_v_f;

	dump_unknown("RAW R_REPLY_STATUS_V180_STATUS_runtime_and_energy", (byte *)q, sizeof(*q));
	ESP_LOGD(TAG, "");

	reg_v = get_uint32(q->unknown1);
	ESP_LOGD(TAG, "unknown1_v180_runtime_and_energy : ");
	ESP_LOGD(TAG, reg_v);
	ESP_LOGD(TAG, " ");
	dump_unknown("", q->unknown1, 4);

	// ===
	reg_v = get_uint32(q->total_runtime);

	ESP_LOGD(TAG, "Total runtime: ");
	ESP_LOGD(TAG, reg_v / 60);
	ESP_LOGD(TAG, ":");
	reg_v = reg_v % 60;
	if(reg_v < 10)
		ESP_LOGD(TAG, "0");
	ESP_LOGD(TAG, reg_v);
	ESP_LOGD(TAG, "");

	// ===
	reg_v = get_uint32(q->compressor_runtime);

	ESP_LOGD(TAG, "Compressor runtime: ");
	ESP_LOGD(TAG, reg_v / 60);
	ESP_LOGD(TAG, ":");
	reg_v = reg_v % 60;
	if(reg_v < 10)
		ESP_LOGD(TAG, "0");
	ESP_LOGD(TAG, reg_v);
	ESP_LOGD(TAG, "");

	// ===
	reg_v = get_uint32(q->compressor_energy);

	reg_v_f = reg_v;
	reg_v_f /= 100.0;

	ESP_LOGD(TAG, "Compressor energy: ");
	ESP_LOGD(TAG, reg_v_f);
	ESP_LOGD(TAG, "kWh");

	// ===
	reg_v = get_uint32(q->add_energy);

	reg_v_f = reg_v;
	reg_v_f /= 100.0;

	ESP_LOGD(TAG, "Add energy: ");
	ESP_LOGD(TAG, reg_v_f);
	ESP_LOGD(TAG, "kWh");

	// ===
	reg_v = get_uint32(q->hot_water_energy);

	reg_v_f = reg_v;
	reg_v_f /= 100.0;

	ESP_LOGD(TAG, "Hot water energy: ");
	ESP_LOGD(TAG, reg_v_f);
	ESP_LOGD(TAG, "kWh");

	ESP_LOGD(TAG, "crc: ");
	if(q->crc < 0x10)
		ESP_LOGD(TAG, "0");
	ESP_LOGD(TAG, q->crc, HEX);
#endif
}

void czdec::reply_r_status_v180_xa1(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
#ifdef DEBUG
	R_REPLY_STATUS_V180_STATUS_xa1 *q = (R_REPLY_STATUS_V180_STATUS_xa1 *)p;

	//int reg_v;
	//float reg_v_f;

	dump_unknown("RAW unknown_v180_xa1", (byte *)q, sizeof(*q));
	ESP_LOGD(TAG, "");
	// ===
	// seems to never change
	dump_unknown("unknown_v180_xa1", q->unknown, sizeof(q->unknown));

	ESP_LOGD(TAG, "crc: ");
	if(q->crc < 0x10)
		ESP_LOGD(TAG, "0");
	ESP_LOGD(TAG, q->crc, HEX);
#endif
}

void czdec::reply_r_status_v180_02(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
	R_REPLY_STATUS_V180_02 *q = (R_REPLY_STATUS_V180_02 *)p;
	int reg_v;

	reg_v = (q->heatpump_status[1] >> 4) & 0x07;

	switch(reg_v)
	{
		case 0:	// off 
		case 1:	// unknown
		case 7:	// unknown
					czhp->comfortzone_status.led_luminosity_setting = reg_v;
					break;

		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
					czhp->comfortzone_status.led_luminosity_setting = reg_v - 1;
					break;
	}

#ifdef DEBUG

	//float reg_v_f;

	dump_unknown("RAW unknown_v180_02", (byte *)q, sizeof(*q));
	ESP_LOGD(TAG, "");
	// ===
	// seems to never change
	dump_unknown("unknown_v180_02a", q->heatpump_status, 1);

	ESP_LOGD(TAG, "* Alarm: ");
	if(q->heatpump_status[0] & 0x80)
		ESP_LOGD(TAG, "yes");
	else
		ESP_LOGD(TAG, "no");

	ESP_LOGD(TAG, "* Compressor running: ");
	if(q->heatpump_status[0] & 0x01)
		ESP_LOGD(TAG, "yes");
	else
		ESP_LOGD(TAG, "no");

	ESP_LOGD(TAG, "* Additional power: ");
	if(q->heatpump_status[0] & 0x10)
		ESP_LOGD(TAG, "yes");
	else
		ESP_LOGD(TAG, "no");

	ESP_LOGD(TAG, "* bit 6-5,3-1 not decoded");

	dump_unknown("unknown_v180_02b", q->heatpump_status+1, 1);

	ESP_LOGD(TAG, "* LED mode: ");
	if(q->heatpump_status[1] & 0x80)
		ESP_LOGD(TAG, " error (red)");
	else
		ESP_LOGD(TAG, " normal (green)");

	ESP_LOGD(TAG, "* Luminosity level: ");

	switch((q->heatpump_status[1] >> 4) & 0x07)
	{
		case 0:
					ESP_LOGD(TAG, "0");
					break;

		case 1:
					ESP_LOGD(TAG, "UNKNOWN: 1");
					break;

		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
					ESP_LOGD(TAG, (q->heatpump_status[1] >> 4) & 0x07) - 1);
					break;
		case 7:
					ESP_LOGD(TAG, "UNKNOWN: 7");
					break;
	}
	ESP_LOGD(TAG, "");

	ESP_LOGD(TAG, "* bit 3-0 not decoded");
	ESP_LOGD(TAG, "* bit 0 is related to heatpump running, perhaps mode (hot water, room heating...");

	ESP_LOGD(TAG, "crc: ");
	if(q->crc < 0x10)
		ESP_LOGD(TAG, "0");
	ESP_LOGD(TAG, q->crc, HEX);
#endif
}

void czdec::reply_r_status_v180_xa3(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
#ifdef DEBUG
	R_REPLY_STATUS_V180_STATUS_xa3 *q = (R_REPLY_STATUS_V180_STATUS_xa3 *)p;

	//int reg_v;
	//float reg_v_f;

	dump_unknown("RAW unknown_v180_xa3", (byte *)q, sizeof(*q));
	ESP_LOGD(TAG, "");

	// ===
	dump_unknown("unknown_v180_xa3", q->unknown, sizeof(q->unknown));

	ESP_LOGD(TAG, "crc: ");
	if(q->crc < 0x10)
		ESP_LOGD(TAG, "0");
	ESP_LOGD(TAG, q->crc, HEX);
#endif
}

void czdec::reply_r_status_v180_xad(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
	R_REPLY_STATUS_V180_STATUS_xad *q = (R_REPLY_STATUS_V180_STATUS_xad *)p;

	int reg_v;
	float reg_v_f;

	czhp->comfortzone_status.hot_water_calculated_setting = get_uint16(q->hot_water_calculated_setting);

	czhp->comfortzone_status.heatpump_current_compressor_frequency = get_uint16(q->heatpump_current_compressor_frequency);
	czhp->comfortzone_status.heatpump_current_compressor_frequency_changed = true;

	czhp->comfortzone_status.heatpump_current_compressor_power = get_uint16(q->heatpump_current_compressor_power);
	czhp->comfortzone_status.heatpump_current_compressor_power_changed = true;
	czhp->comfortzone_status.heatpump_current_add_power = get_uint32(q->heatpump_current_add_power);
	czhp->comfortzone_status.heatpump_current_add_power_changed = true;
	czhp->comfortzone_status.heatpump_current_total_power = get_uint32(q->heatpump_current_total_power1);
	czhp->comfortzone_status.heatpump_current_total_power_changed = true;
	czhp->comfortzone_status.heatpump_current_compressor_input_power = get_uint16(q->heatpump_compressor_input_power);
	czhp->comfortzone_status.heatpump_current_compressor_input_power_changed = true;

#ifdef DEBUG
	int i;
	// ===

	dump_unknown("RAW unknown_v180_xad", (byte *)q, sizeof(*q));
	ESP_LOGD(TAG, "");

	// ===
	reg_v = get_uint16(q->hot_water_calculated_setting);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "Hot water calculated setting: ");
	ESP_LOGD(TAG, reg_v_f);
	ESP_LOGD(TAG, "°C");

	// ===
	for(i = 0; i < STATUS_V180_x8a_NB_TEMP1; i++)
	{
		reg_v = get_int16(q->temp1[i]);

		reg_v_f = reg_v;
		reg_v_f /= 10.0;

		ESP_LOGD(TAG, "xad ?Temp1 #");
		ESP_LOGD(TAG, i);
		ESP_LOGD(TAG, ": ");
		ESP_LOGD(TAG, reg_v_f);
		ESP_LOGD(TAG, "°C");
		ESP_LOGD(TAG, " ");
		dump_unknown("", q->temp1[i], 2);
	}

	// ===
	// seems to never change
	dump_unknown("unknown_v180_xad", q->unknown, sizeof(q->unknown));

	// ===
	for(i = 0; i < STATUS_V180_x8a_NB_TEMP1a; i++)
	{
		reg_v = get_int16(q->temp1a[i]);

		reg_v_f = reg_v;
		reg_v_f /= 10.0;

		ESP_LOGD(TAG, "xad ?Temp1a #");
		ESP_LOGD(TAG, i);
		ESP_LOGD(TAG, ": ");
		ESP_LOGD(TAG, reg_v_f);
		ESP_LOGD(TAG, "°C");
		ESP_LOGD(TAG, " ");
		dump_unknown("", q->temp1a[i], 2);
	}

	// ===
	reg_v = get_uint16(q->condensing_temperature);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "Condensing temperature: ");
	ESP_LOGD(TAG, reg_v_f);
	ESP_LOGD(TAG, "°C");

	// ===
	reg_v = get_uint16(q->condensing_pressure);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "Condensing pressure: ");
	ESP_LOGD(TAG, reg_v_f);
	ESP_LOGD(TAG, "bar");

	// ===
	reg_v = get_uint16(q->evaporator_pressure);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "Evaporator pressure: ");
	ESP_LOGD(TAG, reg_v_f);
	ESP_LOGD(TAG, "bar");

	// ===
	reg_v = get_uint16(q->pressure_ratio);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "Pressure ratio: ");
	ESP_LOGD(TAG, reg_v_f);
	ESP_LOGD(TAG, "");

	// ===
	for(i = 0; i < STATUS_V180_x8a_NB_TEMP2; i++)
	{
		reg_v = get_int16(q->temp2[i]);

		reg_v_f = reg_v;
		reg_v_f /= 10.0;

		ESP_LOGD(TAG, "xad ?Temp2 #");
		ESP_LOGD(TAG, i);
		ESP_LOGD(TAG, ": ");
		ESP_LOGD(TAG, reg_v_f);
		ESP_LOGD(TAG, "°C");
		ESP_LOGD(TAG, " ");
		dump_unknown("", q->temp2[i], 2);
	}

	// ===
	reg_v = get_uint16(q->heatpump_target_compressor_frequency);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "Heatpump - target compressor frequency: ");
	ESP_LOGD(TAG, reg_v_f);
	ESP_LOGD(TAG, "Hz");

	// ===
	dump_unknown("unknown1_v180_xad", q->unknown1, sizeof(q->unknown1));

	// ===
	reg_v = get_uint16(q->heatpump_current_compressor_frequency);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "Heatpump - current compressor frequency: ");
	ESP_LOGD(TAG, reg_v_f);
	ESP_LOGD(TAG, "Hz");

	// ===
	reg_v = get_uint16(q->heating_compressor_max_frequency);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "Heating compressor max freq.: ");
	ESP_LOGD(TAG, reg_v_f);
	ESP_LOGD(TAG, "Hz");

	// ===
	for(i = 0; i < STATUS_V180_x8a_NB_TEMP3; i++)
	{
		reg_v = get_int16(q->temp3[i]);

		reg_v_f = reg_v;
		reg_v_f /= 10.0;

		ESP_LOGD(TAG, "xad ?Temp3 #");
		ESP_LOGD(TAG, i);
		ESP_LOGD(TAG, ": ");
		ESP_LOGD(TAG, reg_v_f);
		ESP_LOGD(TAG, "°C");
		ESP_LOGD(TAG, " ");
		dump_unknown("", q->temp3[i], 2);
	}

	// ===
	reg_v = get_uint16(q->heatpump_current_compressor_power);

	ESP_LOGD(TAG, "Heatpump - current compressor power: ");
	ESP_LOGD(TAG, reg_v);
	ESP_LOGD(TAG, "W");

	// ===
	reg_v = get_uint32(q->heatpump_current_add_power);

	ESP_LOGD(TAG, "Heatpump - current add power: ");
	ESP_LOGD(TAG, reg_v);
	ESP_LOGD(TAG, "W");

	// ===
	reg_v = get_uint32(q->heatpump_current_total_power1);

	ESP_LOGD(TAG, "Heatpump - current total power 1: ");
	ESP_LOGD(TAG, reg_v);
	ESP_LOGD(TAG, "W");

	// ===
	reg_v = get_uint32(q->heatpump_current_total_power2);

	ESP_LOGD(TAG, "Heatpump - current total power 2: ");
	ESP_LOGD(TAG, reg_v);
	ESP_LOGD(TAG, "W");

	// ===
	reg_v = get_uint16(q->heatpump_compressor_input_power);

	ESP_LOGD(TAG, "Heatpump - Compressor input power: ");
	ESP_LOGD(TAG, reg_v);
	ESP_LOGD(TAG, "W");

	// ===
	for(i = 0; i < STATUS_V180_x8a_NB_TEMP4; i++)
	{
		reg_v = get_int16(q->temp4[i]);

		reg_v_f = reg_v;
		reg_v_f /= 10.0;

		ESP_LOGD(TAG, "xad ?Temp4 #");
		ESP_LOGD(TAG, i);
		ESP_LOGD(TAG, ": ");
		ESP_LOGD(TAG, reg_v_f);
		ESP_LOGD(TAG, "°C");
		ESP_LOGD(TAG, " ");
		dump_unknown("", q->temp4[i], 2);
	}

	// ===
	for(i = 0; i < STATUS_V180_x8a_NB_TEMP5; i++)
	{
		reg_v = get_int16(q->temp5[i]);

		reg_v_f = reg_v;
		reg_v_f /= 10.0;

		ESP_LOGD(TAG, "xad ?Temp5 #");
		ESP_LOGD(TAG, i);
		ESP_LOGD(TAG, ": ");
		ESP_LOGD(TAG, reg_v_f);
		ESP_LOGD(TAG, "°C");
		ESP_LOGD(TAG, " ");
		dump_unknown("", q->temp5[i], 2);
	}

	// ===
	reg_v = get_uint16(q->expansion_valve_calculated_setting);
	reg_v_f = reg_v / 10;

	ESP_LOGD(TAG, "Expansion valve - Calculated setting: ");
	ESP_LOGD(TAG, reg_v_f);
	ESP_LOGD(TAG, "K");

	// ===
	reg_v = get_int16(q->vanne_expansion_xxx);
	reg_v_f = reg_v / 10;

	ESP_LOGD(TAG, "Expansion valve - xxx?: ");
	ESP_LOGD(TAG, reg_v_f);
	ESP_LOGD(TAG, "K");

	// ===
	reg_v = get_int16(q->expansion_valve_temperature_difference);
	reg_v_f = reg_v / 10;

	ESP_LOGD(TAG, "Expansion valve - Temperature difference 1: ");
	ESP_LOGD(TAG, reg_v_f);
	ESP_LOGD(TAG, "K");


	ESP_LOGD(TAG, "crc: ");
	if(q->crc < 0x10)
		ESP_LOGD(TAG, "0");
	ESP_LOGD(TAG, q->crc, HEX);
#endif
}

void czdec::reply_r_status_v180_xc7(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
#ifdef DEBUG
	R_REPLY_STATUS_V180_STATUS_xc7 *q = (R_REPLY_STATUS_V180_STATUS_xc7 *)p;

	//int reg_v;
	//float reg_v_f;

	dump_unknown("RAW unknown_v180_xc7", (byte *)q, sizeof(*q));
	ESP_LOGD(TAG, "");

	// ===
	// seems to never change
	dump_unknown("unknown_v180_xc7", q->unknown, sizeof(q->unknown));

	ESP_LOGD(TAG, "crc: ");
	if(q->crc < 0x10)
		ESP_LOGD(TAG, "0");
	ESP_LOGD(TAG, q->crc, HEX);
#endif
}

void czdec::reply_r_status_v180_xc72(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
#ifdef DEBUG
	R_REPLY_STATUS_V180_STATUS_xc72 *q = (R_REPLY_STATUS_V180_STATUS_xc72 *)p;

	//int reg_v;
	//float reg_v_f;
	dump_unknown("RAW unknown_v180_xc72", (byte *)q, sizeof(*q));
	ESP_LOGD(TAG, "");

	// ===
	// seems to never change
	dump_unknown("unknown_v180_xc72", q->unknown, sizeof(q->unknown));

	ESP_LOGD(TAG, "crc: ");
	if(q->crc < 0x10)
		ESP_LOGD(TAG, "0");
	ESP_LOGD(TAG, q->crc, HEX);
#endif
}

void czdec::reply_r_status_v180_xc5(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
#ifdef DEBUG
	R_REPLY_STATUS_V180_STATUS_xc5 *q = (R_REPLY_STATUS_V180_STATUS_xc5 *)p;

	//int reg_v;
	//float reg_v_f;

	// ===
	// seems to never change
	dump_unknown("unknown_v180_xc5", q->unknown, sizeof(q->unknown));

	ESP_LOGD(TAG, "crc: ");
	if(q->crc < 0x10)
		ESP_LOGD(TAG, "0");
	ESP_LOGD(TAG, q->crc, HEX);
#endif
}

void czdec::reply_r_status_v180_xbf(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
#ifdef DEBUG
	R_REPLY_STATUS_V180_STATUS_xbf *q = (R_REPLY_STATUS_V180_STATUS_xbf *)p;

	//int reg_v;
	//float reg_v_f;
	char v[2];

	dump_unknown("RAW unknown_v180_xbf", (byte *)q, sizeof(*q));
	ESP_LOGD(TAG, "");

	// ===
	// seems to never change
	dump_unknown("unknown_v180_xbf", q->unknown, sizeof(q->unknown));

	// ===
	ESP_LOGD(TAG, "UI Software version: ");
	ESP_LOGD(TAG, q->ui_software_major_version);
	ESP_LOGD(TAG, ".");
	ESP_LOGD(TAG, q->ui_software_minor_version);
	ESP_LOGD(TAG, ".");
	ESP_LOGD(TAG, q->ui_software_patch_version);

	v[0] = 'A' + q->ui_hardware_version - 1;
	v[1] = '\0';

	ESP_LOGD(TAG, "UI Hardware version: ");
	ESP_LOGD(TAG, v);

	// ===

	dump_unknown("unknown1_v180_xbf", q->unknown, sizeof(q->unknown1));

	ESP_LOGD(TAG, "crc: ");
	if(q->crc < 0x10)
		ESP_LOGD(TAG, "0");
	ESP_LOGD(TAG, q->crc, HEX);
#endif
}

void czdec::reply_r_status_v180_x6d(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
#ifdef DEBUG
	R_REPLY_STATUS_V180_STATUS_x6d *q = (R_REPLY_STATUS_V180_STATUS_x6d *)p;

	//int reg_v;
	//float reg_v_f;

	dump_unknown("RAW unknown_v180_x6d", (byte *)q, sizeof(*q));
	ESP_LOGD(TAG, "");

	// ===
	// seems to never change
	dump_unknown("unknown_v180_x6d", q->unknown, sizeof(q->unknown));

	ESP_LOGD(TAG, "crc: ");
	if(q->crc < 0x10)
		ESP_LOGD(TAG, "0");
	ESP_LOGD(TAG, q->crc, HEX);
#endif
}

void czdec::reply_r_status_v180_x56(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
	R_REPLY_STATUS_V180_STATUS_x56 *q = (R_REPLY_STATUS_V180_STATUS_x56 *)p;

#ifdef DEBUG

	int reg_v;
	//float reg_v_f;

	dump_unknown("RAW unknown_v180_x56", (byte *)q, sizeof(*q));
	ESP_LOGD(TAG, "");

	// ===
	reg_v = q->bcd_second;

	ESP_LOGD(TAG, "Second (BCD): ");
	ESP_LOGD(TAG, reg_v, HEX);

	// ===
	reg_v = q->bcd_minute;

	ESP_LOGD(TAG, "Minute (BCD): ");
	ESP_LOGD(TAG, reg_v, HEX);

	// ===
	reg_v = q->bcd_hour;

	ESP_LOGD(TAG, "Hour (BCD): ");
	ESP_LOGD(TAG, reg_v, HEX);

	// ===
	dump_unknown("unknown_s08_0 (increase by 1 every day but it is not day of week)", &(q->unknown0), sizeof(q->unknown0));

	// ===
	reg_v = q->bcd_day;

	ESP_LOGD(TAG, "Day (BCD): ");
	ESP_LOGD(TAG, reg_v, HEX);

	// ===
	reg_v = q->bcd_month;

	ESP_LOGD(TAG, "Month (BCD): ");
	ESP_LOGD(TAG, reg_v, HEX);

	// ===
	reg_v = q->bcd_year;

	ESP_LOGD(TAG, "Year (BCD)(20xx): ");
	ESP_LOGD(TAG, reg_v, HEX);


	// ===
	// seems to never change
	dump_unknown("unknown_v180_x56", q->unknown, sizeof(q->unknown));

	ESP_LOGD(TAG, "crc: ");
	if(q->crc < 0x10)
		ESP_LOGD(TAG, "0");
	ESP_LOGD(TAG, q->crc, HEX);
#endif
}

void czdec::reply_r_status_v180_short2(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
#ifdef DEBUG
	R_REPLY_STATUS_V180_STATUS_SHORT2 *q = (R_REPLY_STATUS_V180_STATUS_SHORT2 *)p;

	//int reg_v;
	//float reg_v_f;

	dump_unknown("RAW unknown_v180_SHORT2", (byte *)q, sizeof(*q));
	ESP_LOGD(TAG, "");

	// ===
	// seems to never change
	dump_unknown("unknown_v180_SHORT2", q->unknown, sizeof(q->unknown));

	ESP_LOGD(TAG, "crc: ");
	if(q->crc < 0x10)
		ESP_LOGD(TAG, "0");
	ESP_LOGD(TAG, q->crc, HEX);
#endif
}

void czdec::reply_r_status_v180_ui_versions(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
#ifdef DEBUG
	R_REPLY_STATUS_V180_STATUS_ui_versions *q = (R_REPLY_STATUS_V180_STATUS_ui_versions *)p;

	//int reg_v;
	//float reg_v_f;
	char v[2];

	dump_unknown("RAW unknown_v180_ui_versions", (byte *)q, sizeof(*q));
	ESP_LOGD(TAG, "");

	ESP_LOGD(TAG, "UI Software version: ");
	ESP_LOGD(TAG, q->ui_software_major_version);
	ESP_LOGD(TAG, ".");
	ESP_LOGD(TAG, q->ui_software_minor_version);
	ESP_LOGD(TAG, ".");
	ESP_LOGD(TAG, q->ui_software_patch_version);

	v[0] = 'A' + q->ui_hardware_version - 1;
	v[1] = '\0';

	ESP_LOGD(TAG, "UI Hardware version: ");
	ESP_LOGD(TAG, v);

	// ===
	// seems to never change
	dump_unknown("unknown_v180_ui_versions", q->unknown, sizeof(q->unknown));

	ESP_LOGD(TAG, "crc: ");
	if(q->crc < 0x10)
		ESP_LOGD(TAG, "0");
	ESP_LOGD(TAG, q->crc, HEX);
#endif
}

void czdec::reply_r_status_v180_x2c(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
#ifdef DEBUG
	R_REPLY_STATUS_V180_STATUS_x2c *q = (R_REPLY_STATUS_V180_STATUS_x2c *)p;

	//int reg_v;
	//float reg_v_f;

	dump_unknown("RAW unknown_v180_x2c", (byte *)q, sizeof(*q));
	ESP_LOGD(TAG, "");

	// ===
	// seems to never change
	dump_unknown("unknown_v180_x2c", q->unknown, sizeof(q->unknown));

	ESP_LOGD(TAG, "crc: ");
	if(q->crc < 0x10)
		ESP_LOGD(TAG, "0");
	ESP_LOGD(TAG, q->crc, HEX);
#endif
}

void czdec::reply_r_status_v180_settings(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
	R_REPLY_STATUS_V180_SETTINGS *q = (R_REPLY_STATUS_V180_SETTINGS *)p;

	czhp->comfortzone_status.hot_water_setting = get_uint16(q->hot_water_user_setting);

#ifdef DEBUG

	int reg_v;
	float reg_v_f;

	dump_unknown("RAW unknown_v180_settings", (byte *)q, sizeof(*q));
	ESP_LOGD(TAG, "");

	// ===
	dump_unknown("unknown_v180_settings", q->unknown, sizeof(q->unknown));

	// ===
	reg_v = get_uint16(q->hot_water_user_setting);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "Hot water user setting: ");
	ESP_LOGD(TAG, reg_v_f);
	ESP_LOGD(TAG, "°C");

	// ===
	dump_unknown("unknown1_v180_settings", q->unknown1, sizeof(q->unknown1));

	ESP_LOGD(TAG, "crc: ");
	if(q->crc < 0x10)
		ESP_LOGD(TAG, "0");
	ESP_LOGD(TAG, q->crc, HEX);
#endif
}

void czdec::reply_r_status_v180_c8a(comfortzone_heatpump *czhp, KNOWN_REGISTER *kr, R_REPLY *p)
{
	R_REPLY_STATUS_V180_C8A *q = (R_REPLY_STATUS_V180_C8A *)p;

	czhp->comfortzone_status.fan_speed_duty = get_uint16(q->fan_speed_duty);

	//czhp->comfortzone_status.fan_speed = q->fan_speed;

#ifdef DEBUG

	int reg_v;
	float reg_v_f;

	dump_unknown("RAW unknown_v180_c8a", (byte *)q, sizeof(*q));
	ESP_LOGD(TAG, "");

	// ===
	reg_v = get_uint16(q->fan_speed_duty);

	reg_v_f = reg_v;
	reg_v_f /= 10.0;

	ESP_LOGD(TAG, "Fan Speed duty: ");
	ESP_LOGD(TAG, reg_v_f);
	ESP_LOGD(TAG, "%");

	// ===
	dump_unknown("unknown1_v180_c8a", q->unknown1, sizeof(q->unknown1));

	ESP_LOGD(TAG, "crc: ");
	if(q->crc < 0x10)
		ESP_LOGD(TAG, "0");
	ESP_LOGD(TAG, q->crc, HEX);
#endif
}

}
