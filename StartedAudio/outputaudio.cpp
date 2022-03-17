#include "outputaudio.h"
#include <QApplication>
#include <iostream>

OutputAudio::OutputAudio(QObject* parent) : QObject(parent){
    QAudioFormat format;

    QString path = qApp->applicationDirPath() + "/teste.raw";

    sourceFile.setFileName(path);

    format.setSampleRate(8000);
    format.setChannelCount(1);
    format.setSampleSize(8);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    if (!info.isFormatSupported(format)) {
        std::cout << "Raw audio format not supported by backend, cannot play audio.\n";
        return;
    }

    audio = new QAudioOutput(format);

    connect(audio, SIGNAL(stateChanged(QAudio::State)), this, SLOT(audioStatusChanged(QAudio::State)));

}

void OutputAudio::start() {
    audio->start(&sourceFile);
}



void OutputAudio::audioStatusChanged(QAudio::State newState) {
    switch (newState) {
        case QAudio::IdleState:
            // Finished playing (no more data)
            audio->stop();
            sourceFile.close();
            delete audio;
            std::cout << "Audio ended\n";
        break;

        case QAudio::StoppedState:
            // Stopped for other reasons
            if (audio->error() != QAudio::NoError) {
            // Error handling
            }
        break;

        default:
            // ... other cases as appropriate
        break;
    }
}

