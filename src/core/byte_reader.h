#ifndef BYTE_READER_H
#define BYTE_READER_H

#include <cstddef>
#include <cstring>
#include <span>
#include <stdexcept>

class ByteReader
{
  public:
    ByteReader(std::span<const std::byte> bytes) : bytes_(bytes) {}

    [[nodiscard]] bool empty() const { return position_ >= bytes_.size(); }

    template <typename T> T read()
    {
        static_assert(std::is_trivially_copyable_v<T>);

        if (position_ + sizeof(T) > bytes_.size())
        {
            throw std::runtime_error("unexpected end");
        }

        T value;
        const auto source = bytes_.subspan(position_, sizeof(T));
        std::memcpy(&value, source.data(), sizeof(T));
        position_ += sizeof(T);

        return value;
    }

  private:
    std::span<const std::byte> bytes_;
    std::size_t position_ = 0;
};

#endif
