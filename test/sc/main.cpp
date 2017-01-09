#include "../my_assert.hpp"
#include <iostream>

#include <feetech/sc.hpp>
#include <stream/buffer_stream.hpp>

using namespace Aversive::Base;
using namespace Aversive::Stream;
using namespace Aversive::Feetech;

BufferStream<128> sc_uart;
SC<decltype(sc_uart)> sc(sc_uart);

int main(void) {
  myAssert(sc.ping(0) == false, "Line " S__LINE__ ": SC::ping");

  std::cout << "OK" << std::endl;
  return 0;
}
