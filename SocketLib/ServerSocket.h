#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <vector>

#include "BaseSocket.h"

namespace SocketLibrary {
	class ServerSocket : public BaseSocket
	{
	private:
		std::vector<SOCKET> clients;

		bool CreateSocket() override;
		bool StartListening();

		void EventDispatcher(int fdEvent, SOCKET sender) override;
		void HandleAccept(SOCKET sender);
		void HandleRead(SOCKET sender);
		void HandleClose(SOCKET sender);

	public:
		ServerSocket(char* port);
		~ServerSocket();

		bool Initialize() override;
	};
}