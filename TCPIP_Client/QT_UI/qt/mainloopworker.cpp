#include "mainloopworker.h"
#include "../../TCPIP_CLIENT_DLL/public/ChatClientAPI.h"
MainLoopWorker::MainLoopWorker()
{

}

void MainLoopWorker::doWork()
{
    ClientMainLoop();
}
