#include <iostream>
#include <SocketLib.h>

int main()
{
	SocketLibrary::ServerSocket sock("21");
	std::cout << (sock.Initialize() ? "Server Initialization SUCCESSFUL" : "Client failed to initialize :c") << std::endl;
}