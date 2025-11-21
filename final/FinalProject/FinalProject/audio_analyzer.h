#ifndef AUDIO_ANALYZER_H
#define AUDIO_ANALYZER_H

#include <vector>
#include <functional>
#include <string>

// Audio feature data structure
struct AudioFeatures
{
    float magnitude;             // Peak amplitude (0.0 to 1.0)
    float rms;                   // Root mean square energy
    float pitch;                 // Fundamental frequency in Hz
    float spectralCentroid;      // Brightness measure in Hz
    std::vector<float> spectrum; // Frequency spectrum (magnitude)
    std::vector<float> waveform; // Time domain samples
};

// Audio source mode
enum AudioSourceMode
{
    MICROPHONE,
    AUDIO_FILE
};

// Callback function types
typedef std::function<void(const AudioFeatures&)> AudioCallback;

class AudioAnalyzer
{
public:
    // Constructor for microphone input
    AudioAnalyzer(int sampleRate = 44100, int bufferSize = 1024);

    // Constructor for WAV file input
    AudioAnalyzer(const std::string& filename, int bufferSize = 1024);

    ~AudioAnalyzer();

    // Start/stop audio capture or playback
    bool start();
    void stop();

    // Register callback functions
    void setAudioCallback(AudioCallback callback);

    // Get current features (thread-safe)
    AudioFeatures getFeatures() const;

    // Configuration
    int getSampleRate() const { return m_sampleRate; }
    int getBufferSize() const { return m_bufferSize; }
    bool isPlaying() const;
    AudioSourceMode getSourceMode() const { return m_sourceMode; }

    // File playback controls (file mode only)
    void play();                  // Resume playback
    void pause();                 // Pause playback
    void seek(float seconds);     // Seek to position in seconds
    float getDuration() const;    // Get total duration in seconds
    float getCurrentTime() const; // Get current playback time in seconds

    // Internal processing (public for callback access)
    void processAudioBuffer(const float* buffer, int frameCount);

    // File data access (for callback)
    std::vector<float> m_fileData;
    size_t m_filePosition;
    bool m_isPlaying;

private:
    int m_sampleRate;
    int m_bufferSize;
    AudioFeatures m_features;
    AudioCallback m_callback;
    void* m_stream; // PaStream pointer

    // File playback members
    AudioSourceMode m_sourceMode;
    std::string m_filename;

    // Internal processing
    float calculatePitch(const float* buffer, int frameCount);
    float calculateSpectralCentroid(const std::vector<float>& spectrum);
    void computeFFT(const float* input, int size, std::vector<float>& output);

    // WAV file loading (no external libraries)
    bool loadAudioFile(const std::string& filename);

    friend int audioCallbackInternal(const void*, void*, unsigned long,
        const struct PaStreamCallbackTimeInfo*,
        unsigned long, void*);
    friend int fileCallbackInternal(const void*, void*, unsigned long,
        const struct PaStreamCallbackTimeInfo*,
        unsigned long, void*);
};

#endif
