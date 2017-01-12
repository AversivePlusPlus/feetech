#include "../my_assert.hpp"
#include <iostream>

#include <feetech/packet_writer.hpp>
#include <feetech/packet_reader.hpp>
#include <feetech/sc.hpp>
#include <stream/buffer_stream.hpp>

using namespace Aversive::Base;
using namespace Aversive::Stream;
using namespace Aversive::Feetech;

BufferStream<128> sc_uart;
SC<decltype(sc_uart)> sc(sc_uart);

class MoreInfosPacketReader : public PacketReader {
public:
  MoreInfosPacketReader(const char* buffer, unsigned int size)
    : PacketReader(buffer, size) {
  }

  u8 sum(void) {
    return this->PacketReader::sum();
  }

  bool checkMinSize(void) {
    return this->PacketReader::checkMinSize();
  }

  bool checkStart(void) {
    return this->PacketReader::checkStart();
  }

  bool checkSize(void) {
    return this->PacketReader::checkSize();
  }

  bool checkSum(void) {
    return this->PacketReader::checkSum();
  }

};

int main(void) {
  myAssert(sc.ping(0) == false, "Line " S__LINE__ ": SC::ping");

  char buf[32];
  PacketWriter pw(buf, 32);
  myAssert(pw.data() == buf, "Line " S__LINE__ "PacketWriter::data");
  myAssert(pw.size() == 0, "Line " S__LINE__ "PacketWriter::size");

  pw.ping(1);
  myAssert(pw.size() == 6, "Line " S__LINE__ "PacketWriter::size");
  {
    MoreInfosPacketReader pr(pw.data(), pw.size());
    myAssert(pr.checkMinSize(), "Line " S__LINE__ "PacketReader::checkMinSize");
    myAssert(pr.checkStart(), "Line " S__LINE__ "PacketReader::checkStart");
    myAssert(pr.checkSize(), "Line " S__LINE__ "PacketReader::checkSize");
    //std::cout << (int)(u8)~pr.sum() << " " << (int)((u8)buf[pw.size()-1]) << std::endl;
    myAssert(pr.checkSum(), "Line " S__LINE__ "PacketReader::checkSum");
    myAssert(pr.valid(), "Line " S__LINE__ "PacketReader::valid");
    myAssert(pr.id() == 1, "Line " S__LINE__ "PacketReader::id");
    myAssert(pr.instr() == Protocol::INST_PING, "Line " S__LINE__ "PacketReader::instr");
  }

  pw.write(1, Protocol::P_ID, 2);
  myAssert(pw.size() == 8, "Line " S__LINE__ "PacketWriter::size");
  {
    MoreInfosPacketReader pr(pw.data(), pw.size());
    myAssert(pr.checkMinSize(), "Line " S__LINE__ "PacketReader::checkMinSize");
    myAssert(pr.checkStart(), "Line " S__LINE__ "PacketReader::checkStart");
    myAssert(pr.checkSize(), "Line " S__LINE__ "PacketReader::checkSize");
    //std::cout << (int)(u8)~pr.sum() << " " << (int)((u8)buf[pw.size()-1]) << std::endl;
    myAssert(pr.checkSum(), "Line " S__LINE__ "PacketReader::checkSum");
    myAssert(pr.valid(), "Line " S__LINE__ "PacketReader::valid");
    myAssert(pr.id() == 1, "Line " S__LINE__ "PacketReader::id");
    myAssert(pr.instr() == Protocol::INST_WRITE, "Line " S__LINE__ "PacketReader::instr");
  }

  pw.write(1, Protocol::P_PRESENT_POSITION, 512);
  myAssert(pw.size() == 9, "Line " S__LINE__ "PacketWriter::size");
  {
    MoreInfosPacketReader pr(pw.data(), pw.size());
    myAssert(pr.checkMinSize(), "Line " S__LINE__ "PacketReader::checkMinSize");
    myAssert(pr.checkStart(), "Line " S__LINE__ "PacketReader::checkStart");
    myAssert(pr.checkSize(), "Line " S__LINE__ "PacketReader::checkSize");
    //std::cout << (int)(u8)~pr.sum() << " " << (int)((u8)buf[pw.size()-1]) << std::endl;
    myAssert(pr.checkSum(), "Line " S__LINE__ "PacketReader::checkSum");
    myAssert(pr.valid(), "Line " S__LINE__ "PacketReader::valid");
    myAssert(pr.id() == 1, "Line " S__LINE__ "PacketReader::id");
    myAssert(pr.instr() == Protocol::INST_WRITE, "Line " S__LINE__ "PacketReader::instr");
  }

  pw.write(1, Protocol::P_ID, (u8*)"hello", sizeof("hello"));
  myAssert(pw.size() == 13, "Line " S__LINE__ "PacketWriter::size");
  {
    MoreInfosPacketReader pr(pw.data(), pw.size());
    myAssert(pr.checkMinSize(), "Line " S__LINE__ "PacketReader::checkMinSize");
    myAssert(pr.checkStart(), "Line " S__LINE__ "PacketReader::checkStart");
    myAssert(pr.checkSize(), "Line " S__LINE__ "PacketReader::checkSize");
    //std::cout << (int)(u8)~pr.sum() << " " << (int)((u8)buf[pw.size()-1]) << std::endl;
    myAssert(pr.checkSum(), "Line " S__LINE__ "PacketReader::checkSum");
    myAssert(pr.valid(), "Line " S__LINE__ "PacketReader::valid");
    myAssert(pr.id() == 1, "Line " S__LINE__ "PacketReader::id");
    myAssert(pr.instr() == Protocol::INST_WRITE, "Line " S__LINE__ "PacketReader::instr");
  }

  pw.read(1, Protocol::P_ID, 1);
  myAssert(pw.size() == 8, "Line " S__LINE__ "PacketWriter::size");
  {
    MoreInfosPacketReader pr(pw.data(), pw.size());
    myAssert(pr.checkMinSize(), "Line " S__LINE__ "PacketReader::checkMinSize");
    myAssert(pr.checkStart(), "Line " S__LINE__ "PacketReader::checkStart");
    myAssert(pr.checkSize(), "Line " S__LINE__ "PacketReader::checkSize");
    //std::cout << (int)(u8)~pr.sum() << " " << (int)((u8)buf[pw.size()-1]) << std::endl;
    myAssert(pr.checkSum(), "Line " S__LINE__ "PacketReader::checkSum");
    myAssert(pr.valid(), "Line " S__LINE__ "PacketReader::valid");
    myAssert(pr.id() == 1, "Line " S__LINE__ "PacketReader::id");
    myAssert(pr.instr() == Protocol::INST_READ, "Line " S__LINE__ "PacketReader::instr");
  }

  pw.syncWrite(Protocol::P_ID, 1);
  myAssert(pw.size() == 0, "Line " S__LINE__ "PacketWriter::size");
  {
    MoreInfosPacketReader pr(pw.data(), pw.size());
    myAssert(!pr.checkMinSize(), "Line " S__LINE__ "PacketReader::checkMinSize");
    myAssert(!pr.valid(), "Line " S__LINE__ "PacketReader::valid");
  }

  pw.syncWrite(Protocol::P_ID, 1)
      .add(2,1)
      .add(3,1);
  myAssert(pw.size() == 12, "Line " S__LINE__ "PacketWriter::size");
  {
    MoreInfosPacketReader pr(pw.data(), pw.size());
    myAssert(pr.checkMinSize(), "Line " S__LINE__ "PacketReader::checkMinSize");
    myAssert(pr.checkStart(), "Line " S__LINE__ "PacketReader::checkStart");
    myAssert(pr.checkSize(), "Line " S__LINE__ "PacketReader::checkSize");
    //std::cout << (int)(u8)~pr.sum() << " " << (int)((u8)buf[pw.size()-1]) << std::endl;
    myAssert(pr.checkSum(), "Line " S__LINE__ "PacketReader::checkSum");
    myAssert(pr.valid(), "Line " S__LINE__ "PacketReader::valid");
    myAssert(pr.id() == 0xFF, "Line " S__LINE__ "PacketReader::id");
    myAssert(pr.instr() == Protocol::INST_SYNC_WRITE, "Line " S__LINE__ "PacketReader::instr");
  }

  auto swpw = pw.syncWrite(Protocol::P_ID, 1);
  swpw.add(2,1);
  myAssert(pw.size() == 10, "Line " S__LINE__ "PacketWriter::size");
  swpw.add(3,1);
  myAssert(pw.size() == 12, "Line " S__LINE__ "PacketWriter::size");
  {
    MoreInfosPacketReader pr(pw.data(), pw.size());
    myAssert(pr.checkMinSize(), "Line " S__LINE__ "PacketReader::checkMinSize");
    myAssert(pr.checkStart(), "Line " S__LINE__ "PacketReader::checkStart");
    myAssert(pr.checkSize(), "Line " S__LINE__ "PacketReader::checkSize");
    //std::cout << (int)(u8)~pr.sum() << " " << (int)((u8)buf[pw.size()-1]) << std::endl;
    myAssert(pr.checkSum(), "Line " S__LINE__ "PacketReader::checkSum");
    myAssert(pr.valid(), "Line " S__LINE__ "PacketReader::valid");
    myAssert(pr.id() == 0xFF, "Line " S__LINE__ "PacketReader::id");
    myAssert(pr.instr() == Protocol::INST_SYNC_WRITE, "Line " S__LINE__ "PacketReader::instr");
  }

  pw.syncWrite(Protocol::P_ID, 2)
      .add(2,1)
      .add(3,1);
  myAssert(pw.size() == 0, "Line " S__LINE__ "PacketWriter::size");
  {
    MoreInfosPacketReader pr(pw.data(), pw.size());
    myAssert(!pr.checkMinSize(), "Line " S__LINE__ "PacketReader::checkMinSize");
    myAssert(!pr.valid(), "Line " S__LINE__ "PacketReader::valid");
  }

  pw.syncWrite(Protocol::P_PRESENT_POSITION, 2)
      .add(2,1)
      .add(3,1);
  myAssert(pw.size() == 14, "Line " S__LINE__ "PacketWriter::size");
  {
    MoreInfosPacketReader pr(pw.data(), pw.size());
    myAssert(pr.checkMinSize(), "Line " S__LINE__ "PacketReader::checkMinSize");
    myAssert(pr.checkStart(), "Line " S__LINE__ "PacketReader::checkStart");
    myAssert(pr.checkSize(), "Line " S__LINE__ "PacketReader::checkSize");
    //std::cout << (int)(u8)~pr.sum() << " " << (int)((u8)buf[pw.size()-1]) << std::endl;
    myAssert(pr.checkSum(), "Line " S__LINE__ "PacketReader::checkSum");
    myAssert(pr.valid(), "Line " S__LINE__ "PacketReader::valid");
    myAssert(pr.id() == 0xFF, "Line " S__LINE__ "PacketReader::id");
    myAssert(pr.instr() == Protocol::INST_SYNC_WRITE, "Line " S__LINE__ "PacketReader::instr");
  }

  pw.syncWrite(5, 6)
      .add(1,(u8*)"hello", sizeof("hello"))
      .add(2,(u8*)"hello", sizeof("hello"));
  myAssert(pw.size() == 22, "Line " S__LINE__ "PacketWriter::size");
  {
    MoreInfosPacketReader pr(pw.data(), pw.size());
    myAssert(pr.checkMinSize(), "Line " S__LINE__ "PacketReader::checkMinSize");
    myAssert(pr.checkStart(), "Line " S__LINE__ "PacketReader::checkStart");
    myAssert(pr.checkSize(), "Line " S__LINE__ "PacketReader::checkSize");
    //std::cout << (int)(u8)~pr.sum() << " " << (int)((u8)buf[pw.size()-1]) << std::endl;
    myAssert(pr.checkSum(), "Line " S__LINE__ "PacketReader::checkSum");
    myAssert(pr.valid(), "Line " S__LINE__ "PacketReader::valid");
    myAssert(pr.id() == 0xFF, "Line " S__LINE__ "PacketReader::id");
    myAssert(pr.instr() == Protocol::INST_SYNC_WRITE, "Line " S__LINE__ "PacketReader::instr");
  }

  pw.syncWrite(Protocol::P_ID, 6)
      .add(1,(u8*)"hello", sizeof("hello"))
      .add(2,(u8*)"hello", sizeof("hello"));
  myAssert(pw.size() == 22, "Line " S__LINE__ "PacketWriter::size");
  {
    MoreInfosPacketReader pr(pw.data(), pw.size());
    myAssert(pr.checkMinSize(), "Line " S__LINE__ "PacketReader::checkMinSize");
    myAssert(pr.checkStart(), "Line " S__LINE__ "PacketReader::checkStart");
    myAssert(pr.checkSize(), "Line " S__LINE__ "PacketReader::checkSize");
    //std::cout << (int)(u8)~pr.sum() << " " << (int)((u8)buf[pw.size()-1]) << std::endl;
    myAssert(pr.checkSum(), "Line " S__LINE__ "PacketReader::checkSum");
    myAssert(pr.valid(), "Line " S__LINE__ "PacketReader::valid");
    myAssert(pr.id() == 0xFF, "Line " S__LINE__ "PacketReader::id");
    myAssert(pr.instr() == Protocol::INST_SYNC_WRITE, "Line " S__LINE__ "PacketReader::instr");
  }

  std::cout << "OK" << std::endl;
  return 0;
}
