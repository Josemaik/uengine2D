#pragma once
//#include "../utils/types.hpp"

struct AudioSource
{

public:
	//Constructor
	/*AudioSystem(GameEngine& ge)
	{*/
		//ALuint* source = new ALuint;
		//alGenSources(1, source);
		//if (alGetError() != AL_NO_ERROR)
		//{
		//	printf("Gen Source Error\n");
		//}
		////propierties
		//setLooping(false);
		//SetPitch(1.f);
		//SetGain(1.f);
		//setPosition(0.f, 0.f, 0.f);
		//setVelocity(0.f, 0.f, 0.f);
		////Inidicamos buffer a usar
		//ALuint& buffer = ge.getAudioBuffer(SoundID::MUSIC);
		//if (buffer != 0) {
		//	alSourcei(source[0], AL_BUFFER, buffer);
		//	printf("Source configurado\n");
		//	sources.insert({})
		//}
		//else {
		//	printf("Error: Audio buffer is not valid.\n");
		//}
		////Obtengo referencia a ge
		//geref = &ge;
	//}
	//Destructor
	~AudioSource()
	{
		printf("Destroy source\n");
		DestroySources();
	}
	//Función dónde se crean todos los sonidos
	void CreateSounds(GameEngine& ge)
	{
		LoadSound(ge, "../data/audio/music.wav",SoundID::MUSIC);
		//LoadSound(ge, "../data/audio/knife.wav",SoundID::SHURIKEN_THROW);
	}
	//Cargar sonido
	void LoadSound(GameEngine& ge,const char* filename,SoundID id)
	{
		//Cargo Audio
		ge.loadAudio(filename, id);
		//Creo Source
		CreateSource(id, ge.getAudioBuffer(id));
	}
	//Crear Source
	void CreateSource(SoundID id,ALuint& buffer)
	{
		//genero nuevo source
		ALuint* source = new ALuint;
		alGenSources(1, source);
		//asigno a source
		if (buffer != 0) {
			alSourcei(source[0], AL_BUFFER, buffer);
			printf("Source configurado\n");
			//Inserto source nuevo en map
			sources.insert({ id,source });
		}
		else {
			printf("Error: Audio buffer is not valid.\n");
		}
	}
	//Metodos para setear los valores de la source
	void SetPitch(float pitch,SoundID id)
	{
		alSourcef(*sources.at(id), AL_PITCH, pitch);
	}
	void SetGain(float gain, SoundID id)
	{
		alSourcef(*sources.at(id), AL_GAIN, gain);
	}
	void setLooping(bool loop, SoundID id)
	{
		alSourcei(*sources.at(id), AL_LOOPING, loop);
	}
	void setPosition(float x, float y, float z, SoundID id)
	{
		alSource3f(*sources.at(id), AL_POSITION, x, y, z);
	}
	void setVelocity(float x, float y, float z, SoundID id)
	{
		alSource3f(*sources.at(id), AL_VELOCITY, x, y, z);
	}
	//Metodos manipular Audio
	void Play(SoundID id)
	{
		printf("PLay del sonido\n");
		alSourcePlay(*sources.at(id));
	}
	void Stop(SoundID id)
	{
		alSourceStop(*sources.at(id));
	}
	void Pause(SoundID id)
	{
		alSourcePause(*sources.at(id));
	}
	bool isPlaying(SoundID id) const {
		ALint current_state;
		alGetSourcei(*sources.at(id), AL_SOURCE_STATE, &current_state);
		return current_state == AL_PLAYING;
	}

	void DestroySources() const
	{
		for (auto& source : sources)
		{
			alDeleteSources(1, source.second);
		}
	}

private:
	//ALuint* source{};
	std::unordered_map<SoundID, ALuint*> sources;
	//GameEngine* geref{};
};