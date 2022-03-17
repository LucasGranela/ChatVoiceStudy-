#ifndef OUTPUTAUDIO_H
#define OUTPUTAUDIO_H

#include <QObject>
#include <QtMultimedia/QAudioOutput>
#include <QFile>

class OutputAudio : QObject{
    Q_OBJECT
    Q_DISABLE_COPY(OutputAudio)
private:
    QAudioOutput *audio;
    QFile* sourceFile;
public:
    explicit OutputAudio(QObject *parent = nullptr);
    void start();

private slots:
    void audioStatusChanged(QAudio::State newState);
};

#endif // OUTPUTAUDIO_H
