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
* Description:    Provides services for SDP Security Descriptions.
*
*/




// INCLUDE FILES

#include <e32base.h>
#include <e32std.h>

#include <sdporiginfield.h>
#include <sdpconnectionfield.h>
#include <sdpmediafield.h>
#include <sdpattributefield.h>
#include <sdpcodecstringpool.h>
#include <sdpdocument.h>
#include "mcesrvstream.h"
#include "mcecomcodec.h"
#include "mcemmlogs.h"
#include "mcesecuremediasession.h"
#include "mcesecuredesstream.h"
#include "mceclientserver.h"


// ================= MEMBER FUNCTIONS ==========================================

// -----------------------------------------------------------------------------
// CMceSecureMediaSession::CMceSecureMediaSession
// -----------------------------------------------------------------------------
CMceSecureMediaSession::CMceSecureMediaSession( CMceComSession& aSession,
                                     CMceMediaManager& aManager,
                                     CMccSecureInterface& aSecureInterface)
    :       iManager( aManager ),
            iSession( aSession ),
            iSecureInterface(aSecureInterface),
            iKeyNeedUpdated( ETrue ),
            iCryptoContextUpdate (EFalse),
            iLSReadyToBind ( ETrue ),
            iStringTable( NULL )
    {
    
    }

// -----------------------------------------------------------------------------
// CMceSecureMediaSession::ConstructL
// -----------------------------------------------------------------------------
void CMceSecureMediaSession::ConstructL()
    {
    }


// -----------------------------------------------------------------------------
// CMceSecureMediaSession::NewL
// -----------------------------------------------------------------------------
 CMceSecureMediaSession* CMceSecureMediaSession::NewL( CMceComSession& aSession,
                                     CMceMediaManager& aManager,
                                     CMccSecureInterface& aSecureInterface )
    {
    CMceSecureMediaSession* self = NewLC( aSession, aManager, aSecureInterface );
    CleanupStack::Pop( self ); 
    return self;
    }


// -----------------------------------------------------------------------------
// CMceSecureMediaSession::NewLC
// -----------------------------------------------------------------------------
 CMceSecureMediaSession* CMceSecureMediaSession::NewLC( CMceComSession& aSession,
                                       CMceMediaManager& aManager,
                                       CMccSecureInterface& aSecureInterface )
    {
    CMceSecureMediaSession* self = new ( ELeave ) CMceSecureMediaSession( aSession, 
    																	aManager,
    																	aSecureInterface );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// -----------------------------------------------------------------------------
// CMceSecureMediaSession::~CMceSecureMediaSession
// -----------------------------------------------------------------------------    
 CMceSecureMediaSession::~CMceSecureMediaSession()
    {
    //Remove secure streams
    iMceSecureDesStreams.ResetAndDestroy();
	}


// -----------------------------------------------------------------------------
// CMceSecureMediaSession::
// -----------------------------------------------------------------------------
//
void CMceSecureMediaSession::EncodeSecureDesSdpOfferL(
										CMceComMediaStream& aStream,		
										CSdpMediaField& aMediaLine)
    {
    
    MCEMM_DEBUG("CMceSecureMediaSession::(), Entry ");
    User::LeaveIfNull( &aMediaLine );
    
    // find media lines and create sec stream and add security attribute
    CMceSecureDesStream* secStream=NULL;   
	
	if ( iMceSecureDesStreams.Count()!=0 )
		{
		//find sec stream
	    TInt secStreamCount = iMceSecureDesStreams.Count();
	    for (TInt j=0; j<secStreamCount; j++)
	    	{
	    	CMceSecureDesStream* secDesStream = iMceSecureDesStreams[j];
	    	if (IfStreamMatchMedia(aStream, *secDesStream, aMediaLine) )
	    		{
	    		//out of loop
	    		j=secStreamCount;
	    		secStream = secDesStream;
	    		//remove or clean secure sdp params if any 
	    		secStream->RemvoeSecureSdp(aMediaLine);
	    		}
	    	}
		}
	if (!secStream)
		{
	    //create sec stream depends on how many media lines
		/*RFC 4568 7.1.1
		The inline parameter conveys the SRTP master key used by an endpoint
		to encrypt the SRTP and SRTCP streams transmitted by that endpoint.
		The same key is used by the recipient to decrypt those streams.
		*/
		secStream = CMceSecureDesStream::NewL(
								*this,
								aMediaLine,
								iSecureInterface,
								aStream);
		CleanupStack::PushL(secStream);						
        iMceSecureDesStreams.AppendL(secStream);
		CleanupStack::Pop(secStream);
		}
	//Encode sdp
	if (secStream)
		{
		secStream->EncodeSecureSdpL(aMediaLine, EFalse);
		}
	
    MCEMM_DEBUG("CMceSecureMediaSession::(), Exit ");
    
    }

// -----------------------------------------------------------------------------
// CMceSecureMediaSession::DecodeSecureDesSdpAnswerL
// -----------------------------------------------------------------------------
//
void CMceSecureMediaSession::DecodeSecureDesSdpAnswerL( CMceComMediaStream& aStream,
                                                        CSdpMediaField& aMediaLine)
    {
    MCEMM_DEBUG("CMceSecureMediaSession::DecodeSecureDesSdpAnswerL(), Entry ");
    // decode media lines for answer
    
    User::LeaveIfNull( &aMediaLine );
   
    if ( IsSecureSdpMsgL( aMediaLine ) )
    	{
       	TBool foundCryptoIn=EFalse;	
	   
	    CMceSecureDesStream* secStream=NULL;   
		//one uplink stream for this codec must be set already
		TInt secStreamCount = iMceSecureDesStreams.Count();
		for (TInt i=0; i<secStreamCount; i++)
	    	{
	    	CMceSecureDesStream* secDesStream = iMceSecureDesStreams[i];
	    	if (IfStreamMatchMedia(aStream, *secDesStream, aMediaLine))
	    		{
	    		i=secStreamCount;
	    		secStream = secDesStream;
	    		foundCryptoIn=ETrue;
	    		}
	    	}
	    	//create sec stream depends on how many media lines
		if (!foundCryptoIn && (aStream.iLocalMediaPort == aMediaLine.Port()||
				aStream.iRemoteMediaPort ==aMediaLine.Port()) )
			{
			secStream = CMceSecureDesStream::NewL(*this,
										aMediaLine,
									iSecureInterface,
									aStream);
							
			iMceSecureDesStreams.Append(secStream);						
			}

		//Decode sdp
		if (secStream)
			{
            TRAPD( err, secStream->DecodeSecureSdpAnswerL( aMediaLine ) );
            if ( err )
                {
                TInt index = iMceSecureDesStreams.Find( secStream );
                if ( KErrNotFound != index )
                    {
                    iMceSecureDesStreams.Remove( index );
                    }
                delete secStream;
                secStream = NULL;
                User::Leave( err );
                }
            }
    	}
    else
    	{
    	RemoveSecureCrypto( );
    	//Expected secure sdp answer but not found. leave to end the sesssion
    	User::Leave( KErrArgument );
    	}

    MCEMM_DEBUG("CMceSecureMediaSession::DecodeSecureDesSdpAnswerL(), Exit ");
    
    }

// -----------------------------------------------------------------------------
// CMceSecureMediaSession::DecodeSecureDesSdpOfferL
// -----------------------------------------------------------------------------

void CMceSecureMediaSession::DecodeSecureDesSdpOfferL( CMceComMediaStream& aStream,
														CSdpMediaField& aMediaLine)
    {
    MCEMM_DEBUG("CMceSecureMediaSession::DecodeSecureDesSdpOfferL(), Entry ");
    
    User::LeaveIfNull( &aMediaLine );
   	
   	TBool foundCryptoIn = EFalse;	
   	
    if ( IsSecureSdpMsgL( aMediaLine ) )
    	{
	    CMceSecureDesStream* secStream=NULL;                            
		/*RFC 4568 7.1.1
		The inline parameter conveys the SRTP master key used by an endpoint
		to encrypt the SRTP and SRTCP streams transmitted by that endpoint.
		The same key is used by the recipient to decrypt those streams.
		*/
		TInt secStreamCount = iMceSecureDesStreams.Count();
		for (TInt i=0; i<secStreamCount; i++)
			{
			
	    	CMceSecureDesStream* secDesStream = iMceSecureDesStreams[i];
	    	if (IfStreamMatchMedia(aStream, *secDesStream, aMediaLine))
	    		{
	    		i=secStreamCount;
	    		secStream = secDesStream;
	    		foundCryptoIn=ETrue;
	    		}
	    			
			}
	    
	    	//create sec stream depends on how many media lines
		if (!foundCryptoIn)
			{
			secStream = CMceSecureDesStream::NewL( *this,
													aMediaLine,
													iSecureInterface,
													aStream);
									
			iMceSecureDesStreams.Append( secStream );						
			}
		
			//Decode sdp
		if (secStream)
			{
            TRAPD( err, secStream->DecodeSecureSdpL( aMediaLine ) );
            if ( err )
                {
                TInt index = iMceSecureDesStreams.Find( secStream );
                if ( KErrNotFound != index )
                    {
                    iMceSecureDesStreams.Remove( index );
                    }
                delete secStream;
                secStream = NULL;
                User::Leave( err );
                }
            }
        }
    else
    	{
    	RemoveSecureCrypto( );
    	}
	
    MCEMM_DEBUG("CMceSecureMediaSession::DecodeSecureDesSdpOfferL(), Exit " );
    }      

// -----------------------------------------------------------------------------
// CMceSecureMediaSession::DecodeSecureDesSdpOfferL
// -----------------------------------------------------------------------------

void CMceSecureMediaSession::DecodeSecureDesSdpUpdateL( CMceComMediaStream& aStream,
														CSdpMediaField& aMediaLine)
    {
    MCEMM_DEBUG("CMceSecureMediaSession::DecodeSecureDesSdpOfferL(), Entry ");
    
    User::LeaveIfNull( &aMediaLine );
    if ( IsSecureSdpMsgL( aMediaLine ) )
   		{
   		
	    // find media lines and create sec stream and add security attribute
	   	
	    CMceSecureDesStream* secStream=NULL;                            
	   
		//one uplink stream for this codec must be set already
		TInt secStreamCount = iMceSecureDesStreams.Count();
		for (TInt i=0; i<secStreamCount; i++ )
	    	{
	    	secStream = iMceSecureDesStreams[i];
	    	if (IfStreamMatchMedia(	aStream, *secStream,aMediaLine))
	    		{
	    		secStream->DecodeSecureSdpL( aMediaLine );    
				
				}
				    
	    	}
   		}
   	else
   		{
   		RemoveSecureCrypto( );
   		}
   	
    MCEMM_DEBUG("CMceSecureMediaSession::DecodeSecureDesSdpOfferL(), Exit ");
    }          

// -----------------------------------------------------------------------------
// CMceSecureMediaSession::EncodeSecureDesSdpAnswerL
// -----------------------------------------------------------------------------
void CMceSecureMediaSession::EncodeSecureDesSdpAnswerL( CMceComMediaStream& aStream, 
														CSdpMediaField& aMediaLine )
    {
    MCEMM_DEBUG("CMceSecureMediaSession::EncodeSecureDesSdpAnswerL(), Entry ");
    User::LeaveIfNull( &aMediaLine);
    
   	TBool foundCryptoOut=EFalse;	
   
   	// find media lines and create sec stream and add security attribute
	CMceSecureDesStream* secStream=NULL;                            
  
	TInt secStreamCount = iMceSecureDesStreams.Count();
	for (TInt i=0; i<secStreamCount; i++)
    	{
    	CMceSecureDesStream* secDesStream = iMceSecureDesStreams[i];
    	if (IfStreamMatchMedia(aStream, *secDesStream, aMediaLine))
    		{
    		//HARDCode here as refresh first later on should also know when to 
			// set as updated mode 	
		
			i=secStreamCount;
    		secStream = secDesStream;
    		//remove or clean secure sdp params if any 
    		secStream->RemvoeSecureSdp(aMediaLine);
    	
    		foundCryptoOut=ETrue;
    		}
    	
    	}
    	//create sec stream depends on how many media lines
	if (!foundCryptoOut)
		{
		secStream = CMceSecureDesStream::NewL(*this,
								aMediaLine,
								iSecureInterface,
								aStream);
		CleanupStack::PushL( secStream);						
		iMceSecureDesStreams.AppendL(secStream);
		CleanupStack::Pop(secStream);
								
		}
	
	//Decode sdp
	if (secStream)
		{
		secStream->EncodeSecureSdpL(aMediaLine, ETrue);    
		}

    MCEMM_DEBUG("CMceSecureMediaSession::EncodeSecureDesSdpAnswerL(), Exit ");
    }       

	
// -----------------------------------------------------------------------------
// CMceSecureMediaSession:: CleanSecureCryptoInfo()
// -----------------------------------------------------------------------------
void CMceSecureMediaSession:: CleanSecureCryptoInfoL(CSdpMediaField& aMediaLine)
    {
    MCEMM_DEBUG("CMceSecureMediaSession::CleanSecureCryptoInfoL(), Entry ");
    User::LeaveIfNull( &aMediaLine );
    
    // find media lines and create sec stream and add security attribute
    CMceSecureDesStream* secStream=NULL; 
    
    //find sec stream
    TInt secStreamCount = iMceSecureDesStreams.Count();
    for (TInt j=0; j<secStreamCount; j++)
    	{
    	CMceSecureDesStream* secDesStream = iMceSecureDesStreams[j];
    	if (IfStreamMatchMedia(secDesStream->MediaStream(), *secDesStream, aMediaLine))
    		{
    		//out of loop
    		j=secStreamCount;
    		secStream = secDesStream;
    		secStream->RemvoeSecureSdp(aMediaLine );
    		}
    	}
			    
	User::LeaveIfError( aMediaLine.IsValid() );
    MCEMM_DEBUG("CMceSecureMediaSession::CleanSecureCryptoInfoL(), Exit ");
   
    }

// -----------------------------------------------------------------------------
// CMceSecureMediaSession::Bind()
// -----------------------------------------------------------------------------
TInt CMceSecureMediaSession::BindStreamCrypto( )
    					
	{
	MCEMM_DEBUG("CMceSecureDesStream::BindStreamCrypto(), Entry");
	TInt err(KErrNone);
	if (iSession.iClientCryptoSuites.Count())
		{
		for (TInt i=0; i<iSession.MccStreams().Count(); i++)
			{
			CMceSrvStream* stream = iSession.MccStreams()[i];
			MCEMM_DEBUG_DVALUE("	CMceSrvStream id =", stream->Data().Id().iId );	
			MCEMM_DEBUG_DVALUE("	App id", stream->Data().Id().iAppId );		
			TInt secStreamCount = iMceSecureDesStreams.Count();
    		for (TInt j=0; j<secStreamCount; j++)
				{
				CMceSecureDesStream* secureStream=iMceSecureDesStreams[j]; 
				MCEMM_DEBUG_DVALUE("	SrvStream id in SecureStream ", secureStream->MediaStream().Id().iId);	
				MCEMM_DEBUG_DVALUE("	App id in SecureStream ", secureStream->MediaStream().Id().iAppId);	
				if (stream->Data().Id()== secureStream->MediaStream().Id() || 
					 ( stream->Data().BoundStream() && 
					   stream->Data().iLinkedStream->Id() == secureStream->MediaStream().Id() ) ) 
					{
					//bind
					secureStream->BindCrypto(*stream);
					
					}
				}
			}
		}
	MCEMM_DEBUG("CMceSecureDesStream::BindStreamCrypto(), Exit");
    return err;									
	}


// -----------------------------------------------------------------------------
// CMceSecureMediaSession::ContextNeedUpdated
// -----------------------------------------------------------------------------

void CMceSecureMediaSession::ContextNeedUpdated( TUint32 aContextId )
    {
   	TInt secStreamCount = iMceSecureDesStreams.Count();
    for (TInt j=0; j<secStreamCount; j++)
    	{
    	CMceSecureDesStream* secDesStream = iMceSecureDesStreams[j];
    	if (secDesStream->CompareContextId(aContextId))
    		{
    		secDesStream->iCryptoUpdateNeeded = ETrue;
    		}
    	}
    }  

// -----------------------------------------------------------------------------
// CMceSecureMediaSession::IfStreamMatchMedia
// -----------------------------------------------------------------------------

TBool CMceSecureMediaSession::IfStreamMatchMedia(	CMceComMediaStream& aMediaStream,
													CMceSecureDesStream& aStream,
													CSdpMediaField& aMediaLine)
	{
	TBool match=EFalse;
	if (aStream.MediaField().Media()== aMediaLine.Media()  && 
		aStream.MediaStream().iLocalMediaPort==aMediaStream.iLocalMediaPort &&
		aStream.MediaStream().iRemoteMediaPort==aMediaStream.iRemoteMediaPort )
		{
		match =ETrue;
	
		if (aMediaStream.iLocalMediaPort != aMediaLine.Port()&&
			aMediaStream.iRemoteMediaPort !=aMediaLine.Port())
			{
			match=EFalse;
			}
		}
	return match;	
	}
// -----------------------------------------------------------------------------
// CMceSecureMediaSession::CloneStreams
// -----------------------------------------------------------------------------

void CMceSecureMediaSession::CopyStreamsL(CMceSecureMediaSession& aSession)
	{
	iKeyNeedUpdated = aSession.iKeyNeedUpdated;
	TInt secStreamCount = aSession.iMceSecureDesStreams.Count();
    for (TInt j=0; j<secStreamCount; j++)
    	{
    	CMceSecureDesStream* copyFrom =aSession.iMceSecureDesStreams[j];
    	CMceComMediaStream* mediaStream=NULL;
    	for (TInt i=0; i<iSession.Streams().Count();i++)
    		{
    		mediaStream =  static_cast <CMceComMediaStream*> (iSession.Streams()[i]);
    		
    		if (mediaStream->iLocalMediaPort == copyFrom->MediaStream().iLocalMediaPort &&
    			mediaStream->iRemoteMediaPort == copyFrom->MediaStream().iRemoteMediaPort)
    			{
    			CMceSecureDesStream* copy = CMceSecureDesStream::NewL(*this,
    														copyFrom->MediaField(),
    														iSecureInterface,
    														*mediaStream);
    			CleanupStack::PushL( copy);											
    			copy->CopyStreamCryptoL(*copyFrom);
    			iMceSecureDesStreams.AppendL(copy);
        		CleanupStack::Pop( copy );											
    			}
    		}
    	
        
    	}
	}


//-----------------------------------------------------------------------------
// CMceSecureMediaSession::IsSecureSdpMsg ()
// 
// -----------------------------------------------------------------------------
//
TBool CMceSecureMediaSession::IsSecureSdpMsgL( CSdpMediaField& aMediaLine )
	{
	//answer or offer/update or refresh
	TInt findSAVP = aMediaLine.Protocol().DesC().Find( KProtocolSAVP );
	TInt findSAVPF = aMediaLine.Protocol().DesC().Find( KProtocolSAVPF );	
    TInt findAVP = aMediaLine.Protocol().DesC().Find( KProtocolAVP );	
    TBool isSecureMsg = ETrue;
    TInt attrCount = SdpCryptoAttributeCount( aMediaLine );
    
    
	if ( findAVP != KErrNotFound && attrCount )
		{
		iSession.Modifier( KMceSecureSession ) = KMceSecurePlainAVP;
		}
	else if ( ( findSAVP != KErrNotFound || findSAVPF != KErrNotFound ) &&
			attrCount )
		{
		iSession.Modifier( KMceSecureSession ) = KMceSecureNormal;
		}
	else if ( ( findSAVP != KErrNotFound || findSAVPF != KErrNotFound ) &&
			!attrCount )
		{
		User::Leave( KErrArgument );
		}
	else
		{
		return !isSecureMsg;
		}
	
	
	return isSecureMsg;
	}
	
// -----------------------------------------------------------------------------
// CMceSecureMediaSession::SdpCryptoAttributeCount()
// 
// -----------------------------------------------------------------------------
//    
TInt CMceSecureMediaSession::SdpCryptoAttributeCount( CSdpMediaField& aMediaLine)
	{
	MCEMM_DEBUG("CMceSecureMediaSession::SdpCryptoAttributeCount(), Entry");   
	RPointerArray< CSdpAttributeField > attrList = aMediaLine.AttributeFields();
    CSdpAttributeField* attributeField = NULL;                
    TInt attrCount = attrList.Count();
    TInt attrbuiteCryptoCount = 0;
    for (TInt j = 0; j < attrCount; j++ )
        {
       	attributeField = attrList[j];
      
        RStringF attribute = attributeField->Attribute();
     
        // check if attribute is 'crypto' 
        if ( KErrNotFound != attribute.DesC().Match( KCrypto ) )
            {
            attrbuiteCryptoCount++;
           	}
		}
	
	MCEMM_DEBUG("CMceSecureMediaSession::SdpCryptoAttributeCount(), Exit");   
	return attrbuiteCryptoCount;
	}	

// -----------------------------------------------------------------------------
// CMceSecureMediaSession::SdpCryptoAttributeCount()
// 
// -----------------------------------------------------------------------------
//    
void CMceSecureMediaSession::RemoveSecureCrypto( )
	{    	
	//clear comsession crypto
	TInt count = iSession.iClientCryptoSuites.Count();
	for (TInt i = count; i > 0; i--)
		{
		iSession.iClientCryptoSuites.Remove( i-1 );	
		iSession.iIsSecureSession = EFalse;
		}
    }					
// -----------------------------------------------------------------------------
// CMceSecureMediaSession::ForceUpdateSecureStreamL
// 
// -----------------------------------------------------------------------------
//  
void CMceSecureMediaSession::ForceUpdateSecureStreamL(CMceComMediaStream& aStream,
		CSdpMediaField& aMediaLine )
	{
	MCEMM_DEBUG("CMceSecureMediaSession::ForceUpdateSecureStreamL(), Entry ");
	User::LeaveIfNull( &aStream );
	User::LeaveIfNull( &aMediaLine );
	CMceSecureDesStream* secStream=NULL; 
	TInt secStreamCount = iMceSecureDesStreams.Count();
	for (TInt j=0; j<secStreamCount; j++)
		{
		CMceSecureDesStream* secDesStream = iMceSecureDesStreams[j];
		if (IfStreamMatchMedia( aStream, *secDesStream, aMediaLine))
	    		{
	    		j=secStreamCount;
	    		secStream = secDesStream;
	    		}
		}
	if( secStream )
		{
		secStream->ForceUpdateStreamL();
		}
	MCEMM_DEBUG("CMceSecureMediaSession::ForceUpdateSecureStreamL(), Exit ");
	}
//  End of File  
