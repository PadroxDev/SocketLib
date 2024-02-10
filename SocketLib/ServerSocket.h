#pragma once
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
		void HandleWrite(SOCKET sender);
		void HandleRead(SOCKET sender);
		void HandleClose(SOCKET sender);

	public:
		ServerSocket(const char* port);
		~ServerSocket();

		bool Initialize() override;
	};
}