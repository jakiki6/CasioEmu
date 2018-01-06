#pragma once
#include "../Config.hpp"

#include "../Logger.hpp"

#include <cstdint>
#include <string>

namespace casioemu
{
	class Emulator;

	struct MMURegion
	{
		typedef uint8_t (*ReadFunction)(MMURegion *, size_t);
		typedef void (*WriteFunction)(MMURegion *, size_t, uint8_t);

		size_t base, size;
		std::string description;
		void *userdata;
		ReadFunction read;
		WriteFunction write;
		bool setup_done;
		Emulator *emulator;

		MMURegion();
		~MMURegion();
		void Setup(size_t base, size_t size, std::string description, void *userdata, ReadFunction read, WriteFunction write, Emulator &emulator);
		void Kill();

		template<uint8_t read_value>
		static uint8_t IgnoreRead(MMURegion *region, size_t offset)
		{
			(void)region;
			(void)offset;
			return read_value;
		}

		static void IgnoreWrite(MMURegion *region, size_t offset, uint8_t data)
		{
			(void)region;
			(void)offset;
			(void)data;
		}

		template<typename value_type, value_type mask = (value_type)-1>
		static uint8_t DefaultRead(MMURegion *region, size_t offset)
		{
			value_type *value = (value_type *)(region->userdata);
			return ((*value) & mask) >> ((offset - region->base) * 8);
		}

		template<typename value_type, value_type mask = (value_type)-1>
		static void DefaultWrite(MMURegion *region, size_t offset, uint8_t data)
		{
			value_type *value = (value_type *)(region->userdata);
			*value &= ~(((value_type)0xFF) << ((offset - region->base) * 8));
			*value |= ((value_type)data) << ((offset - region->base) * 8);
			*value &= mask;
		}

		template<typename value_type, value_type mask = (value_type)-1>
		static uint8_t DefaultReadLog(MMURegion *region, size_t offset)
		{
			logger::Info("SFR read from %06X\n", offset);
			value_type *value = (value_type *)(region->userdata);
			return ((*value) & mask) >> ((offset - region->base) * 8);
		}

		template<typename value_type, value_type mask = (value_type)-1>
		static void DefaultWriteLog(MMURegion *region, size_t offset, uint8_t data)
		{
			value_type *value = (value_type *)(region->userdata);
			*value &= ~(((value_type)0xFF) << ((offset - region->base) * 8));
			*value |= ((value_type)data) << ((offset - region->base) * 8);
			*value &= mask;
			logger::Info("SFR write to %06X (%02X)\n", offset, data);
		}
	};
}
