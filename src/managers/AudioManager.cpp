#include "AudioManager.hpp"
#include <cstddef>
#include <stdio.h>
#include <string.h>
#include <cstdint>

////////////////
//Constructor

AudioManager::AudioManager()
{
  Device = alcOpenDevice(NULL);
  if (Device) {
    Context = alcCreateContext(Device, NULL);
    alcMakeContextCurrent(Context);
  }
}

////////////////
//Destructor

AudioManager::~AudioManager()
{
  alcDestroyContext(Context);
  alcCloseDevice(Device);
  //alDeleteBuffers(1, bufferAL);
  for (auto& pair : buffers)
  {
    alDeleteBuffers(1, pair.second);
  }
}

void AudioManager::loadsound(const char* filename,SoundID soundid)
{
  ALuint* buffer = new ALuint();
  alGenBuffers(1, buffer);

  if (!loadwavfile(*buffer, filename)) {
    alDeleteBuffers(1, buffer);
    printf("ERROR: wav loading has file!\n");
  }

  buffers.insert(std::make_pair(soundid, buffer)); // Asociar el nombre al buffer
}

bool AudioManager::loadwavfile(ALuint& buffer,const char* filename)
{
  //variables
  FILE* wavfile = nullptr;
  wav_hdr wavHeader{};
  long filesize;
  int headerSize = sizeof(wav_hdr); //filelength = 0;
  //Cargar WAV
  wavfile = fopen(filename, "r");
  if (wavfile)
  {
    //Se ha podido abrir correctamente el archivo
    //Obtenemos tamaño
    fseek(wavfile, 0, SEEK_END);
    filesize = ftell(wavfile);
    fseek(wavfile, 0, SEEK_SET);
    //Leemos cabecera
    fread(&wavHeader, headerSize, 1, wavfile);
    if (strncmp(wavHeader.RIFF, "RIFF", 4) != 0 || strncmp(wavHeader.WAVE, "WAVE", 4) != 0)
    {
      printf("this is not a valid WAVE file\n");
      fclose(wavfile);
      return false;
    }

    // Verificar el tamaño del subchunk "fmt "
    unsigned long fmtChunkSize = wavHeader.Subchunk1Size;
    if (fmtChunkSize > 16) {
      // Leer ExtraParamsSize y saltar ese número de bytes
      uint16_t extraParamsSize;
      fread(&extraParamsSize, sizeof(uint16_t), 1, wavfile);
      fseek(wavfile, extraParamsSize, SEEK_CUR);
    }

    // Búsqueda del bloque "data"
    char chunkId[5];      // Identificador del bloque
    fread(chunkId, sizeof(char), 4, wavfile);
    chunkId[4] = '\0';
    //unsigned long chunkSize;   // Tamaño del bloque
    //bool dataBlockFound = false;

    while (strncmp(chunkId, "data", sizeof(char)) != 0) {
      fread(chunkId, sizeof(char), 4, wavfile);
    }

    uint32_t chunkSize; 
    fread(&chunkSize, sizeof(uint32_t), 1, wavfile);
   
    //Reservo memoria
    char* dataBuffer = new char[chunkSize];
    fread(dataBuffer, sizeof(char), chunkSize, wavfile);
    //Formato
    ALenum format;
    unsigned short channels = wavHeader.NumOfChan;
    if (wavHeader.bitsPerSample == 8)
      format = (channels == 1) ? AL_FORMAT_MONO8 : AL_FORMAT_STEREO8;
    else
      format = (channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;

    //Generar buffer OpenAL
    bool succes{ false };
    alBufferData(buffer, format, dataBuffer, chunkSize, wavHeader.SampleRate);
    if (alGetError() != AL_NO_ERROR) {
      printf("Error while loading buffer data\n");
      succes = false;
    }
    else {
      printf("WAV cargado correctamente\n");
      succes = true;
    }
    //Liberamos buffer
    delete[] dataBuffer;
    //Cierro el archivo
    fclose(wavfile);
    return succes;
  }
  else {
    printf("ERROR, Opening WAV FIle!\n");
    return false;
  }
}
