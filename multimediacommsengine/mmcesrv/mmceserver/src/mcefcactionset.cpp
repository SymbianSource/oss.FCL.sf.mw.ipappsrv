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



#include <e32math.h>
#include <sdpfmtattributefield.h>
#include "mcefcactionset.h"
#include "mcesipsession.h"
#include "mcesipconnection.h"
#include "mcemediamanager.h"
#include "mceservercore.h"
#include "fcpluginengine.h"
#include "mceaudiostream.h"
#include "mcertpsource.h"
#include "mcecommediasource.h"
#include "mceserial.h"
#include "mcesrvlogs.h"

_LIT8( KFCTBCP, "TBCP" );
const TUint32 KPortZero = 0;

// -----------------------------------------------------------------------------
// TMceFCActionSet::TMceFCActionSet
// -----------------------------------------------------------------------------
//
TMceFCActionSet::TMceFCActionSet( CMceSipSession& aSession )
  : iSession ( aSession )
	{
	}

// -----------------------------------------------------------------------------
// TMceFCActionSet::UpdateFCOfferL
// -----------------------------------------------------------------------------
//
void TMceFCActionSet::UpdateFCOfferL( CSdpDocument& aOffer )
    {
	MCESRV_DEBUG("TMceFCActionSet::UpdateFCOfferL, Entry");
    if ( iSession.FCSession() && iSession.UseFC() )
    	{
    	User::LeaveIfError( iSession.Body() ? KErrNone : KErrArgument );
        User::LeaveIfError( iSession.Body()->Streams().Count() > 0 ? 
            KErrNone : KErrArgument );
        
        CFCPlugInEngine& fc = iSession.ServerCore().FCPluginEngine();
        
        if ( !fc.IsReceiverL( *iSession.FCSession() ) )
            {
            fc.SetSessionPortL( *iSession.iFCSession, RTCPPort() );
            }
       	fc.UpdateOfferL( *iSession.iFCSession, aOffer );    	
    	}    
	MCESRV_DEBUG("TMceFCActionSet::UpdateFCOfferL, Exit");
    }

// -----------------------------------------------------------------------------
// TMceFCActionSet::UpdateFCAnswerL
// -----------------------------------------------------------------------------
//
void TMceFCActionSet::UpdateFCAnswerL( CSdpDocument& aOffer )
    {
	MCESRV_DEBUG("TMceFCActionSet::UpdateFCAnswerL, Entry");
    if ( iSession.FCSession() && iSession.UseFC() )
    	{
    	User::LeaveIfError( iSession.Body() ? KErrNone : KErrArgument );
        User::LeaveIfError( iSession.Body()->Streams().Count() > 0 ? 
            KErrNone : KErrArgument );
        
    	CFCPlugInEngine& fc = iSession.ServerCore().FCPluginEngine();
    	
    	if ( !fc.IsReceiverL( *iSession.FCSession() ) )
            {
            fc.SetSessionPortL( *iSession.iFCSession, RTCPPort() );
            }
    	fc.UpdateAnswerL( *iSession.iFCSession, aOffer );    	
    	}    
	MCESRV_DEBUG("TMceFCActionSet::UpdateFCAnswerL, Exit");
    }

// -----------------------------------------------------------------------------
// TMceFCActionSet::PeekFCDocumentL
// -----------------------------------------------------------------------------
//
void TMceFCActionSet::PeekFCDocumentL( CSdpDocument& aOffer )
    {
	MCESRV_DEBUG("TMceFCActionSet::PeekFCDocumentL, Entry");
    if ( iSession.FCSession() )
    	{
    	CFCPlugInEngine& fc = iSession.ServerCore().FCPluginEngine();
    	fc.PeekDocumentL( *iSession.FCSession(), aOffer );    	
    	}    
	MCESRV_DEBUG("TMceFCActionSet::PeekFCDocumentL, Exit");
    }

// -----------------------------------------------------------------------------
// TMceFCActionSet::StartFCL
// -----------------------------------------------------------------------------
//
void TMceFCActionSet::StartFCL()
    {
	MCESRV_DEBUG("TMceFCActionSet::StartFCL, Entry");
    if ( iSession.FCSession() && iSession.UseFC() )
    	{
    	CFCPlugInEngine& fc = iSession.ServerCore().FCPluginEngine();
    	fc.StartConnectionL( *iSession.FCSession() );    	
    	}    
	MCESRV_DEBUG("TMceFCActionSet::StartFCL, Exit");
    }
   
// -----------------------------------------------------------------------------
// TMceFCActionSet::InitializeFCL
// -----------------------------------------------------------------------------
//
void TMceFCActionSet::InitializeFCL ()
	{	
	MCESRV_DEBUG("TMceFCActionSet::InitializeFCL, Entry");
    MCESRV_DEBUG("FC session initalization for incoming INVITE");
    	
	__ASSERT_ALWAYS( !iSession.FCSession(), User::Leave( KErrArgument ) );
	                                             
	iSession.iFCSession = &(iSession.ServerCore().FCPluginEngine().
	    CreateNewSessionL( KFCTBCP, 
	                       iSession.SIPConnection().AccessPointId(),
	                       KPortZero ) );
	
	iSession.ServerCore().
	    RegisterFCSessionL( iSession, *iSession.FCSession() );
	MCESRV_DEBUG("TMceFCActionSet::InitializeFCL, Exit");
	}

// -----------------------------------------------------------------------------
// TMceFCActionSet::InitializeFCL
// -----------------------------------------------------------------------------
//
void TMceFCActionSet::InitializeFCL (TMceIds& aIds, const TDesC8& aType )
	{
	MCESRV_DEBUG("TMceFCActionSet::InitializeFCL, Entry");
	    
	TUint32 port = aIds.Get( KMceIdsIndexFCPort );
	
    MCESRV_DEBUG_DVALUE("FC session initalization with port", port );
    	
	__ASSERT_ALWAYS( !iSession.FCSession(), User::Leave( KErrArgument ) );
	    
    iSession.iFCSession = &(iSession.ServerCore().FCPluginEngine().
	    CreateNewSessionL( aType, 
	                       iSession.SIPConnection().AccessPointId(),
	                       port ) ); 

	iSession.ServerCore().
	    RegisterFCSessionL( iSession, *iSession.FCSession() );
	
	MCESRV_DEBUG("TMceFCActionSet::InitializeFCL, Exit");
	}

// -----------------------------------------------------------------------------
// TMceFCActionSet::UpdateFC
// -----------------------------------------------------------------------------
//
void TMceFCActionSet::UpdateFCL( TMceIds& aIds, const TDesC8& /*aType*/ )
    {
	MCESRV_DEBUG("TMceFCActionSet::UpdateFCL, Entry");
	
    TUint32 port = aIds.Get( KMceIdsIndexFCPort );
    
	MCESRV_DEBUG_DVALUE("FC session updating with port", port)
		
	__ASSERT_ALWAYS( MCE_NOT_NULL_PTR( iSession.FCSession() ) , User::Leave( KErrArgument ) );
	iSession.ServerCore().FCPluginEngine().
	    UpdateSessionL( *iSession.iFCSession, port );
	
	MCESRV_DEBUG("TMceFCActionSet::UpdateFCL, Exit");
	
    }

// -----------------------------------------------------------------------------
// TMceFCActionSet::Release
// -----------------------------------------------------------------------------
//
void TMceFCActionSet::ReleaseFC()
    {
	MCE_DEBUG2("TMceFCActionSet::ReleaseFC", "Entry")
	MCESRV_DEBUG("TMceFCActionSet::ReleaseFC, Entry");
		
	if ( iSession.FCSession() )
	    {
    	iSession.ServerCore().FCPluginEngine().
    	    ReleaseSession( *iSession.iFCSession);

        iSession.iFCSession = NULL;
	    }
	MCE_DEBUG2("TMceFCActionSet::ReleaseFC", "Exit")
	    
	MCESRV_DEBUG("TMceFCActionSet::ReleaseFC, Exit");
	
    }

// -----------------------------------------------------------------------------
// TMceFCActionSet::FCGetMediaLinesL
// -----------------------------------------------------------------------------
//
HBufC8* TMceFCActionSet::FCGetMediaLinesL()
	{		
	MCESRV_DEBUG("TMceFCActionSet::FCGetMediaLinesL, Entry");
	
	HBufC8* returnMessage = NULL;
	
	RPointerArray<CSdpFmtAttributeField> attribs = iSession.ServerCore().
		FCPluginEngine().FormatAttributeFieldsL( *iSession.FCSession() );

	CDesC8ArrayFlat* mediaLines = 
	    new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
	CleanupStack::PushL( mediaLines );
	
	for ( int i = 0; i < attribs.Count(); i++ )
		{
		HBufC8* mediaLine = HBufC8::NewLC( KMceSdpMaxMediaLineLength );
		TPtr8 mediaLinePtr = mediaLine->Des();
		RDesWriteStream writeStream( mediaLinePtr );
		writeStream.PushL();

		CSdpFmtAttributeField* field = attribs[ i ];
		field->EncodeL( writeStream );
		
		TInt len = writeStream.Sink()->TellL(MStreamBuf::EWrite).Offset();
		mediaLine->Des().SetLength( len );
		mediaLines->AppendL( mediaLine->Des() );
		
		CleanupStack::PopAndDestroy();//writeStream
		CleanupStack::PopAndDestroy( mediaLine );//mediaLine				
		}

    CMceMsgTextArray* msgLines = new (ELeave) CMceMsgTextArray( *mediaLines );
    CleanupStack::PushL( msgLines );
    msgLines->EncodeL();
    
    returnMessage = msgLines->EncodeBuffer().Ptr(0).AllocL();

	CleanupStack::PopAndDestroy(msgLines); //mediaLines	
	CleanupStack::PopAndDestroy(mediaLines); //mediaLines	

	MCESRV_DEBUG("TMceFCActionSet::FCGetMediaLinesL, Exit");
	
	return returnMessage;
	
	}


// -----------------------------------------------------------------------------
// TMceFCActionSet::FCSetMediaLinesL
// -----------------------------------------------------------------------------
//
void TMceFCActionSet::FCSetMediaLinesL( CMceMsgBase& aMediaLines )
	{
	MCESRV_DEBUG("TMceFCActionSet::FCSetMediaLinesL, Entry");
	
    CMceMsgTextArray* msgLines = static_cast<CMceMsgTextArray*> (&aMediaLines );

    RPointerArray<CSdpFmtAttributeField>* attribs = 
        new (ELeave) RPointerArray<CSdpFmtAttributeField>;
	CleanupStack::PushL( TCleanupItem( ArrayCleanup, attribs ) );	
    for( int i=0;i<msgLines->iArray->Count();i++)
        {
        attribs->AppendL( 
            CSdpFmtAttributeField::DecodeL( msgLines->iArray->MdcaPoint(i) ) );
        }

	iSession.ServerCore().FCPluginEngine().
	   SetFormatAttributeFieldsL( *iSession.FCSession(), attribs );
	CleanupStack::Pop( attribs ); //attribs
	    	
    msgLines->Close();
	
	MCESRV_DEBUG("TMceFCActionSet::FCSetMediaLinesL, Exit");
	}

// -----------------------------------------------------------------------------
// TMceFCActionSet::SendFCMsgL
// -----------------------------------------------------------------------------
//
void TMceFCActionSet::SendFCMsgL ( const TDesC8& aMessage )
    {
    MCESRV_DEBUG("TMceFCActionSet::SendFCMsgL, Entry");
    User::LeaveIfError( iSession.FCSession() ? KErrNone : KErrArgument );
    User::LeaveIfError( iSession.Body() ? KErrNone : KErrArgument );
    
    if ( iSession.ServerCore().FCPluginEngine().
        IsReceiverL( *iSession.FCSession() ) )
        {
        // Send using FC subsystem.
    	HBufC8* sesbuf = aMessage.AllocLC();
		
	    iSession.ServerCore().FCPluginEngine().SendToNetL( 
	                                                *iSession.FCSession(), 
	                                                sesbuf );
        CleanupStack::Pop( sesbuf ); //sesbuf	
        }
    else
        {   
        CMceMediaManager& mediaManager = iSession.Manager().MediaManager();
    	CMceComMediaStream* stream = NULL;
    	// go through streams and possible bound 
    	// streams and find first uplink stream.
    	TInt index = 0;
    	while( !stream && index < iSession.Body()->Streams().Count() )
            {
	        stream = iSession.Body()->Streams()[ index++ ];
            if ( stream->iType == KMceAudio )
            	{
            	stream = stream->SendStream();
            	}
            else
                {
                stream = NULL;
                }
            }
    
        if ( stream )
            {
            mediaManager.SendRTCPAnyDataL( *stream, aMessage );
            }
	else
	    {
	    User::Leave( KErrNotSupported );
	    }
	}
    MCESRV_DEBUG("TMceFCActionSet::SendFCMsgL, Exit");
    }


// -----------------------------------------------------------------------------
// TMceFCActionSet::NeedToSendOffer
// -----------------------------------------------------------------------------
//
TBool TMceFCActionSet::NeedToSendOffer()
    {
    MCESRV_DEBUG("TMceFCActionSet::NeedToSendOffer, Entry");
    
    TBool needToSendOffer = EFalse;
    const CFCSession* fcSession = iSession.FCSession();
    
    //if fc session is not in use fc will return ETrue by default
    if ( fcSession && iSession.UseFC() )

        {
        CFCPlugInEngine& fc = iSession.ServerCore().FCPluginEngine();
        
        
        TRAPD( error, needToSendOffer = fc.UpdateNeededL( *fcSession ) );
        
        if ( error != KErrNone )
            {
            MCESRV_DEBUG_DVALUE("TMceFCActionSet::NeedToSendOffer, ERROR:", error );
            needToSendOffer = EFalse;
            }
        }
	
	MCESRV_DEBUG("TMceFCActionSet::NeedToSendOffer, Exit");
    return needToSendOffer;
    }


// -----------------------------------------------------------------------------
// TMceFCActionSet::RTCPPort
// -----------------------------------------------------------------------------
//
TUint TMceFCActionSet::RTCPPort()
    {
    CMceComMediaStream* stream = NULL;
    if ( iSession.BodyCandidate() )
        {
        stream = iSession.BodyCandidate()->Streams()[0];
        }
    else
        {
        stream = iSession.Body()->Streams()[0];
        }
    return stream->iLocalMediaPort + 1;
    }
// -----------------------------------------------------------------------------
// TMceFCActionSet::ArrayCleanup
// -----------------------------------------------------------------------------
//
void TMceFCActionSet::ArrayCleanup(TAny* aArray)
	{
    RPointerArray<CSdpFmtAttributeField>* array = 
        reinterpret_cast<RPointerArray<CSdpFmtAttributeField>*>(aArray);
    if (array)
        {
        array->ResetAndDestroy();
        array->Close();
        delete array;
        }
	}
	

// End of File

