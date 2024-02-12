#include "pch.h"
#include "BaseSocket.h"

namespace SocketLibrary {
	BaseSocket::BaseSocket(const char* port) : BaseSocket(port, nullptr)
	{}

	BaseSocket::BaseSocket(const char* port, EventListener* eListener) : _socket(INVALID_SOCKET), _window(NULL),
		_port(port), _eListener(eListener) {
	}

	BaseSocket::~BaseSocket() {
		Cleanup();
	}

	bool BaseSocket::Initialize() {
		return false;
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

	bool BaseSocket::CreateEventWindow(const LPCWSTR className) {
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

	void BaseSocket::EventDispatcher(int fdEvent, SOCKET sender) {
		if (_eListener == nullptr) {
			std::cout << "You must assign an EventListener if you want to be able to read received event !" << std::endl;
			return;
		}

		switch (fdEvent) {
		case FD_ACCEPT:
			_eListener->HandleAccept(sender);
			break;
		case FD_READ:
			_eListener->HandleRead(sender);
			break;
		case FD_CLOSE:
			_eListener->HandleClose(sender);
			break;
		default:
			std::cout << "Event not found: " << fdEvent << std::endl;
			break;
		}
	}

	LRESULT CALLBACK BaseSocket::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		BaseSocket* socketHandler = (BaseSocket*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

		//std::cout << socketHandler->_port << std::endl;

		switch (uMsg) {
		case WM_USER + 1:
		{
			int fdEvent = WSAGETSELECTEVENT(lParam);
			SOCKET sender = wParam;

			socketHandler->EventDispatcher(fdEvent, sender);
			break;
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