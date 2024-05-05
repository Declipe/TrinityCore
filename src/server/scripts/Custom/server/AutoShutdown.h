
#ifndef _AUTO_SHUTDOWN_H_
#define _AUTO_SHUTDOWN_H_

#include "Common.h"

class AutoShutdown
{
public:
    static AutoShutdown* instance();

    void Init();
    void OnUpdate(uint32 diff);

private:
    bool isEnable = false;
};

#define Ark AutoShutdown::instance()

#endif
