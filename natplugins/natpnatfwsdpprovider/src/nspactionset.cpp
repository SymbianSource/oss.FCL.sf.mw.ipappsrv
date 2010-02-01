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
* Description:  State machine actions set implementation
*
*/

#include <sdpdocument.h>
#include "nspactionset.h"
#include "nspcontentparser.h"
#include "nspsession.h"
#include "nspmediastreamcontainer.h"
#include "nsputil.h"
#include "nspdefs.h"

#define MISMATCH( aData ) \
    ( aData.SubState() == CNSPSessionData::ESubMismatch ||\
      aData.SubState() == CNSPSessionData::ESubAll )
#define REMOTELITE( aData ) \
    ( aData.SubState() == CNSPSessionData::ESubRemoteLite ||\
      aData.SubState() == CNSPSessionData::ESubAll )

// ======== MEMBER FUNCTIONS ========
// ---------------------------------------------------------------------------
// TNSPActionSet::TNSPActionSet
// ---------------------------------------------------------------------------
//
TNSPActionSet::TNSPActionSet( CNSPSession& aSession )
    : iSession( aSession )
    {
    }


// ---------------------------------------------------------------------------
// TNSPActionSet::CreateStreamsL
// ---------------------------------------------------------------------------
//
TNatReturnStatus TNSPActionSet::CreateStreamsL( CSdpDocument& aDoc )
    {
    NSPLOG_STR( "TNSPActionSet::CreateStreamsL, Entry" )
    
    TNatReturnStatus status = iSession.Container().CreateStreamsL( aDoc );
    
    NSPLOG_INT( "TNSPActionSet::CreateStreamsL, status:", status )
    NSPLOG_STR( "TNSPActionSet::CreateStreamsL, Exit" )
    
    return status;    
    }


// ---------------------------------------------------------------------------
// TNSPActionSet::RemoveStreams
// ---------------------------------------------------------------------------
//
TNatReturnStatus TNSPActionSet::RemoveStreamsL( CSdpDocument& aDoc )
    {
    NSPLOG_STR( "TNSPActionSet::RemoveStreamsL, Entry" )
    
    TNatReturnStatus status = iSession.Container().RemoveStreamsL( aDoc );
    
    NSPLOG_INT( "TNSPActionSet::RemoveStreamsL, status:", status )
    NSPLOG_STR( "TNSPActionSet::RemoveStreamsL, Exit" )
    
    return status;
    }

// ---------------------------------------------------------------------------
// TNSPActionSet::ControlMediaL
// ---------------------------------------------------------------------------
//
TNatReturnStatus TNSPActionSet::ControlMediaL( TUint aStreamId,
		MNATFWConnectivityObserver::TNATFWConnectivityEvent aEvent,
		TInt aError, TAny* aData )
	{
	NSPLOG_STR( "TNSPActionSet::ControlMediaL, Entry" )
	
	TNatReturnStatus status =  iSession.Container().ControlMediaL(
			aStreamId, aEvent, aError, aData );
	
	NSPLOG_INT( "TNSPActionSet::ControlMediaL, status:", status )
	NSPLOG_STR( "TNSPActionSet::ControlMediaL, Exit" )
	
	return status;
	}

// ---------------------------------------------------------------------------
// TNSPActionSet::FirstRoundL
// ---------------------------------------------------------------------------
//
void TNSPActionSet::FirstRoundL( CSdpDocument& aDoc, TBool aStoreSdp )
    {
    NSPLOG_STR( "TNSPActionSet::FirstRoundL, Entry" )
    
    iSession.Container().ModStunSolvedL( aDoc );
    
    if ( aStoreSdp )
        {
        iSession.Data().SetUpdatedSdp( aDoc.CloneL() );
        }
    
    if ( iSession.Data().UseIce() )
        {
        iSession.Container().AddIceContentL( aDoc );
        }
    
    NSPLOG_STR( "TNSPActionSet::FirstRoundL, Exit" )
    }


// ---------------------------------------------------------------------------
// TNSPActionSet::LatterRoundL
// ---------------------------------------------------------------------------
//
void TNSPActionSet::LatterRoundL( CSdpDocument& aDoc )
    {
    NSPLOG_STR( "TNSPActionSet::LatterRoundL, Entry" )
    
    if ( iSession.Data().UseIce() )
        {
        iSession.Container().ModIceSolvedL( aDoc );
        }
    else
        {
        iSession.Container().ModStunSolvedL( aDoc );
        }
    
    NSPLOG_STR( "TNSPActionSet::LatterRoundL, Exit" )
    }


// ---------------------------------------------------------------------------
// TNSPActionSet::LocalRoundL
// ---------------------------------------------------------------------------
//
void TNSPActionSet::LocalRoundL( CSdpDocument& aDoc )
    {
    NSPLOG_STR( "TNSPActionSet::LocalRoundL, Entry" )
    
    iSession.Container().ModLocalSolvedL( aDoc );
    iSession.Parser().RemoveIceContent( aDoc );
    
    NSPLOG_STR( "TNSPActionSet::LocalRoundL, Exit" )
    }


// ---------------------------------------------------------------------------
// TNSPActionSet::UpdateRoundL
// ---------------------------------------------------------------------------
//
void TNSPActionSet::UpdateRoundL( CSdpDocument& aDoc )
    {
    NSPLOG_STR( "TNSPActionSet::UpdateRoundL, Entry" )
    
    if ( iSession.Data().UseIce() &&
         EIceRoleControlling == iSession.Data().Role() )
        {
        CSdpDocument* update = iSession.Container().CheckUpdateL( aDoc );
        
        if ( update )
            {
        	CleanupStack::PushL( update );
            iSession.Parser().RemoveIceContent( *update );
            iSession.Container().AddRemoteCandidatesL( *update );
            
            CleanupStack::Pop( update );
            iSession.UpdateSdpAsyncL( update ); // ownership changed
        	}
        }
    
    NSPLOG_STR( "TNSPActionSet::UpdateRoundL, Exit" )
    }


// ---------------------------------------------------------------------------
// TNSPActionSet::GetCredentialsL
// ---------------------------------------------------------------------------
//
TNatReturnStatus TNSPActionSet::GetCredentialsL( CSdpDocument& aDoc )
    {
    NSPLOG_STR( "TNSPActionSet::GetCredentialsL, Entry" )
    
    TNatReturnStatus status = KNatReady;
    CNATFWCredentials& credentials = iSession.Data().Credentials();
    
    if ( iSession.Data().UseIce() )
        {
        status = iSession.Container().GetCredentialsL( aDoc, credentials );
        }
    
    NSPLOG_INT( "TNSPActionSet::GetCredentialsL, status:", status )
    NSPLOG_STR( "TNSPActionSet::GetCredentialsL, Exit" )
    
    return status;
    }


// ---------------------------------------------------------------------------
// TNSPActionSet::GetCandidatesL
// ---------------------------------------------------------------------------
//
TNatReturnStatus TNSPActionSet::GetCandidatesL( CSdpDocument& aDoc )
    {
    NSPLOG_STR( "TNSPActionSet::GetCandidatesL, Entry" )
    
    TNatReturnStatus status = iSession.Container().GetCandidatesL( aDoc,
            iSession.Data().RemoteCandidates() );
    
    NSPLOG_INT( "TNSPActionSet::GetCandidatesL, status:", status )
    NSPLOG_STR( "TNSPActionSet::GetCandidatesL, Exit" )
    
    return status;
    }


// ---------------------------------------------------------------------------
// TNSPActionSet::GetSessionModifiersL
// ---------------------------------------------------------------------------
//
TNatReturnStatus TNSPActionSet::GetSessionModifiersL( CSdpDocument& aDoc,
        CNSPSession::TSdpRole aRole )
    {
    NSPLOG_STR( "TNSPActionSet::GetSessionModifiersL, Entry" )
    
    TNatReturnStatus status = KNatReady;
    CNSPSessionData& data = iSession.Data();
    CNSPSessionData::TSubState subState = data.SubState();
    TBool isIce = data.UseIce();
    
    TBool useIce = iSession.Parser().IsIceSupported( aDoc );
    TBool mismatch = iSession.Parser().IsMismatchL( aDoc );
    TBool lite = iSession.Parser().IsLiteAttribute( aDoc );
    
    // # 1. Mismatch
    data.SetSubState( mismatch ? CNSPSessionData::ESubMismatch : data.SubState() );
    data.SetUseIce( isIce && useIce && !MISMATCH( data ) );
    
    // # 2. Remote lite
    data.SetSubState( lite ? CNSPSessionData::ESubRemoteLite : data.SubState() );
    
    // # 3. Ice role
    if ( CNSPSession::EUndefined != aRole )
        {
        data.SetRole( OFFERER( aRole ) || REMOTELITE( data ) ?
                EIceRoleControlling : EIceRoleControlled );
        }
    
    status = ( ( data.SubState() == subState &&
                 isIce == data.UseIce() ) ? KNatReady : KNatAsync );
    
    NSPLOG_INT( "TNSPActionSet::GetSessionModifiersL, status:", status )
    NSPLOG_STR( "TNSPActionSet::GetSessionModifiersL, Exit" )
    
    return status;
    }


// ---------------------------------------------------------------------------
// TNSPActionSet::GetSessionConnInfoL
// ---------------------------------------------------------------------------
//
TNatReturnStatus TNSPActionSet::GetSessionConnInfoL( CSdpDocument& aDoc )
    {
    NSPLOG_STR( "TNSPActionSet::GetSessionConnInfoL, Entry" )
    
    TNatReturnStatus status = iSession.Container().GetSessionConnInfoL( aDoc );
    
    NSPLOG_INT( "TNSPActionSet::GetSessionConnInfoL, status:", status )
    NSPLOG_STR( "TNSPActionSet::GetSessionConnInfoL, Exit" )
    
    return status;
    }


// ---------------------------------------------------------------------------
// TNSPActionSet::RestartWithoutIce
// ---------------------------------------------------------------------------
//
void TNSPActionSet::RestartWithoutIce()
    {
    iSession.Container().ResetAndDestroyCandidates();
    iSession.RemoveIce( iSession.Plugins() );
    }


// ---------------------------------------------------------------------------
// TNSPActionSet::LoadPluginL
// ---------------------------------------------------------------------------
//
void TNSPActionSet::LoadPluginL()
    {
    NSPLOG_STR( "TNSPActionSet::LoadPluginL, Entry" )
    
    __ASSERT_ALWAYS( iSession.Plugins().MdcaCount(), User::Leave( KErrNotFound ) );
    
    TInt pluginIndex = 0;
    iSession.Container().LoadPluginL( iSession.Plugins(), pluginIndex );
    
    NSPLOG8_STR2( "TNSPActionSet::LoadPluginL, loaded:", iSession.Plugins()[pluginIndex] )
    NSPLOG_INT( "TNSPActionSet::LoadPluginL, useIce:", iSession.Data().UseIce() )
    NSPLOG_INT( "TNSPActionSet::LoadPluginL, pluginIndex:", pluginIndex )
    
    TBool isIce = iSession.IsIce( iSession.Plugins().MdcaPoint( pluginIndex ) );
    User::LeaveIfError( iSession.Data().UseIce() == isIce ? KErrNone : KErrNotFound );
    
    NSPLOG_STR( "TNSPActionSet::LoadPluginL, Exit" )
    }


// ---------------------------------------------------------------------------
// TNSPActionSet::ActivateL
// ---------------------------------------------------------------------------
//
void TNSPActionSet::ActivateL( CSdpDocument& aDoc )
    {
    if ( iSession.Data().UseIce() )
        {
        iSession.Container().ActivateL();
        }
    else
        {
        iSession.Container().ActivateL( aDoc );
        }
    }


// ---------------------------------------------------------------------------
// TNSPActionSet::DeActivateL
// ---------------------------------------------------------------------------
//
void TNSPActionSet::DeActivateL()
    {
    iSession.Container().DeActivateL();
    }


// ---------------------------------------------------------------------------
// TNSPActionSet::FetchL
// ---------------------------------------------------------------------------
//
void TNSPActionSet::FetchL()
    {
    if ( iSession.Data().UseIce() )
        {
        iSession.Container().FetchCandidatesL();
        }
    else
        {
        iSession.Container().FetchCandidateL();
        }
    }


// ---------------------------------------------------------------------------
// TNSPActionSet::SetRoleL
// ---------------------------------------------------------------------------
//
void TNSPActionSet::SetRoleL()
    {
    __ASSERT_ALWAYS( iSession.Data().UseIce(), User::Leave( KErrGeneral ) );
    iSession.Container().SetRoleL( iSession.Data().Role() );
    }


// ---------------------------------------------------------------------------
// TNSPActionSet::SetInboundCredentialsL
// ---------------------------------------------------------------------------
//
void TNSPActionSet::SetInboundCredentialsL()
    {
    if ( iSession.Data().UseIce() )
        {
        iSession.Container().SetInboundCredentialsL();
        }
    }


// ---------------------------------------------------------------------------
// TNSPActionSet::SetOutboundCredentialsL
// ---------------------------------------------------------------------------
//
void TNSPActionSet::SetOutboundCredentialsL()
    {
    if ( iSession.Data().UseIce() )
        {
        iSession.Container().SetOutboundCredentialsL();
        }
    }


// ---------------------------------------------------------------------------
// TNSPActionSet::ConnChecksL
// ---------------------------------------------------------------------------
//
void TNSPActionSet::ConnChecksL()
    {
    __ASSERT_ALWAYS( iSession.Data().UseIce(), User::Leave( KErrGeneral ) );
    iSession.Container().PerformCandidateChecksL( iSession.Data().RemoteCandidates() );
    }


// ---------------------------------------------------------------------------
// TNSPActionSet::UpdateL
// ---------------------------------------------------------------------------
//
void TNSPActionSet::UpdateL()
    {
    __ASSERT_ALWAYS( iSession.Data().UseIce(), User::Leave( KErrGeneral ) );
    iSession.Container().UpdateL( iSession.Data().RemoteCandidates() );
    }

