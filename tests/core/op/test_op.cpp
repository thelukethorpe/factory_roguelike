#include <core/op/op.h>
#include <doctest/doctest.h>

TEST_CASE("Op serialization")
{
    enum TestOp : OpId
    {
        Op1 = 0x01,
        Op2 = 0x02,
    };

    struct TestPayload
    {
        int x;
        float y;
    };

    TestOp op = TestOp::Op1;
    TestPayload payload{42, 3.14f};

    auto serialized = serializeOp<TestPayload>(op, payload);

    // Verify that the serialized data has the expected length.
    CHECK(serialized.size() == sizeof(TestOp) + sizeof(TestPayload));

    // Verify that the op code is correct.
    TestOp deserialized_op;
    std::memcpy(&deserialized_op, serialized.data(), sizeof(deserialized_op));
    CHECK(deserialized_op == op);

    // Verify that the payload is correct.
    TestPayload deserialized_payload;
    std::memcpy(&deserialized_payload, serialized.data() + sizeof(deserialized_op),
                sizeof(deserialized_payload));
    CHECK(deserialized_payload.x == payload.x);
    CHECK(deserialized_payload.y == payload.y);
}
