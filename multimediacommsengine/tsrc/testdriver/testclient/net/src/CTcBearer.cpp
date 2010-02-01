/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation
*
*/

#include "CTcBearer.h"
#include "MTcBearerFactory.h"
#include "MTcBearerManager.h"

EXPORT_C CTcBearer* CTcBearer::NewL( const MTcBearerFactory& aFactory )
	{
	CTcBearer* self = new( ELeave ) CTcBearer;

	CleanupStack::PushL( self );
	self->ConstructL( aFactory );
	CleanupStack::Pop( self );

	return self;
	}

EXPORT_C CTcBearer::~CTcBearer()
	{
	delete iManager;
	}

CTcBearer::CTcBearer()
	{
	}

void CTcBearer::ConstructL( const MTcBearerFactory& aFactory )
	{
	iManager = aFactory.CreateManagerL();
	}

EXPORT_C void CTcBearer::ConnectL( TInetAddr* aRemoteAddr )
	{
	iManager->ConnectL( aRemoteAddr );
	}

EXPORT_C void CTcBearer::Close()
	{
	iManager->Close();
	}

EXPORT_C void CTcBearer::Send( const TDesC8& aDes )
	{
	iManager->Send( aDes );
	}

EXPORT_C void CTcBearer::Receive( TDes8& aDes )
	{
	iManager->Receive( aDes );
	}

EXPORT_C void CTcBearer::ReceiveOneOrMore( TDes8& aDes )
	{
	iManager->ReceiveOneOrMore( aDes );
	}

EXPORT_C void CTcBearer::SetObserver( MTcBearerObserver* aObserver )
	{
	iManager->SetObserver( aObserver );
	}

EXPORT_C void CTcBearer::GetLocalAddressL( TDes& aDes )
	{
	iManager->GetLocalAddressL( aDes );
	}
