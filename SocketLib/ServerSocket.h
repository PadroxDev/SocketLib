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

	public:
		ServerSocket(const char* port);
		ServerSocket(const char* port, EventListener* eListener);
		~ServerSocket();

		bool Initialize() override;
	};
}