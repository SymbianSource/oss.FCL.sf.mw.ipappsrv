/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  
*
*/

#ifndef NSPTESTCONSOLESESSION_H
#define NSPTESTCONSOLESESSION_H

#include "nspsessionobserver.h"

class CRepository;
class CSdpDocument;
class CNSPPlugin;
class CTestConsoleStream;
class MAsyncService;


class CNSPTestConsoleSession : public CBase, public MNSPSessionObserver
	{
public: // Enums

	enum TSessionState
		{
		EInitializing = 0,
		EIdle,
		ECreateOffer,
		ECreateAnswer,
		EDecodeOffer,
		EDecodeAnswer,
		EUpdating
		};

public: // Methods
	static CNSPTestConsoleSession* NewL( CNSPPlugin& aApi, CRepository& aCenrep,
			 MAsyncService& aAsync );
	static CNSPTestConsoleSession* NewLC( CNSPPlugin& aApi, CRepository& aCenrep,
			 MAsyncService& aAsync );
	virtual ~CNSPTestConsoleSession();
	
	// From MNSPSessionObserver
    virtual void Initialized( TUint aSessionId );
    virtual void OfferReady( TUint aSessionId, CSdpDocument* aOffer );
	virtual void AnswerReady( TUint aSessionId, CSdpDocument* aAnswer );
    virtual void UpdateSdp( TUint aSessionId, CSdpDocument* aOffer );
    virtual void ErrorOccurred( TUint aSessionId, TInt aError );
    virtual void IcmpErrorOccurred( TUint aSessionId, TInt aError );
    
    // New methods
    TInt CreateOfferL( CTestConsoleStream& aOutStream );
    TInt ResolveL( CTestConsoleStream& aInStream, CTestConsoleStream& aOutStream );
    TInt DecodeAnswerL( CTestConsoleStream& aInStream );
    void UpdateL( CTestConsoleStream& aInStream );
    TSessionState State() const;
    void StoreOutStream( CTestConsoleStream* aOutStream );
    void StoreInStream( CTestConsoleStream* aInStream );

protected: // Methods
	CNSPTestConsoleSession( CNSPPlugin& aApi, MAsyncService& aAsync );
	void ConstructL( CRepository& aCenrep );
	
	void StoreOffer( CSdpDocument* aOffer );
	void StoreAnswer( CSdpDocument* aAnswer );

private: // data	
	TSessionState iState;
	TUint iSessionId;
	CNSPPlugin& iApi;
	MAsyncService& iAsync;
	CSdpDocument* iOffer; // own.
	CSdpDocument* iAnswer; // own.
	CTestConsoleStream* iOutStream; // own.
	CTestConsoleStream* iInStream; // own.
	};

#endif // NSPTESTCONSOLESESSION_H
