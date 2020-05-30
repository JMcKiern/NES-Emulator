#pragma once

#include <cstdint>
#include <queue>
#include <portaudio.h>
#include "../3rdparty/readerwriterqueue/readerwriterqueue.h"

using namespace moodycamel;

class SoundBuffer {
private:
	PaStream* stream;
	//std::queue<int16_t> queue;
	ReaderWriterQueue<int16_t> q;

	const int bufferSize = 2048;

public:
	SoundBuffer();
	~SoundBuffer();
	void Initialize(int sampleRate);
	void Write(int16_t* data, int frameCount);
};