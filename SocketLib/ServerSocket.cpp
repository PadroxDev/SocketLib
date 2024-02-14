#define NOMINMAX
#include "pch.h"
#include "ServerSocket.h"

#define DEFAULT_BUFLEN 512

namespace SocketLibrary {
	ServerSocket::ServerSocket(const char* port) : ServerSocket(port, nullptr)
	{}

	ServerSocket::ServerSocket(const char* port, EventListener* eListener) : BaseSocket(port, eListener)
	{}

	ServerSocket::~ServerSocket()
	{}

	bool ServerSocket::CreateSocket() {
		struct addrinfo* result = NULL,
			* ptr = NULL,
			hints;

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;

		int iResult = getaddrinfo(nullptr, _port, &hints, &result);
		if (iResult != 0) {
			std::cout << "Get address info failed: " << iResult << std::endl;
			return false;
		}

		_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if (_socket == INVALID_SOCKET) {
			std::cout << "Socket instantiation: " << WSAGetLastError() << std::endl;
			freeaddrinfo(result);
			Cleanup();
			return false;
		}

		iResult = bind(_socket, result->ai_addr, (int)result->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			std::cout << "Bind failed: " << WSAGetLastError() << std::endl;
			freeaddrinfo(result);
			Cleanup();
			return false;
		}

		freeaddrinfo(result);
		return true;
	}

	bool ServerSocket::StartListening() {
		if (listen(_socket, SOMAXCONN) == SOCKET_ERROR) {
			printf("listen failed: %d\n", WSAGetLastError());
			Cleanup();
			return false;
		}
		return true;
	}

	bool ServerSocket::Initialize() {
		if (!InitializeWinsock()
			|| !CreateSocket()
			|| !StartListening()
			|| !CreateEventWindow(L"ServerEventWindow")
			|| !AssociateWithWindow(FD_ACCEPT | FD_READ | FD_CLOSE)) // Send the events here, MAY CAUSE TROUBLE with event triggers collection
		{
			Cleanup();
			return false;
		}

		return true;
	}
}