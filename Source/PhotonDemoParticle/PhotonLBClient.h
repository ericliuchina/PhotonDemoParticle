// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "demo_particle_common/LoadBalancingListener.h"
#include "GameFramework/Actor.h"
#include "PhotonLBClient.generated.h"

/**
 * 
 */
UCLASS()
class APhotonLBClient : public AActor, public BaseView
{
	GENERATED_UCLASS_BODY()
	void BeginPlay();
	void Tick(float DeltaSeconds);

public:
	// Demo BaseView overrrides (View implementation).
	virtual void info(const char* format, ...);
	virtual void warn(const char* format, ...);
	virtual void error(const char* format, ...);
//	virtual void initPlayers(void) {}

	// Some of the overrides used as events in UE blueprints direcltly with only int's replaced with int32's since int unsupported by UFUNCTION.	
	UFUNCTION(BlueprintImplementableEvent, Category = "Demo")
	void removePlayer(int32 playerNr);
	UFUNCTION(BlueprintImplementableEvent, Category = "Demo")
	void changePlayerPos(int32 playerNr, int32 x, int32 y);
	UFUNCTION(BlueprintImplementableEvent, Category = "Demo")
	void setupScene(int32 gridSize);

	// Other overrides wrap UE events for convestion parameters to FString or FColor.
	virtual void changePlayerColor(int32 playerNr, int32 col)
	{
		changePlayerColor(playerNr, FColor(uint8(col >> 16), uint8(col >> 8), uint8(col), uint8(col >> 24)));
	}
	UFUNCTION(BlueprintImplementableEvent, Category = "Demo")
	void changePlayerColor(int32 playerNr, FColor col);

	virtual void updateState(int state, const char* stateStr, const char* joinedRoomName)
	{
		updateState(state, FString(stateStr), FString(joinedRoomName));
	}
	UFUNCTION(BlueprintImplementableEvent, Category = "Demo")
	void updateState(int32 state, const FString& stateStr, const FString& joinedRoomName);

	virtual void addPlayer(int playerNr, const char* playerName, bool local)
	{ 
		addPlayer(playerNr, FString(playerName), local);
	}
	UFUNCTION(BlueprintImplementableEvent, Category = "Demo")
	void addPlayer(int32 playerNr, const FString& playerName, bool local);

	virtual void updateRoomList(const char* roomNames[], unsigned int size)
	{
		TArray<FString> a;
		for(unsigned int i=0; i<size; ++i)
			a.Push(FString(roomNames[i]));
		updateRoomList(a);
	}
	UFUNCTION(BlueprintImplementableEvent, Category = "Demo")
	void updateRoomList(const TArray<FString>& roomNames);

	// end of Demo BaseView overrrides 

	// Exposing demo properties to blueprint 
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Demo)
//	bool gridSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Demo")
	FString serverAddress;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Demo")
	FString AppID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Demo")
	FString appVersion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Demo")
	bool automove;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Demo")
	bool useGroups;

	// Exposing demo methods to blueprint 
	UFUNCTION(BlueprintCallable, Category = "Demo")
	void RandomizeColor();
	UFUNCTION(BlueprintCallable, Category = "Demo")
	void NextGridSize();
	UFUNCTION(BlueprintCallable, Category = "Demo")
	void NewGame();
	UFUNCTION(BlueprintCallable, Category = "Demo")
	void Leave();
	UFUNCTION(BlueprintCallable, Category = "Demo")
	void JoinRoom(FString gameId);
	UFUNCTION(BlueprintCallable, Category = "Demo")
	void SetLocalPlayerPos(int32 x, int32 y);

	// Util
	UFUNCTION(BlueprintCallable, Category = "Demo")
	bool IsInLobby();
	UFUNCTION(BlueprintCallable, Category = "Demo")
	bool IsInRoom();

private:
	ExitGames::LoadBalancing::Client* client;
	LoadBalancingListener* listener;
};