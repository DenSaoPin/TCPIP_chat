#include "mainloopworker.h"
#include <ChatClientAPI.h>

MainLoopWorker::MainLoopWorker()
{

}

void MainLoopWorker::doWork()
{
    ClientMainLoop();
}
