#include <iostream>
#include <SocketLib.h>

int main()
{
	SocketLibrary::ServerSocket sock("21");
	std::cout << (sock.Initialize() ? "Server Initialization SUCCESSFUL" : "Client failed to initialize :c") << std::endl;

	MSG msg;
	while ( GetMessage(&msg, 0, 0, 0) )
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}