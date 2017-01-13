#ifndef FEETECH_SERVO_STREAM_HPP
#define FEETECH_SERVO_STREAM_HPP

#include <stream/input_stream.hpp>
#include <stream/output_stream.hpp>
#include <stream/random_access_stream.hpp>

#include <feetech/packet_writer.hpp>
#include <feetech/packet_reader.hpp>

namespace Aversive {
namespace Feetech {

template<typename _Stream>
class ServoStream :
    public Stream::InputStream<char, unsigned int>,
    public Stream::OutputStream<char, unsigned int>,
    public Stream::RandomAccessStream<unsigned int> {
private:
  using Stream = _Stream;

private:
  Stream& _stream;
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
  ServoStream(Stream& stream, u8 id)
    : _stream(stream), _id(id) {
  }

  inline void put(char c) {
    constexpr unsigned int size = 8;
    char buffer[size];
    PacketWriter pw(buffer, size);
    pw.write(_id, (u8)this->tell(), (u8*)&c, 1);
    _stream.write(pw.data(), pw.size());
    seek(this->tell()+1);

    readTimeout(buffer, 6);
  }

  inline unsigned int write(const char* data, unsigned int length) {
    constexpr unsigned int size = 128;
    if(size < length) {
      return 0;
    }

    char buffer[size];
    PacketWriter pw(buffer, size);
    pw.write(_id, (u8)this->tell(), (u8*)data, length);
    _stream.write(pw.data(), pw.size());
    seek(this->tell()+length);

    char buff[6];
    readTimeout(buff, 6);
    return length;
  }

  inline unsigned int writable(void) {
    return 255-this->tell();
  }

  inline char get(void) {
    constexpr unsigned int size = 8;
    char buffer[size];
    PacketWriter pw(buffer, size);
    pw.read(_id, (u8)this->tell(), 1);
    _stream.write(pw.data(), pw.size());
    seek(this->tell()+1);

    readTimeout(buffer, 7);
    ResponsePacketReader pr(buffer, 7);
    if(!pr.valid()) {
      return 0;
    }
    return pr.data()[0];
  }

  inline unsigned int read(char* data, unsigned int length) {
    constexpr unsigned int size = 128;
    if(size < length) {
      return 0;
    }

    char buffer[size];
    PacketWriter pw(buffer, size);
    pw.read(_id, (u8)this->tell(), length);
    _stream.write(pw.data(), pw.size());

    readTimeout(buffer, 6+length);
    ResponsePacketReader pr(buffer, 6+length);
    if(!pr.valid()) {
      return 0;
    }

    for(unsigned int i = 0 ; i < pr.size() ; i++) {
      data[i] = pr.data()[i];
    }

    seek(this->tell()+pr.size());
    return pr.size();
  }

  inline unsigned int readable(void) {
    return 255-this->tell();
  }
};

}
}

#endif//FEETECH_SERVO_STREAM_HPP
