#pragma once
#include "../Config.hpp"

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

		template<uint8_t R>
		static uint8_t DefaultRead(MMURegion *region, size_t offset)
		{
			return R;
		}

		static void DefaultWrite(MMURegion *region, size_t offset, uint8_t data)
		{
		}
	};
}

