#include <iostream>
#include <SocketLib.h>

using namespace std;

const char* SERVER_IP = "192.168.1.18";

int main()
{
	SYSTEMTIME st;
	GetSystemTime(&st);
	WORD startTime = st.wSecond;

	cout << "Start..." << endl;

	SocketLibrary::ServerSocket server("21");
	cout << (server.Initialize() ? "Initialized Server SUCCESSFULLY" : "Oh shit :'(") << endl;
	SocketLibrary::ClientSocket client(SERVER_IP, "21");
	cout << (client.Initialize() ? "Initialized Client SUCCESSFULLY" : "Oh mince ;'(") << endl;

	do { GetSystemTime(&st); } while ((st.wSecond - startTime) < 5);

	const char* msg = "Coucou tu veux pas marcher stp :)";
	client.Send(msg);

	do { GetSystemTime(&st); } while ((st.wSecond - startTime) < 5);
}