#include "inputaudio.h"
#include <QTimer>
#include <iostream>

InputAudio::InputAudio(QObject *parent) : QObject(parent) {
    destinationFile = new QFile("test.raw");

    destinationFile->open( QIODevice::WriteOnly | QIODevice::Truncate );
    QAudioFormat format;
    // Set up the desired format, for example:
    format.setSampleRate(88200);
    format.setChannelCount(1);
    format.setSampleSize(8);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
    if (!info.isFormatSupported(format)) {
        std::cout << "Default format not supported, trying to use the nearest.";
        format = info.nearestFormat(format);
    }

    audio = new QAudioInput(format, this);
    connect(audio, SIGNAL(stateChanged(QAudio::State)), this, SLOT(audioStatusChanged(QAudio::State)));
}

void InputAudio::start() {
    QTimer::singleShot(5000, this, SLOT(stopRecording()));
    audio->start(destinationFile);
    // Records audio for 3000ms
}

void InputAudio::audioStatusChanged(QAudio::State newState) {
    switch (newState) {
    case QAudio::StoppedState:
        if (audio->error() != QAudio::NoError) {
            // Error handling
        } else {
            // Finished recording
            std::cout << "audio ended";
        }
        break;

    case QAudio::ActiveState:
        // Started recording - read from IO device
        break;

    default:
        // ... other cases as appropriate
        break;
    }
}

void InputAudio::stopRecording() {
    audio->stop();
    destinationFile->close();
    std::cout << "audio ended";
    delete audio;
    delete destinationFile;
}
