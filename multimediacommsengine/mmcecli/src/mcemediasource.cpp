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




#include "mcemediasource.h"
#include "mcesession.h"
#include "mcemanager.h"
#include "mcestreamobserver.h"
#include "mcecommediasource.h"
#include "mceitcsender.h"
#include "mcefactory.h"
#include "mceevents.h"
#include "mceclientserver.h"
#include "mceserial.h"
#include "mceaudiostream.h"
#include "mceclilogs.h"
#include "mcedtmfobserver.h"


#define _FLAT_DATA static_cast<CMceComMediaSource*>( iFlatData )
#define FLAT_DATA( data ) _FLAT_DATA->data

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CMceMediaSource::~CMceMediaSource
// -----------------------------------------------------------------------------
//
EXPORT_C CMceMediaSource::~CMceMediaSource()
    {
    delete iFlatData;
    iParents.Reset();
    iParents.Close();
    }
    
// -----------------------------------------------------------------------------
// CMceMediaSource::Type
// -----------------------------------------------------------------------------
//
EXPORT_C TMceSourceType CMceMediaSource::Type() const
    {
    return FLAT_DATA( Type() );
    }
    

// -----------------------------------------------------------------------------
// CMceMediaSource::IsEnabled
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CMceMediaSource::IsEnabled() const
    {
    return FLAT_DATA( IsEnabled() );
    }

// -----------------------------------------------------------------------------
// CMceMediaSource::DtmfAvailable
// -----------------------------------------------------------------------------
//
TBool CMceMediaSource::DtmfAvailable()
	{
	TBool rtpSourceExists( EFalse );
	TBool rtpSinkExists( EFalse );
	TBool dtmfCodecExists( EFalse );
	
	for ( TInt i = 0; i < iParents.Count() && 
	      !( rtpSourceExists && rtpSinkExists && dtmfCodecExists ); i++ )
	    {
	    if ( iParents[ i ]->Type() == KMceAudio )
	        {
	        CMceAudioStream* audioStream = 
	            static_cast<CMceAudioStream*>( iParents[ i ] );
	            
	        audioStream->FindDtmfRelatedComponents( rtpSourceExists, 
                	                                rtpSinkExists, 
                	                                dtmfCodecExists,
                	                                audioStream->Binder() );
	        }
	    }
	return ( rtpSourceExists && rtpSinkExists && dtmfCodecExists );	
	}

// -----------------------------------------------------------------------------
// CMceMediaSource::DtmfActive
// -----------------------------------------------------------------------------
//
TBool CMceMediaSource::DtmfActive()
	{
	MCECLI_DEBUG( "CMceMediaSource::DtmfActive" )
	
	TBool dtmfActive( EFalse );
	if ( MCE_ENDPOINT_ITC_ALLOWED( *this ) )
        {
        CMceSession* session = iStream->Session();
         
    	TMceIds ids;
    	session->PrepareForITC( ids );
    	ids.iMediaID   = iStream->Id();
    	ids.iSourceID  = Id();
    	ids.iSpare1     = dtmfActive;
    	
        TRAP_IGNORE( session->ITCSender().SendL( ids, 
                        static_cast<TMceItcFunctions>( EMceItcIsDtmfActive ) ) );  

        dtmfActive = static_cast<TBool>( ids.iSpare1 );
        }
    
    MCECLI_DEBUG_DVALUE( "CMceMediaSource::DtmfActive, Exit, active:", 
                         dtmfActive )   
    return dtmfActive;		
	}

// -----------------------------------------------------------------------------
// CMceMediaSource::StartDtmfToneL
// -----------------------------------------------------------------------------
//
void CMceMediaSource::StartDtmfToneL( const TChar& aTone )
	{
	MCECLI_DEBUG_DVALUE( "CMceMediaSource::StartDtmfToneL, tone (char as int):", 
	                     static_cast<TInt>( aTone ) )  
	
	__ASSERT_ALWAYS( MCE_ENDPOINT_ITC_ALLOWED( *this ), 
	                 User::Leave( KErrNotReady ) );	
     	                 
	DoITCSendL( EMceItcStartDtmfTone, IsEnabled(), static_cast<TUint32>( aTone ) );
	
	MCECLI_DEBUG( "CMceMediaSource::StartDtmfToneL, Exit" )	
	}

// -----------------------------------------------------------------------------
// CMceMediaSource::StopDtmfToneL
// -----------------------------------------------------------------------------
//
void CMceMediaSource::StopDtmfToneL()           	
	{
	MCECLI_DEBUG( "CMceMediaSource::StopDtmfToneL" )
	
	__ASSERT_ALWAYS( MCE_ENDPOINT_ITC_ALLOWED( *this ), 
	                 User::Leave( KErrNotReady ) );	
          
	DoITCSendL( EMceItcStopDtmfTone, IsEnabled() );
	
	MCECLI_DEBUG( "CMceMediaSource::StopDtmfToneL, Exit" )
	}

// -----------------------------------------------------------------------------
// CMceMediaSource::SendDtmfToneL
// -----------------------------------------------------------------------------
//
void CMceMediaSource::SendDtmfToneL( const TChar& aTone,
	TTimeIntervalMicroSeconds32 aDuration )
	{
	MCECLI_DEBUG_DVALUE( "CMceMediaSource::SendDtmfToneL, tone (char as int):", 
	                     static_cast<TInt>( aTone ) )  
	MCECLI_DEBUG_DVALUE( "CMceMediaSource::SendDtmfToneL, duration (microsecs):", 
	                     aDuration.Int() )  
	
	__ASSERT_ALWAYS( MCE_ENDPOINT_ITC_ALLOWED( *this ), 
	                 User::Leave( KErrNotReady ) );	
	
	__ASSERT_ALWAYS( aDuration.Int() >= 0 && aDuration.Int() <= (TInt)KMaxTUint32, 
	                 User::Leave( KErrArgument ) );  
	                           
    DoITCSendL( EMceItcSendDtmfTone, 
                IsEnabled(), 
                static_cast<TUint32>( aTone ), 
                static_cast<TUint32>( aDuration.Int() ) );
                
    MCECLI_DEBUG( "CMceMediaSource::SendDtmfToneL, Exit" )
	}

// -----------------------------------------------------------------------------
// CMceMediaSource::SendDtmfToneSequenceL
// -----------------------------------------------------------------------------
//
void CMceMediaSource::SendDtmfToneSequenceL( const TDesC& aSequence )
	{
	MCECLI_DEBUG_SVALUE( "CMceMediaSource::SendDtmfToneSequenceL, sequence:", 
	                     aSequence )  
	                     
    __ASSERT_ALWAYS( MCE_ENDPOINT_ITC_ALLOWED( *this ), 
	                 User::Leave( KErrNotReady ) );	
	                 
	const TInt KMceDesConversionMultiplier = 4;
	HBufC8* sequence = 
	    HBufC8::NewLC( aSequence.Length() * KMceDesConversionMultiplier );
	TPtr8 prtSequence( sequence->Des() );   
	CnvUtfConverter::ConvertFromUnicodeToUtf8( prtSequence, aSequence );
	
	CMceSession* session = iStream->Session();
		
    TMceIds ids;
	session->PrepareForITC( ids );
	ids.iMediaID   = iStream->Id();
	ids.iSourceID  = Id();
	ids.iState     = IsEnabled();  	
	
	session->ITCSender().WriteL( ids, EMceItcSendDtmfToneSequence, prtSequence );	
	
	CleanupStack::PopAndDestroy( sequence );	
	
	MCECLI_DEBUG( "CMceMediaSource::SendDtmfToneSequenceL, Exit" )
	}

// -----------------------------------------------------------------------------
// CMceMediaSource::CancelDtmfToneSequenceL
// -----------------------------------------------------------------------------
//
void CMceMediaSource::CancelDtmfToneSequenceL()
	{
	MCECLI_DEBUG( "CMceMediaSource::CancelDtmfToneSequenceL, Entry" )  
    
    __ASSERT_ALWAYS( MCE_ENDPOINT_ITC_ALLOWED( *this ), 
                     User::Leave( KErrNotReady ) );
    
	CMceSession* session = iStream->Session();
    
    TMceIds ids;
	session->PrepareForITC( ids );
	ids.iMediaID   = iStream->Id();
	ids.iSourceID  = Id();
	ids.iState     = IsEnabled();
	
	session->ITCSender().SendL( ids, EMceItcCancelSendDtmfToneSequence );		
	
	MCECLI_DEBUG( "CMceMediaSource::CancelDtmfToneSequenceL, Exit" )
	}


// -----------------------------------------------------------------------------
// CMceMediaSource::Id
// -----------------------------------------------------------------------------
//
TMceMediaId CMceMediaSource::Id() const
    {
    return FLAT_DATA( iID );
    }
    

// -----------------------------------------------------------------------------
// CMceMediaSource::ReferenceCount
// -----------------------------------------------------------------------------
//
TInt& CMceMediaSource::ReferenceCount()
    {
    return FLAT_DATA( ReferenceCount() );
    }

// -----------------------------------------------------------------------------
// CMceMediaSource::CMceMediaSource
// -----------------------------------------------------------------------------
//
CMceMediaSource::CMceMediaSource()
    {
    }
  
    
// -----------------------------------------------------------------------------
// CMceMediaSource::ConstructL
// -----------------------------------------------------------------------------
//
void CMceMediaSource::ConstructL( CMceComMediaSource* aFlatData )
    {
    __ASSERT_ALWAYS( aFlatData , User::Leave( KErrArgument ) );
    iFlatData = aFlatData;
    FLAT_DATA( iID ) = TMceMediaId();//as not assigned
    
    }
      

// -----------------------------------------------------------------------------
// CMceMediaSource::SerializationId
// -----------------------------------------------------------------------------
//
TUint64 CMceMediaSource::SerializationId() const
    {
    return FLAT_DATA( SerializationId() );
    }

// -----------------------------------------------------------------------------
// CMceMediaSource::InternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceMediaSource::InternalizeFlatL( RReadStream& aReadStream )
    {
    __ASSERT_ALWAYS( iFlatData , User::Leave( KErrNotReady ) );
    _FLAT_DATA->InternalizeFlatL( aReadStream );
    }

// -----------------------------------------------------------------------------
// CMceMediaSource::ExternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceMediaSource::ExternalizeFlatL( RWriteStream& aWriteStream )
    {
    __ASSERT_ALWAYS( iFlatData , User::Leave( KErrNotReady ) );
    _FLAT_DATA->ExternalizeFlatL( aWriteStream );
    }

// -----------------------------------------------------------------------------
// CMceMediaSource::InternalizeL
// -----------------------------------------------------------------------------
//
void CMceMediaSource::InternalizeL( MMceComSerializationContext& aSerCtx )
    {
    InternalizeFlatL( aSerCtx.ReadStream() );
    }

// -----------------------------------------------------------------------------
// CMceMediaSource::ExternalizeL
// -----------------------------------------------------------------------------
//
void CMceMediaSource::ExternalizeL( MMceComSerializationContext& aSerCtx )
    {
    ExternalizeFlatL( aSerCtx.WriteStream() );
    }
    


// -----------------------------------------------------------------------------
// CMceMediaSource::Factory
// -----------------------------------------------------------------------------
//
TMceSourceFactory CMceMediaSource::Factory()
    {
    return TMceSourceFactory();
    }


// -----------------------------------------------------------------------------
// CMceMediaSource::InitializeL
// -----------------------------------------------------------------------------
//
void CMceMediaSource::InitializeL( CMceManager* /*aManager*/ )
    {
    // NOP
    }

// -----------------------------------------------------------------------------
// CMceMediaSource::InitializeL
// -----------------------------------------------------------------------------
//
void CMceMediaSource::InitializeL( CMceMediaStream& aParent )
    {

    if ( !Id().IsAssigned() )
        {
        __ASSERT_ALWAYS( aParent.Session() , User::Leave( KErrNotFound ) );        
        FLAT_DATA( iID ) = aParent.Session()->Manager().NextMediaId();
        MCECLI_DEBUG_DVALUES2("CMceMediaSource::InitializeL, this: type", Type(),
                              "id", Id().iId, "enabled", IsEnabled() );
        
        }
    
    }

// -----------------------------------------------------------------------------
// CMceMediaSource::StreamAddedL
// -----------------------------------------------------------------------------
//
void CMceMediaSource::StreamAddedL( CMceMediaStream& aParent )
    {
    
    if ( iParents.Find( &aParent ) == KErrNotFound )
        {
        iParents.AppendL( &aParent );
        }
        
    iStream = iParents[0];
    
    }
    
// -----------------------------------------------------------------------------
// CMceMediaSource::UnInitialize
// -----------------------------------------------------------------------------
//
void CMceMediaSource::UnInitialize( CMceMediaStream& aParent )
    {
    TInt index = iParents.Find( &aParent );
    if ( index >= 0 )
        {
        iParents.Remove( index );
        }
    iStream = NULL;
    
    if ( iParents.Count() > 0 )
        {
        iStream = iParents[0];
        }
    }


// -----------------------------------------------------------------------------
// CMceMediaSource::DoITCSendL
// -----------------------------------------------------------------------------
//
void CMceMediaSource::DoITCSendL(
    TUint8 aITCFunction, 
    TBool aEnable,
    TUint32 aData1,
    TUint32 aData2 )
    {
    __ASSERT_ALWAYS( iStream , User::Leave( KErrNotReady ) );
    CMceSession* session = iStream->Session();
    __ASSERT_ALWAYS( session , User::Leave( KErrNotReady ) );

	TMceIds ids;
	session->PrepareForITC( ids );
	ids.iMediaID   = iStream->Id();
	ids.iSourceID  = Id();
	ids.iState     = aEnable;
	ids.iSpare1    = aData1;
	ids.iSpare2    = aData2;
	
    session->ITCSender().SendL( ids, static_cast<TMceItcFunctions>( aITCFunction ) );
    
	Enabled( static_cast<TBool>( ids.iState ) );
    }
    

// -----------------------------------------------------------------------------
// CMceMediaSource::DoEnableL
// -----------------------------------------------------------------------------
//
void CMceMediaSource::DoEnableL() 
    {
    if ( MCE_ENDPOINT_ITC_ALLOWED( *this ) )
        {
        __ASSERT_ALWAYS( iStream->State() != CMceMediaStream::ETranscodingRequired, 
                         User::Leave( KErrNotReady ) );
    	DoITCSendL( EMceItcEnable, ETrue );    
        }
    else
        {
    	Enabled( ETrue );
        MCECLI_DEBUG("CMceMediaSource::DoEnableL, done locally");
        }
    }

// -----------------------------------------------------------------------------
// CMceMediaSource::DoDisableL
// -----------------------------------------------------------------------------
//
void CMceMediaSource::DoDisableL() 
    {
    if ( MCE_ENDPOINT_ITC_ALLOWED( *this ) )
        {
    	DoITCSendL( EMceItcDisable, EFalse );        
        }
    else
        {
    	Enabled( EFalse );
        MCECLI_DEBUG("CMceMediaSource::DoDisableL, done locally");
        }
    }


// -----------------------------------------------------------------------------
// CMceMediaSource::Enabled
// -----------------------------------------------------------------------------
//
void CMceMediaSource::Enabled( TBool aValue )
    {
	_FLAT_DATA->Enabled( aValue );
    }



// -----------------------------------------------------------------------------
// CMceMediaSource::EventReceivedL
// -----------------------------------------------------------------------------
//
TInt CMceMediaSource::EventReceivedL( TMceEvent& aEvent )
    {
    TInt status = KMceEventNotOwner;
    TBool idMatch = aEvent.Id().iSourceID == Id();
    if ( !idMatch )
        {
        return KMceEventNotOwner;
        }
        
    if ( aEvent.Id().IsSourceId() )
        {
        status = HandleEvent( aEvent );
        }
    else
        {
        status = KMceEventNotConsumed;
        }
    
    return status;    
    }

// -----------------------------------------------------------------------------
// CMceMediaSource::UpdateL
// -----------------------------------------------------------------------------
//
void CMceMediaSource::UpdateL( CMceMediaSource& aUpdate )
    {
    MCECLI_DEBUG("CMceMediaSource::UpdateL, Entry");
    MCECLI_DEBUG_DVALUES("this: type", Type(), "id", Id().iId );
    iFlatData->UpdateL( *aUpdate.iFlatData );
    MCECLI_DEBUG_DVALUE("CMceMediaSource::UpdateL, before exit. enabled", IsEnabled() );
    MCECLI_DEBUG("CMceMediaSource::UpdateL, Exit");
    }


// -----------------------------------------------------------------------------
// CMceMediaSource::Updated
// -----------------------------------------------------------------------------
//
void CMceMediaSource::Updated()
    {
    MMceStreamObserver* observer = iStream->Session()->Manager().MediaObserver();
    
    if ( observer )
        {
        observer->StreamStateChanged( *iStream, *this );
        }
    
    }


// -----------------------------------------------------------------------------
// CMceMediaSource::HandleEvent
// -----------------------------------------------------------------------------
//
TInt CMceMediaSource::HandleEvent( TMceEvent& aEvent )
    {
    MCECLI_DEBUG("CMceMediaSource::HandleEvent, Entry");
    MCECLI_DEBUG_DVALUES("this: type", Type(), "id", Id().iId );
    TInt status = KMceEventNotConsumed;

    if ( aEvent.MessageType() == EMceItcMsgTypeSource )
        {
        MCECLI_DEBUG("CMceMediaSource::HandleEvent, content of source changed");
        status = KMceEventUpdate;
        }
    else if ( aEvent.Action() == EMceItcStateChanged ) 
        {
        MCECLI_DEBUG("CMceMediaSource::HandleEvent, state changed");
        
    	Enabled( static_cast<TBool>( aEvent.ActionData() ) );
        
        if ( IsEnabled() )
            {
            iStream->SetState( CMceMediaStream::EStreaming );
            }
        else
            {
            iStream->SetState( CMceMediaStream::EDisabled );
            }
        Updated();
        status = KMceEventConsumed;
        }
    else if ( aEvent.Action() == EMceItcInProgress ) 
        {
        MCECLI_DEBUG("CMceMediaSource::HandleEvent, stream state changed");
        
        TInt stateContent( 0 );
        TBool isEnabled( EFalse );
        TUint32 actionData = aEvent.ActionData();
        MCE_DECODE_DOUBLE_STATE( actionData, stateContent, isEnabled );
        CMceMediaStream::TState state = static_cast<CMceMediaStream::TState>( stateContent );
        
        MCECLI_DEBUG_DVALUE("CMceMediaSource::HandleEvent, state:", state );
        MCECLI_DEBUG_DVALUE("CMceMediaSource::HandleEvent, enabled:", isEnabled );
        
        Enabled( isEnabled );
        
        TBool stateChanged = iStream->SetState( state );
        
        if ( !iStream->HandleMediaError( state, isEnabled, aEvent.Id().iStatus ) && 
             stateChanged )
            {
            Updated();
            }
        status = KMceEventConsumed;
        }
    else if ( aEvent.Action() == EMceItcDtmfSendStarted ||
              aEvent.Action() == EMceItcDtmfSendCompleted ||
              aEvent.Action() == EMceItcDtmfSendError )
        {
        MCECLI_DEBUG_DVALUE( "CMceMediaSource::HandleEvent, received dtmf event:", 
                             aEvent.Action() )
        
        MMceDtmfObserver* observer = iStream->Session()->Manager().DtmfObserver();
        if ( observer && iStream->Type() == KMceAudio )
            {
            if ( aEvent.Action() == EMceItcDtmfSendError )
                {
                observer->DtmfErrorOccured( *( iStream->Session() ), 
                                         *static_cast<CMceAudioStream*>( iStream ), 
                                         *this, 
                                         KErrGeneral );
                
                }
            else
                {
                MMceDtmfObserver::TMceDtmfEvent dtmfEvent = 
                    ( aEvent.Action() == EMceItcDtmfSendStarted ) ? 
                        MMceDtmfObserver::EDtmfSendStarted : 
                        MMceDtmfObserver::EDtmfSendCompleted;
                observer->DtmfEventReceived( *( iStream->Session() ), 
                                         *static_cast<CMceAudioStream*>( iStream ), 
                                         *this, 
                                         dtmfEvent );
                }
            }
        status = KMceEventConsumed;
        }
    else
        {
        //NOP
        MCECLI_DEBUG("CMceMediaSource::HandleEvent, not consumed");
        }
        
    MCECLI_DEBUG_DVALUE("CMceMediaSource::HandleEvent, before exit. enabled", IsEnabled() );
    MCECLI_DEBUG_DVALUE("CMceMediaSource::HandleEvent, Exit. status", status );
    return status;
    }
