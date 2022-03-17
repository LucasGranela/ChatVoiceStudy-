#include <QCoreApplication>
#include "inputaudio.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    InputAudio *audio = new InputAudio();

    audio->start();

    return a.exec();
}
