#ifndef FEETECH_PACKET_WRITER_HPP
#define FEETECH_PACKET_WRITER_HPP

#include <feetech/protocol.hpp>

namespace Aversive {
namespace Feetech {

class PacketWriter {
protected:
  char* _buffer;
  unsigned int _size;
  const unsigned int _limit;

protected:
  PacketWriter(PacketWriter& other)
    : _buffer(other._buffer), _size(other._size), _limit(other._limit) {
  }

public:
  PacketWriter(char* buffer, unsigned int size)
    : _buffer(buffer), _size(0), _limit(size) {
  }

  const char* data(void) const {
    return _buffer;
  }

  unsigned int size(void) const {
    return _size;
  }

public:
  void ping(u8 id) {
    const u8 len = 2;
    if(4+len < _limit) {
      _size = 4+len;

      _buffer[0] = Protocol::START;
      _buffer[1] = Protocol::START;
      _buffer[2] = id;
      _buffer[3] = len;
      _buffer[4] = Protocol::INST_PING;
      _buffer[5] = (~(id+2+Protocol::INST_PING));
    }
    else {
      _size = 0;
    }
  }

  void write(u8 id, u8 reg, u8 value) {
    const u8 len = 4;
    if(4+len < _limit) {
      _size = 4+len;

      _buffer[0] = Protocol::START;
      _buffer[1] = Protocol::START;
      _buffer[2] = id;
      _buffer[3] = len;
      _buffer[4] = Protocol::INST_WRITE;

      _buffer[5] = reg;
      _buffer[6] = value;

      _buffer[7] = (~(id+len+Protocol::INST_PING));
    }
    else {
      _size = 0;
    }
  }

  void write(u8 id, u8 reg, u16 value) {
    const u8 len = 5;
    if(4+len < _limit) {
      _size = 4+len;

      _buffer[0] = Protocol::START;
      _buffer[1] = Protocol::START;
      _buffer[2] = id;
      _buffer[3] = len;
      _buffer[4] = Protocol::INST_WRITE;

      _buffer[5] = reg;
      _buffer[6] = value&0xFF;
      _buffer[7] = (value>>8)&0xFF;

      _buffer[8] = (~(id+len+Protocol::INST_PING));
    }
    else {
      _size = 0;
    }
  }

  void write(u8 id, u8 reg, const u8* buffer, unsigned int size) {
    const u8 len = 3+size;
    if(4+len < _limit) {
      _size = 4+len;

      _buffer[0] = Protocol::START;
      _buffer[1] = Protocol::START;
      _buffer[2] = id;
      _buffer[3] = len;
      _buffer[4] = Protocol::INST_WRITE;

      _buffer[5] = reg;
      for(unsigned int i = 0 ; i < size ; i++) {
        _buffer[6+i] = buffer[i];
      }

      _buffer[_size-1] = (~(id+len+Protocol::INST_PING));
    }
    else {
      _size = 0;
    }
  }

  void read(u8 id, u8 reg, unsigned int size) {
    const u8 len = 4;
    if(4+len <= _limit) {
      _size = 4+len;
      _buffer[0] = Protocol::START;
      _buffer[1] = Protocol::START;
      _buffer[2] = id;
      _buffer[3] = len;
      _buffer[4] = Protocol::INST_READ;

      _buffer[5] = reg;
      _buffer[6] = (u8)size;

      _buffer[7] = (~(id+len+Protocol::INST_PING));
    }
  }

  friend class SyncWriteHelper;
  class SyncWriteHelper {
  private:
    PacketWriter& _pw;

    unsigned int required(void) {
      if(8 <= _pw._size) {
        return _pw._buffer[6];
      }
      return 0;
    }

  public:
    SyncWriteHelper(PacketWriter& pw) :
      _pw(pw) {
    }

    ~SyncWriteHelper(void) {
      // no item added => invalid
      if(_pw._size <= 8) {
        _pw._size = 0;
      }
    }

    SyncWriteHelper& add(u8 id, u8 value) {
      if(required() == 1 && _pw._size+2 <= _pw._limit) {
        u8 sum = ~_pw._buffer[_pw._size-1];

        _pw._buffer[_pw._size-1] = id;
        _pw._buffer[_pw._size] = value;

        _pw._buffer[_pw._size+1] = ~(sum+id+value);
        _pw._size += 2;
      }
      else {
        _pw._size = 0;
      }

      return *this;
    }

    SyncWriteHelper& add(u8 id, u16 value) {
      if(required() == 2 && _pw._size+3 <= _pw._limit) {
        u8 sum = ~_pw._buffer[_pw._size-1];

        _pw._buffer[_pw._size-1] = id;
        _pw._buffer[_pw._size] = value&0xFF;
        _pw._buffer[_pw._size+1] = (value>>8)&0xFF;

        _pw._buffer[_pw._size+2] = ~(sum+id+value);
        _pw._size += 3;
      }
      else {
        _pw._size = 0;
      }

      return *this;
    }

    SyncWriteHelper& add(u8 id, const u8* buffer, unsigned int size) {
      if(required() == size && _pw._size+size+1 <= _pw._limit) {
        u8 sum = ~_pw._buffer[_pw._size-1];

        _pw._buffer[_pw._size-1] = id;
        for(unsigned int i = 0 ; i < size ; i++) {
          _pw._buffer[_pw._size+i] = buffer[i];
        }

        _pw._buffer[_pw._size+size] = ~(sum+id);
        _pw._size += size+1;
      }
      else {
        _pw._size = 0;
      }

      return *this;
    }
  };

  SyncWriteHelper syncWrite(u8 reg, unsigned int size) {
    const u8 len = 4;
    if(4+len < _limit) {
      _size = 4+len;

      _buffer[0] = Protocol::START;
      _buffer[1] = Protocol::START;
      _buffer[2] = 0xFF;
      _buffer[3] = len;
      _buffer[4] = Protocol::INST_SYNC_WRITE;

      _buffer[5] = reg;
      _buffer[6] = (u8)size;

      _buffer[7] = (~(0));
    }

    return SyncWriteHelper(*this);
  }
};

}
}

#endif//FEETECH_PACKET_WRITER_HPP
