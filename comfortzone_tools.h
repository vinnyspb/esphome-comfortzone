#pragma once

namespace esphome::comfortzone {

static inline uint16_t get_uint16(uint8_t *ptr)
{
	uint16_t v;

	v = (ptr[1] << 8) | ptr[0];

	return v;
}

static inline int16_t get_int16(uint8_t *ptr)
{
	int16_t v;

	v = (ptr[1] << 8) | ptr[0];

	return v;
}

static inline uint32_t get_uint32(uint8_t *ptr)
{
	uint32_t v;

	v = (ptr[3] << 24) | (ptr[2] << 16) | (ptr[1] << 8) | ptr[0];

	return v;
}

}
