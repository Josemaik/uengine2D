#pragma once

#include "../openal/AL/al.h"
#include "../openal/AL/alc.h"

#include <stdio.h>
#include <string.h>
#include <cstdint>

#ifdef _MSC_VER
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif
#include <cstddef>
#include <unordered_map>

enum class SoundID : uint8_t
{
  MUSIC,
  SHURIKEN_THROW,
  FILE
};


struct AudioManager
{
  static const int NUM_BUFFERS{ 3 };

  typedef struct  WAV_HEADER {
    char                RIFF[4];        // RIFF Header      Magic header
    unsigned long       ChunkSize;      // RIFF Chunk Size  
    char                WAVE[4];        // WAVE Header      
    char                fmt[4];         // FMT header       
    unsigned long       Subchunk1Size;  // Size of the fmt chunk                                
    unsigned short      AudioFormat;    // Audio format 1=PCM,6=mulaw,7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM 
    unsigned short      NumOfChan;      // Number of channels 1=Mono 2=Sterio                   
    unsigned long       SampleRate;  // Sampling Frequency in Hz                             
    unsigned long       bytesPerSec;    // bytes per second 
    unsigned short      blockAlign;     // 2=16-bit mono, 4=16-bit stereo 
    unsigned short      bitsPerSample;  // Number of bits per sample      
    char                ExtraParamsSize[2]; // "data"  string   
    unsigned long       ExtraParams;  // Sampled data length    

  }wav_hdr;

	AudioManager(); //Iniciamos librería
	~AudioManager(); //Eliminamos contexto y dispositivo

	
	void loadsound(const char* filename,SoundID soundid);
  bool loadwavfile(ALuint& buffer,const char* filename);

  static AudioManager& getInstance() {
    static AudioManager instance;
    return instance;
  }

  ALuint& getAudioBuffer(SoundID id)
  {
    return *buffers.at(id);
  }

private:
	ALCdevice* Device{};
	ALCcontext* Context{};

	//ALuint* bufferAL;
  //Map para almacenar los sonidos
  std::unordered_map<SoundID, ALuint*> buffers;
};

