#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "BaseSocket.h"

namespace SocketLibrary {
	class ClientSocket : public BaseSocket
	{
	private:
		char* _serverIpAddr;

		bool CreateSocket() override;
		
		void EventDispatcher(int fdEvent, SOCKET sender) override;
		void HandleRead(SOCKET sender);
		void HandleClose(SOCKET sender);

	public:
		ClientSocket(char* serverIpAddr, char* port);
		~ClientSocket();

		bool Initialize() override;
		bool Send(SOCKET to, const char* buffer);
	};
}