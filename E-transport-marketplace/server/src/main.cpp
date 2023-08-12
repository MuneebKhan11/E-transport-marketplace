#include <iostream>
#include "include/ServerManager.h"
#include "include/NotificationManager.h"
#include "include/Exceptions.h"
#include "include/Utils.h"
#include "thread"
#include "include/Database.h"

using namespace std;

int main() {
    try {
        setupDatabase();
        NotificationManager *notificationManager = NotificationManager::getInstance(8081);
        std::thread notificationThread(&NotificationManager::start, notificationManager);
        notificationThread.detach();
        ServerManager *serverManager = ServerManager::getInstance(8080);
        serverManager->start();
    } catch (const ConnectionException &e) {
        cout << "Caught exception: " << e.what() << endl;
    }
    return 0;
}






