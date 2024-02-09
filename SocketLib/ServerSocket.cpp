#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "pch.h"
#include "ServerSocket.h"

#define DEFAULT_BUFLEN 512

namespace SocketLibrary {
	ServerSocket::ServerSocket(char* port) : BaseSocket(port)
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

	void ServerSocket::EventDispatcher(int fdEvent, SOCKET sender) {
		switch (fdEvent) {
		case FD_ACCEPT:
			HandleAccept(sender);
			break;
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

	void ServerSocket::HandleAccept(SOCKET sender) {
		SOCKET incomingSocket;
		incomingSocket = accept(sender, NULL, NULL);
		if (incomingSocket == INVALID_SOCKET) {
			Cleanup();
			std::cout << "Error accepting an incomming socket !" << std::endl;
			return;
		}
		clients.push_back(incomingSocket);
		WSAAsyncSelect(incomingSocket, _window, WM_USER + 1, FD_READ | FD_CLOSE);
	}

	void ServerSocket::HandleRead(SOCKET sender) {
		char recvbuf[DEFAULT_BUFLEN];
		int bytesRead = recv(sender, recvbuf, DEFAULT_BUFLEN, 0);
		if (bytesRead > 0) {
			printf("%.*s\n", bytesRead, recvbuf);
			
			std::string msg(recvbuf, bytesRead);
			std::cout << msg << std::endl;

			// Echo back the received data
			send(sender, recvbuf, bytesRead, 0);
		}
	}

	void ServerSocket::HandleClose(SOCKET sender) {
		std::cout << "Connection shutdown" << std::endl;
		for (int i = clients.size() - 1; i >= 0; i--)
		{
			if (clients[i] == sender) {
				clients.erase(clients.begin() + i);
				break;
			}
		}
		closesocket(sender);
	}

	bool ServerSocket::Initialize() {
		if (!InitializeWinsock()
			&& !CreateSocket()
			&& !StartListening()
			&& !CreateEventWindow()
			&& !AssociateWithWindow(FD_ACCEPT)) // Send the events here, MAY CAUSE TROUBLE with event triggers collection
		{
			Cleanup();
			return false;
		}

		return true;
	}
}