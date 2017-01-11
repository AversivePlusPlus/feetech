#include "../my_assert.hpp"
#include <iostream>

#include <feetech/packet_writer.hpp>
#include <feetech/sc.hpp>
#include <stream/buffer_stream.hpp>

using namespace Aversive::Base;
using namespace Aversive::Stream;
using namespace Aversive::Feetech;

BufferStream<128> sc_uart;
SC<decltype(sc_uart)> sc(sc_uart);

int main(void) {
  myAssert(sc.ping(0) == false, "Line " S__LINE__ ": SC::ping");

  char buf[32];
  PacketWriter pw(buf, 32);
  myAssert(pw.data() == buf, "Line " S__LINE__ "PacketWriter::data");
  myAssert(pw.size() == 0, "Line " S__LINE__ "PacketWriter::size");

  pw.ping(1);
  myAssert(pw.size() == 6, "Line " S__LINE__ "PacketWriter::size");

  pw.write(1, Protocol::P_ID, u8(2));
  myAssert(pw.size() == 8, "Line " S__LINE__ "PacketWriter::size");

  pw.write(1, Protocol::P_PRESENT_POSITION, u16(512));
  myAssert(pw.size() == 9, "Line " S__LINE__ "PacketWriter::size");

  pw.write(1, Protocol::P_ID, (u8*)"hello", sizeof("hello"));
  myAssert(pw.size() == 13, "Line " S__LINE__ "PacketWriter::size");

  pw.read(1, Protocol::P_ID, 1);
  myAssert(pw.size() == 8, "Line " S__LINE__ "PacketWriter::size");

  pw.syncWrite(Protocol::P_ID, 1);
  myAssert(pw.size() == 0, "Line " S__LINE__ "PacketWriter::size");

  pw.syncWrite(Protocol::P_ID, 1)
      .add(2,u8(1))
      .add(3,u8(1));
  myAssert(pw.size() == 12, "Line " S__LINE__ "PacketWriter::size");

  auto swpw = pw.syncWrite(Protocol::P_ID, 1);
  swpw.add(2,u8(1));
  myAssert(pw.size() == 10, "Line " S__LINE__ "PacketWriter::size");
  swpw.add(3,u8(1));
  myAssert(pw.size() == 12, "Line " S__LINE__ "PacketWriter::size");

  pw.syncWrite(Protocol::P_ID, 1)
      .add(2,u16(1))
      .add(3,u16(1));
  myAssert(pw.size() == 0, "Line " S__LINE__ "PacketWriter::size");

  pw.syncWrite(Protocol::P_ID, 2)
      .add(2,u16(1))
      .add(3,u16(1));
  myAssert(pw.size() == 14, "Line " S__LINE__ "PacketWriter::size");

  pw.syncWrite(Protocol::P_ID, 6)
      .add(1,(u8*)"hello", sizeof("hello"))
      .add(2,(u8*)"hello", sizeof("hello"));
  myAssert(pw.size() == 22, "Line " S__LINE__ "PacketWriter::size");

  std::cout << "OK" << std::endl;
  return 0;
}
