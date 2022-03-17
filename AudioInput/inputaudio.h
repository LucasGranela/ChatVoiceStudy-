#ifndef INPUTAUDIO_H
#define INPUTAUDIO_H

#include <QObject>
#include <QtMultimedia/QAudioInput>
#include <QFile>

class InputAudio : public QObject
{
    Q_OBJECT
private:
    QAudioInput *audio;
    QFile* destinationFile;
public:
    explicit InputAudio(QObject *parent = nullptr);
    void start();

private slots:
    void audioStatusChanged(QAudio::State newState);
    void stopRecording();
};

#endif // INPUTAUDIO_H
