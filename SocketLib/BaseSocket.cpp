#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <stdlib.h>
#include <ws2tcpip.h>

#include "pch.h"
#include "BaseSocket.h"

const LPCWSTR className = L"EventWindowClass";

namespace SocketLibrary {
	BaseSocket::BaseSocket(char* port) : _socket(INVALID_SOCKET), _window(NULL), _port(port)
	{}

	BaseSocket::~BaseSocket() {
		Cleanup();
	}

	bool BaseSocket::Initialize() {

	}

	bool BaseSocket::InitializeWinsock() {
		WSADATA wsaData;
		int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0) {
			std::cout << "WSAStartup failed: " << WSAGetLastError() << std::endl;
			return false;
		}
		return true;
	}

	bool BaseSocket::CreateEventWindow() {

		WNDCLASS windowClass = { 0 };
		windowClass.lpfnWndProc = WindowProc;
		windowClass.hInstance = GetModuleHandle(NULL);
		windowClass.lpszClassName = className;

		if (windowClass.hInstance == NULL) {
			std::cout << "Getting module handle failed with error : " << GetLastError() << std::endl;
			return false;
		}

		if (!RegisterClass(&windowClass)) {
			std::cout << "Registering class failed with error: " << GetLastError() << std::endl;
			return false;
		}

		_window = CreateWindowEx(0, className, NULL, 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, windowClass.hInstance, NULL);
		if (_window == NULL) {
			std::cout << "Creating window failed with error: " << GetLastError() << ::std::endl;
			return false;
		}

		return true;
	}

	bool BaseSocket::AssociateWithWindow(LONG events) {
		// Add this class instance as serialized data in the window
		SetWindowLongPtr(_window, GWLP_USERDATA, (LONG_PTR)this);

		int iResult = WSAAsyncSelect(_socket, _window, WM_USER + 1, events);
		if (iResult == SOCKET_ERROR) {
			std::cout << "WSAAsyncSelect failed with error: " <<  WSAGetLastError() << std::endl;
			return false;
		}
		return true;
	}

	LRESULT CALLBACK BaseSocket::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		BaseSocket* socketHandler = (BaseSocket*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		
		switch (uMsg) {
		case WM_USER + 1:
		{
			int fdEvent = WSAGETSELECTEVENT(lParam);
			SOCKET sender = wParam;

			socketHandler->EventDispatcher(fdEvent, sender);
		}
		default:
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
		
		return 0;
	}

	void BaseSocket::Cleanup() {
		if (_socket) closesocket(_socket);
		WSACleanup();
	}
}