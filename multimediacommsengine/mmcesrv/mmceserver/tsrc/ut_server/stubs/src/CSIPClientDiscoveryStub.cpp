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
* Description:  
*
*/



#include "SipClientDiscovery.h"
#include "sipclientdiscoveryobserver.h"
#include "cmcetls.h"


// -----------------------------------------------------------------------------
// CSIPClientDiscovery::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CSIPClientDiscovery* CSIPClientDiscovery::NewL (
                                    MSIPClientDiscoveryObserver& aObserver,
                                    TUid aSelf)	
    {
    CSIPClientDiscovery* self = CSIPClientDiscovery::NewLC (aObserver, aSelf);
    CleanupStack::Pop (self);
    return self;
    }

// -----------------------------------------------------------------------------
// CSIPClientDiscovery::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CSIPClientDiscovery* CSIPClientDiscovery::NewLC (
                                    MSIPClientDiscoveryObserver& aObserver,
                                    TUid aSelf)
    {
    CSIPClientDiscovery* self = new(ELeave)CSIPClientDiscovery;
    CleanupStack::PushL (self);
    self->ConstructL (aObserver, aSelf);
    return self;
    }
    
// -----------------------------------------------------------------------------
// CSIPClientDiscovery::ConstructL
// -----------------------------------------------------------------------------
//
void CSIPClientDiscovery::ConstructL (
                                MSIPClientDiscoveryObserver& aObserver,
                                TUid /*aSelf*/)
    {
    iRequestId=1;
    iObserver = &aObserver;
    }

// -----------------------------------------------------------------------------
// CSIPClientDiscovery::~CSIPClientDiscovery
// -----------------------------------------------------------------------------
//
EXPORT_C CSIPClientDiscovery::~CSIPClientDiscovery()
    {
    }

// -----------------------------------------------------------------------------
// CSIPClientDiscovery::RegisterL
// -----------------------------------------------------------------------------
//
EXPORT_C void CSIPClientDiscovery::RegisterL(TUid aChannel)
    {
    iChannel.iUid = aChannel.iUid;
    }
        
// -----------------------------------------------------------------------------
// CSIPClientDiscovery::Deregister
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CSIPClientDiscovery::Deregister(TUid /*aChannel*/)
    {
    return KErrNone;
    }
        
        
// -----------------------------------------------------------------------------
// CSIPClientDiscovery::ChannelL
// -----------------------------------------------------------------------------
//
EXPORT_C TUint32 CSIPClientDiscovery::ChannelL(RStringF /*aRequestMethod*/,
                                const TDesC8& /*aRequestUri*/,
                                const RPointerArray<CSIPHeaderBase>& /*aHeaders*/,
                                const TDesC8& /*aContent*/,
                                const CSIPContentTypeHeader* /*aContentType*/)
    {
    CMCETls* tls = CMCETls::Storage();
    if ( tls )
        {
        User::LeaveIfError( tls->LeaveError() );
        tls->SetCallback( TCallBack( CSIPClientDiscovery::ChannelResolved, this ) );
        }
     
    return iRequestId;
    }
        
// -----------------------------------------------------------------------------
// CSIPClientDiscovery::ChannelL
// -----------------------------------------------------------------------------
//
EXPORT_C TUint32 CSIPClientDiscovery::ChannelL(TUid /*aResolver*/,
                                RStringF /*aRequestMethod*/,
                                const TDesC8& /*aRequestUri*/,
                                const RPointerArray<CSIPHeaderBase>& /*aHeaders*/,
                                const TDesC8& /*aContent*/,
                                const CSIPContentTypeHeader* /*aContentType*/)
    {
    CMCETls* tls = CMCETls::Storage();
    if ( tls )
        {
        tls->SetCallback( TCallBack( CSIPClientDiscovery::ChannelResolved, this ) );
        }
    
    return iRequestId;
    }

// -----------------------------------------------------------------------------
// CSIPClientDiscovery::Cancel
// -----------------------------------------------------------------------------
//
EXPORT_C  void CSIPClientDiscovery::Cancel(TUint32 /*aRequestId*/)
    {
    CancelAll();    
    }
        
// -----------------------------------------------------------------------------
// CSIPClientDiscovery::CancelAll
// -----------------------------------------------------------------------------
//
EXPORT_C void CSIPClientDiscovery::CancelAll()
    {
    }


// -----------------------------------------------------------------------------
// CSIPClientDiscovery::ChannelResolved
// -----------------------------------------------------------------------------
//
TInt CSIPClientDiscovery::ChannelResolved( TAny* aThisInstance )
    {
    CSIPClientDiscovery* thisPtr = static_cast<CSIPClientDiscovery*>( aThisInstance );
    
    thisPtr->iObserver->ChannelResolvedL( thisPtr->iChannel, 
                                         thisPtr->iRequestId );
    thisPtr->iRequestId++;
    
    return KErrNone;
    
    }

//  End of File  
