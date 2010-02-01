/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation.
*
*/

#include "ttcn2gresolverplugin.h"
#include "sdpmediafield.h"

// -----------------------------------------------------------------------------
// CTTCN2GResolverPlugin::NewL
// -----------------------------------------------------------------------------
//
CTTCN2GResolverPlugin* CTTCN2GResolverPlugin::NewL()
	{
	CTTCN2GResolverPlugin* self = new( ELeave )CTTCN2GResolverPlugin;
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );
	return self;
	}

// -----------------------------------------------------------------------------
// CTTCN2GResolverPlugin::CTTCN2GResolverPlugin
// -----------------------------------------------------------------------------
//	
CTTCN2GResolverPlugin::CTTCN2GResolverPlugin()
    {
    }

// -----------------------------------------------------------------------------
// CTTCN2GResolverPlugin::ConstructL
// -----------------------------------------------------------------------------
//    
void CTTCN2GResolverPlugin::ConstructL()
    {
    User::LeaveIfError( iSimulator.Connect() );
    }

// -----------------------------------------------------------------------------
// CTTCN2GResolverPlugin::~CTTCN2GResolverPlugin
// -----------------------------------------------------------------------------
//	
CTTCN2GResolverPlugin::~CTTCN2GResolverPlugin()
    {
    iSimulator.Close();
    }

// -----------------------------------------------------------------------------
// CTTCN2GResolverPlugin::MatchAcceptContactsL
// From CSIPResolvedClient2
// -----------------------------------------------------------------------------
//
TBool CTTCN2GResolverPlugin::MatchAcceptContactsL(
    RStringF /*aMethod*/,
    const CUri8& /*aRequestUri*/,
    const RPointerArray<CSIPHeaderBase>& /*aHeaders*/,
    const TDesC8& /*aContent*/,
    const CSIPContentTypeHeader* /*aContentType*/,
    TUid& aClientUid)
    {
    return Match( 
        RSipClientSimulator::ESipAcceptContactHeaderMatch, aClientUid );
    }

// -----------------------------------------------------------------------------
// CTTCN2GResolverPlugin::MatchEventL
// From CSIPResolvedClient2
// -----------------------------------------------------------------------------
//
TBool CTTCN2GResolverPlugin::MatchEventL(
    RStringF /*aMethod*/,
    const CUri8& /*aRequestUri*/,
    const RPointerArray<CSIPHeaderBase>& /*aHeaders*/,
    const TDesC8& /*aContent*/,
    const CSIPContentTypeHeader* /*aContentType*/,
    TUid& aClientUid)
    {
    return Match( RSipClientSimulator::ESipEventHeaderMatch, aClientUid );
    }

// -----------------------------------------------------------------------------
// CTTCN2GResolverPlugin::MatchRequestL
// From CSIPResolvedClient2
// -----------------------------------------------------------------------------
//
TBool CTTCN2GResolverPlugin::MatchRequestL(
    RStringF /*aMethod*/,
    const CUri8& /*aRequestUri*/,
    const RPointerArray<CSIPHeaderBase>& /*aHeaders*/,
    const TDesC8& /*aContent*/,
    const CSIPContentTypeHeader* /*aContentType*/,
    TUid& aClientUid)
    {
    return Match( RSipClientSimulator::ESipRequestMatch, aClientUid );
    }

// -----------------------------------------------------------------------------
// CTTCN2GResolverPlugin::ConnectSupported
// From CSIPResolvedClient2
// -----------------------------------------------------------------------------
//
TBool CTTCN2GResolverPlugin::ConnectSupported()
    {
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CTTCN2GResolverPlugin::ConnectL
// From CSIPResolvedClient2
// -----------------------------------------------------------------------------
//
void CTTCN2GResolverPlugin::ConnectL( 
    const TUid& aClientUid )
    {
    User::LeaveIfError( iSimulator.ConnectClient( aClientUid ) );
    }

// -----------------------------------------------------------------------------
// CTTCN2GResolverPlugin::CancelConnect
// From CSIPResolvedClient2
// -----------------------------------------------------------------------------
//        
void CTTCN2GResolverPlugin::CancelConnect(
    const TUid& /*aClientUid*/ )
    {
    }

// -----------------------------------------------------------------------------
// CTTCN2GResolverPlugin::SupportedContentTypesL
// From CSIPResolvedClient2
// -----------------------------------------------------------------------------
//        
RPointerArray<CSIPContentTypeHeader> 
					CTTCN2GResolverPlugin::SupportedContentTypesL()
    {
    TUid tmpUid;
    return (iSimulator.ClientSupportedContentTypesL( tmpUid ));
    }

// -----------------------------------------------------------------------------
// CTTCN2GResolverPlugin::SupportedSdpMediasL
// From CSIPResolvedClient2
// -----------------------------------------------------------------------------
//        
RPointerArray<CSdpMediaField> 
					CTTCN2GResolverPlugin::SupportedSdpMediasL()
    {
    TUid tmpUid;
    return (iSimulator.ClientSupportedMediaFieldsL( tmpUid ));
    }

// -----------------------------------------------------------------------------
// CTTCN2GResolverPlugin::AddClientSpecificHeadersForOptionsResponseL
// From CSIPResolvedClient2
// -----------------------------------------------------------------------------
//        
void CTTCN2GResolverPlugin::AddClientSpecificHeadersForOptionsResponseL( 
								RPointerArray<CSIPHeaderBase>& /*aHeaders*/ )
    {
    }

// -----------------------------------------------------------------------------
// CTTCN2GResolverPlugin::Match
// -----------------------------------------------------------------------------
//
TBool CTTCN2GResolverPlugin::Match(
    RSipClientSimulator::TMatchType aMatchType,
    TUid& aClientUid )
    {
    TBool match( EFalse );
    RSipClientSimulator::TMatchType matchType;
    TUid tmpUid;
    User::LeaveIfError( iSimulator.GetClientMatchType( matchType, tmpUid ) );
    if ( matchType == aMatchType )
        {
        match = ETrue;
        aClientUid.iUid = tmpUid.iUid;
        }
    return match;
    }
