#include <iostream>
#include <SocketLib.h>

int main()
{
	SocketLibrary::ClientSocket sock("192.168.1.18", "21");
	SYSTEMTIME st;
	GetSystemTime(&st);
	WORD start = st.wSecond;

	do { GetSystemTime(&st);} while ((st.wSecond - start) < 3);

	std::cout << "Initializing Client" << std::endl;
	std::cout << (sock.Initialize() ? "Client Initialization SUCCESSFUL" : "Client failed to initialize :c") << std::endl;
}