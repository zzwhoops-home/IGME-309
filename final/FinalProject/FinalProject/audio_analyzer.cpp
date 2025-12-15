#include "audio_analyzer.h"
#include <portaudio.h>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <cstring>
#include <fstream>
#include <stdint.h>

// WAV file header structures
struct WAVHeader
{
    char riff[4];           // "RIFF"
    uint32_t fileSize;      // File size - 8
    char wave[4];           // "WAVE"
    char fmt[4];            // "fmt "
    uint32_t fmtSize;       // Format chunk size
    uint16_t audioFormat;   // Audio format (1 = PCM)
    uint16_t numChannels;   // Number of channels
    uint32_t sampleRate;    // Sample rate
    uint32_t byteRate;      // Byte rate
    uint16_t blockAlign;    // Block align
    uint16_t bitsPerSample; // Bits per sample
};

struct WAVDataChunk
{
    char id[4];    // "data"
    uint32_t size; // Data size
};

// Internal PortAudio callback for microphone
int audioCallbackInternal(const void* inputBuffer, void* outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void* userData)
{

    AudioAnalyzer* analyzer = static_cast<AudioAnalyzer*>(userData);
    const float* in = static_cast<const float*>(inputBuffer);

    if (in != nullptr && analyzer != nullptr)
    {
        analyzer->processAudioBuffer(in, framesPerBuffer);
    }

    return paContinue;
}

// Internal PortAudio callback for file playback
int fileCallbackInternal(const void* inputBuffer, void* outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void* userData)
{

    AudioAnalyzer* analyzer = static_cast<AudioAnalyzer*>(userData);
    float* out = static_cast<float*>(outputBuffer);

    // Debug: Print occasionally to monitor playback
    static int callCount = 0;
    callCount++;
    if (callCount % 100 == 0)
    {
        std::cout << "Callback #" << callCount
            << " - Position: " << analyzer->m_filePosition
            << " / " << analyzer->m_fileData.size()
            << " (" << analyzer->getCurrentTime() << "s / "
            << analyzer->getDuration() << "s)" << std::endl;
    }

    if (out != nullptr && analyzer != nullptr && analyzer->m_isPlaying)
    {
        // Fill output buffer from file data
        size_t remaining = analyzer->m_fileData.size() - analyzer->m_filePosition;
        size_t toCopy = std::min((size_t)framesPerBuffer, remaining);

        if (toCopy > 0)
        {
            // Copy audio data
            std::memcpy(out, &analyzer->m_fileData[analyzer->m_filePosition],
                toCopy * sizeof(float));

            // Process for visualization
            analyzer->processAudioBuffer(out, toCopy);

            analyzer->m_filePosition += toCopy;

            // Fill remaining with silence if needed
            if (toCopy < framesPerBuffer)
            {
                std::memset(&out[toCopy], 0, (framesPerBuffer - toCopy) * sizeof(float));
            }
        }
        else
        {
            // End of file - loop back to start (DEFAULT BEHAVIOR)
            analyzer->m_filePosition = 0;
            std::memset(out, 0, framesPerBuffer * sizeof(float));

            // OPTIONAL: To stop at end instead of looping, uncomment:
            // analyzer->m_isPlaying = false;
            // return paComplete;
        }
    }
    else
    {
        // Silence if paused
        std::memset(out, 0, framesPerBuffer * sizeof(float));
    }

    return paContinue;
}

// Constructor for microphone input
AudioAnalyzer::AudioAnalyzer(int sampleRate, int bufferSize)
    : m_sampleRate(sampleRate), m_bufferSize(bufferSize), m_stream(nullptr),
    m_sourceMode(MICROPHONE), m_filePosition(0), m_isPlaying(false)
{

    m_features.magnitude = 0.0f;
    m_features.rms = 0.0f;
    m_features.pitch = 0.0f;
    m_features.spectralCentroid = 0.0f;
}

// Constructor for file input
AudioAnalyzer::AudioAnalyzer(const std::string& filename, int bufferSize)
    : m_bufferSize(bufferSize), m_stream(nullptr), m_sourceMode(AUDIO_FILE),
    m_filePosition(0), m_isPlaying(false), m_filename(filename)
{

    m_features.magnitude = 0.0f;
    m_features.rms = 0.0f;
    m_features.pitch = 0.0f;
    m_features.spectralCentroid = 0.0f;

    if (!loadAudioFile(filename))
    {
        std::cerr << "Failed to load audio file: " << filename << std::endl;
    }
}

AudioAnalyzer::~AudioAnalyzer()
{
    stop();
}

bool AudioAnalyzer::start()
{
    PaError err = Pa_Initialize();
    if (err != paNoError)
    {
        std::cerr << "PortAudio init error: " << Pa_GetErrorText(err) << std::endl;
        return false;
    }

    PaStream* stream = nullptr;

    if (m_sourceMode == MICROPHONE)
    {
        // Open input stream for microphone
        err = Pa_OpenDefaultStream(&stream,
            1, // input channels
            0, // output channels
            paFloat32,
            m_sampleRate,
            m_bufferSize,
            audioCallbackInternal,
            this);
    }
    else
    {
        // For file playback, use more explicit stream parameters
        std::cout << "Opening audio output at " << m_sampleRate << " Hz" << std::endl;

        PaStreamParameters outputParams;
        outputParams.device = Pa_GetDefaultOutputDevice();
        if (outputParams.device == paNoDevice)
        {
            std::cerr << "No default output device found!" << std::endl;
            Pa_Terminate();
            return false;
        }

        outputParams.channelCount = 1; // Mono output
        outputParams.sampleFormat = paFloat32;
        outputParams.suggestedLatency = Pa_GetDeviceInfo(outputParams.device)->defaultLowOutputLatency;
        outputParams.hostApiSpecificStreamInfo = nullptr;

        // Open stream with explicit parameters to force the correct sample rate
        err = Pa_OpenStream(&stream,
            nullptr,       // No input
            &outputParams, // Output parameters
            m_sampleRate,  // FORCE this exact sample rate
            m_bufferSize,
            paClipOff, // No clipping
            fileCallbackInternal,
            this);

        if (err != paNoError)
        {
            std::cerr << "Failed to open at " << m_sampleRate << " Hz" << std::endl;
            std::cerr << "Trying to find supported sample rate..." << std::endl;

            // Check if this sample rate is supported
            err = Pa_IsFormatSupported(nullptr, &outputParams, m_sampleRate);
            if (err == paFormatIsSupported)
            {
                std::cerr << "Sample rate IS supported, but stream failed to open!" << std::endl;
            }
            else
            {
                std::cerr << "Sample rate NOT supported by device: " << Pa_GetErrorText(err) << std::endl;
                std::cerr << "Your audio device may not support " << m_sampleRate << " Hz" << std::endl;
            }
            Pa_Terminate();
            return false;
        }

        // Verify the actual sample rate being used
        const PaStreamInfo* streamInfo = Pa_GetStreamInfo(stream);
        if (streamInfo)
        {
            std::cout << "Stream opened successfully at " << streamInfo->sampleRate << " Hz" << std::endl;
            if (std::abs(streamInfo->sampleRate - m_sampleRate) > 1.0)
            {
                std::cerr << "WARNING: Requested " << m_sampleRate << " Hz but got "
                    << streamInfo->sampleRate << " Hz!" << std::endl;
                std::cerr << "Audio will play at wrong speed!" << std::endl;
            }
        }
    }

    if (err != paNoError)
    {
        std::cerr << "PortAudio open error: " << Pa_GetErrorText(err) << std::endl;
        Pa_Terminate();
        return false;
    }

    m_stream = stream;

    err = Pa_StartStream(stream);
    if (err != paNoError)
    {
        std::cerr << "PortAudio start error: " << Pa_GetErrorText(err) << std::endl;
        Pa_CloseStream(stream);
        Pa_Terminate();
        return false;
    }

    if (m_sourceMode == AUDIO_FILE)
    {
        m_isPlaying = true; // Auto-play file
    }

    return true;
}

void AudioAnalyzer::stop()
{
    if (m_stream)
    {
        Pa_StopStream(static_cast<PaStream*>(m_stream));
        Pa_CloseStream(static_cast<PaStream*>(m_stream));
        Pa_Terminate();
        m_stream = nullptr;
    }
}

void AudioAnalyzer::setAudioCallback(AudioCallback callback)
{
    m_callback = callback;
}

AudioFeatures AudioAnalyzer::getFeatures() const
{
    return m_features;
}

bool AudioAnalyzer::isPlaying() const
{
    return m_isPlaying;
}

void AudioAnalyzer::play()
{
    if (m_sourceMode == AUDIO_FILE)
    {
        m_isPlaying = true;
    }
}

void AudioAnalyzer::pause()
{
    if (m_sourceMode == AUDIO_FILE)
    {
        m_isPlaying = false;
    }
}

void AudioAnalyzer::seek(float seconds)
{
    if (m_sourceMode == AUDIO_FILE)
    {
        size_t newPosition = (size_t)(seconds * m_sampleRate);
        if (newPosition < m_fileData.size())
        {
            m_filePosition = newPosition;
        }
    }
}

float AudioAnalyzer::getDuration() const
{
    if (m_sourceMode == AUDIO_FILE && !m_fileData.empty())
    {
        return (float)m_fileData.size() / m_sampleRate;
    }
    return 0.0f;
}

float AudioAnalyzer::getCurrentTime() const
{
    if (m_sourceMode == AUDIO_FILE && m_sampleRate > 0)
    {
        return (float)m_filePosition / m_sampleRate;
    }
    return 0.0f;
}

bool AudioAnalyzer::loadAudioFile(const std::string& filename)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file)
    {
        std::cerr << "Cannot open file: " << filename << std::endl;
        return false;
    }

    // Read WAV header
    WAVHeader header;
    file.read(reinterpret_cast<char*>(&header), sizeof(WAVHeader));

    // Validate WAV file
    if (std::strncmp(header.riff, "RIFF", 4) != 0 ||
        std::strncmp(header.wave, "WAVE", 4) != 0)
    {
        std::cerr << "Not a valid WAV file!" << std::endl;
        return false;
    }

    if (header.audioFormat != 1)
    {
        std::cerr << "Only PCM WAV files are supported!" << std::endl;
        return false;
    }

    // Find data chunk (skip any extra chunks)
    WAVDataChunk dataChunk;
    while (file.read(reinterpret_cast<char*>(&dataChunk), sizeof(WAVDataChunk)))
    {
        if (std::strncmp(dataChunk.id, "data", 4) == 0)
        {
            break;
        }
        // Skip this chunk
        file.seekg(dataChunk.size, std::ios::cur);
    }

    if (std::strncmp(dataChunk.id, "data", 4) != 0)
    {
        std::cerr << "Could not find data chunk in WAV file!" << std::endl;
        return false;
    }

    m_sampleRate = header.sampleRate;
    int numChannels = header.numChannels;
    int bitsPerSample = header.bitsPerSample;
    int bytesPerSample = bitsPerSample / 8;

    // CRITICAL: numFrames is the number of FRAMES (one per time point)
    // NOT the total number of samples (which would be frames * channels)
    size_t numFrames = dataChunk.size / (numChannels * bytesPerSample);

    std::cout << "Loading WAV file:" << std::endl;
    std::cout << "  Sample rate: " << m_sampleRate << " Hz" << std::endl;
    std::cout << "  Channels: " << numChannels << std::endl;
    std::cout << "  Bits per sample: " << bitsPerSample << std::endl;
    std::cout << "  Bytes per sample: " << bytesPerSample << std::endl;
    std::cout << "  Data chunk size: " << dataChunk.size << " bytes" << std::endl;
    std::cout << "  Total frames: " << numFrames << std::endl;
    std::cout << "  Expected duration: " << (float)numFrames / m_sampleRate << " seconds" << std::endl;

    // Read and convert audio data to float
    if (bitsPerSample == 16)
    {
        // 16-bit PCM
        size_t totalSamples = numFrames * numChannels;
        std::vector<int16_t> rawData(totalSamples);
        file.read(reinterpret_cast<char*>(rawData.data()), dataChunk.size);

        std::cout << "  Read " << totalSamples << " samples (" << numFrames << " frames x "
            << numChannels << " channels)" << std::endl;

        // Convert to mono float (-1.0 to 1.0)
        m_fileData.resize(numFrames);
        for (size_t i = 0; i < numFrames; i++)
        {
            float sum = 0.0f;
            for (int ch = 0; ch < numChannels; ch++)
            {
                sum += rawData[i * numChannels + ch] / 32768.0f;
            }
            m_fileData[i] = sum / numChannels;
        }

        std::cout << "  Converted to " << m_fileData.size() << " mono samples" << std::endl;
    }
    else if (bitsPerSample == 8)
    {
        // 8-bit PCM (unsigned)
        size_t totalSamples = numFrames * numChannels;
        std::vector<uint8_t> rawData(totalSamples);
        file.read(reinterpret_cast<char*>(rawData.data()), dataChunk.size);

        std::cout << "  Read " << totalSamples << " samples (" << numFrames << " frames x "
            << numChannels << " channels)" << std::endl;

        // Convert to mono float (-1.0 to 1.0)
        m_fileData.resize(numFrames);
        for (size_t i = 0; i < numFrames; i++)
        {
            float sum = 0.0f;
            for (int ch = 0; ch < numChannels; ch++)
            {
                sum += (rawData[i * numChannels + ch] - 128) / 128.0f;
            }
            m_fileData[i] = sum / numChannels;
        }

        std::cout << "  Converted to " << m_fileData.size() << " mono samples" << std::endl;
    }
    else if (bitsPerSample == 32)
    {
        // 32-bit float PCM
        size_t totalSamples = numFrames * numChannels;
        std::vector<float> rawData(totalSamples);
        file.read(reinterpret_cast<char*>(rawData.data()), dataChunk.size);

        std::cout << "  Read " << totalSamples << " samples (" << numFrames << " frames x "
            << numChannels << " channels)" << std::endl;

        // Convert to mono
        m_fileData.resize(numFrames);
        for (size_t i = 0; i < numFrames; i++)
        {
            float sum = 0.0f;
            for (int ch = 0; ch < numChannels; ch++)
            {
                sum += rawData[i * numChannels + ch];
            }
            m_fileData[i] = sum / numChannels;
        }

        std::cout << "  Converted to " << m_fileData.size() << " mono samples" << std::endl;
    }
    else
    {
        std::cerr << "Unsupported bits per sample: " << bitsPerSample << std::endl;
        std::cerr << "Supported formats: 8-bit, 16-bit, 32-bit float PCM" << std::endl;
        return false;
    }

    file.close();
    m_filePosition = 0;

    std::cout << "WAV file loaded successfully!" << std::endl;
    std::cout << "  Final buffer size: " << m_fileData.size() << " samples" << std::endl;
    std::cout << "  Calculated duration: " << (float)m_fileData.size() / m_sampleRate << " seconds" << std::endl;
    return true;
}

void AudioAnalyzer::processAudioBuffer(const float* buffer, int frameCount)
{
    // Calculate RMS and magnitude
    float sumSquares = 0.0f;
    float maxVal = 0.0f;

    for (int i = 0; i < frameCount; i++)
    {
        sumSquares += buffer[i] * buffer[i];
        maxVal = std::max(maxVal, std::abs(buffer[i]));
    }

    m_features.rms = std::sqrt(sumSquares / frameCount);
    m_features.magnitude = maxVal;

    // Calculate pitch
    m_features.pitch = calculatePitch(buffer, frameCount);

    // Compute frequency spectrum
    computeFFT(buffer, frameCount, m_features.spectrum);

    // Calculate spectral centroid
    m_features.spectralCentroid = calculateSpectralCentroid(m_features.spectrum);

    // Store waveform
    m_features.waveform.assign(buffer, buffer + frameCount);

    // Call user callback if registered
    if (m_callback)
    {
        m_callback(m_features);
    }
}

float AudioAnalyzer::calculatePitch(const float* buffer, int frameCount)
{
    if (m_features.rms < 0.001f)
    {
        return 0.0f;
    }

    std::vector<float> autocorr(frameCount / 2, 0.0f);

    for (int lag = 0; lag < frameCount / 2; lag++)
    {
        for (int i = 0; i < frameCount - lag; i++)
        {
            autocorr[lag] += buffer[i] * buffer[i + lag];
        }
    }

    int minLag = 20;
    int bestLag = -1;
    float maxVal = 0.0f;

    for (int i = minLag; i < frameCount / 2; i++)
    {
        if (autocorr[i] > maxVal)
        {
            maxVal = autocorr[i];
            bestLag = i;
        }
    }

    // --- STAGE 4: Octave Error Correction (Sub-harmonic check) ---
    // Look for an earlier peak that is "strong enough"
    // A strong earlier peak suggests the real pitch is higher than what we found.

    float subHarmonicThreshold = 0.90f; // 90% strength required to override global max
    int searchLimit = bestLag / 2; // Optimization: don't search too close to 0

    // We scan from minLag up to our current bestLag
    for (int i = minLag; i < bestLag; i++) {
        // Check if this index is a local peak (higher than neighbors)
        if (autocorr[i] > autocorr[i - 1] && autocorr[i] > autocorr[i + 1]) {
            // If this earlier peak is strong enough relative to the global max...
            if (autocorr[i] > (maxVal * subHarmonicThreshold)) {
                bestLag = i; // ...we prefer the shorter period (higher frequency)
                maxVal = autocorr[i]; // Update maxVal if needed for further checks
                break; // We found the fundamental, stop searching
            }
        }
    }

    return (float)m_sampleRate / bestLag;
}

float AudioAnalyzer::calculateSpectralCentroid(const std::vector<float>& spectrum)
{
    if (spectrum.empty())
        return 0.0f;

    float weightedSum = 0.0f;
    float sum = 0.0f;

    for (size_t i = 0; i < spectrum.size(); i++)
    {
        float frequency = (float)i * m_sampleRate / (2.0f * spectrum.size());
        weightedSum += frequency * spectrum[i];
        sum += spectrum[i];
    }

    return (sum > 0.0f) ? weightedSum / sum : 0.0f;
}

void AudioAnalyzer::computeFFT(const float* input, int size, std::vector<float>& output)
{
    // Simplified FFT for demonstration
    // Students can replace this with FFTW or other library
    output.resize(size / 2);

    for (int i = 0; i < size / 2; i++)
    {
        output[i] = std::abs(input[i * 2]) + std::abs(input[i * 2 + 1]);
    }
}
