/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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



#include <utf.h>
#include "cnatfwunsafquerydata.h"
#include "cnatfwunsafquerybase.h"
#include "cnatfwunsafquerya_aaaa.h"
#include "mnatfwunsafhostresolver.h"
#include "natfwunsaflog.h"
#include "natfwunsafserverresolverobserver.h"

// ----------------------------------------------------------------------------
// CNATFWUNSAFQueryData::CNATFWUNSAFQueryData
// ----------------------------------------------------------------------------
//
CNATFWUNSAFQueryData::CNATFWUNSAFQueryData( MNATFWUNSAFHostResolver& aResolver,
                                  RArray<TInetAddr>& aResult )
:iResolver ( aResolver ), iResultArray ( aResult )
    {
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFQueryData::ConstructL
// ----------------------------------------------------------------------------
//
void CNATFWUNSAFQueryData::ConstructL(
                                  MNATFWUNSAFServerResolverObserver* aObserver,
                                  TUint aPort,
                                  const TDesC8& aProtocol,
                                  const TDesC8& aOriginalTarget,
                                  const TDesC8& aServiceName )
    {
    SetDefaultPort(aPort);
    iProtocol = aProtocol.AllocL();
    iTarget = aOriginalTarget.AllocL();
    iOriginalTarget = aOriginalTarget.AllocL();
    iServiceName = aServiceName.AllocL();
    iObserver = aObserver;
    NATFWUNSAF_INTLOG("CNATFWUNSAFQueryData::iDefaultPort", iDefaultPort)
    NATFWUNSAF_STRLOG("CNATFWUNSAFQueryData::iProtocol", aProtocol)
    NATFWUNSAF_STRLOG("CNATFWUNSAFQueryData::iTarget", aOriginalTarget)
    NATFWUNSAF_STRLOG("CNATFWUNSAFQueryData::iServiceName", aServiceName)
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFQueryData::NewL
// ----------------------------------------------------------------------------
//
CNATFWUNSAFQueryData* CNATFWUNSAFQueryData::NewL(
                               MNATFWUNSAFServerResolverObserver* aObserver,
                               TUint aPort,
                               const TDesC8& aProtocol,
                               const TDesC8& aOriginalTarget,
                               const TDesC8& aServiceName,
                               MNATFWUNSAFHostResolver& aResolver,
                               RArray<TInetAddr>& aResult )
    {
    CNATFWUNSAFQueryData* self =
        new ( ELeave ) CNATFWUNSAFQueryData( aResolver, aResult );
    CleanupStack::PushL( self );
    self->ConstructL( aObserver, aPort,
                      aProtocol, aOriginalTarget, aServiceName );
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFQueryData::~CNATFWUNSAFQueryData
// ----------------------------------------------------------------------------
//
CNATFWUNSAFQueryData::~CNATFWUNSAFQueryData()
    {
    delete iTarget;
    delete iOriginalTarget;
    delete iProtocol;
    delete iServiceName;
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFQueryData::ResultArray
// ----------------------------------------------------------------------------
//
RArray<TInetAddr>& CNATFWUNSAFQueryData::ResultArray()
    {
    return iResultArray;
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFQueryData::Observer
// ----------------------------------------------------------------------------
//
MNATFWUNSAFServerResolverObserver* CNATFWUNSAFQueryData::Observer()
    {
    return iObserver;
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFQueryData::Port
// ----------------------------------------------------------------------------
//
TUint CNATFWUNSAFQueryData::Port() const
    {
    return iPort;
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFQueryData::DefaultPort
// ----------------------------------------------------------------------------
//
TUint CNATFWUNSAFQueryData::DefaultPort() const
    {
    return iDefaultPort;
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFQueryData::Protocol
// ----------------------------------------------------------------------------
//
const TDesC8& CNATFWUNSAFQueryData::Protocol() const
    {
    return *iProtocol;
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFQueryData::Target
// ----------------------------------------------------------------------------
//
const TDesC8& CNATFWUNSAFQueryData::Target() const
    {
    return *iTarget;
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFQueryData::OriginalTarget
// ----------------------------------------------------------------------------
//
const TDesC8& CNATFWUNSAFQueryData::OriginalTarget() const
    {
    return *iOriginalTarget;
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFQueryData::ServiceName
// ----------------------------------------------------------------------------
//
const TDesC8& CNATFWUNSAFQueryData::ServiceName() const
    {
    return *iServiceName;
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFQueryData::Resolver
// ----------------------------------------------------------------------------
//
MNATFWUNSAFHostResolver& CNATFWUNSAFQueryData::Resolver()
    {
    return iResolver;
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFQueryData::SetPort
// ----------------------------------------------------------------------------
//
void CNATFWUNSAFQueryData::SetPort( TUint aPort )
    {
    iPort = aPort;
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFQueryData::SetDefaultPort
// ----------------------------------------------------------------------------
//
void CNATFWUNSAFQueryData::SetDefaultPort( TUint aPort )
    {
    iDefaultPort = aPort;
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFQueryData::SetProtocolL
// ----------------------------------------------------------------------------
//
void CNATFWUNSAFQueryData::SetProtocolL( const TDesC8& aProtocol )
    {
    HBufC8* temp = aProtocol.AllocL();
    delete iProtocol;
    iProtocol = temp;
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFQueryData::SetTargetL
// ----------------------------------------------------------------------------
//
void CNATFWUNSAFQueryData::SetTargetL( const TDesC8& aTarget )
    {
    HBufC8* temp = aTarget.AllocL();
    delete iTarget;
    iTarget = temp;
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFQueryData::SetOriginalTargetL
// ----------------------------------------------------------------------------
//
void CNATFWUNSAFQueryData::SetOriginalTargetL( const TDesC8& aOriginalTarget )
    {
    HBufC8* temp = aOriginalTarget.AllocLC();
    delete iOriginalTarget;
    iOriginalTarget = temp;
    CleanupStack::Pop( temp );
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFQueryData::CreateResultL
// ----------------------------------------------------------------------------
//
TBool CNATFWUNSAFQueryData::CreateResultL( CNATFWUNSAFQueryA_AAAA& aQuery )
    {
    TBool result = EFalse;
    iQuery = &aQuery;
    if ( AddResultToArrayL() )
        {
        result = ETrue;
        }
    while ( iQuery->QueryNext( iResolver ) )
        {
        if ( AddResultToArrayL() )
            {
            result = ETrue;
            }
        }
    return result;
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFQueryData::AddResultToArrayL
// ----------------------------------------------------------------------------
//
TBool CNATFWUNSAFQueryData::AddResultToArrayL()
    {
    TInetAddr addr( iQuery->QueryResultBuf() );
    addr.SetPort( iPort );

    User::LeaveIfError( ResultArray().Append( addr ) );

    return ETrue;
    }
