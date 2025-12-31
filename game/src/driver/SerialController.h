#pragma once

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

class SerialController
{
   private:
    struct termios;

   public:
    SerialController();
};
