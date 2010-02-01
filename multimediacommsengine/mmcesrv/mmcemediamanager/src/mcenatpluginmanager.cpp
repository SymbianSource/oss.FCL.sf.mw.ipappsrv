/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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



#include <e32def.h>
#include <badesca.h>
#include <ecom/ecom.h>
#include <in_pkt.h>
#include <sdpdocument.h>
#include "mcenatpluginmanager.h"
#include "mcemmlogs.h"
#include "mcecomsession.h"
#include "mceloglineparser.h"

const TUint KProtocolUdp = 2;
    
// -----------------------------------------------------------------------------
// CMceNatPluginManager::NewL
// -----------------------------------------------------------------------------
//
CMceNatPluginManager* CMceNatPluginManager::NewL()
    {
    CMceNatPluginManager* self = new (ELeave) CMceNatPluginManager();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMceNatPluginManager::ConstructL
// -----------------------------------------------------------------------------
//
void CMceNatPluginManager::ConstructL()
    {
    }

// -----------------------------------------------------------------------------
// CMceNatPluginManager::CMceNatPluginManager
// -----------------------------------------------------------------------------
//
CMceNatPluginManager::CMceNatPluginManager()
    {
    }

// -----------------------------------------------------------------------------
// CMceNatPluginManager::~CMceNatPluginManager
// -----------------------------------------------------------------------------
//
CMceNatPluginManager::~CMceNatPluginManager()
    {
    if( iPlugin )
        {
        delete iPlugin;
        }
    }

// -----------------------------------------------------------------------------
// CMceNatPluginManager::GetPluginL
// -----------------------------------------------------------------------------
//
void CMceNatPluginManager::GetPluginL()
    {
    MCEMM_DEBUG( "********* CMceNatPluginManager::GetPluginL, Entry" );
    
    if ( NULL == iPlugin )
        {
        MCEMM_DEBUG( "********* CMceNatPluginManager::GetPluginL, INIT" )

	    RImplInfoPtrArray infoArray;
	    CleanupStack::PushL( TCleanupItem( ResetAndDestroyInfo, &infoArray ) );
	    REComSession::ListImplementationsL( KNSPPluginIFUid, infoArray );    

	    if ( infoArray.Count() > 0 && infoArray[ 0 ] )
	        {
		    iPlugin = reinterpret_cast< CNSPPlugin* >(
			    REComSession::CreateImplementationL(
				    infoArray[0]->ImplementationUid(),
				    _FOFF( CNSPPlugin, iDtor_ID_Key ) ) );
	        }
	    else
	        {
	        User::Leave( KErrNotFound );
	        }
	    CleanupStack::Pop(); // TCleanupItem
	    infoArray.ResetAndDestroy();
        }
    
    MCEMM_DEBUG( "********* CMceNatPluginManager::GetPluginL, Exit" );
    }

// -----------------------------------------------------------------------------
// CMceNatPluginManager::NewSessionL
// -----------------------------------------------------------------------------
//
TInt CMceNatPluginManager::NewSessionL( MNSPSessionObserver& aNatSessionObserver,
        TUint32 aIapId, const TDesC8& aDomain )
    {
    MCEMM_DEBUG( "********* CMceNatPluginManager::NewSessionL, Entry" );
    
    TUint sessionId( 0 );
    
    if ( NULL != iPlugin )
        {
        sessionId = iPlugin->NewSessionL( aNatSessionObserver,
                aIapId, aDomain, KProtocolUdp );
        }
    else
        {
        User::Leave( KErrNotFound );
        }
    
    MCEMM_DEBUG_DVALUE( "********* CMceNatPluginManager::NewSessionL, Exit, sessionId", 
                        sessionId );
    
    return sessionId;
    }

// -----------------------------------------------------------------------------
// CMceNatPluginManager::CloseSessionL
// -----------------------------------------------------------------------------
//
void CMceNatPluginManager::CloseSessionL( TUint aSessionId )
    {
    MCEMM_DEBUG( "********* CMceNatPluginManager::CloseSessionL, Entry" );
    
    if ( NULL != iPlugin )
        {
        iPlugin->CloseSessionL( aSessionId );
        }
    else
        {
        User::Leave( KErrNotFound );
        }
    
    MCEMM_DEBUG( "********* CMceNatPluginManager::CloseSessionL, Exit" );
    }

// -----------------------------------------------------------------------------
// CMceNatPluginManager::CreateOfferL
// -----------------------------------------------------------------------------
//
TNatReturnStatus CMceNatPluginManager::CreateOfferL( 
    TUint aSessionId,
    CSdpDocument*& aOffer )
    {
    MCEMM_DEBUG( "********* CMceNatPluginManager::CreateOfferL, Entry" );
   
    WriteSdpToLog( _L("Offer on entry"), aOffer );
    
    TNatReturnStatus status = KErrNotFound;
    
    if ( NULL != iPlugin )
        {
        status = iPlugin->CreateOfferL( aSessionId, aOffer );
        }
    else
        {
        User::Leave( KErrNotFound );
        }
    
    WriteSdpToLog( _L("Offer on exit"), aOffer );
    
    MCEMM_DEBUG_DVALUE( "********* CMceNatPluginManager::CreateOfferL, Exit(status)=", status );
    
    return status;
    }

// -----------------------------------------------------------------------------
// CMceNatPluginManager::ResolveL
// -----------------------------------------------------------------------------
//
TNatReturnStatus CMceNatPluginManager::ResolveL( 
    TUint aSessionId,
    CSdpDocument*& aOffer,
    CSdpDocument*& aAnswer )
    {
    MCEMM_DEBUG( "********* CMceNatPluginManager::ResolveL, Entry" );
    
    WriteSdpToLog( _L("Offer on entry"), aOffer );
    
    WriteSdpToLog( _L("Answer on entry"), aAnswer );
    
    TNatReturnStatus status = KErrNotFound;
    
    if ( NULL != iPlugin )
        {
        status = iPlugin->ResolveL( aSessionId, aOffer, aAnswer );
        }
    else
        {
        User::Leave( KErrNotFound );
        }
    
    WriteSdpToLog( _L("Offer on exit"), aOffer );
    
    WriteSdpToLog( _L("Answer on exit"), aAnswer );    
    
    MCEMM_DEBUG_DVALUE( "********* CMceNatPluginManager::ResolveL, Exit(status)=", status );
    
    return status;
    }

// -----------------------------------------------------------------------------
// CMceNatPluginManager::DecodeAnswerL
// -----------------------------------------------------------------------------
//
TNatReturnStatus CMceNatPluginManager::DecodeAnswerL( 
    TUint aSessionId,
    CSdpDocument*& aAnswer )
    {
    MCEMM_DEBUG( "********* CMceNatPluginManager::DecodeAnswerL, Entry" );
    
    WriteSdpToLog( _L("Answer on entry"), aAnswer );    
    
    TNatReturnStatus status = KErrNotFound;
    
    if ( NULL != iPlugin )
        {
        status = iPlugin->DecodeAnswerL( aSessionId, aAnswer );
        }
    else
        {
        User::Leave( KErrNotFound );
        }
    
    WriteSdpToLog( _L("Answer on exit"), aAnswer );
    
    MCEMM_DEBUG_DVALUE( "********* CMceNatPluginManager::DecodeAnswerL, Exit(status)=", status );
    
    return status;
    }

// -----------------------------------------------------------------------------
// CMceNatPluginManager::UpdateL
// -----------------------------------------------------------------------------
//
void CMceNatPluginManager::UpdateL( TUint aSessionId, CSdpDocument*& aOffer )
    {
    MCEMM_DEBUG( "********* CMceNatPluginManager::UpdateL, Entry" );
    
    WriteSdpToLog( _L("Offer on entry"), aOffer );
    
    if ( NULL != iPlugin )
        {
        iPlugin->UpdateL( aSessionId, aOffer );
        }
    else
        {
        User::Leave( KErrNotFound );
        }
    
    WriteSdpToLog( _L("Offer on exit"), aOffer );
    
    MCEMM_DEBUG( "********* CMceNatPluginManager::UpdateL, Exit" );
    }

// -----------------------------------------------------------------------------
// CMceNatPluginManager::RestartL
// -----------------------------------------------------------------------------
//
TNatReturnStatus CMceNatPluginManager::RestartL( TUint aSessionId )
    {
    MCEMM_DEBUG( "********* CMceNatPluginManager::RestartL, Entry" );
    
    TNatReturnStatus status = KErrNotFound;
    
    if ( NULL != iPlugin )
        {
        status = iPlugin->RestartL( aSessionId );
        }
    else
        {
        User::Leave( KErrNotFound );
        }
    
    MCEMM_DEBUG_DVALUE( "********* CMceNatPluginManager::RestartL, Exit(status)=", status );
    
    return status;
    }

// -----------------------------------------------------------------------------
// CMceNatPluginManager::NatEnabled
// -----------------------------------------------------------------------------
//
TBool CMceNatPluginManager::NatEnabledL()
    {
    TRAPD( error, GetPluginL() );
    if(KErrNoMemory == error)
    	{
    	User::Leave( KErrNoMemory );
    	}
    if ( KErrNone == error )
        {
        if ( NULL != iPlugin )
            {
            MCEMM_DEBUG( "********* CMceNatPluginManager::NatEnabled: TRUE" );
            return ETrue;
            }
        else
            {
            MCEMM_DEBUG( "********* CMceNatPluginManager::NatEnabled: FALSE" );
            return EFalse;
            }
        }
    else
        {
        MCEMM_DEBUG( "********* CMceNatPluginManager::NatEnabled: FALSE(ELeave)" );
        return EFalse;
        }
    }

// -----------------------------------------------------------------------------
// CMceNatPluginManager::SetSessionParam
// -----------------------------------------------------------------------------
//
TInt CMceNatPluginManager::SetSessionParam( 
    TUint aSessionId,
    CNSPPlugin::TNSPSessionParamKey aParamKey,
    TUint aParamValue )
	{
	MCEMM_DEBUG( "********* CMceNatPluginManager::SetSessionParam, Entry" );
	
	TInt status = KErrNone;
	
	if( iPlugin == NULL)
		{
		status = KErrNotFound;
		}
	else
		{
		status = iPlugin->SetSessionParam( aSessionId, aParamKey, aParamValue );
		}
	
	MCEMM_DEBUG_DVALUE( "********* CMceNatPluginManager::SetSessionParam, Exit(status)=", status );
	
	return status;
   	}

// -----------------------------------------------------------------------------
// CMceNatPluginManager::GetSessionParam 
// -----------------------------------------------------------------------------
//
TInt CMceNatPluginManager::GetSessionParam( 
    TUint aSessionId,
    CNSPPlugin::TNSPSessionParamKey aParamKey )
	{
	MCEMM_DEBUG( "********* CMceNatPluginManager::GetSessionParam, Entry" );
	
	TInt status = KErrNone;
	
	if( iPlugin == NULL)
		{
		status = KErrNotFound;
		}
	else
		{
		status = iPlugin->GetSessionParam( aSessionId, aParamKey );
		}
	
	MCEMM_DEBUG_DVALUE( "********* CMceNatPluginManager::GetSessionParam, Exit(status)=", status );
	
	return status;
   	}

// -----------------------------------------------------------------------------
// CMceNatPluginManager::WriteSdpToLog 
// -----------------------------------------------------------------------------
//
#ifdef _DEBUG
void CMceNatPluginManager::WriteSdpToLog( 
    const TDesC& aMsg,
	CSdpDocument* aSdp )
	{
	if ( aSdp )
		{
		MCEMM_DEBUG_SVALUE( "", aMsg )
		CBufBase* buf = NULL;
		TRAPD( err, buf = EncodeSdpL( *aSdp ) )

		if ( err == KErrNone && buf != NULL )
			{
			TMceLogLineParser lineParser( buf->Ptr( 0 ), 80 );

			while ( !lineParser.End() )
				{
				MCEMM_DEBUG_SVALUE( "", lineParser.GetLine() )
				} 
			}
		delete buf;
		}
	else
		{
		MCEMM_DEBUG( "SDP document NULL" );
		}
    }
#else
void CMceNatPluginManager::WriteSdpToLog( const TDesC& /* aMsg */,
	CSdpDocument* /* aSdp */ )
	{

	}
#endif

// -----------------------------------------------------------------------------
// CMceNatPluginManager::EncodeSdpL 
// -----------------------------------------------------------------------------
//
CBufBase* CMceNatPluginManager::EncodeSdpL( 
    CSdpDocument& aSdp )
    {
    CBufFlat* buf = CBufFlat::NewL( 100 );
    CleanupStack::PushL( buf );
    RBufWriteStream writeStream;
    writeStream.Open( *buf, 0 );
    aSdp.EncodeL( writeStream );
    writeStream.Close();
    CleanupStack::Pop( buf );
    return buf;
    }

// -----------------------------------------------------------------------------
// CMceNatPluginManager::ResetAndDestroyInfo
// -----------------------------------------------------------------------------
//   
void CMceNatPluginManager::ResetAndDestroyInfo(TAny* aArray)
    {
    RPointerArray< CImplementationInformation >* array =
    	static_cast< RPointerArray< CImplementationInformation >* >( aArray );
    array->ResetAndDestroy();
    }  
   
// end of file
