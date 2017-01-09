#ifndef FEETECH_SERVO_MAP_STREAM_DECORATOR_HPP
#define FEETECH_SERVO_MAP_STREAM_DECORATOR_HPP

#include <feetech/bus_frame_stream_decorator.hpp>
#include <stream/input_stream.hpp>
#include <stream/output_stream.hpp>
#include <stream/random_access_stream.hpp>

namespace Aversive {

namespace Feetech {

  template<u8 BUFFER_SIZE, typename _Stream>
  class ServoMapStreamDecorator :
      public Stream::InputStream<char, unsigned int>,
      public Stream::OutputStream<char, unsigned int>,
      public Stream::RandomAccessStream<unsigned int> {
  private:
    BusFrameStreamDecorator<BUFFER_SIZE, _Stream> _bfs;
    _Stream& _stream;
    u8 _id;

  private:
    unsigned int readTimeout(char* data, unsigned int length) {
      unsigned int len = 0;
      for(u32 i = 0 ; i < Protocol::TIMEOUT && len < length ; i++) {
          u16 rlen = _stream.readable();
          if(rlen) {
              _stream.read(data+len, rlen);
              len += rlen;
            }
        }
      return len;
    }

  public:
    ServoMapStreamDecorator(_Stream& stream)
      : _bfs(stream), _stream(stream), _id(0xfe) {
    }

    inline void setServo(u8 id) {
      _id = id;
    }

    inline void put(char c) {
      _bfs.start(_id);
      _bfs.put(Protocol::INST_WRITE);
      _bfs.put(_cursor);
      _bfs.put(c);
      _bfs.end();
      seek(tell()+1);

      char buff[6];
      readTimeout(buff, 6);
    }

    inline unsigned int write(char* data, unsigned int length) {
      _bfs.start(_id);
      _bfs.put(Protocol::INST_WRITE);
      _bfs.put(_cursor);
      for(unsigned int i = 0 ; i < length ; i++) {
          _bfs.put(data[i]);
        }
      _bfs.end();

      char buff[6];
      if(readTimeout(buff, 6) != 6) {
          return 0;
        }

      seek(tell()+length);
      return length;
    }

    inline unsigned int writable(void) {
      return BUFFER_SIZE-7;
    }

    inline char get(void) {
      _bfs.start(_id);
      _bfs.put(Protocol::INST_READ);
      _bfs.put(_cursor);
      _bfs.put(1);
      _bfs.end();

      char buff[7];

      if(readTimeout(buff, 7) != 7) {
          return 0;
        }

      seek(tell()+1);
      return buff[5];
    }

    inline unsigned int read(char* data, unsigned int length) {
      _bfs.start(_id);
      _bfs.put(Protocol::INST_READ);
      _bfs.put(_cursor);
      _bfs.put(length);
      _bfs.end();

      char buff[BUFFER_SIZE];
      unsigned int msg_len = length + 6;

      if(readTimeout(buff, msg_len) != msg_len) {
          return 0;
        }

      for(unsigned int i = 0 ; i < length ; i++) {
          data[i] = buff[i+5];
        }

      seek(tell()+length);
      return length;
    }

    inline unsigned int readable(void) {
      return BUFFER_SIZE-6;
    }
  };

}

}

#endif//FEETECH_SERVO_MAP_STREAM_DECORATOR_HPP
