#include "include/Utils.h"
#include "include/UI/UI.h"
#include <iostream>
#include "include/Exceptions.h"
#include "include/SocketConnection.h"
#include "include/NotificationConnection.h"

using namespace std;

int main(int argc, char *argv[])
{
    print("Connecting to server...");
    try {
        SocketConnection* server = SocketConnection::getInstance();
        NotificationConnection* notificationConnection = NotificationConnection::getInstance();
    } catch (ConnectionException) {
        std::cerr << "We ran into an error connecting to the server, please try again." << std::endl;
        GetInputString("");
        return 0;
    }

    ClearScreen();
    int choice;
    do
    {
        DisplayMainMenu();
        choice = GetInputInt("What would you like to do? ", 3);
        switch(choice) {
            case 1:
                try{
                    DisplayLoginMenu();
                }catch (const AuthenticationException& e){
                    ClearScreen();
                    printError(e.what());
                }
                break;
            case 2:
                ClearScreen();
                DisplayRegisterMenu();
                break;
            case 3:
                ClearScreen();
                print("Goodbye!");
                choice = 3;
                break;
            default:
                break;
        }
    } while(choice!=3);
    return 0;
}