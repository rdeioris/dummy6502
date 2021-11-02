#pragma once
#include <cstdint>
#include <memory>
#include <sstream>
#include <utility>
#include <vector>

namespace dummy6502
{
	class IMemoryMappable
	{
	public:
		virtual uint8_t Read8(const uint16_t address) = 0;
		virtual uint16_t Read16(const uint16_t address) = 0;
		virtual void Write8(const uint16_t address, const uint8_t value) = 0;
		virtual void Write16(const uint16_t address, const uint16_t value) = 0;
	};

	class UnmappedMemoryException : public std::exception
	{
	public:
		UnmappedMemoryException(uint16_t address)
		{
			std::stringstream stream;
			stream << "Unmapped address $" << std::uppercase << std::hex << static_cast<uint32_t>(address);
			message = stream.str();
		}
		const char* what() const noexcept override
		{
			return message.c_str();
		}
	protected:
		std::string message;
	};

	class OverlappingMemoryException : public std::exception
	{

	};

	class InvalidMemoryRangeException : public std::exception
	{

	};

	class IMemoryController
	{
	public:
		virtual uint8_t Read8(const uint16_t address) = 0;
		virtual uint16_t Read16(const uint16_t address) = 0;
		virtual void Write8(const uint16_t address, const uint8_t value) = 0;
	};

	template<typename T>
	class TMemoryController : public IMemoryController
	{
	public:
		struct MemoryMap
		{
			std::pair<uint16_t, uint16_t> start_end_addresses;
			T memory_mappable;

			MemoryMap(const uint16_t start, const uint16_t end, T in_memory_mappable) :
				memory_mappable(in_memory_mappable)
			{
				start_end_addresses = { start, end };
			}
		};

		uint8_t Read8(const uint16_t address) override
		{
			uint16_t relative_address = 0;
			return GetMemoryMappable(address, relative_address).Read8(relative_address);
		}

		uint16_t Read16(const uint16_t address) override
		{
			uint16_t relative_address = 0;
			return GetMemoryMappable(address, relative_address).Read16(relative_address);
		}

		void Write8(const uint16_t address, const uint8_t value) override
		{
			uint16_t relative_address = 0;
			GetMemoryMappable(address, relative_address).Write8(relative_address, value);
		}

		void Add(const uint16_t start, const uint16_t end, T memory_mappable)
		{
			if (end < start)
			{
				throw InvalidMemoryRangeException();
			}

			for (auto& pair : memory_maps)
			{
				auto current_start = pair.start_end_addresses.first;
				auto current_end = pair.start_end_addresses.second;

				if (start <= current_end && end >= current_start)
				{
					throw OverlappingMemoryException();
				}
			}
			memory_maps.push_back(MemoryMap(start, end, memory_mappable));
		}

		void Remap(const uint16_t start, const uint16_t end, T memory_mappable)
		{
			for (auto& pair : memory_maps)
			{
				auto current_start = pair.start_end_addresses.first;
				auto current_end = pair.start_end_addresses.second;

				if (start == current_start && end == current_end)
				{
					pair.memory_mappable = memory_mappable;
					return;
				}
			}

			throw InvalidMemoryRangeException();
		}

	protected:
		decltype(auto) GetMemoryMappable(const uint16_t address, uint16_t& relative_address)
		{
			for (auto& pair : memory_maps)
			{
				auto start = pair.start_end_addresses.first;
				auto end = pair.start_end_addresses.second;

				if (address >= start && address <= end)
				{
					relative_address = address - start;
					if constexpr (std::is_reference<T>::value)
					{
						return pair.memory_mappable;
					}
					else
					{
						return *pair.memory_mappable;
					}
				}
			}

			throw UnmappedMemoryException(address);
		}

		std::vector<MemoryMap> memory_maps;
	};

	using RefMemoryController = TMemoryController<IMemoryMappable&>;
	using SharedPtrMemoryController = TMemoryController<std::shared_ptr<IMemoryMappable>>;
	using UniquePtrMemoryController = TMemoryController<std::unique_ptr<IMemoryMappable>>;
	using PtrMemoryController = TMemoryController<IMemoryMappable*>;
}

