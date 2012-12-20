#include "common/ProcessOptions.h"
#include "ImgDupDetector/ImgDupProcess.h"
#include <b5m-manager/img_dup_detector.h>
#include <iostream>
#include <unistd.h>

using namespace sf1r;

__pid_t getPid();

int main(int argc, char* argv[])
{
    setupDefaultSignalHandlers();
    bool caughtException = false;

    try
    {
        ProcessOptions po;
        std::vector<std::string> args(argv + 1, argv + argc);

        if (po.setLogServerProcessArgs(argv[0], args))
        {
            __pid_t pid = getPid();
            LOG(INFO) << "\tLog Server Process : pid=" << pid;

            ImgDupProcess imgDupProcess;

            if (imgDupProcess.init(po.getConfigFile()))
            {
                imgDupProcess.start();
                imgDupProcess.join();
            }
            else
            {
                imgDupProcess.stop();
            }
        }
    }
    catch (const std::exception& e)
    {
        caughtException = true;
        std::cerr << e.what() << std::endl;
    }

    return caughtException ? 1 : 0;
}

__pid_t getPid()
{
    return ::getpid();
}
