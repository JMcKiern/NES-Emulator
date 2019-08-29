#include <portaudio.h>
#include <cstdint>
#include <stdexcept>
#include "SoundBuffer.h"

using namespace moodycamel;

int callback(const void *input, void *output,
	unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags, void *userData) {
	
	ReaderWriterQueue<int16_t>* qPtr = reinterpret_cast<ReaderWriterQueue<int16_t>*>(userData);

	for (unsigned int i = 0; i < frameCount; i++) {
		int16_t frameOut;
		if (!qPtr->try_dequeue(frameOut)) {
			// queue empty
			frameOut = 0;
		}
		*(reinterpret_cast<int16_t*>(output) + i) = frameOut;
	}

	return paContinue;
}
SoundBuffer::SoundBuffer() :
	q(65536)
{}
SoundBuffer::~SoundBuffer() {
	Pa_AbortStream(stream);
	Pa_CloseStream(stream);
	Pa_Terminate();
}
void SoundBuffer::Initialize(int sampleRate) {
	PaError err;

	err = Pa_Initialize();
	if (err != paNoError)
		throw std::runtime_error("Failed to initialize PortAudio!");

	err = Pa_OpenDefaultStream(
		&stream,
		0, // No input
		1, // Mono, not stereo
		paInt16,
		sampleRate,
		bufferSize, // buffer size
		callback,
		&q
	);
	if (err != paNoError)
		throw std::runtime_error("Failed to open audio stream!");

	err = Pa_StartStream(stream);
	if (err != paNoError)
		throw std::runtime_error("Failed to start audio stream!");

}
void SoundBuffer::Write(int16_t* data, int frameCount) {
	for (int i = 0; i < frameCount; i++) {
		q.try_enqueue(*(data + i));
	}
}
