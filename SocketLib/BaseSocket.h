#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>

namespace SocketLibrary {
	class BaseSocket
	{
	protected:
		SOCKET _socket;
		HWND _window;
		char* _port;

		BaseSocket(char* port);
		~BaseSocket();

		bool InitializeWinsock();
		virtual bool CreateSocket() {};
		
		bool CreateEventWindow();
		bool AssociateWithWindow(LONG events);

		virtual void EventDispatcher(int fdEvent, SOCKET sender) {};
		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		void Cleanup();

	public:
		virtual bool Initialize() = 0;
	};
}