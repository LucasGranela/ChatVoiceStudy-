#include <iostream>
#include "controlerServer/headers/controler.hh"

int main(int argc, char *argv[]){
    std::cout << "Hello World" << std::endl;
    Controler *rpgStudioServerControler = new Controler();

    delete rpgStudioServerControler;

    return 0;
}