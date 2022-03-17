#include <QCoreApplication>
#include "outputaudio.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    OutputAudio *audio = new OutputAudio();

    audio->start();

    return a.exec();
}
