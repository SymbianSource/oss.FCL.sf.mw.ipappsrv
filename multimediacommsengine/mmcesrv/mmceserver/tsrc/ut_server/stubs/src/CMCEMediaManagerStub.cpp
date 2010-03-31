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




// INCLUDE FILES
#include <audiopreference.h>
#include <sdpdocument.h>
#include <MmccInterfacedef.h>
#include "mcemediamanager.h"
#include "mcemediaobserver.h"
#include "mceamrcodec.h"
#include "mcesdpsession.h"
#include "mcemediadefs.h"

#include "mcecommediastream.h"
#include "mcecomaudiostream.h"
#include "mcecomsession.h"
#include "mcecomaudiocodec.h"
#include "mcecomamrcodec.h"
#include "mcecomamrcodec.h"
#include "mcecomh263codec.h"
#include "mcecommediasource.h"
#include "mcecommediasink.h"
#include "mcecomrtpsource.h"
#include "mcecomrtpsink.h"

#include "mcemediasource.h"
#include "mcemediastream.h"
#include "mceaudiostream.h"
#include "mcemicsource.h"
#include "mcertpsource.h"
#include "mcertpsink.h"
#include "mceaudiocodec.h"
#include "mcelogs.h"
#include "cmcetls.h"
#include "mcenatpluginmanager.h"

#include "MCETestHelper.h"

#include "SdpCodecStringPool.h"

// ================ MEMBER FUNCTIONS =======================



// ---------------------------------------------------------
// CMceMediaManager::CMceMediaManager
// C++ default constructor can NOT contain any code, that
// might leave.
// Status : Draft
// ---------------------------------------------------------
CMceMediaManager::CMceMediaManager()
    {
    }

// ---------------------------------------------------------
// CMceMediaManager::ConstructL
// Symbian 2nd phase constructor can leave.
// Status : Draft
// ---------------------------------------------------------
void CMceMediaManager::ConstructL( MMceMediaGeneralObserver& aObserver )
    {  
    iObserver = &aObserver;
    
    iNatPluginManager = CMceNatPluginManager::NewL();
    }


// ---------------------------------------------------------
// CMceMediaManager::NewL
// Static constructor.
// Status : Draft
// ---------------------------------------------------------
CMceMediaManager* CMceMediaManager::NewL( MMceMediaGeneralObserver& aObserver )
    {
    CMceMediaManager* self = NewLC( aObserver );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------
// CMceMediaManager::NewLC
// Static constructor.
// Status : Draft
// ---------------------------------------------------------
CMceMediaManager* CMceMediaManager::NewLC( MMceMediaGeneralObserver& aObserver )
    {
    CMceMediaManager* self = new ( ELeave ) CMceMediaManager;
    CleanupStack::PushL( self );
    self->ConstructL( aObserver );
    return self;
    }


// ---------------------------------------------------------
// CMceMediaManager::~CMceMediaManager
// Destructor
// Status : Draft
// ---------------------------------------------------------    
CMceMediaManager::~CMceMediaManager()
    {
    delete iNatPluginManager;
    }   

// ---------------------------------------------------------
// CMceMediaManager::NatPluginManager
// ---------------------------------------------------------    
CMceNatPluginManager& CMceMediaManager::NatPluginManager()
    {
    return (*iNatPluginManager);    
    }

// ---------------------------------------------------------
// CMceMediaManager::SendRTCPAnyDataL 
// ---------------------------------------------------------   
// 
void CMceMediaManager::SendRTCPAnyDataL( const CMceComMediaStream& aMediaStream, 
                                         const TDesC8& aData )
    {
    MCE_DEBUG1("CMceMediaManager::SendRTCPAnyDataL");
    MCE_DEBUG6("DATA", aData );
    
    CMCETls* tls = CMCETls::Storage();
    if ( tls )
        {
        tls->MediaManagerAction( CMCETls::ESendRTCPAnyData, &aMediaStream );
        }
        	
    if ( tls )
        {
        User::LeaveIfError( tls->iMediaManagerDecodeStatus );
        }
    }

// ---------------------------------------------------------
// CMceMediaManager::PauseL 
// ---------------------------------------------------------   
// 
void CMceMediaManager::PauseL( CMceComMediaSink& aSink )
    {
    MCE_DEBUG1("CMceMediaManager::PauseL( sink )");
    CMCETls* tls = CMCETls::Storage();
    if ( tls )
        {
        tls->MediaManagerAction( CMCETls::EPauseSink, &aSink );
        User::LeaveIfError( tls->LeaveError() );
        
        }
    }
    
// ---------------------------------------------------------
// CMceMediaManager::PauseL
// ---------------------------------------------------------  
// 
void CMceMediaManager::PauseL( CMceComMediaSource& aSource )
    { 
    MCE_DEBUG1("CMceMediaManager::PauseL( source )");
    CMCETls* tls = CMCETls::Storage();
    if ( tls )
        {
        tls->MediaManagerAction( CMCETls::EPauseSource, &aSource );
        User::LeaveIfError( tls->LeaveError() );      
        }
    }
    
// ---------------------------------------------------------
// CMceMediaManager::ResumeL 
// ---------------------------------------------------------    
//
void CMceMediaManager::ResumeL( CMceComMediaSink& aSink )
    { 
    MCE_DEBUG1("CMceMediaManager::ResumeL( sink )");
    
    CMCETls* tls = CMCETls::Storage();
    if ( tls )
        {
        tls->MediaManagerAction( CMCETls::EResumeSink, &aSink );
        User::LeaveIfError( tls->LeaveError() );      
        }
    }
    
// ---------------------------------------------------------
// CMceMediaManager::ResumeL 
// ---------------------------------------------------------  
//  
void CMceMediaManager::ResumeL( CMceComMediaSource& aSource )
    { 
    MCE_DEBUG1("CMceMediaManager::ResumeL( source )");
    CMCETls* tls = CMCETls::Storage();
    if ( tls )
        {
        tls->MediaManagerAction( CMCETls::EResumeSource, &aSource );
        User::LeaveIfError( tls->LeaveError() );      
        }
    }

// ---------------------------------------------------------
// CMceMediaManager::CloseSession
// Closes MCC session and removes the internal sdp session
// Status : Draft
// ---------------------------------------------------------    
TInt CMceMediaManager::CloseSession( CMceComSession& aSession )
    { 
    MCE_DEBUG1("CMceMediaManager::CloseSession");
    CMCETls* tls = CMCETls::Storage();
    if ( tls )
        {
        tls->MediaManagerAction( CMCETls::ECloseSession, &aSession );
        }
    return KErrNone;
    }


// ---------------------------------------------------------
// CMceMediaManager::StartInactivityTimerL 
// ---------------------------------------------------------    
//
void CMceMediaManager::StartInactivityTimerL( 
                        const CMceComRtpSource& aRtpSource,
                        TUint32 /*aInactivityTimeout*/ )
    { 
    MCE_DEBUG1("CMceMediaManager::StartInactivityTimerL");
    
    CMCETls* tls = CMCETls::Storage();
    if ( tls )
        {
        tls->MediaManagerAction( CMCETls::EStartInactivityTimer, &aRtpSource );
        }
    }
    
// ---------------------------------------------------------
// CMceMediaManager::StopInactivityTimerL 
// ---------------------------------------------------------    
//
void CMceMediaManager::StopInactivityTimerL( const CMceComRtpSource& aRtpSource )
    { 
    MCE_DEBUG1("CMceMediaManager::StopInactivityTimerL");
    CMCETls* tls = CMCETls::Storage();
    if ( tls )
        {
        tls->MediaManagerAction( CMCETls::EStopInactivityTimer, &aRtpSource );
        }
    }


// ---------------------------------------------------------
// CMceMediaManager::SendRTCPReceiverReportL 
// ---------------------------------------------------------    
//
void CMceMediaManager::SendRTCPReceiverReportL( const CMceComRtpSource& aRtpSource )
    {
    MCE_DEBUG1("CMceMediaManager::SendRTCPReceiverReportL");
    CMCETls* tls = CMCETls::Storage();
    if ( tls )
        {
        tls->MediaManagerAction( CMCETls::ESendRR, &aRtpSource );
        }
    }
    
// ---------------------------------------------------------
// CMceMediaManager::SendRTCPSenderReportL 
// ---------------------------------------------------------   
// 
void CMceMediaManager::SendRTCPSenderReportL( const CMceComRtpSink& aRtpSink )
    { 
    MCE_DEBUG1("CMceMediaManager::SendRTCPSenderReportL");
    CMCETls* tls = CMCETls::Storage();
    if ( tls )
        {
        tls->MediaManagerAction( CMCETls::ESendSR, &aRtpSink );
        }
    }   

// ---------------------------------------------------------
// CMceMediaManager::SetBalanceL 
// ---------------------------------------------------------    
//
void CMceMediaManager::SetBalanceL( const CMceComMediaSink& aSink, 
                                    TInt /*aBalance*/ )
    { 
    MCE_DEBUG1("CMceMediaManager::SetBalanceL");
    CMCETls* tls = CMCETls::Storage();
    if ( tls )
        {
        tls->MediaManagerAction( CMCETls::ESetBalance, &aSink );
        }
    }  

// ---------------------------------------------------------
// CMceMediaManager::SetBalanceL 
// ---------------------------------------------------------    
//
void CMceMediaManager::SetBalanceL( const CMceComMediaSource& aSource,
                                    TInt /*aBalance*/ )
    { 
    MCE_DEBUG1("CMceMediaManager::SetBalanceL");
    CMCETls* tls = CMCETls::Storage();
    if ( tls )
        {
        tls->MediaManagerAction( CMCETls::ESetBalance, &aSource );
        }
    }  

// ---------------------------------------------------------
// CMceMediaManager::GetGainL 
// ---------------------------------------------------------    
//
void CMceMediaManager::GetGainL( const CMceComMediaSource& aSource,
                                 TInt& aGain )
    {
    MCE_DEBUG1("CMceMediaManager::GetGainL");
    CMCETls* tls = CMCETls::Storage();
    if ( tls )
        {
        aGain = 1;
        tls->MediaManagerAction( CMCETls::EGetGain, &aSource );
        }
    }  

// ---------------------------------------------------------
// CMceMediaManager::SetGainL 
// --------------------------------------------------------- 
//   
void CMceMediaManager::SetGainL( const CMceComMediaSource& aSource,
                                 TInt /*aGain*/ )
    { 
    MCE_DEBUG1("CMceMediaManager::SetGainL");
    CMCETls* tls = CMCETls::Storage();
    if ( tls )
        {
        tls->MediaManagerAction( CMCETls::ESetGain, &aSource );
        }
    }   

// ---------------------------------------------------------
// CMceMediaManager::SetVolumeL 
// --------------------------------------------------------- 
//   
void CMceMediaManager::SetVolumeL( const CMceComMediaSink& aSink, 
                                  TInt /*aVolume*/ )
    {
    MCE_DEBUG1("CMceMediaManager::SetVolumeL");
    CMCETls* tls = CMCETls::Storage();
    if ( tls )
        {
        tls->MediaManagerAction( CMCETls::ESetVolume, &aSink );
        }
    }

// ---------------------------------------------------------
// CMceMediaManager::GetVolumeL 
// --------------------------------------------------------- 
//   
void CMceMediaManager::GetVolumeL( const CMceComMediaSink& aSink, 
                                  TInt& aVolume )
    {
    MCE_DEBUG1("CMceMediaManager::GetVolumeL");
    CMCETls* tls = CMCETls::Storage();
    if ( tls )
        {
        aVolume = 1;
        tls->MediaManagerAction( CMCETls::EGetVolume, &aSink );
        }
    }

// ---------------------------------------------------------
// CMceMediaManager::GetMaxVolumeL 
// --------------------------------------------------------- 
//   
void CMceMediaManager::GetMaxVolumeL( TInt& aMaxVolume )
    {
    MCE_DEBUG1("CMceMediaManager::GetMaxVolumeL");
    CMCETls* tls = CMCETls::Storage();
    if ( tls )
        {
        aMaxVolume = 10;
        tls->MediaManagerAction( CMCETls::EGetMaxVolume, NULL );
        }
    }

// ---------------------------------------------------------
// CMceMediaManager::SetAudioRoutingL 
// --------------------------------------------------------- 
//           
void CMceMediaManager::SetAudioRoutingL( const CMceComSpeakerSink& aSpeaker, 
                                        TInt /*aRoutingDestination*/ )
    {
    MCE_DEBUG1("CMceMediaManager::SetAudioRoutingL");
    CMCETls* tls = CMCETls::Storage();
    if ( tls )
        {
        tls->MediaManagerAction( CMCETls::ESetAudioRouting, &aSpeaker );
        }
    }

// ---------------------------------------------------------
// CMceMediaManager::GetAudioRoutingL 
// --------------------------------------------------------- 
//           
void CMceMediaManager::GetAudioRoutingL( const CMceComSpeakerSink& aSpeaker,
                                        TInt& aRoutingDestination )
    {
    MCE_DEBUG1("CMceMediaManager::GetAudioRoutingL");
    CMCETls* tls = CMCETls::Storage();
    if ( tls )
        {
        aRoutingDestination = 1;
        tls->MediaManagerAction( CMCETls::EGetAudioRouting, &aSpeaker );
        }
    }

// ---------------------------------------------------------
// CMceMediaManager::GetMaxGainL 
// --------------------------------------------------------- 
//           
void CMceMediaManager::GetMaxGainL( TInt& aMaxGain )
    {
    MCE_DEBUG1("CMceMediaManager::GetMaxGainL");
    CMCETls* tls = CMCETls::Storage();
    if ( tls )
        {
        aMaxGain = 10;
        tls->MediaManagerAction( CMCETls::EGetMaxGain, NULL );
        }
    }
                        
// ---------------------------------------------------------
// CMceMediaManager::GetSupportedCodecs 
// ---------------------------------------------------------    
//
TInt CMceMediaManager::GetSupportedCodecs( RArray<TFourCC>& /*aCodecs*/  )
    { 
    TInt err = KErrNone;
    return err;
    }     

// ---------------------------------------------------------
// CMceMediaManager::GetCapabilities 
// ---------------------------------------------------------  
//  
void CMceMediaManager::GetCapabilitiesL( 
 						RPointerArray<CMceComAudioCodec>& aCapabilities  )
    { 
	CMceComAMRCodec* comcodec = CMceComAMRCodec::NewL( _L8("AMR") );
	CleanupStack::PushL( comcodec );
	aCapabilities.AppendL( static_cast<CMceComAudioCodec*> ( comcodec ) );
	CleanupStack::Pop( comcodec ); 
	comcodec->iSamplingFreq = 100;
	comcodec->iPayloadType = 97;
	comcodec->iBitrate = 3600;
	comcodec->iMaxPTime = 400;
	comcodec->iPTime = 300;
	comcodec->iEnableVAD = EFalse;
    }

// ---------------------------------------------------------
// CMceMediaManager::GetCapabilities 
// ---------------------------------------------------------  
//  
void CMceMediaManager::GetCapabilitiesL( 
 						RPointerArray<CMceComVideoCodec>& aCapabilities  )
    { 
	CMceComH263Codec* comcodec = CMceComH263Codec::NewL( _L8("H263-2000") );
	CleanupStack::PushL( comcodec );
	aCapabilities.AppendL( static_cast<CMceComVideoCodec*> ( comcodec ) );
	CleanupStack::Pop( comcodec ); 
	comcodec->iPayloadType = 96;
    }

// ---------------------------------------------------------
// CMceMediaManager::EncodeL
// ---------------------------------------------------------    
CSdpDocument* CMceMediaManager::EncodeL( CMceComSession& aSession )
    {

    MCE_DEBUG1("MediaManager: CMceMediaManager::CreateOfferL() ")

    CSdpDocument* offer = NULL;
    CMCETls* tls = CMCETls::Storage();
    if ( tls )
        {
        tls->MediaManagerSdpAction( CMCETls::EEncode, &aSession );
        User::LeaveIfError( tls->LeaveError() );
        offer = MCETestHelper::OfferL();
        }
    return offer;
    }


// ---------------------------------------------------------
// CMceMediaManager::DecodeL
// ---------------------------------------------------------    
TMceSipWarningCode CMceMediaManager::DecodeL( 
                            CMceComSession& aSession,
                            CSdpDocument& /*aSdpDocument*/ )
    {
    
    TMceSipWarningCode result = KErrNone;
    CMCETls* tls = CMCETls::Storage();
    if ( tls )
        {
        tls->MediaManagerSdpAction( CMCETls::EDecode, &aSession );
        User::LeaveIfError( tls->LeaveError() );
        result = tls->iMediaManagerDecodeStatus;
        }
    
    return result;
    
    }

// ---------------------------------------------------------
// CMceMediaManager::UpdateL
// ---------------------------------------------------------    
TMceReturnStatus CMceMediaManager::UpdateL( CMceComSession& aSession )
    {

    TMceReturnStatus result = KErrNone;
    CMCETls* tls = CMCETls::Storage();
    if ( tls )
        {
        tls->MediaManagerAction( CMCETls::EUpdate, &aSession );
        User::LeaveIfError( tls->LeaveError() );
        result = tls->iMediaManagerUpdateStatus;
        if ( result >= KErrNone )
            {
            // success
            if ( &aSession && aSession.iType == CMceComSession::EInSession )
                {
                CMceComSession* decodedSession = MCETestHelper::ComSessionL( 0 );
                CleanupStack::PushL( decodedSession );
                aSession.AddStreamL( decodedSession->Streams()[0] );
                decodedSession->Streams().Remove( 0 );
                CleanupStack::PopAndDestroy( decodedSession );
                aSession.InitializeL();
                }
            }
        }
    
    return result;
    }
              
// ---------------------------------------------------------
// CMceMediaManager::ReserveL
// ---------------------------------------------------------    
TMceReturnStatus CMceMediaManager::ReserveL( CMceComSession& aSession, 
                                             const TDesC8& /*aToken*/ )
    {
    TMceReturnStatus result = KErrNone;
    CMCETls* tls = CMCETls::Storage();
    if ( tls )
        {
        tls->MediaManagerAction( CMCETls::EReserve, &aSession );
        User::LeaveIfError( tls->LeaveError() );
        result = tls->iMediaManagerReserveStatus;
        }
    
    return result;
    }

// ---------------------------------------------------------
// CMceMediaManager::ReservationNeeded
// ---------------------------------------------------------    
TBool CMceMediaManager::ReservationNeeded( CMceComSession& /*aSession*/ )
    {
    return EFalse;
    }

// ---------------------------------------------------------
// CMceMediaManager::NeedToNegotiate
// ---------------------------------------------------------    
TBool CMceMediaManager::NeedToNegotiate( CMceComSession& /*aSession*/,
                                         CSdpDocument* /*aSdpDocument*/ )
    {
    CMCETls* tls = CMCETls::Storage();
    TBool result( EFalse );
    if( tls )
        {
        result = tls->iMediaManagerNeedToNegotiate;
        }
    return result;
    }
    


// ---------------------------------------------------------
// CMceMediaManager::GetRole
// ---------------------------------------------------------    
TMceNegotiationRole CMceMediaManager::GetRole( CMceComSession& /*aSession*/ )
                                  
    {
    CMCETls* tls = CMCETls::Storage();
    TMceNegotiationRole result( EMceRoleAnswerer );
    if( tls )
    	{
    	result = tls->iRole;	
    		
    	}
    return result;
    }
    
    
    
    

// ---------------------------------------------------------
// CMceMediaManager::NeedToNegotiate
// ---------------------------------------------------------    
TBool CMceMediaManager::ConfirmationNeeded( CMceComSession& /*aSession*/ )
    {
    CMCETls* tls = CMCETls::Storage();
    TBool result( EFalse );
    if( tls )
        {
        result = tls->iMediaManagerNeedToNegotiate;
        }
    return result;
    }
    
// ---------------------------------------------------------------------------
// CMceMediaManager::EnableViewFinderL
// ---------------------------------------------------------------------------
//
void CMceMediaManager::EnableViewFinderL( const CMceComMediaSink& aMediaSink, 
                                          const TDesC8& /*aParam*/ )
    {
    MCE_DEBUG1("CMceMediaManager::EnableViewFinderL");
    CMCETls* tls = CMCETls::Storage();
    if ( tls )
        {
        tls->MediaManagerAction( CMCETls::EEnableViewFinder, &aMediaSink );
        }
    }


// ---------------------------------------------------------------------------
// CMceMediaManager::DisableViewFinderL
// ---------------------------------------------------------------------------
//
void CMceMediaManager::DisableViewFinderL( const CMceComMediaSink& aMediaSink )
    {
    MCE_DEBUG1("CMceMediaManager::DisableViewFinderL");
    CMCETls* tls = CMCETls::Storage();
    if ( tls )
        {
        tls->MediaManagerAction( CMCETls::EDisableViewFinder, &aMediaSink );
        }
    }

// ---------------------------------------------------------------------------
// CMceMediaManager::GetDisplayRectL
// ---------------------------------------------------------------------------
//
void CMceMediaManager::GetDisplayRectL( CMceComDisplaySink& aSink, 
                                        TRect& aDisplayRect )
    {
    MCE_DEBUG1("CMceMediaManager::GetDisplayRectL");
    CMCETls* tls = CMCETls::Storage();
    if ( tls )
        {
        aDisplayRect = TRect( 10, 10 ,20 ,20 );
        tls->MediaManagerAction( CMCETls::EGetDisplayRect, &aSink );
        }
    }

// ---------------------------------------------------------------------------
// CMceMediaManager::SetDisplayRectL
// ---------------------------------------------------------------------------
//
void CMceMediaManager::SetDisplayRectL( CMceComDisplaySink& aSink, 
                                        TRect /*aDisplayRect*/ )
    {
    MCE_DEBUG1("CMceMediaManager::SetDisplayRectL");
    CMCETls* tls = CMCETls::Storage();
    if ( tls )
        {
        tls->MediaManagerAction( CMCETls::ESetDisplayRect, &aSink );
        }
    }

// ---------------------------------------------------------------------------
// CMceMediaManager::GetDisplayPriorityL
// ---------------------------------------------------------------------------
//
void CMceMediaManager::GetDisplayPriorityL( CMceComDisplaySink& aSink, 
                                            TInt& aDisplayPriority )
    {
    MCE_DEBUG1("CMceMediaManager::GetDisplayPriorityL");
    CMCETls* tls = CMCETls::Storage();
    if ( tls )
        {
        aDisplayPriority = 2;
        tls->MediaManagerAction( CMCETls::EGetDisplayPriority, &aSink );
        }
    }

// ---------------------------------------------------------------------------
// CMceMediaManager::SetDisplayPriorityL
// ---------------------------------------------------------------------------
//
void CMceMediaManager::SetDisplayPriorityL( CMceComDisplaySink& aSink, 
                                            TInt /*aDisplayPriority*/ )
    {
    MCE_DEBUG1("CMceMediaManager::SetDisplayPriorityL");
    CMCETls* tls = CMCETls::Storage();
    if ( tls )
        {
        tls->MediaManagerAction( CMCETls::ESetDisplayPriority, &aSink );
        }
    }   

// ---------------------------------------------------------------------------
// CMceMediaManager::SetDisplayIndexL
// ---------------------------------------------------------------------------
//
void CMceMediaManager::SetDisplayIndexL( CMceComDisplaySink& aSink, 
                                         TInt /*aDisplayIndex*/ )
    {
    MCE_DEBUG1("CMceMediaManager::SetDisplayIndexL");
    CMCETls* tls = CMCETls::Storage();
    if ( tls )
        {
        tls->MediaManagerAction( CMCETls::ESetDisplayIndex, &aSink );
        }
    }

// ---------------------------------------------------------
// CMceMediaManager::GetDisplayRotationL 
// --------------------------------------------------------- 
//   
void CMceMediaManager::GetDisplayRotationL( CMceComDisplaySink& aSink, 
                                            TInt& aDisplayRotation )
    {
    MCE_DEBUG1("CMceMediaManager::GetDisplayRotationL");
    CMCETls* tls = CMCETls::Storage();
    if ( tls )
        {
        aDisplayRotation = 1;
        tls->MediaManagerAction( CMCETls::EGetDisplayRotation, &aSink );
        }
    }

// ---------------------------------------------------------
// CMceMediaManager::SetDisplayRotationL 
// --------------------------------------------------------- 
//   
void CMceMediaManager::SetDisplayRotationL( CMceComDisplaySink& aSink, 
                                            TInt /*aDisplayRotation*/ )
    {
    MCE_DEBUG1("CMceMediaManager::SetDisplayRotationL");
    CMCETls* tls = CMCETls::Storage();
    if ( tls )
        {
        tls->MediaManagerAction( CMCETls::ESetDisplayRotation, &aSink );
        }
    }


// ---------------------------------------------------------
// CMceMediaManager::SetCNameL
// ---------------------------------------------------------    
//
void CMceMediaManager::SetCNameL( 
    const CMceComEndpoint& /*aEndpoint*/, 
    const TDesC8& /*aCName*/ )
    {
    //NOP
    }

// ---------------------------------------------------------
// CMceMediaManager::DtmfL
// ---------------------------------------------------------    
//
void CMceMediaManager::DtmfL( 
    CMceComMediaSource& aSource, 
    TMceComEvent& /*aEvent*/,
    const TDesC8& /*aSequence*/ )
    {
    MCE_DEBUG1("CMceMediaManager::DtmfL");
    CMCETls* tls = CMCETls::Storage();
    if ( tls )
        {
        tls->MediaManagerAction( CMCETls::EDtmf, &aSource );
        }
    }

// ---------------------------------------------------------------------------
// CMceMediaManager::GetZoomFactorL     
// ---------------------------------------------------------------------------
//
void CMceMediaManager::GetZoomFactorL( CMceComCameraSource& aSource, 
                             TInt& aZoomFactor )
    {
    
    MCE_DEBUG1("CMceMediaManager::GetZoomFactorL");
    CMCETls* tls = CMCETls::Storage();
    if ( tls )
        {
        aZoomFactor = 1;
        tls->MediaManagerAction( CMCETls::EGetZoomFactor, &aSource );
        }
    }

// ---------------------------------------------------------------------------
// CMceMediaManager::GetDigitalZoomFactorL  
// ---------------------------------------------------------------------------
//
void CMceMediaManager::GetDigitalZoomFactorL( CMceComCameraSource& aSource, 
                                    TInt& aDigitalZoomFactor )
    {
    
    MCE_DEBUG1("CMceMediaManager::GetDigitalZoomFactorL");
    CMCETls* tls = CMCETls::Storage();
    if ( tls )
        {
        aDigitalZoomFactor = 1;
        tls->MediaManagerAction( CMCETls::EGetDigitalZoomFactor, &aSource );
        }
    
    }

// ---------------------------------------------------------------------------
// CMceMediaManager::GetContrastL       
// ---------------------------------------------------------------------------
//
void CMceMediaManager::GetContrastL( CMceComCameraSource& aSource, 
                                     TInt& aContrast )
    {
    
    MCE_DEBUG1("CMceMediaManager::GetContrastL");
    CMCETls* tls = CMCETls::Storage();
    if ( tls )
        {
        aContrast = 1;
        tls->MediaManagerAction( CMCETls::EGetContrast, &aSource );
        }
      
    }

// ---------------------------------------------------------------------------
// CMceMediaManager::GetBrightnessL     
// ---------------------------------------------------------------------------
//
void CMceMediaManager::GetBrightnessL( CMceComCameraSource& aSource, 
                                       TInt& aBrightness )
    {
    
    MCE_DEBUG1("CMceMediaManager::GetBrightnessL");
    CMCETls* tls = CMCETls::Storage();
    if ( tls )
        {
        aBrightness = 1;
        tls->MediaManagerAction( CMCETls::EGetBrightness, &aSource );
        }
   
    }

// ---------------------------------------------------------------------------
// CMceMediaManager::GetExposureL   
// ---------------------------------------------------------------------------
//
void CMceMediaManager::GetExposureL( CMceComCameraSource& aSource, 
                                     TInt& aExposure )
    {
    
    MCE_DEBUG1("CMceMediaManager::GetExposureL");
    CMCETls* tls = CMCETls::Storage();
    if ( tls )
        {
        aExposure = 1;
        tls->MediaManagerAction( CMCETls::EGetExposure, &aSource );
        }
    
    }

// ---------------------------------------------------------------------------
// CMceMediaManager::GetWhiteBalanceL   
// ---------------------------------------------------------------------------
//
void CMceMediaManager::GetWhiteBalanceL( CMceComCameraSource& aSource, 
                                         TInt& aWhiteBalance )
    {
    
    MCE_DEBUG1("CMceMediaManager::GetWhiteBalanceL");
    CMCETls* tls = CMCETls::Storage();
    if ( tls )
        {
        aWhiteBalance = 1;
        tls->MediaManagerAction( CMCETls::EGetWhiteBalance, &aSource );
        }
    
    }

// ---------------------------------------------------------------------------
// CMceMediaManager::SetCameraIndexL    
// ---------------------------------------------------------------------------
//
void CMceMediaManager::SetCameraIndexL( CMceComCameraSource& aSource, 
                                        TInt aCameraIndex,
                                        TCameraInfo& aCameraInfo )
    {
    
    
    MCE_DEBUG1("CMceMediaManager::SetCameraIndexL");
    CMCETls* tls = CMCETls::Storage();
    if ( tls )
        {
        aCameraIndex = 1;
        aCameraInfo.iMinZoom = 0;
        aCameraInfo.iMaxZoom = 72;
        tls->MediaManagerAction( CMCETls::ESetCameraIndex, &aSource );
        }
    
    }
                              
// ---------------------------------------------------------------------------
// CMceMediaManager::SetZoomFactorL     
// ---------------------------------------------------------------------------
//
void CMceMediaManager::SetZoomFactorL( CMceComCameraSource& aSource, 
                                       TInt aZoomFactor )
    {
   
    MCE_DEBUG1("CMceMediaManager::SetZoomFactorL");
    CMCETls* tls = CMCETls::Storage();
    if ( tls )
        {
        aZoomFactor = 1;
        tls->MediaManagerAction( CMCETls::ESetZoomFactor, &aSource );
        }
    
    }

// ---------------------------------------------------------------------------
// CMceMediaManager::SetDigitalZoomFactorL  
// ---------------------------------------------------------------------------
//
void CMceMediaManager::SetDigitalZoomFactorL( CMceComCameraSource& aSource, 
                                              TInt aDigitalZoomFactor )
    {
    
    MCE_DEBUG1("CMceMediaManager::SetDigitalZoomFactorL");
    CMCETls* tls = CMCETls::Storage();
    if ( tls )
        {
        aDigitalZoomFactor = 1;
        tls->MediaManagerAction( CMCETls::ESetDigitalZoomFactor, &aSource );
        }
    
    }

// ---------------------------------------------------------------------------
// CMceMediaManager::SetContrastL   
// ---------------------------------------------------------------------------
//
void CMceMediaManager::SetContrastL( CMceComCameraSource& aSource, 
                                     TInt aContrast )
    {
    
    MCE_DEBUG1("CMceMediaManager::SetContrastL");
    CMCETls* tls = CMCETls::Storage();
    if ( tls )
        {
        aContrast = 1;
        tls->MediaManagerAction( CMCETls::ESetContrast, &aSource );
        }
    
    }

// ---------------------------------------------------------------------------
// CMceMediaManager::SetBrightnessL     
// ---------------------------------------------------------------------------
//
void CMceMediaManager::SetBrightnessL( CMceComCameraSource& aSource, 
                                       TInt aBrightness )
    {
    
    MCE_DEBUG1("CMceMediaManager::SetBrightnessL");
    CMCETls* tls = CMCETls::Storage();
    if ( tls )
        {
        aBrightness = 1;
        tls->MediaManagerAction( CMCETls::ESetBrightness, &aSource );
        }
   
    }

// ---------------------------------------------------------------------------
// CMceMediaManager::SetExposureL   
// ---------------------------------------------------------------------------
//
void CMceMediaManager::SetExposureL( CMceComCameraSource& aSource, 
                                     TInt aExposure )
    {
    
    MCE_DEBUG1("CMceMediaManager::SetExposureL");
    CMCETls* tls = CMCETls::Storage();
    if ( tls )
        {
        aExposure = 1;
        tls->MediaManagerAction( CMCETls::ESetExposure, &aSource );
        }
    
    }

// ---------------------------------------------------------------------------
// CMceMediaManager::SetWhiteBalanceL   
// ---------------------------------------------------------------------------
//
void CMceMediaManager::SetWhiteBalanceL( CMceComCameraSource& aSource, 
                                         TInt aWhiteBalance )
    {
    
    MCE_DEBUG1("CMceMediaManager::SetWhiteBalanceL");
    CMCETls* tls = CMCETls::Storage();
    if ( tls )
        {
        aWhiteBalance = 1;
        tls->MediaManagerAction( CMCETls::ESetWhiteBalance, &aSource );
        }
        
    }

// ---------------------------------------------------------------------------
// CMceMediaManager::GetFileInfoL
// ---------------------------------------------------------------------------
//
void CMceMediaManager::GetFileInfoL( CMceComFileSource& /*aSource*/, 
                                     TMceFileInfo& /*aFileInfo*/ )
    {
    // NOP
    }

// ---------------------------------------------------------------------------
// CMceMediaManager::GetFilePositionL
// ---------------------------------------------------------------------------
//
void CMceMediaManager::GetFilePositionL( CMceComFileSource& /*aSource*/, 
                                         TTimeIntervalMicroSeconds& /*aFilePosition*/ )
    {
    // NOP
    }

// ---------------------------------------------------------------------------
// CMceMediaManager::SetFilePositionL
// ---------------------------------------------------------------------------
//
void CMceMediaManager::SetFilePositionL( CMceComFileSource& /*aSource*/, 
                                         TTimeIntervalMicroSeconds& /*aFilePosition*/ )
    {
    // NOP
    }

// ---------------------------------------------------------------------------
// CMceMediaManager::FastForwardL
// ---------------------------------------------------------------------------
//
void CMceMediaManager::FastForwardL( CMceComFileSource& /*aSource*/, 
                                     TBool& /*aOnOff*/ )
    {
    // NOP
    }

// ---------------------------------------------------------------------------
// CMceMediaManager::FastRewindL
// ---------------------------------------------------------------------------
//
void CMceMediaManager::FastRewindL( CMceComFileSource& /*aSource*/, 
                                    TBool& /*aOnOff*/ )
    {
    // NOP
    }

// ---------------------------------------------------------------------------
// CMceMediaManager::GetCameraInfoL
// ---------------------------------------------------------------------------
//                              
void CMceMediaManager::GetCameraInfoL( TInt /*aCameraIndex*/, 
                                       TCameraInfo& /*aCameraInfo*/ )


    {
    // NOP
    }

// ---------------------------------------------------------------------------
// CMceMediaManager::GetCamerasAvailableL
// ---------------------------------------------------------------------------
//                              
void CMceMediaManager::GetCamerasAvailableL( TInt& /*aCamerasAvailable*/ )
    {
    // NOP
    }

// ---------------------------------------------------------------------------
// CMceMediaManager::GetDisplaysAvailableL
// ---------------------------------------------------------------------------
//                               
void CMceMediaManager::GetDisplaysAvailableL( TInt& /*aDisplaysAvailable*/ )
    {
    // NOP
    }

// ---------------------------------------------------------------------------
// CMceMediaManager::GetFileInfoL
// ---------------------------------------------------------------------------
//                               
void CMceMediaManager::GetFileInfoL( TMceFileInfo& /*aFileInfo*/ )
    {
    // NOP
    }


// ---------------------------------------------------------------------------
// CMceMediaManager::SetFileInfoL
// ---------------------------------------------------------------------------
//                               
void CMceMediaManager::SetFileInfoL( CMceComFileSource& /*aSource*/, 
                                     TMceFileInfo& /*aFileInfo*/ )
    {
    // NOP
    }

// ---------------------------------------------------------------------------
// CMceMediaManager::TranscodeFileL
// ---------------------------------------------------------------------------
//                               
TUint32 CMceMediaManager::TranscodeFileL( CMceComFileSource* aFile )
    {
    delete aFile; // Ownership taken
    return 0;
    }

// ---------------------------------------------------------------------------
// CMceMediaManager::CancelTranscodeFileL
// ---------------------------------------------------------------------------
//                               
void CMceMediaManager::CancelTranscodeFileL( TUint32 /*aSessionId*/ )
    {
    // NOP
    }

// ---------------------------------------------------------------------------
// CMceMediaManager::SetMccParameterL
// ---------------------------------------------------------------------------
//     
void CMceMediaManager::SetMccParameterL( 
    const CMceComMediaSink& /*aMediaSink*/,
    TUint32 /*aParam*/,
    const TDesC8& /*aValue*/,
    TBool /*aAllowNonPrepared*/ ) const
    {
    }
    
/*
** From MMcpCtrlObserver ***
*/

// ---------------------------------------------------------
// CMceMediaManager::MccMediaSignalReceived
// --------------------------------------------------------- 
//   	
void CMceMediaManager::MccEventReceived( const TMccEvent& /*aEvent*/)
	{
	}
			                             

// ---------------------------------------------------------
// CMceMediaManager::MccCtrlError
// ---------------------------------------------------------   
// 	

void CMceMediaManager::MccCtrlError( TInt /*aError*/,
                                     TUint32 /*aSessionId*/,
                                     TUint32 /*aLinkId*/,
                                     TUint32 /*aStreamId*/,
                                     TUint32 /*aSinkSourceId*/ )
    {
    }

// ---------------------------------------------------------
// CMceMediaManager::MccMediaActive
// ---------------------------------------------------------    
//  

void CMceMediaManager::MccMediaActive( TUint32 /*aSessionId*/,
                                       TUint32 /*aLinkId*/,
                                       TUint32 /*aStreamId*/,
                                       TUint32 /*aSinkSourceId*/ )
    {
    }

// ---------------------------------------------------------
// CMceMediaManager::MccMediaInactive
// ---------------------------------------------------------    
//  

void CMceMediaManager::MccMediaInactive( TUint32 /*aSessionId*/,
                                         TUint32 /*aLinkId*/,
                                         TUint32 /*aStreamId*/,
                                         TUint32 /*aSinkSourceId*/ )
    {
    }
    
// ---------------------------------------------------------
// CMceMediaManager::MccMediaResumed
// --------------------------------------------------------- 
//      
void CMceMediaManager::MccMediaResumed( TUint32 /*aSessionId*/,
                                        TUint32 /*aLinkId*/,
                                        TUint32 /*aStreamId*/,
                                        TUint32 /*aSinkSourceId*/ )
    {
    }

// ---------------------------------------------------------
// CMceMediaManager::MccMediaPaused
// ---------------------------------------------------------  
//      
void CMceMediaManager::MccMediaPaused( TUint32 /*aSessionId*/,
                                       TUint32 /*aLinkId*/,
                                       TUint32 /*aStreamId*/,
                                       TUint32 /*aSinkSourceId*/ )
    {
    }

// ---------------------------------------------------------
// CMceMediaManager::MccMediaStopped
// --------------------------------------------------------- 
//      
void CMceMediaManager::MccMediaStopped( TUint32 /*aSessionId*/,
                                        TUint32 /*aLinkId*/,     
                                        TUint32 /*aStreamId*/,
                                        TUint32 /*aSinkSourceId*/ )
    {
    }

// ---------------------------------------------------------
// CMceMediaManager::MccMediaStarted
// --------------------------------------------------------- 
//   
void CMceMediaManager::MccMediaStarted( TUint32 /*aSessionId*/,
                                        TUint32 /*aLinkId*/,     
                                        TUint32 /*aStreamId*/,
                                        TUint32 /*aSinkSourceId*/ )
    {
    }

// ---------------------------------------------------------
// CMceMediaManager::MccMediaPrepared 
// ---------------------------------------------------------    
//
void CMceMediaManager::MccMediaPrepared( TUint32 /*aSessionId*/,
                                         TUint32 /*aLinkId*/,
                                         TUint32 /*aStreamId*/,
                                         TUint32 /*aSinkSourceId*/ )
    {
    }

// ---------------------------------------------------------
// CMceMediaManager::SetRemoteAddressL 
// ---------------------------------------------------------    
//
void CMceMediaManager::SetRemoteAddressL( CMceSrvStream& /*aStream*/ )
    {
    }

// ---------------------------------------------------------
// CMceMediaManager::GetSSRCL 
// ---------------------------------------------------------    
//
TUint32 CMceMediaManager::GetSSRCL( CMceSrvStream& /*aStream*/ )
    {
    return 0;
    }

// ---------------------------------------------------------
// CMceMediaManager::ReuseLink 
// ---------------------------------------------------------    
//                
TBool CMceMediaManager::ReuseLink( CMceSrvStream& /*aStream*/ )
    {
    return EFalse;
    }

// ---------------------------------------------------------
// CMceMediaManager::ReuseSource 
// ---------------------------------------------------------    
//    
TBool CMceMediaManager::ReuseSource( CMceSrvStream& /*aStream*/ )
    {
    return EFalse;
    }

// ---------------------------------------------------------
// CMceMediaManager::ReuseSink 
// ---------------------------------------------------------    
//    
TBool CMceMediaManager::ReuseSink( CMceSrvStream& /*aStream*/ )
    {
    return EFalse;
    }

// ---------------------------------------------------------
// CMceMediaManager::CanReleaseEndpoint 
// ---------------------------------------------------------    
//     
TBool CMceMediaManager::CanReleaseEndpoint( 
    CMceSrvStream& /*aStream*/, TUint32 /*aEndpointId*/ )
    {
    return ETrue;
    }

// ---------------------------------------------------------
// CMceMediaManager::ReleaseSink 
// ---------------------------------------------------------    
//
TInt CMceMediaManager::ReleaseSink( 
    CMceSrvStream& /*aStream*/, TUint32 /*aEndpointId*/ )
    {
    return 0;
    }

// ---------------------------------------------------------
// CMceMediaManager::ReleaseSource 
// ---------------------------------------------------------    
//
TInt CMceMediaManager::ReleaseSource( 
    CMceSrvStream& /*aStream*/, TUint32 /*aEndpointId*/ )
    {
    return 0;
    }
    

// ---------------------------------------------------------
// CMceMediaManager::UpdateConfigKeyL 
// ---------------------------------------------------------    
//
    
void CMceMediaManager::UpdateConfigKeyL( const CMceComMediaSource& /*aSource*/ )    
	{
	//Dummy Allocation of memory
	const TInt KMaxConfigKeyLen = 100;
	HBufC8* configKey = HBufC8::NewLC( KMaxConfigKeyLen );
	CleanupStack::PopAndDestroy( configKey );		
	}
	
// ---------------------------------------------------------
// CMceMediaManager::UpDateStreamStateL 
// ---------------------------------------------------------    
//
void CMceMediaManager::UpDateStreamStateL(CMceComSession& /*currentSession*/,
		                    CMceComSession& /*forkSession*/ )
	{
	}
	
// ---------------------------------------------------------
// CMceMediaManager::ForceEnableSinkState
// ---------------------------------------------------------    
//	
void CMceMediaManager::ForceEnableSinkState(CMceComSession& /*aSession*/ )
	{
	}	
	

// ---------------------------------------------------------
// CMceMediaManager::SdpCodec
// ---------------------------------------------------------
CMceSdpCodec* CMceMediaManager::SdpCodec( const CSdpMediaField& /*aMedia*/ )
    {    
    return 0;
    }

// ---------------------------------------------------------
// CMceMediaManager::SdpCodec
// ---------------------------------------------------------
CMceSdpCodec* CMceMediaManager::SdpCodec( CMceComMediaStream& /*aStream*/ )
    {    
    return 0;
    }

// ---------------------------------------------------------
// CMceMediaManager::CreateSessionL
// ---------------------------------------------------------    
CMceSdpSession& CMceMediaManager::CreateSessionL( CMceComSession& aSession )
    {
    CMceSdpSession* sdpSession = CMceSdpSession::NewL( aSession, *this );
    CleanupStack::PushL( sdpSession );

    aSession.AttachSDPSessionL( *sdpSession );
    User::LeaveIfError (iMceSdpSessions.Append( sdpSession ));
    CleanupStack::Pop( sdpSession ); 

    return *sdpSession;
    }

// ---------------------------------------------------------
// CMceMediaManager::SynchronizeMccStreamL 
// ---------------------------------------------------------    
//
void CMceMediaManager::SynchronizeMccStreamL( 
    CMceSrvStream& /*aStream*/, 
    TMceNegotiationRole /*aRole*/,
    TBool /*aSyncCodecOnly*/ )
    {
    }
    
// ---------------------------------------------------------
// CMceMediaManager::SynchronizeMccStreamL 
// ---------------------------------------------------------    
//
void CMceMediaManager::SynchronizeMccStreamL( 
    CMceSrvStream& /*aStream*/, 
    TUint32 /*aForceEnableEndpoint*/, 
    TUint32 /*aForceDisableEndpoint*/ )
    {
    }

// ---------------------------------------------------------
// CMceMediaManager::CreateMccSessionL
// ---------------------------------------------------------    
//
void CMceMediaManager::CreateMccSessionL( CMceComSession& /*aSession*/ )
    {
    }

// ---------------------------------------------------------
// CMceMediaManager::CreateMccLinkL
// ---------------------------------------------------------    
//
void CMceMediaManager::CreateMccLinkL( CMceSrvStream& /*aStream*/ )
    {      
    }

// ---------------------------------------------------------
// CMceMediaManager::CreateMccStreamL
// ---------------------------------------------------------    
//
void CMceMediaManager::CreateMccStreamL( 
            CMceSrvStream& /*aStream*/,
            TMceNegotiationRole /*aRole*/ )
    {
    }

// ---------------------------------------------------------
// CMceMediaManager::InitializeMccStreamL
// ---------------------------------------------------------    
//
void CMceMediaManager::InitializeMccStreamL( CMceSrvStream& /*aStream*/ )
    {
    }

// ---------------------------------------------------------
// CMceMediaManager::ValidateMccStreamL
// ---------------------------------------------------------   
//
void CMceMediaManager::ValidateMccStreamL( CMceSrvStream& /*aStream*/ )
    {
    }
// ---------------------------------------------------------
// CMceMediaManager::AdjustMccStreamL
// ---------------------------------------------------------   
//
void CMceMediaManager::AdjustMccStreamL( CMceSrvStream& /*aStream*/ )
    {    
    }

// ---------------------------------------------------------
// CMceMediaManager::PrepareMccStreamL
// ---------------------------------------------------------    
//
void CMceMediaManager::PrepareMccStreamL( 
            CMceSrvStream& /*aStream*/,
            TMceNegotiationRole /*aRole*/ )
    {
    }

// ---------------------------------------------------------
// CMceMediaManager::StartMccStreamL
// ---------------------------------------------------------   
//
void CMceMediaManager::StartMccStreamL( CMceSrvStream& /*aStream*/ )
    {

    }

// ---------------------------------------------------------
// CMceMediaManager::PrepareUpdateL 
// ---------------------------------------------------------    
//
void CMceMediaManager::PrepareUpdateL( CMceSdpSession& /*aSession*/ )
    {
    }

// ---------------------------------------------------------
// CMceMediaManager::CleanupL
// ---------------------------------------------------------   
//
void CMceMediaManager::CleanupL( CMceComSession& /*aSession*/ )
    {
    }

// End Of File
	

