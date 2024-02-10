#pragma once

#include "BaseSocket.h"

namespace SocketLibrary {
	class ClientSocket : public BaseSocket
	{
	private:
		const char* _serverIpAddr;

		bool CreateSocket() override;
		
		void EventDispatcher(int fdEvent, SOCKET sender) override;
		void HandleConnect(SOCKET sender);
		void HandleWrite(SOCKET sender);
		void HandleRead(SOCKET sender);
		void HandleClose(SOCKET sender);

	public:
		ClientSocket(const char* serverIpAddr, const char* port);
		~ClientSocket();

		bool Initialize() override;
		bool Send(const char* buffer);
	};
}