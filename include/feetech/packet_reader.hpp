#ifndef FEETECH_PACKET_READER_HPP
#define FEETECH_PACKET_READER_HPP

#include <feetech/protocol.hpp>

namespace Aversive {
namespace Feetech {

class PacketReader {
protected:
  u8* _buffer;
  unsigned int _size;

protected:
  PacketReader(PacketReader& other)
    : _buffer(other._buffer), _size(other._size) {
  }

protected:
  u8 sum(void) {
    u8 sum = 0;
    for(unsigned int i = 2 ; i < _size-1 ; i++) {
      sum += _buffer[i];
    }
    return sum;
  }

protected:
  bool checkMinSize(void) {
    return 5 < _size;
  }

  bool checkStart(void) {
    return
        _buffer[0] == Protocol::START &&
        _buffer[1] == Protocol::START;
  }

  bool checkSize(void) {
    return _size == 4+_buffer[3];
  }

  bool checkSum(void) {
    return (u8)(~sum()) == (u8)_buffer[_size-1];
  }

public:
  PacketReader(const char* buffer, unsigned int size)
    : _buffer((u8*)buffer), _size(size) {
  }

  bool valid(void) {
    return checkMinSize() && checkStart() && checkSize() && checkSum();
  }

  u8 id(void) {
    return _buffer[2];
  }

  u8 instr(void) {
    return _buffer[4];
  }
};

}
}

#endif//FEETECH_PACKET_READER_HPP
