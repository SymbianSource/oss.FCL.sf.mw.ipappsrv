/*
* Copyright (c) 2006-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    inline implementation of the plug-in API
*
*/




#include <ecom/ecom.h>
#include "natfwpluginapi.h"
#include "mnatfwpluginobserver.h"

inline CNATFWPluginApi::TNATFWPluginInitParams::
    TNATFWPluginInitParams( 
        MNATFWPluginObserver& aObserver, 
        MNcmConnectionMultiplexer& aMultiplexer,
        const TDesC8& aDomain, 
        TUint32 aIapId )
    :
    iObserver( aObserver ),
    iMultiplexer( aMultiplexer ),
    iDomain( aDomain ),
    iIapId( aIapId )
    {
    }


// -----------------------------------------------------------------------------
// CNATFWPluginApi::NewL
// -----------------------------------------------------------------------------
//
inline CNATFWPluginApi* CNATFWPluginApi::NewL( 
        TUid aImplementationUid,
        TNATFWPluginInitParams& aInitParams )
    {
    CNATFWPluginApi* ptr = reinterpret_cast<CNATFWPluginApi*>(  
                            REComSession::CreateImplementationL(
                            aImplementationUid,
                            _FOFF( CNATFWPluginApi, iDtor_ID_Key ),
                            static_cast<TAny*>(&aInitParams) ) );
    
    return ptr;
    }


// -----------------------------------------------------------------------------
// CNATFWPluginApi::NewL
// -----------------------------------------------------------------------------
//
inline CNATFWPluginApi* CNATFWPluginApi::NewL( 
        const TDesC8& aPluginId,
        TNATFWPluginInitParams& aInitParams )
    {
    TEComResolverParams default_type;
    default_type.SetDataType(aPluginId);
    
    CNATFWPluginApi* ptr = reinterpret_cast<CNATFWPluginApi*>(  
                            REComSession::CreateImplementationL(
                            KInterfaceUid,
                            _FOFF( CNATFWPluginApi, iDtor_ID_Key ),
                            static_cast<TAny*>(&aInitParams),
                            default_type ) );
    
    return ptr;
    }


// -----------------------------------------------------------------------------
// CNATFWPluginApi::ListImplementationsL
// -----------------------------------------------------------------------------
//        
inline void CNATFWPluginApi::ListAllImplementationsL( 
    RImplInfoPtrArray& aImplInfoArray )
    {
    REComSession::ListImplementationsL( KInterfaceUid, aImplInfoArray );
    }


inline CNATFWPluginApi::CNATFWPluginApi()
    {
    }


inline CNATFWPluginApi::~CNATFWPluginApi()
    {
    // Destroy any instance variables and then
    // inform the framework that this specific 
    // instance of the interface has been destroyed.
    REComSession::DestroyedImplementation( iDtor_ID_Key );
    }


// -----------------------------------------------------------------------------
// CNATFWPluginApi::FetchCandidateL
// -----------------------------------------------------------------------------
//  
inline void CNATFWPluginApi::FetchCandidateL( 
        TUint /*aStreamId*/, TUint /*aRtoValue*/, TUint /*aAddrFamily*/ )
    {
    User::Leave( KErrNotSupported );
    }


// -----------------------------------------------------------------------------
// CNATFWPluginApi::FetchCandidateL
// -----------------------------------------------------------------------------
//  
inline void CNATFWPluginApi::FetchCandidateL( TUint /*aStreamId*/, 
        TUint /*aRtoValue*/, const TInetAddr& /*aBaseAddr*/ )
    {
    User::Leave( KErrNotSupported );
    }

    
// -----------------------------------------------------------------------------
// CNATFWPluginApi::FetchCandidatesL
// -----------------------------------------------------------------------------
//  
inline void CNATFWPluginApi::FetchCandidatesL( 
        TUint /*aStreamCollectionId*/,
        TUint /*aStreamId*/, TUint /*aComponentId*/, TUint /*aAddrFamily*/ )
    {
    User::Leave( KErrNotSupported );
    }


// -----------------------------------------------------------------------------
// CNATFWPluginApi::SetRoleL
// -----------------------------------------------------------------------------
//  
inline void CNATFWPluginApi::SetRoleL( TNATFWIceRole /*aRole*/ )
    {
    User::Leave( KErrNotSupported );
    }


// -----------------------------------------------------------------------------
// CNATFWPluginApi::SetCredentialsL
// -----------------------------------------------------------------------------
//  
inline void CNATFWPluginApi::SetCredentialsL(
        const CNATFWCandidate& /*aCandidate*/,
        const CNATFWCredentials& /*aCredentials*/ )
    {
    User::Leave( KErrNotSupported );
    }


// -----------------------------------------------------------------------------
// CNATFWPluginApi::PerformConnectivityChecksL
// -----------------------------------------------------------------------------
//  
inline void CNATFWPluginApi::PerformConnectivityChecksL( 
        RPointerArray<CNATFWCandidate>& /*aRemoteCandidates*/ )
    {
    User::Leave( KErrNotSupported );
    }


// -----------------------------------------------------------------------------
// CNATFWPluginApi::UpdateIceProcessingL
// -----------------------------------------------------------------------------
//
inline void CNATFWPluginApi::UpdateIceProcessingL( 
        RPointerArray<CNATFWCandidatePair>& /*aPeerSelectedPairs*/ )
    {
    User::Leave( KErrNotSupported );
    }


// -----------------------------------------------------------------------------
// CNATFWPluginApi::UpdateIceProcessingL
// -----------------------------------------------------------------------------
//
inline void CNATFWPluginApi::UpdateIceProcessingL( 
        RPointerArray<CNATFWCandidate>& /*aRemoteCands*/ )
    {
    User::Leave( KErrNotSupported );
    }
