/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include "Core/DataStructures/RingBuffer.h"
#include "Core/Time/Clock.h"
#include "Networking/Messages.h"
#include "yojimbo/yojimbo.h"

namespace Isetta {
class NetworkingModule {
 private:
  static CustomAdapter NetworkAdapter;

  // TODO(Caleb): Look into making the ports user-definable
  const uint16_t ClientPort = 30000;
  const uint16_t ServerPort = 40000;
  const int KeyBytes = 32;
  const int MaxClients = 64;
  const int NumIterations = 100;
  const int ClientQueueSize = 256;
  const int ServerQueueSizePerClient = 256;

  Clock clock;

  yojimbo::Address clientAddress;
  yojimbo::Client* client;
  yojimbo::Address serverAddress;
  yojimbo::Server* server;
  yojimbo::ClientServerConfig networkConfig;

  uint8_t* privateKey;
  uint64_t clientId;

  RingBuffer<yojimbo::Message*> clientSendBuffer;
  RingBuffer<yojimbo::Message*>* serverSendBufferArray;

  // Constructors

  NetworkingModule() = default;
  ~NetworkingModule() = default;

  // Module functions

  void StartUp();
  void Update(float deltaTime);
  void ShutDown();

  // Functions

  void AddClientToServerMessage(yojimbo::Message* message);
  void AddServerToClientMessage(int clientIdx, yojimbo::Message* message);

  void PumpClientServerUpdate(double time);

  void SendClientToServerMessages();
  void SendServerToClientMessages(int clientIdx);
  void ProcessClientToServerMessages(int clientIdx);
  void ProcessServerToClientMessages();

  void Connect(const char* serverAddress);
  void Disconnect();

  void CreateServer(const char* address, int port);
  void CloseServer();

  // Other

  friend class ModuleManager;
};
}  // namespace Isetta