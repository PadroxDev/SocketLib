#pragma once

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

namespace SocketLibrary {
	class BaseSocket
	{
	protected:
		SOCKET _socket;
		HWND _window;
		const char* _port;

		BaseSocket(const char* port);
		~BaseSocket();

		bool InitializeWinsock();
		virtual bool CreateSocket() { return false; };
		
		bool CreateEventWindow(const LPCWSTR className);
		bool AssociateWithWindow(LONG events);

		virtual void EventDispatcher(int fdEvent, SOCKET sender) {};
		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		void Cleanup();

	public:
		virtual bool Initialize() = 0;
	};
}