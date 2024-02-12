#include <iostream>
#include <SocketLib.h>

int main()
{
	SocketLibrary::ClientSocket sock("10.1.170.20", "21");
	SYSTEMTIME st;
	GetSystemTime(&st);
	WORD start = st.wSecond;

	do { GetSystemTime(&st);} while ((st.wSecond - start) < 3);
	std::cout << (sock.Initialize() ? "Client Initialization SUCCESSFUL" : "Client failed to initialize :c") << std::endl;
}