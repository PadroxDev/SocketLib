#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "pch.h"
#include "ClientSocket.h"

#define DEFAULT_BUFLEN 512

namespace SocketLibrary {
	ClientSocket::ClientSocket(char* serverIpAddr, char* port) :
		BaseSocket(port), _serverIpAddr(serverIpAddr)
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
				closesocket(_socket);
				_socket = INVALID_SOCKET;
				continue;
			}
			break;
		}

		freeaddrinfo(result);
		return true;
	}

	void ClientSocket::EventDispatcher(int fdEvent, SOCKET sender) {
		switch (fdEvent) {
		case FD_READ:
			HandleRead(sender);
			break;
		case FD_CLOSE:
			HandleClose(sender);
			break;
		default:
			std::cout << "Event not found: " << fdEvent << std::endl;
			break;
		}
	}

	void ClientSocket::HandleRead(SOCKET sender) {
		char buffer[DEFAULT_BUFLEN];
		int bytesRead = recv(sender, buffer, DEFAULT_BUFLEN, 0);
		if (bytesRead > 0) {
			std::cout << "Bytes received: " << bytesRead << std::endl;
			std::cout << "Data received: " << buffer << std::endl;
		}
	}

	void ClientSocket::HandleClose(SOCKET sender) {
		std::cout << "Connection closed" << std::endl;
		closesocket(sender);
		PostQuitMessage(0);
	}

	bool ClientSocket::Initialize() {
		if (!InitializeWinsock()
			&& !CreateSocket() // Also connects
			&& !CreateEventWindow()
			&& !AssociateWithWindow(FD_READ | FD_CLOSE)) // Send the events here, MAY CAUSE TROUBLE with event triggers collection
		{
			Cleanup();
			return false;
		}

		return true;
	}

	bool ClientSocket::Send(SOCKET to, const char* buffer) {
		int iResult = send(to, buffer, strlen(buffer), 0);
		if (iResult == SOCKET_ERROR) {
			std::cout << "Sending buffer (" << buffer << ") failed with error: " << WSAGetLastError() << std::endl;
			Cleanup();
			return false;
		}
		std::cout << "Bytes Sent: " << iResult << std::endl;
		return true;
	}
}