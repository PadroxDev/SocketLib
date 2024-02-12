#include "pch.h"
#include "ClientSocket.h"

#define DEFAULT_BUFLEN 512

namespace SocketLibrary {
	ClientSocket::ClientSocket(const char* serverIpAddr, const char* port) : ClientSocket(serverIpAddr, port, nullptr)
	{}

	ClientSocket::ClientSocket(const char* serverIpAddr, const char* port, EventListener* eListener) :
		BaseSocket(port, eListener), _serverIpAddr(serverIpAddr)
	{}

	ClientSocket::~ClientSocket()
	{}

	bool ClientSocket::CreateSocket() {
		struct addrinfo* result = NULL,
			* ptr = NULL,
			hints;


		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;

		int iResult = getaddrinfo(_serverIpAddr, _port, &hints, &result);
		if (iResult != 0) {
			std::cout << "Get address info failed: " << iResult << std::endl;
			return false;
		}

		// Attempt to connect to an address until one succeeds
		for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
			// Create a SOCKET for connecting to server
			_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
			if (_socket == INVALID_SOCKET) {
				std::cout << "Socket instantiation: " << WSAGetLastError() << std::endl;
				continue;
			}

			// Connect to server.
			iResult = connect(_socket, ptr->ai_addr, (int)ptr->ai_addrlen);
			if (iResult == SOCKET_ERROR) {
				std::cout << "Socket connection failed: " << WSAGetLastError() << std::endl;
				closesocket(_socket);
				_socket = INVALID_SOCKET;
				continue;
			}
			break;
		}

		if (_socket == INVALID_SOCKET) {
			std::cout << "No available address to connect to!" << std::endl;
			return false;
		}

		freeaddrinfo(result);
		return true;
	}

	bool ClientSocket::Initialize() {
		if (!InitializeWinsock()
			|| !CreateSocket() // Also handle connect()
			|| !CreateEventWindow(L"ClientEventWindow")
			|| !AssociateWithWindow(FD_READ | FD_CLOSE)) // Send the events here, MAY CAUSE TROUBLE with event triggers collection
		{
			Cleanup();
			return false;
		}

		return true;
	}

	bool ClientSocket::Send(const char* buffer) {
		int iResult = send(_socket, buffer, strlen(buffer), 0);
		if (iResult == SOCKET_ERROR) {
			Cleanup();
			return false;
		}
		std::cout << "Bytes Sent: " << iResult << std::endl;
		return true;
	}
}