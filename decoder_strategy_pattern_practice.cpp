/**
*** Author: R-CO
*** E-Mail: daniel1820kobe@gmail.com
*** Date: 2017-11-16
**/
#include <cstdarg>
#include <cstdio>
#include <cstdlib>

#include <string>

namespace rco {

class Decoder
{
public:
  enum DecoderType {
    kMp3Decoder,
    kWavDecoder
  };

  virtual void excute() = 0;
private:
};

class Mp3Decoder : public Decoder
{
public:
  Mp3Decoder(int arg1, std::string arg2) 
    : arg1_(arg1), arg2_(arg2) 
  {
    // do something
  }
  
  void excute() {
    printf("---MP3 decode start---\n");
    printf("arg1 = [%d]\n", arg1_);
    printf("arg2 = [%s]\n", arg2_.c_str());
    printf("---MP3 decode finish---\n");
  }
  
private:
  int         arg1_;
  std::string arg2_;
  
  // not allowed default constructor
  Mp3Decoder() {
  }
};

class WavDecoder : public Decoder
{
public:
  WavDecoder(int arg1, int arg2, int arg3) 
    : arg1_(arg1), arg2_(arg2), arg3_(arg3)
  {
    // do something
  }

  void excute() {
    printf("---WAV decode start---\n");
    printf("arg1 = [%d]\n", arg1_);
    printf("arg2 = [%d]\n", arg2_);
    printf("arg3 = [%d]\n", arg3_);
    printf("---WAV decode finish---\n");
  }

private:
  int arg1_;
  int arg2_;
  int arg3_;
  
  // not allowed default constructor
  WavDecoder() {
  }
};

// try to using simple factory
#define RCO_SIMPLE_DECODER_FACTORY 1
#if RCO_SIMPLE_DECODER_FACTORY
class SimpleDecoderFactory
{
public:
  static Decoder *CreateDecoder(Decoder::DecoderType decoder_type, va_list var_list) {
    Decoder *decoder = NULL;

    switch (decoder_type) {
      case Decoder::kMp3Decoder: 
        decoder = CreateMp3Decoder(var_list);
        break;

      case Decoder::kWavDecoder: {
        decoder = CreateWavDecoder(var_list);
        break;
      }
      
      default:
        printf("Opps! Program should not arrive here...\n");
        break;
    }

    return decoder;
  }

private:
  static Mp3Decoder *CreateMp3Decoder(va_list var_list) {
    // retrive args
    int         arg1 = va_arg(var_list, int);
    std::string arg2 = va_arg(var_list, char *);
    
    // create instance of Mp3Decoder
    return  new (std::nothrow) Mp3Decoder(arg1, arg2);
  }
  
  static WavDecoder *CreateWavDecoder(va_list var_list) {
    // retrive args
    int arg1 = va_arg(var_list, int);
    int arg2 = va_arg(var_list, int);
    int arg3 = va_arg(var_list, int);
    
    // create instance of WavDecoder
    return new (std::nothrow) WavDecoder(arg1, arg2, arg3);
  }
};
#endif

class DecoderManager
{
public:
  DecoderManager(Decoder::DecoderType decoder_type, ...)
    : decoder_(NULL)
  {
#if !RCO_SIMPLE_DECODER_FACTORY
    switch (decoder_type) {
      case Decoder::kMp3Decoder: {
        va_list   var_list;
        
        // retrive args
        va_start(var_list, decoder_type);
        int         arg1 = va_arg(var_list, int);
        std::string arg2 = va_arg(var_list, char *);
        va_end(var_list);
        
        // create instance of Mp3Decoder
        decoder_ = new Mp3Decoder(arg1, arg2);
        break;
      }
      
      case Decoder::kWavDecoder: {
        va_list var_list;
        
        // retrive args
        va_start(var_list, decoder_type);
        int arg1 = va_arg(var_list, int);
        int arg2 = va_arg(var_list, int);
        int arg3 = va_arg(var_list, int);
        va_end(var_list);
        
        // create instance of WavDecoder
        decoder_ = new WavDecoder(arg1, arg2, arg3);
        break;
      }
      
      default:
        printf("Opps! Program should not arrive here...\n");
        break;
    }
#else
  va_list   var_list;
  va_start(var_list, decoder_type);
  decoder_ = SimpleDecoderFactory::CreateDecoder(decoder_type, var_list);
  va_end(var_list);
#endif
  }
  
  ~DecoderManager() {
    delete decoder_;
  }
  
  void decode() {
    decoder_->excute();
  }
  
private:
  Decoder *decoder_;
  
  // not allowed default constructor
  DecoderManager() {
  }
};

} // end of namespace `rco`

int main(int argc, char *argv[])
{
  rco::DecoderManager mp3_decoder(rco::Decoder::kMp3Decoder, 99, "Hi");
  mp3_decoder.decode();

  rco::DecoderManager wav_decoder(rco::Decoder::kWavDecoder, 9, 4, 6);
  wav_decoder.decode();

#ifdef WIN32 && _DEBUG
  system("pause");
#endif
  return EXIT_SUCCESS;
}
