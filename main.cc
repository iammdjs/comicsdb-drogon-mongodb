#include <drogon/drogon.h>
#include <mongocxx/instance.hpp>
#include <mongocxx/pool.hpp>
#include "controllers/comicService_v1_Service.h"

int main()
{
    try
    {
        unsigned short int portNum = 8000;
        unsigned int numThreads = std::thread::hardware_concurrency();
        // unsigned int numThreads = 0;

        // Set HTTP listener address and port
        drogon::app().setLogPath("./")
                    .addListener("0.0.0.0", portNum)
                    .setThreadNum(numThreads);
        
        std::cout << "Starting on port " << portNum << " with " << numThreads << " threads.\n";
        // Load config file
        drogon::app().loadConfigFile("../config.json");
        // drogon::app().loadConfigFile("../config.yaml");
        // Run HTTP framework,the method will block in the internal event loop
        drogon::app().run();
    }
    catch (const std::exception &bang)
    {
        std::cerr << bang.what() << '\n';
        return 1;
    }
    catch (...)
    {
        return 1;
    }

    return 0;
}
