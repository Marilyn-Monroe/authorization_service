#include "hello.hpp"

#include <userver/utest/utest.hpp>

UTEST(SayHelloTo, Basic) {
  EXPECT_EQ(authorization_service::SayHelloTo("Developer"), "Hello, Developer!\n");
  EXPECT_EQ(authorization_service::SayHelloTo({}), "Hello, unknown user!\n");
}
