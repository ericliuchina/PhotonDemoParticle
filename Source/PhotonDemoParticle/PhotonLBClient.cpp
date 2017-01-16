// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "PhotonDemoParticle.h"
#include "PhotonLBClient.h"

void log(FString const& s, FColor color)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, color, s);
}

APhotonLBClient::APhotonLBClient(const class FObjectInitializer& PCIP)
	: Super(PCIP),
	serverAddress("ns.exitgames.com"),
	AppID("<no-app-id>"),
	appVersion("1.0"),
	client(NULL),
	listener(NULL)
{
	PrimaryActorTick.bCanEverTick = true;
}

void APhotonLBClient::BeginPlay(void)
{
	Super::BeginPlay();
	srand(GETTIMEMS());
	listener = new LoadBalancingListener(this);
	client = new ExitGames::LoadBalancing::Client(*listener, *AppID, *appVersion);
	listener->setLBC(client);

	automove = listener->getAutomove();
	useGroups = listener->getUseGroups();

	ExitGames::Common::Hashtable ht;
	ht.put(1, 1);
	ht.put(2, 2);
	info("Connecting...");
	info("appID is set to %ls", *AppID);
	client->connect(ExitGames::LoadBalancing::AuthenticationValues(), ExitGames::Common::JString(L"UR") + GETTIMEMS(), *serverAddress);

}

void APhotonLBClient::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(client)
	{
		client->service();

		// should be called on props change only but how do one listen to those changes?
		listener->setAutomove(automove);
		listener->setUseGroups(useGroups);

		listener->service();
	}
}

void APhotonLBClient::info(const char* format, ...)
{
	char buf[1024];
	va_list argptr;
	va_start(argptr, format);
	vsprintf(buf, format, argptr);
	va_end(argptr);

	log(FString("INFO: ") + buf, FColor::Green);
}

void APhotonLBClient::warn(const char* format, ...)
{
	char buf[1024];
	va_list argptr;
	va_start(argptr, format);
	vsprintf(buf, format, argptr);
	va_end(argptr);

	log(FString("WARN: ") + buf, FColor::Yellow);
}

void APhotonLBClient::error(const char* format, ...)
{
	char buf[1024];
	va_list argptr;
	va_start(argptr, format);
	vsprintf(buf, format, argptr);
	va_end(argptr);

	log(FString("ERROR: ") + buf, FColor::Red);
}

void APhotonLBClient::RandomizeColor(void)
{
	listener->changeRandomColor();
}

void APhotonLBClient::NextGridSize(void)
{
	listener->nextGridSize();
}

void APhotonLBClient::NewGame(void)
{
	listener->createRoom();
}

void APhotonLBClient::Leave(void)
{
	listener->leave();
}

void APhotonLBClient::JoinRoom(FString gameId)
{
	client->opJoinRoom(ExitGames::Common::JString(*gameId));
}

void APhotonLBClient::SetLocalPlayerPos(int32 x, int32 y)
{
	listener->setLocalPlayerPos(x, y);
}

bool APhotonLBClient::IsInLobby(void)
{
	return client->getState() == ExitGames::LoadBalancing::PeerStates::JoinedLobby || client->getState() == ExitGames::LoadBalancing::PeerStates::AuthenticatedComingFromGameserver;
}

bool APhotonLBClient::IsInRoom(void)
{
	return client->getState() == ExitGames::LoadBalancing::PeerStates::Joined;
}