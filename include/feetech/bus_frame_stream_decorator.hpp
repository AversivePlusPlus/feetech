#ifndef FEETECH_BUS_FRAME_STREAM_DECORATOR_HPP
#define FEETECH_BUS_FRAME_STREAM_DECORATOR_HPP

#include <feetech/protocol.hpp>
#include <stream/output_stream.hpp>
#include <container/buffer.hpp>

namespace Aversive {

namespace Feetech {

  template<u16 BUFFER_SIZE, typename _Stream>
  class BusFrameStreamDecorator : Stream::OutputStream<char, unsigned int> {
  private:
    _Stream& _stream;
    Container::Buffer<BUFFER_SIZE, char> _write_buffer;
    u8 _id;

  public:
    BusFrameStreamDecorator(_Stream& stream)
      : _stream(stream), _id(0) {
    }

    inline void start(u8 id) {
      _id = id;
    }

    inline void end(void) {
      _stream.put(Protocol::START);
      _stream.put(Protocol::START);
      _stream.put(_id);
      u8 sum = _id;
      u8 s = _write_buffer.usedSpace()+1;
      _stream.put(s);
      sum += s;
      while(!_write_buffer.isEmpty()) {
          _stream.put(_write_buffer.head());
          sum += _write_buffer.head();
          _write_buffer.dequeue();
        }
      _stream.put((~sum)&0xFF);
    }

    inline void put(char c) {
      _write_buffer.enqueue(c);
    }

    inline unsigned int write(char *data, unsigned int length) {

      for(unsigned int i = 0 ; i < length ; i++) {
          put(data[i]);
        }

      return length;
    }

    inline unsigned int writable(void) {
      return BUFFER_SIZE - _write_buffer.usedSpace();
    }
  };

}

}

#endif//FEETECH_BUS_FRAME_STREAM_DECORATOR_HPP
