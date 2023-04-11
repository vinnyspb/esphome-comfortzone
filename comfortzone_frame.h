#pragma once

#include <cstdint>

// This file contains the description of RS485 frames

namespace esphome::comfortzone {

/*
 Standard packets have the following format:

 xx xx xx xx pp pp yy yy yy yy ss tt ..... crc

    xx is source address on 4 bytes
    pp is packet type ?: D3 5E = command, 07 8A = reply
    yy is destination address on 4 bytes
    ss is packet size (from xx to crc, all included)
    tt can be either (W: write command, w: write reply, R: read command, r: read reply)

*/
typedef struct __attribute__ ((packed)) cz_packet_header
{
	uint8_t destination[4];
	uint8_t destination_crc;		// crc-maxim of source
	uint8_t comp1_destination_crc;	// crc-maxim of comp1's of byte of source 
	uint8_t source[4];
	uint8_t packet_size;		// packet size in byte
	uint8_t cmd;				// 'W': write command, 'w': write reply, 'R': read command, 'r': read reply)
	uint8_t reg_num[9];
} CZ_PACKET_HEADER;

// =====================================
// == basic frames
// =====================================

// r cmd is: cz_header yy cc
// - yy is ?? (wanted reply size ?)
// - cc = crc
typedef struct __attribute__ ((packed))
{
	CZ_PACKET_HEADER cz_head;
	uint8_t wanted_reply_size;
	uint8_t crc;
} R_CMD;

// w cmd is: cz_header yy yy cc
// - yy is reg value (2 bytes, little endian)
// - cc = crc
typedef struct __attribute__ ((packed))
{
	CZ_PACKET_HEADER cz_head;
	uint8_t reg_value[2];
	uint8_t crc;
} W_CMD;

// w cmd is: cz_header yy yy cc
// - yy is reg value (1 byte)
// - cc = crc
typedef struct __attribute__ ((packed))
{
	CZ_PACKET_HEADER cz_head;
	uint8_t reg_value;
	uint8_t crc;
} W_SMALL_CMD;

// r reply is: cz_header yy yy cc
// - yy is reg value (2 bytes, little endian)
// - cc = crc
typedef struct __attribute__ ((packed))
{
	CZ_PACKET_HEADER cz_head;
	uint8_t reg_value[2];
	uint8_t crc;
} R_REPLY;

// r reply is: cz_header yy cc
// - yy is reg value (1 byte)
// - cc = crc
typedef struct __attribute__ ((packed))
{
	CZ_PACKET_HEADER cz_head;
	uint8_t reg_value;
	uint8_t crc;
} R_SMALL_REPLY;

// w reply is: cz_header yy cc
//  - yy = status code: 00 = ok
//  - cc = crc
typedef struct __attribute__ ((packed))
{
	CZ_PACKET_HEADER cz_head;
	uint8_t return_code;
	uint8_t crc;
} W_REPLY;

// =====================================
// == status frames
// =====================================

typedef struct __attribute__ ((packed))
{
	CZ_PACKET_HEADER cz_head;
	
	uint8_t unknown[2];

	uint8_t extra_hot_water;					// 0x00 = on, 0xFF = off
	uint8_t hot_water_user_setting[2];			// °C, LSB, 2 bytes, * 10 (it is the temperature selected by user)
	uint8_t hot_water_hysteresis[2];		// °C, LSB, 2 bytes, * 10

	//  +3 comes from 3 non displayed steps
#define STATUS_01_NB_HW_NORMAL_STEPS (12+3)
	uint8_t hot_water_normal_steps[STATUS_01_NB_HW_NORMAL_STEPS][2];	// °C, LSB, 2 bytes, signed, * 10

	//  +3 comes from 3 non displayed steps
#define STATUS_01_NB_HW_HIGH_STEPS (12+3)
	uint8_t hot_water_high_steps[STATUS_01_NB_HW_HIGH_STEPS][2];	// °C, LSB, 2 bytes, signed, * 10

	//  +3 comes from 3 non displayed steps
#define STATUS_01_NB_HW_EXTRA_STEPS (12+3)
	uint8_t hot_water_extra_steps[STATUS_01_NB_HW_EXTRA_STEPS][2];	// °C, LSB, 2 bytes, signed, * 10

	uint8_t unknown5a[1];

	uint8_t hot_water_max_runtime;								// minute
	uint8_t hot_water_pause_time;							// minute

	uint8_t unknown5b[8][2];		// looks like 8 signed int16

	uint8_t hot_water_compressor_min_frequency[2];		// Hz, LSB, 2bytes, * 10
	uint8_t hot_water_compressor_max_frequency[2];		// Hz, LSB, 2bytes, * 10

	uint8_t unknown6[4];

	uint8_t hot_water_extra_setting[2];					// °C, LSB, 2bytes, * 10
	uint8_t hot_water_extra_time[2];				// minute, LSB, 2bytes

	uint8_t unknown7[7];
	
	uint8_t normal_fan_speed[2];						// %, LSB, 2 bytes, * 10
	uint8_t reduce_fan_speed[2];						// %, LSB, 2 bytes, * 10, signed
	uint8_t fan_boost_increase[2];					// %, LSB, 2 bytes, * 10

	uint8_t unknown8[16];

	uint8_t supply_fan_t12_adjust[2];				// %, LSB, 2 bytes, * 10

	uint8_t unknown8a[6];

	uint8_t fan_time_to_filter_change[2];	// days, LSB, 2 bytes

	uint8_t unknown9[6];

	uint8_t heating_compressor_min_frequency[2];		// Hz, LSB, 2 bytes, * 10
	uint8_t heating_compressor_max_frequency[2];		// Hz, LSB, 2 bytes, * 10
	uint8_t unknown9a[1];

	uint8_t crc;
} R_REPLY_STATUS_01;


typedef struct __attribute__ ((packed))
{
	CZ_PACKET_HEADER cz_head;
	
	uint8_t unknown[4];

	uint8_t hour1;
	uint8_t minute1;
	uint8_t second1;
	uint8_t day;
	uint8_t month;
	uint8_t year;
	uint8_t day_of_week;		// 1 = monday, 7 = sunday

	uint8_t unknown2;			// maybe daylight saving?

	uint8_t hour2;
	uint8_t minute2;
	uint8_t second2;

	uint8_t unknown3[10];

	uint8_t general_status[5];
								// 0x88 0A 00 00 10    // heatpump stopped, no defrost, no heating, no hot water, no add
								// 0x88 0E 00 00 10    // heatpump stopped, no defrost, no heating, no hot water, no add
								// 0x8B 0E 00 00 10    // heatpump stopped, no defrost, no heating, no hot water, no add
								// 0x88 08 00 00 10    // heatpump stopped, no defrost, no heating, no hot water, no add
								// 0x88 2E 00 00 10    // heatpump running, no defrost, no heating,    hot water, no add
								// 0x8B AE 00 00 10    // heatpump running, no defrost, no heating,    hot water, no add
								// 0xA8 1A 00 00 14    // heatpump stopped,    defrost,    heating or hot water,     add
								// 0xA8 2A 00 00 10    // heatpump running, no defrost,    heating, no hot water,    add
								// 0xA8 3A 00 00 10    // heatpump running, no defrost,    heating, no hot water,    add
								// 0x88 2A 00 00 10    // heatpump running, no defrost,    heating, no hot water, no add
								// 0x8B 2E 00 00 10    // heatpump running, no defrost, no heating,    hot water, no add
								// 0x88 AA 00 00 10    // heatpump running, no defrost,    heating, no hot water, no add
								// 0x80 08 00 00 10    // heatpump stopped, no defrost, no heating, no hot water, no add
								// 0xA0 08 00 00 10    // heatpump stopped, no defrost, no heating, no hot water, no add
								// 0x80 2A 00 00 10    // heatpump running, no defrost,    heating, no hot water, no add

								// byte 0:  | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
								//           =1 ----------------------------------> always
								//               =0 ------------------------------> always
								//                   =x --------------------------> 1 = add energy on, 0 = add energy off
								//                       =0 ----------------------> always
								//                           =x ------------------> ?
								//                               =0 --------------> always
								//                                    x---y-------> xy = 11 (hot water mode), 00 (heating mode, default when stopped)
	
								// byte 1:  | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
								//           =0 ----------------------------------> always
								//               =0 ------------------------------> always
								//                    x---y-----------------------> xy = 10 (heatpump is running), 11 (heatpump is powering down), 01 (heatpump stopped), 00 ( heatpump stopped?)
								//                           =1 ------------------> always
								//                               =x---y-----------> xy = 11 (hot water mode), 00 (idle mode), 01 (heating mode, default mode)
								//                                       =0 ------> always

								// byte 2:  | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
								//            always 0

								// byte 3:  | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
								//            always 0 or rarely changing

								// byte 4:  | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
								//           =0 ----------------------------------> always
								//               =0 ------------------------------> always
								//                   =0 --------------------------> always
								//                       =0 ----------------------> always
								//                           =0 ------------------> always
								//                               =x --------------> x = 1 (defrost in progress), 0 (no defrost in progress)
								//                                   =0 ----------> always
								//                                       =0 ------> always

	uint8_t unknown3b[3];

	uint8_t pending_alarm[2];		// bitmask of current alarms
										// byte 0: | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
										//                                  =1 -----> filter replacement alarm

	uint8_t unknown3c[6];

	uint8_t acknowledged_alarm[2];
										// bitmask of acknowledged alarms
										// before the corresponding bits are cleared from pending_alarm[], is it set in acknowledged_alarm[] using
										// an access to register 0x01, 0x02, 0x03, 0x04, 0x0B, 0x09, 0x81, 0x29, 0x00. Register value will be loaded in this array

	uint8_t unknown3d[11];

	// Note: at least 24, not sure above
#define STATUS_02_NB_SENSORS 62
	uint8_t sensors[STATUS_02_NB_SENSORS][2];	// bunch of sensors (TEx)

	uint8_t crc;
} R_REPLY_STATUS_02;

typedef struct __attribute__ ((packed))
{
	CZ_PACKET_HEADER cz_head;
	
	uint8_t unknown[178];
	uint8_t crc;
} R_REPLY_STATUS_03;


typedef struct __attribute__ ((packed))
{
	CZ_PACKET_HEADER cz_head;
	
	uint8_t unknown[170];
	
	uint8_t chauffage_puissance_consommee1[2];	// W, LSB, 2 bytes

	uint8_t unknown8[2];
	
	uint8_t chauffage_puissance_consommee2[2];	// W, LSB, 2 bytes

	uint8_t hot_water_production;						// 0x00 = off, 0x78 or 0x77 (rare) = production in progress

	uint8_t unknown9[1];	

	// 1 time, hot_water_production + unknown9 was 0x44 0x44. At the same time, unknown8 was 0x02 0xD3 else, was always 00 00
	uint8_t crc;
} R_REPLY_STATUS_04;


typedef struct __attribute__ ((packed))
{
	CZ_PACKET_HEADER cz_head;
	
	uint8_t hot_water_production;						// 0x00 = off, 0x78 or 0x77 (rare) = production in progress
	uint8_t unknown[1];

	uint8_t heating_calculated_setting[2];	// °C, LSB, 2 bytes, * 10

	uint8_t unknown0[30];

#define STATUS_05_TE3_INDOOR_TEMP_HISTORY_NB 21
	uint8_t te3_indoor_temp_history[STATUS_05_TE3_INDOOR_TEMP_HISTORY_NB][2];				// °C, LSB, 2 bytes, signed, TE3 (indoor temp.) history, new values first, 1 value/10s

#define STATUS_05_TE2_RETURN_WATER_HISTORY_NB 21
	uint8_t te2_return_water_history[STATUS_05_TE2_RETURN_WATER_HISTORY_NB][2];			// °C, LSB, 2 bytes, signed, TE2 (return water) history, new values first, 1 value/10s

	uint8_t room_heating_in_progress[2];					// LSB, 2bytes, 0x012C = room heating off, 0x0000 = room heating on

	uint8_t unknown1[35];							// always seems to be 84 03 84 03 00 00 22 FC 22 FC 00 00 00 00 00 00 00 00 00 00 00 00 00 00 FF FF 19 FC FF FF F1 D8 F1 D8 FF
														// rarely starts by   83 03 83 03

	uint8_t hot_water_calculated_setting[2];				// °C, LSB, 2bytes (it is the current hot water the heatpump tries to reach)
																	// it is either the user one or a different one if extra hot water is enabled
	uint8_t unknown2[2];							// always 0x00 00 (previous field may be 4 bytes length)
	uint8_t unknown_count_down[2];				// second, LSB, 2 bytes, * 10
	uint8_t unknown2b[2];						// always 0x00 00 (previous field may be 4 bytes length)
	uint8_t extra_hot_water;						// 0x00 = off, 0x0F = on
	uint8_t unknown2c[1];						// always 0x03

	uint8_t fan_speed;							// 1 = slow, 2 = normal, 3 = fast
	uint8_t fan_speed_duty[2];					// %, LSB, 2 bytes, * 10

	uint8_t unknown3[6];

	uint8_t condensing_temperature[2];					// °C, LSB, 2 bytes, * 10
	uint8_t condensing_pressure[2];						// bar, LSB, 2 bytes, * 10
	uint8_t crc;
} R_REPLY_STATUS_05;


typedef struct __attribute__ ((packed))
{
	CZ_PACKET_HEADER cz_head;
	
	uint8_t evaporator_pressure[2];				// bar, LSB, 2 bytes, * 10

	uint8_t pressure_ratio[2];				// LSB, 2 bytes, * 10

	uint8_t unknown0a[2];		// 2bytes, LSB, either a pressure or a temperature. Goes down when PAC is off but increase when PAC is started

	uint8_t heatpump_compressor_max_frequency1[2];

	uint8_t hot_water_active_max_frequency[2];	// Hz, LSB, 2bytes, * 10. forced to 0hz when hot water production is off (may be target frequency)

	uint8_t heatpump_active_max_frequency1[2];		// Hz, LSB, 2bytes, * 10. During defrost, forced to 0Hz else set to heating compressor max frequency

	uint8_t unknown0c[10];

	uint8_t heatpump_active_max_frequency2[2];		// Hz, LSB, 2bytes, * 10. During defrost, forced to 0Hz else set to heating compressor max frequency
	uint8_t heatpump_active_max_frequency3[2];		// Hz, LSB, 2bytes, * 10. During defrost, forced to 0Hz else set to heating compressor max frequency
	uint8_t heatpump_current_compressor_frequency[2];	// Hz, LSB, 2bytes, * 10.
	uint8_t chauffage_compressor_max_frequency3[2];

	uint8_t unknown0d[2];

	uint8_t heating_compressor_min_frequency[2];		// Hz, LSB, 2 bytes, * 10. Compressor min frequency when running in room heating mode
	uint8_t heating_compressor_max_frequency[2];		// Hz, LSB, 2 bytes, * 10. Compressor max frequency when running in room heating mode
	
	uint8_t unknown0[30];

	uint8_t heatpump_current_compressor_power[2];		// W, LSB, 2 bytes
	uint8_t heatpump_current_add_power[2];			// W, LSB, 2 bytes
	uint8_t heatpump_current_total_power1[2];		// W, LSB, 2 bytes
	uint8_t heatpump_current_total_power2[2];		// W, LSB, 2 bytes
	uint8_t heatpump_compressor_input_power[2];					// W, LSB, 2 bytes

	uint8_t unknown1a[4];

	uint8_t unknown_count_down[2];				// = 00 00 when heatpump is stopped else countdown from approximately 0x0383

	uint8_t unknown1b[6];

	uint8_t heatpump_defrost_delay[2];	// minute, 2 bytes, LSB, *10 (erroneous)

	uint8_t unknown2[12];

	uint8_t expansion_valve_calculated_setting[2];	// K, LSB, 2 bytes, * 10
	uint8_t vanne_expansion_xxx[2];						// negative number, expansion_valve_calculated_setting + xxx = temperature difference
	uint8_t expansion_valve_temperature_difference1[2];		// K, LSB, 2 bytes, * 10
	uint8_t expansion_valve_temperature_difference2[2];		// K, LSB, 2 bytes, * 10

	uint8_t unknown2a[30];

	uint8_t expansion_valve_valve_position1[2];		// %, LSB, 2 bytes, * 10
	uint8_t expansion_valve_valve_position2[2];		// %, LSB, 2 bytes, * 10

	uint8_t unknown2b[34];

	uint8_t crc;
} R_REPLY_STATUS_06;


typedef struct __attribute__ ((packed))
{
	CZ_PACKET_HEADER cz_head;
	
	uint8_t unknown[113];
	uint8_t input_power_limit[2];	// W, 2 bytes, LSB

	uint8_t unknown2[63];
	uint8_t crc;
} R_REPLY_STATUS_07;


typedef struct __attribute__ ((packed))
{
	CZ_PACKET_HEADER cz_head;

	uint8_t bcd_second;
	uint8_t bcd_minute;
	uint8_t bcd_hour;

	uint8_t unknown0;		// padding ?

	uint8_t bcd_day;
	uint8_t bcd_month;
	uint8_t bcd_year;

	uint8_t unknown0a[5];	// byte 0: padding ?
	uint8_t unknown[32];

	uint8_t compressor_energy[4];		// kWh, 4 bytes, LSB, * 100
	uint8_t add_energy[4];				// kWh, 4 bytes, LSB, * 100
	uint8_t hot_water_energy[4];		// kWh, 4 bytes, LSB, * 100
	uint8_t compressor_runtime[4];	// minutes, 4 bytes LSB
	uint8_t total_runtime[4];			// minutes, 4 bytes LSB
	uint8_t crc;
} R_REPLY_STATUS_08;


typedef struct __attribute__ ((packed))
{
	CZ_PACKET_HEADER cz_head;
	
	uint8_t unknown[14];
	uint8_t hotwater_priority[2];

	uint8_t unknown2[107];
#define STATUS_09_NB_AJUSTMENT 8
	uint8_t hardware_settings_adjustments_teX_adjust[STATUS_09_NB_AJUSTMENT];

	uint8_t unknown6[47];

	uint8_t crc;
} R_REPLY_STATUS_09;


typedef struct __attribute__ ((packed))
{
	CZ_PACKET_HEADER cz_head;
	
	uint8_t unknown[98];
	uint8_t holiday_temperature_reduction[2];			// °C, LSB, 2 bytes, * 10
	uint8_t holiday_minimal_room_temperature[2];		// °C, LSB, 2 bytes, * 10
	uint8_t cw_minimal_temperature[2];				// °C, LSB, 2 bytes, * 10
	uint8_t cw_maximal_temperature[2];				// °C, LSB, 2 bytes, * 10
	uint8_t cw_overheat_hysteresis[2];				// °C, LSB, 2 bytes, * 10

	uint8_t unknown2a[4];

	uint8_t cw_return_maximal_temperature[2];	// °C, LSB, 2 bytes, * 10

	uint8_t unknown2b[10];

	uint8_t cw_pump[2];									// %, LSB, 2 bytes, * 10

	uint8_t unknown2[25];
#define STATUS_10_STEP_WISE_HEATING_NB 8

	uint8_t step_wise_heating_temperature[STATUS_10_STEP_WISE_HEATING_NB][2];		// °C, LSB, 2 bytes, * 10
	uint8_t step_wise_heating_days[STATUS_10_STEP_WISE_HEATING_NB];				// day, 1 byte, * 10
	
	uint8_t unknown3[5];
	uint8_t crc;
} R_REPLY_STATUS_10;


typedef struct __attribute__ ((packed))
{
	CZ_PACKET_HEADER cz_head;
	
	uint8_t unknown[145];
	uint8_t led_luminosity;
	uint8_t unknown2[4];
	uint8_t holiday_reduction[2];		// in day

	uint8_t unknown3[26];
	uint8_t crc;
} R_REPLY_STATUS_11;


typedef struct __attribute__ ((packed))
{
	CZ_PACKET_HEADER cz_head;
	
	uint8_t unknown[9];
	
	uint8_t heatpump_compressor_blocked_frequency1[2];			// Hz, 2 bytes, LSB, * 10
	uint8_t heatpump_compressor_blocked_frequency2[2];			// Hz, 2 bytes, LSB, * 10
	uint8_t heatpump_compressor_blocked_frequency3[2];			// Hz, 2 bytes, LSB, * 10

	uint8_t unknown3[107];

	uint8_t heatpump_defrost_delay[2];	// min, 2 bytes, LSB, * 10;

	uint8_t unknown8[54];
	uint8_t crc;
} R_REPLY_STATUS_12;


typedef struct __attribute__ ((packed))
{
	CZ_PACKET_HEADER cz_head;
	
	uint8_t unknown[83];
	uint8_t crc;
} R_REPLY_STATUS_13;


typedef struct __attribute__ ((packed))
{
	CZ_PACKET_HEADER cz_head;
	
	uint8_t unknown[44];

	uint8_t language;
	
	uint8_t unknownd[11];
	uint8_t crc;
} R_REPLY_STATUS_14;


typedef struct __attribute__ ((packed))
{
	CZ_PACKET_HEADER cz_head;
	
	uint8_t unknown[178];
	uint8_t crc;
} R_REPLY_STATUS_15;


typedef struct __attribute__ ((packed))
{
	CZ_PACKET_HEADER cz_head;
	
	uint8_t unknown[178];
	uint8_t crc;
} R_REPLY_STATUS_16;


typedef struct __attribute__ ((packed))
{
	CZ_PACKET_HEADER cz_head;
	
	uint8_t unknown[178];
	uint8_t crc;
} R_REPLY_STATUS_17;


typedef struct __attribute__ ((packed))
{
	CZ_PACKET_HEADER cz_head;
	
	uint8_t unknown[77];
	uint8_t crc;
} R_REPLY_STATUS_18;


typedef struct __attribute__ ((packed))
{
	CZ_PACKET_HEADER cz_head;
	
	uint8_t unknown[178];
	uint8_t crc;
} R_REPLY_STATUS_19;

typedef struct __attribute__ ((packed))
{
	CZ_PACKET_HEADER cz_head;
	
	uint8_t unknown[22];
	uint8_t crc;
} R_REPLY_STATUS_20;


typedef struct __attribute__ ((packed))
{
	CZ_PACKET_HEADER cz_head;
	
	uint8_t unknown[178];
	uint8_t crc;
} R_REPLY_STATUS_22;


typedef struct __attribute__ ((packed))
{
	CZ_PACKET_HEADER cz_head;
	
#define STATUS_23_UNKNOWN1_NB 16
	uint8_t unknown1[STATUS_23_UNKNOWN1_NB][2];

	uint8_t unknown[28];

#define STATUS_23_UNKNOWN2_NB 16
	uint8_t unknown2[STATUS_23_UNKNOWN2_NB][2];

	uint8_t unknown3[86];
	uint8_t crc;
} R_REPLY_STATUS_23;


typedef struct __attribute__ ((packed))
{
	CZ_PACKET_HEADER cz_head;
	
	uint8_t unknown[178];
	uint8_t crc;
} R_REPLY_STATUS_24;


typedef struct __attribute__ ((packed))
{
	CZ_PACKET_HEADER cz_head;
	
	uint8_t unknown[178];
	uint8_t crc;
} R_REPLY_STATUS_25;

}
