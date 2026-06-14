#ifndef OP_H
#define OP_H

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>

using OpId = std::uint8_t;

template <typename OpPayload> constexpr auto serializeOp(OpId id, const OpPayload &payload)
{
    static_assert(std::is_trivially_copyable_v<OpPayload>);
    constexpr std::size_t length = sizeof(OpId) + sizeof(OpPayload);
    std::array<std::byte, length> result{};
    std::memcpy(result.data(), &id, sizeof(id));
    auto payload_bytes = std::bit_cast<std::array<std::byte, sizeof(OpPayload)>>(payload);
    std::copy(payload_bytes.begin(), payload_bytes.end(), result.begin() + sizeof(id));
    return result;
}

#endif
