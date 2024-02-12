#include <iostream>
#include <SocketLib.h>

int main()
{
	SocketLibrary::ClientSocket sock("10.1.144.30", "21");
	SYSTEMTIME st;
	GetSystemTime(&st);
	WORD start = st.wSecond;

	std::cout << (sock.Initialize() ? "Client Initialization SUCCESSFUL" : "Client failed to initialize :c") << std::endl;
	sock.Send("test");

	MSG msg;
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}