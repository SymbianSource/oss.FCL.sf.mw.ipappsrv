/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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

#include "mcemediadefs.h"
#include "mcemediasdpcodec.h"
#include "mcecomcodec.h"
#include "mcemmlogs.h"
#include "mcepreconditions.h"
#include "mcesdpsession.h"
#include "mcesip.h"
#include "mcemediamanager.h"
#include "mcenatpluginmanager.h"

#include <sdpconnectionfield.h>
#include <sdpdocument.h>
#include <sdpcodecstringconstants.h>
#include <sdpcodecstringpool.h>
#include <sdpmediafield.h>
#include <sdpattributefield.h>
#include <sdprtpmapvalue.h>
#include <sdpfmtattributefield.h>
#include <sdporiginfield.h>
#include <sdpbandwidthfield.h>
#include <mmcccodecinformation.h>
#include <sdpcodecstringpool.h>
#include <delimitedpathsegment8.h>


// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CMceMediaSdpCodec::CMceMediaSdpCodec
// -----------------------------------------------------------------------------
CMceMediaSdpCodec::CMceMediaSdpCodec( RStringF aMedia )
    : CMceSdpCodec( aMedia )
    {
    }


// -----------------------------------------------------------------------------
// CMceMediaSdpCodec::~CMceMediaSdpCodec
// -----------------------------------------------------------------------------
CMceMediaSdpCodec::~CMceMediaSdpCodec()
    {
    }



// -----------------------------------------------------------------------------
// CMceMediaSdpCodec::EncodeMediaOfferL
// -----------------------------------------------------------------------------
//
void CMceMediaSdpCodec::EncodeMediaOfferL( 
    CMceComMediaStream& aStream, 
    CSdpMediaField& aMediaLine, 
    CSdpDocument& aSdpDocument )
    { 
    MCEMM_DEBUG("CMceMediaSdpCodec::EncodeMediaOfferL(), Entry ")
    
    // get all the codecs to be offered for this media stream.
    const RPointerArray<CMceComCodec>& allCodecs = CodecsL( aStream );

    CMceComCodec* codec = NULL;
    CMceComCodec::TIterator codecs( allCodecs, CMceComCodec::TIterator::EFilterIsNegotiated );
    
    // Set the fmt list containing all supported payload 
    // types supported by this media
    // i.e. all the rtpmap fields in the pointer array
    HBufC8* fmtlist = CreateFormatListL( codecs );
    CleanupStack::PushL( fmtlist ); 
    codecs.Reset();

    aMediaLine.SetFormatListL( *fmtlist );
    CleanupStack::PopAndDestroy( fmtlist ); 
    
    MCEMM_DEBUG_SVALUE("encoded formatlist", aMediaLine.FormatList() )
    
    // For each codec supported by this stream create a rtpmap field
    // e.g. a=rtpmap: 97 AMR/8000
    // and ptime and maxptime
    // e.g. a=ptime:20 a=maxptime:40
    while( codecs.Next( codec ) )
        {
        CSdpFmtAttributeField* rtpmap = EncodeRtpmapAttributeLC( *codec );

        EncodeMediaAttributesL( *codec, aMediaLine, *rtpmap );

        aMediaLine.FormatAttributeFields().AppendL( rtpmap );
        CleanupStack::Pop( rtpmap );
        
        EncodeFmtpAttributeL( *codec, aMediaLine );
        
        }  
    
    //encode direction
    EncodeDirectionL( aStream, aMediaLine, aSdpDocument, EMceRoleOfferer );
    
    //encode client attributes if any
    EncodeClientAttributesL( aStream, aMediaLine );
        
    //encode secure session if any
    EncodeSecureSessionL( aStream, aMediaLine, EMceRoleOfferer );
    
    //encode preconditions if any
    EncodePreconditionsL( aStream, aMediaLine, EMceRoleOfferer );
    
    //encode rtcp port 
    EncodelocalRtcpAttrL( aMediaLine, aStream );
    
    MCEMM_DEBUG("CMceMediaSdpCodec::EncodeMediaOfferL(), Exit ")
    }


// -----------------------------------------------------------------------------
// CMceMediaSdpCodec::DecodeMediaAnswerL
// -----------------------------------------------------------------------------
//
TMceSipWarningCode CMceMediaSdpCodec::DecodeMediaAnswerL( 
    CSdpMediaField& aMediaLine, 
    CMceComMediaStream& aStream,
    CSdpDocument& aSdpDocument )
    {
    MCEMM_DEBUG("CMceMediaSdpCodec::DecodeMediaAnswerL(), Entry ")
    
    //should have secure session created

    const RPointerArray<CMceComCodec>& allCodecs = CodecsL( aStream );
    CMceComCodec::TIterator codecs( allCodecs );
    
    //decode direction or old school hold if necessary                                            
    DecodeDirectionL( aMediaLine, aStream, aSdpDocument, EMceRoleOfferer );
	        
    __ASSERT_ALWAYS( codecs.Count() > 0, User::Leave( KErrNotReady ) );
    __ASSERT_ALWAYS( aMediaLine.Port() != 0 , User::Leave( KErrNotReady ) );

    //decode based on rtpmaps + their media attributes
    TInt decoded = DecodePayloadsL( aMediaLine, aStream, EMceRoleOfferer );

    if ( !decoded )
        {
        MCEMM_DEBUG("ERROR: No codecs decoded")
        User::Leave( KErrNotReady );
        }

	 // check if remote RTCP port is set according to RFC 3605
    DecodeRemoteRtcpFieldL( aMediaLine, aStream );
    aStream.SetRemoteMediaPort( aMediaLine.Port() );

    MCEMM_DEBUG_DVALUE("decoded remote port", aStream.RemoteMediaPort() )
    //decode based on fmtps
    DecodeFmtpLinesL( aMediaLine, codecs, EMceRoleOfferer );

	 //decode secure session if any
    DecodeSecureSessionL( aMediaLine, aStream, EMceRoleOfferer );
    
    //decode preconditions if any
    TMceSipWarningCode warnings = 
        DecodePreconditionsL( aMediaLine, aStream, EMceRoleOfferer );
    
    //decode client attributes if any
    DecodeClientAttributesL( aMediaLine, aStream );

    MCEMM_DEBUG("CMceMediaSdpCodec::DecodeMediaAnswerL(), Exit ")
    return warnings;
    }    
    
// -----------------------------------------------------------------------------
// CMceMediaSdpCodec::DecodeMediaOfferL
// -----------------------------------------------------------------------------
//
TMceSipWarningCode CMceMediaSdpCodec::DecodeMediaOfferL( 
    CSdpMediaField& aMediaLine,
    CMceComMediaStream*& aStream,
    CMceComSession& aSession,
    CSdpDocument& aSdpDocument )
    {
    MCEMM_DEBUG("CMceMediaSdpCodec::DecodeMediaOfferL(), Entry ")
    
    TMceSipWarningCode warning = ValidateSdpL( aMediaLine, aSdpDocument );
    if ( warning != KErrNone )
        {
        return warning;
        }
    
    TInt type = Type( aMediaLine, aSdpDocument.AttributeFields() );

    // create server stream based on the media direction 
    // attribute (sendonly, sendrcv, rcvonly)
    aStream = CreateStreamLC( type );
    aStream->InitializeL( aSession );
    	
    //decode direction or old school hold if necessary                                            
    DecodeDirectionL( aMediaLine, *aStream, aSdpDocument, EMceRoleAnswerer );
    	
    const RPointerArray<CMceComCodec>& allCodecs = CodecsL( *aStream );
    CMceComCodec::TIterator codecs( allCodecs );
    
    //decode based on rtpmaps + their media attributes
    TInt decoded = DecodePayloadsL( aMediaLine, *aStream, EMceRoleAnswerer );
    
    if ( decoded )
        {

        // check if remote RTCP port is set according to RFC 3605
        DecodeRemoteRtcpFieldL( aMediaLine, *aStream );
        aStream->SetRemoteMediaPort( aMediaLine.Port() );

        MCEMM_DEBUG_DVALUE("decoded remote port", aStream->RemoteMediaPort() )
        
        //decode based on fmtps
        DecodeFmtpLinesL( aMediaLine, codecs, EMceRoleAnswerer );
        
        //decode session level media attributes        
        DecodeSessionMediaAttributesL( *aStream, aSdpDocument );
        
        
        //decode secure session if any
       	DecodeSecureSessionL( aMediaLine, *aStream, EMceRoleAnswerer );
       	//decode preconditions if any
        warning = DecodePreconditionsL( aMediaLine, *aStream, EMceRoleAnswerer );
	
		    
        //Decode client attributes if any
        DecodeClientAttributesL( aMediaLine, *aStream );
		
        aSession.Streams().AppendL( aStream );
        CleanupStack::Pop( aStream );
        
        }
    else
        {
        MCEMM_DEBUG("No acceptable codecs found for current media line ")
            
        // we did not find any acceptable codecs for this stream, return warning
        
        CleanupStack::PopAndDestroy( aStream );
        aStream = NULL;
        warning = KMceSipWarnIncompatibleMediaFormat;
        }
    
    MCEMM_DEBUG("CMceMediaSdpCodec::DecodeMediaOfferL(), Exit ")
    
    return warning;
    
    }
    
// -----------------------------------------------------------------------------
// CMceMediaSdpCodec::DecodeMediaUpdateL
// -----------------------------------------------------------------------------
//
TMceSipWarningCode CMceMediaSdpCodec::DecodeMediaUpdateL( 
    CSdpMediaField& aMediaLine, 
    CMceComMediaStream& aStream,
    CSdpDocument& aSdpDocument )
    {
    
    MCEMM_DEBUG("CMceMediaSdpCodec::DecodeMediaUpdateL(), Entry ")
    
    TMceSipWarningCode warning = ValidateSdpL( aMediaLine, aSdpDocument );
    if ( warning != KErrNone )
        {
        return warning;
        }

    const RPointerArray<CMceComCodec>& allCodecs = CodecsL( aStream );
    CMceComCodec::TIterator codecs( allCodecs );
    
    aStream.SetRemoteMediaPort( aMediaLine.Port() );
    MCEMM_DEBUG_DVALUE("decoded remote port", aStream.RemoteMediaPort() )
    
    //should have secure media session existed
    
    //decode based on rtpmaps + their media attributes
    TInt decoded = DecodePayloadsL( aMediaLine, aStream, EMceRoleAnswerer );
    
    if ( decoded )
        {
        // check if remote RTCP port is set according to RFC 3605
        DecodeRemoteRtcpFieldL( aMediaLine, aStream );
        

        //decode based on fmtps
        DecodeFmtpLinesL( aMediaLine, codecs, EMceRoleAnswerer );

        //decode direction or old school hold if necessary                                            
        UpdateDirectionL( aMediaLine, aStream, aSdpDocument );

        //decode secure session if any
       	DecodeSecureSessionL( aMediaLine, aStream, EMceRoleAnswerer, KMceMediaCodecUpdate );


		//decode preconditions if any
        warning = DecodePreconditionsL( aMediaLine, aStream, EMceRoleAnswerer );
        
        //decode client attributes if any
        DecodeClientAttributesL( aMediaLine, aStream );
    
	    }
    else
        {
        MCEMM_DEBUG("ERROR: No codecs decoded")
        warning = KMceSipWarnIncompatibleMediaFormat;
        }
    
    MCEMM_DEBUG("CMceMediaSdpCodec::DecodeMediaUpdateL(), Exit ")
    return warning;
    }

// -----------------------------------------------------------------------------
// CMceAudioSdpCodec::EncodeMediaAnswerL
// -----------------------------------------------------------------------------
//
void CMceMediaSdpCodec::EncodeMediaAnswerL( 
    CMceComMediaStream& aStream,
    CSdpMediaField& aMediaLine,
    CSdpDocument& aSdpDocument )
    {
    MCEMM_DEBUG("CMceMediaSdpCodec::EncodeMediaAnswerL(), Entry ")
    
    const RPointerArray<CMceComCodec>& allCodecs = CodecsL( aStream );  
    CMceComCodec* codec = NULL;
    CMceComCodec::TIterator codecs( allCodecs, CMceComCodec::TIterator::EFilterIsNegotiated );

    if ( codecs.Count() > 0 ) 
        {
        
        // Set the fmt list containing all supported payload 
        // types supported by this media
        // i.e. all the rtpmap fields in the pointer array
        HBufC8* fmtlist = CreateFormatListL( codecs );
        CleanupStack::PushL( fmtlist ); 
        codecs.Reset();

        aMediaLine.SetFormatListL( *fmtlist );
        CleanupStack::PopAndDestroy( fmtlist ); 
        
        MCEMM_DEBUG_SVALUE("encoded formatlist", aMediaLine.FormatList() )
        
        aMediaLine.SetPortL( aStream.iLocalMediaPort );
        MCEMM_DEBUG_DVALUE("encoded local port", aMediaLine.Port() )
        
        // add the direction attribute
        EncodeDirectionL( aStream, aMediaLine, aSdpDocument, EMceRoleAnswerer );

        while( codecs.Next( codec ) )
            {
            CSdpFmtAttributeField* rtpmap = EncodeRtpmapAttributeLC( *codec );

            EncodeMediaAttributesL( *codec, aMediaLine, *rtpmap );

            aMediaLine.FormatAttributeFields().AppendL( rtpmap );
            CleanupStack::Pop( rtpmap );
            
            EncodeFmtpAttributeL( *codec, aMediaLine );
            
            }

        //encode secure session if any
        EncodeSecureSessionL( aStream, aMediaLine, EMceRoleAnswerer );
        
        //encode preconditions if any
        EncodePreconditionsL( aStream, aMediaLine, EMceRoleAnswerer );
        
        }
    else
        { // there was no selected codec for this stream
        MCEMM_DEBUG("ERROR: No selected codec found! Reject ")
        aMediaLine.RejectMedia();
        }
        
    // encode client attributes
    EncodeClientAttributesL( aStream, aMediaLine );

	// encode rtcp attributes
	EncodelocalRtcpAttrL( aMediaLine, aStream );
    MCEMM_DEBUG("CMceMediaSdpCodec::EncodeMediaAnswerL(), Exit ")
    }

// -----------------------------------------------------------------------------
// CMceMediaSdpCodec::TypeL
// -----------------------------------------------------------------------------
//
TInt CMceMediaSdpCodec::Type( 
    CSdpMediaField& aMedia, 
    RPointerArray<CSdpAttributeField>& aSessionAttributes ) const
    {
    TInt type = Direction( aMedia, aSessionAttributes );
    type = type == KErrNotFound ? SdpCodecStringConstants::EAttributeSendrecv : type; 
    return type;
    }

// -----------------------------------------------------------------------------
// CMceMediaSdpCodec::PrepareForDecodeL
// -----------------------------------------------------------------------------
//
void CMceMediaSdpCodec::PrepareForDecodeL( 
    CSdpMediaField& /*aMediaLine*/,
    CMceComMediaStream* aStream )
    {
    MCEMM_DEBUG("CMceMediaSdpCodec::PrepareForDecodeL(), Entry ")
    
    if ( aStream )
        {
        const RPointerArray<CMceComCodec>& allCodecs = CodecsL( *aStream );
        CMceComCodec* codec = NULL;
        CMceComCodec::TIterator codecs( allCodecs );
        while( codecs.Next( codec ) )
            {
            codec->iIsNegotiated = EFalse;
            }
        }
        
    MCEMM_DEBUG("CMceMediaSdpCodec::PrepareForDecodeL(), Exit ")
    }

// -----------------------------------------------------------------------------
// CMceMediaSdpCodec::DecodeRtpmapLinesL
// -----------------------------------------------------------------------------
//
TInt CMceMediaSdpCodec::DecodeRtpmapLinesL( 
    CSdpMediaField& aMediaLine,
    CMceComMediaStream& aStream,
    TMceNegotiationRole aRole ) const
    {
    MCEMM_DEBUG("CMceMediaSdpCodec::DecodeRtpmapLinesL(), Entry ")
    TInt decoded = 0;
	
    const RPointerArray<CMceComCodec>& allCodecs = CodecsL( aStream );
    CMceComCodec* codec = NULL;
    CMceComCodec::TIterator codecs( allCodecs );
    
    RArray<TUint> payloadTypesInMediaLine;
    CleanupClosePushL( payloadTypesInMediaLine );
   
    User::LeaveIfError( DecodeFormatList( aMediaLine, payloadTypesInMediaLine ) );
 
    RPointerArray< CSdpFmtAttributeField >& formatLines =  
                                            aMediaLine.FormatAttributeFields();
    
	for ( TInt i = 0; i < formatLines.Count(); i++ )
		{
		CSdpFmtAttributeField* formatLine = formatLines[ i ];
        if ( formatLine->Attribute() == 
                    SDP_STRING( SdpCodecStringConstants::EAttributeRtpmap ) )
            //codec
            {
            TPtrC8 fmtpVal = GetCorrespondingFmtpLineL( aMediaLine, *formatLine );
            
            MCE_ITERATOR_FIND_NEXT( 
                codecs, codec, 
                codec->Decodes( formatLine->Value(), fmtpVal ) );
                
            codecs.Reset();
            
            if ( !codec && aRole == EMceRoleOfferer )
                {
                // No exact match, do less strict matching for better interoperability
                MCE_ITERATOR_FIND_NEXT( 
                codecs, codec, 
                codec->Decodes( formatLine->Value(), fmtpVal, EFalse ) );
                codecs.Reset();
                }
                
            if ( codec )
                {
                MCEMM_DEBUG_SVALUE("decoding codec", codec->iSdpName )
                
                const TDesC8& payloadTypeInRtpMap = formatLine->Format();
                TUint rtpMapPT = ConvertDesToUintL( payloadTypeInRtpMap );
                
                TInt val = payloadTypesInMediaLine.Find( rtpMapPT );
                if ( val != KErrNotFound )
                    {
                    TUint payload = ConvertDesToUintL( formatLine->Format() );
                    if ( !IsPayloadTypeSupported( codec->iSdpName, payload ) ) 
                    	{
						// not supported payload, ignore this codec
                    	continue;
                    	}
                    if ( !codec->iIsNegotiated )
                        {
                        codec->iPayloadType = payload;
                        }
                    codec->iIsNegotiated = ETrue;   
                    //decode media attributes
                    DecodeMediaAttributesL( aMediaLine, *codec, *formatLine );
                    decoded++;
                    }
                else
                    {
                    // Discarding as there was no matching pt in medialine for
                    // this rtmpmap line
                    codec->iIsNegotiated = EFalse;
                    if ( aStream.BoundStream() )
                        {
                        CMceComCodec* sync = NULL;
                        sync = aStream.BoundStreamL().FindCodecL( *codec );
                        if ( sync )
                            {
                            sync->iIsNegotiated = EFalse; 
                            }
                        }
                    }
                }
            else if ( aRole == EMceRoleAnswerer )
                {
                if ( DecodeRtpmapLineL( *formatLine, aMediaLine, aStream, fmtpVal ) )
                    {
                    decoded++;
                    }
                }
            else
                {
                // NOP
                }
                    
            }
		}

    CleanupStack::PopAndDestroy( &payloadTypesInMediaLine );
        
    MCEMM_DEBUG_DVALUE("decoded codecs", decoded )
    MCEMM_DEBUG("CMceMediaSdpCodec::DecodeRtpmapLinesL(), Exit ")
    return decoded;	
    }

// -----------------------------------------------------------------------------
// CMceMediaSdpCodec::DecodeRtpmapLinesL
// -----------------------------------------------------------------------------
//
TInt CMceMediaSdpCodec::DecodeRtpmapLinesL( 
    CSdpMediaField& aMediaLine,
    CMceComMediaStream& aStream ) const
    {
    MCEMM_DEBUG("CMceMediaSdpCodec::DecodeRtpmapLinesL(), Entry ")
    TInt decoded = 0;

     // Read format level attributes
    RPointerArray< CSdpFmtAttributeField >& formatLines =  
                                            aMediaLine.FormatAttributeFields();
    
    // For all codec specific lines in the offer        
    for ( TInt i = 0; i < formatLines.Count(); i++ )
        {
        CSdpFmtAttributeField* formatLine = formatLines[i];
        if ( formatLine->Attribute() == SDP_STRING( SdpCodecStringConstants::EAttributeRtpmap ) &&
             DecodeRtpmapLineL( *formatLine, aMediaLine, aStream ) )
            {
            decoded++;
            }
        }

    MCEMM_DEBUG_DVALUE("decoded codecs", decoded )
    MCEMM_DEBUG("CMceMediaSdpCodec::DecodeRtpmapLinesL(), Exit ")
    return decoded;
    }

// -----------------------------------------------------------------------------
// CMceMediaSdpCodec::DecodeStaticPayloadsL
// -----------------------------------------------------------------------------
//
TInt CMceMediaSdpCodec::DecodeStaticPayloadsL( 
    CSdpMediaField& aMediaLine,
    CMceComMediaStream& aStream,
    RArray<TUint>& aPayloadTypes,
    TMceNegotiationRole aRole ) const
    {
    MCEMM_DEBUG("CMceMediaSdpCodec::DecodeStaticPayloadsL(), Entry ")
    TInt decoded = 0;

    DecodeFormatListL( aMediaLine, aPayloadTypes );
    TInt index = 0;
    
    while( index < aPayloadTypes.Count() )
        {
        TUint payload = aPayloadTypes[ index++ ];
        if ( payload < KMinDynamicPT )
            {
            CMceComCodec* codec = FindCodec( payload, aStream );
            if ( aRole == EMceRoleAnswerer )
                {
                if ( !codec )
                    {
                    codec = CreateCodecLC( payload, aMediaLine );
                    if ( codec )
                        {
                        codec->InitializeL( aStream );
                        codec->iIsNegotiated = ETrue;
                        MCEMM_DEBUG_SVALUE("adding codec", codec->iSdpName )
                        aStream.AddCodecL( codec );
                        CleanupStack::Pop( codec );
                        decoded++;
                        }
                    }
                else
                    {
                    codec->iIsNegotiated = ETrue;
                    decoded++;
                    }
                }
            else if ( codec && aRole == EMceRoleOfferer )
                {
                codec->iIsNegotiated = ETrue;
                decoded++;
                }
            }
        }
        
    MCEMM_DEBUG_DVALUE("decoded codecs", decoded )
    MCEMM_DEBUG("CMceMediaSdpCodec::DecodeStaticPayloadsL(), Exit ")
    return decoded;
    }

// -----------------------------------------------------------------------------
// CMceMediaSdpCodec::FindCodec
// -----------------------------------------------------------------------------
//
CMceComCodec* CMceMediaSdpCodec::FindCodec( 
    TInt aPayloadType, 
    CMceComMediaStream& aStream ) const
    {
    TInt error = KErrNone;
    
    RPointerArray< CMceComCodec > allCodecs;
    
    TRAP( error, allCodecs = CodecsL( aStream ) );  
    CMceComCodec* codec = NULL;
    if( error == KErrNone )
    	{
    	CMceComCodec::TIterator codecs( allCodecs );
    
    	MCE_ITERATOR_FIND_NEXT( codecs, codec, codec->iPayloadType == aPayloadType );	
    	}

    return codec;
    }

// -----------------------------------------------------------------------------
// CMceMediaSdpCodec::DecodeRtpmapLineL
// -----------------------------------------------------------------------------
//
CMceComCodec* CMceMediaSdpCodec::DecodeRtpmapLineL( 
    CSdpFmtAttributeField& aRtpMaptLine,
    CSdpMediaField& aMediaLine,
    CMceComMediaStream& aStream,
    const TDesC8& aFmtpValue ) const
    {
    MCEMM_DEBUG("CMceMediaSdpCodec::DecodeFmtpLineL(), Entry ")
    
    RArray<TUint> payloadTypesInMediaLine;
    CleanupClosePushL( payloadTypesInMediaLine );
   
    User::LeaveIfError( DecodeFormatList( aMediaLine, payloadTypesInMediaLine ) );
    	
    const TDesC8& payloadTypeInRtpMap = aRtpMaptLine.Format();
    
    TUint rtpMapPT = ConvertDesToUintL( payloadTypeInRtpMap );
    CMceComCodec* codec = NULL;
    
    // Ingoring if there is no matching pt in medialine for this rtmpmap line
    if ( payloadTypesInMediaLine.Find( rtpMapPT ) != KErrNotFound )
        {
        codec = CreateCodecLC( aRtpMaptLine );
        if ( codec )
            {
            codec->InitializeL( aStream );
            if ( !codec->SetFmtpProposalL( aFmtpValue ) )
                {
                // Fmtp not valid, ignore codec
                CleanupStack::PopAndDestroy( codec );
                codec = NULL;
                }
            else
                {
                // check if ptime or maxptime attributes are present
                DecodeMediaAttributesL( aMediaLine, *codec, aRtpMaptLine );
                codec->iIsNegotiated = ETrue;
                MCEMM_DEBUG_SVALUE("adding codec", codec->iSdpName )
                aStream.AddCodecL( codec );
                CleanupStack::Pop( codec );
                }
            }    
        }

    CleanupStack::PopAndDestroy( &payloadTypesInMediaLine );

    MCEMM_DEBUG("CMceMediaSdpCodec::DecodeFmtpLineL(), Exit ")
    return codec;        
    }
    
// -----------------------------------------------------------------------------
// CMceMediaSdpCodec::DecodeFmtpLinesL
// -----------------------------------------------------------------------------
//
void CMceMediaSdpCodec::DecodeFmtpLinesL( 
    CSdpMediaField& aMediaLine,
    CMceComCodec::TIterator& aCodecs,
    TMceNegotiationRole aRole ) const
    {
    MCEMM_DEBUG("CMceMediaSdpCodec::DecodeFmtpLinesL(), Entry ")

    RPointerArray< CSdpFmtAttributeField >& formatLines =  
        aMediaLine.FormatAttributeFields();
    
    CMceComCodec* codec = NULL;
    while( aCodecs.Next( codec ) )
        {
   	    TBool first = ETrue;
        // for all codec specific lines in the offer
        TBool found = EFalse;
    	for ( TInt j = 0; j < formatLines.Count(); j++ )
    		{
    		CSdpFmtAttributeField* formatLine = formatLines[ j ];
    		if ( formatLine->Attribute() == 
    		            SDP_STRING( SdpCodecStringConstants::EAttributeFmtp ) )
    			{
    			TUint payloadType = ConvertDesToUintL( formatLine->Format() );
				if ( codec->iPayloadType == payloadType )
				    {
                    MCEMM_DEBUG_SVALUE("decoding format", formatLine->Format() )
					const TDesC8& fmtpValue = formatLine->Value();
                    MCEMM_DEBUG_SVALUE("decoding fmtp", fmtpValue )
                    found = ETrue;
                    TBool isAnswerer( aRole == EMceRoleAnswerer );
					if ( first )
					    {
					    codec->SetFmtpFromIncomingSdpL( 
					                fmtpValue, isAnswerer, KMceCodecFmtpReset );
					    first = EFalse;
					    }
                    else
                        {
        				codec->SetFmtpFromIncomingSdpL( 
        				            fmtpValue, isAnswerer, KMceCodecFmtpAppend );
                        }
				    }
    			}
    		}
    	if ( !found )
        	{
            // No fmtp line found, use default values
            codec->SetDefaultFmtpAttributeL();
            }
        MCEMM_DEBUG_SVALUE("decoded fmtp", *codec->iFmtpAttr )         
        }
   	    
    MCEMM_DEBUG("CMceMediaSdpCodec::DecodeFmtpLinesL(), Exit ")
    }

// -----------------------------------------------------------------------------
// CMceMediaSdpCodec::EncodeFmtpAttributeL
// -----------------------------------------------------------------------------
//
void CMceMediaSdpCodec::EncodeFmtpAttributeL( 
    CMceComCodec& aCodec,
    CSdpMediaField& aMedia ) const
    {
    MCEMM_DEBUG("CMceMediaSdpCodec::EncodeFmtpAttributeL(), Entry ")
    
    RStringF attributeFmtp = 
        SDP_STRING( SdpCodecStringConstants::EAttributeFmtp );
    
    if( aCodec.iFmtpAttr && aCodec.iFmtpAttr->Size() != 0 )
        {
        MCE_DEFINE_DECSTR( payload, aCodec.iPayloadType )

        HBufC8* fmtpAttr = aCodec.FtmpForOutgoingSdpLC();
        CSdpFmtAttributeField* fmtpLine = 
            CSdpFmtAttributeField::NewL( attributeFmtp, payload, *fmtpAttr );
        CleanupStack::PopAndDestroy( fmtpAttr );
        CleanupStack::PushL( fmtpLine );                         
        aMedia.FormatAttributeFields().AppendL( fmtpLine );
        CleanupStack::Pop( fmtpLine );
        
        MCEMM_DEBUG_SVALUE( "encoded fmtp", fmtpLine->Value() )
        }
        
    MCEMM_DEBUG("CMceMediaSdpCodec::EncodeFmtpAttributeL(), Exit ")
    }
        
// -----------------------------------------------------------------------------
// CMceMediaSdpCodec::CreateFormatListL
// Creates fmtlist that can be used as media field fmt list -attribute
// (other items were commented in a header).
// Status : Draft 
// -----------------------------------------------------------------------------
//
HBufC8* CMceMediaSdpCodec::CreateFormatListL( 
    CMceComCodec::TIterator& aCodecs ) const
    {
    MCEMM_DEBUG("CMceMediaSdpCodec::CreateFmtListL(), Entry ")
    
    HBufC8* fmtList = HBufC8::NewLC( KMceSdpFormatListLength );
    TPtr8 fmtListPtr( fmtList->Des() );

    CMceComCodec* codec = NULL;
    for ( TInt count = 0; count <= aCodecs.Count()-1; count++ )
    	{
    	aCodecs.Next( codec );
    	MCE_DEFINE_DECSTR( payload, codec->iPayloadType )
    	fmtListPtr.Append( payload );
       	if ( count != aCodecs.Count()-1)
       		{
       		fmtListPtr.Append( KMceSipBlank );
       		}
    	}
    
    CleanupStack::Pop( fmtList );
    
    MCEMM_DEBUG("CMceMediaSdpCodec::CreateFmtListL(), Exit ")
    
    return fmtList;
    }

// -----------------------------------------------------------------------------
// CMceMediaSdpCodec::DecodeFormatListL
// -----------------------------------------------------------------------------
//
void CMceMediaSdpCodec::DecodeFormatListL( 
    CSdpMediaField& aMedia, 
    RArray<TUint>& aPayloadTypes ) const
    {
    MCEMM_DEBUG("CMceMediaSdpCodec::DecodeFormatListL(), Entry ")
    
    User::LeaveIfError( DecodeFormatList( aMedia, aPayloadTypes ) );
        
    MCEMM_DEBUG("CMceMediaSdpCodec::DecodeFormatListL(), Exit ")   
    }

// -----------------------------------------------------------------------------
// CMceMediaSdpCodec::DecodeFormatList
// -----------------------------------------------------------------------------
//
TInt CMceMediaSdpCodec::DecodeFormatList( 
    CSdpMediaField& aMedia, 
    RArray<TUint>& aPayloadTypes ) const
    {
    MCEMM_DEBUG("CMceMediaSdpCodec::DecodeFormatList(), Entry ")
    
    TInt index = 0;
    const TDesC8& payloadlist = aMedia.FormatList();
    MCEMM_DEBUG_SVALUE( "decoded format list", payloadlist )
    TLex8 payloads( payloadlist );
    TInt error = KErrNone;
    
    while( error == KErrNone && !payloads.Eos() && index < KMceMaxPayloadTypes )
        {
        TPtrC8 payload = payloads.NextToken();
        TUint convertedUnit = 0;
        TLex8 lexConv( payload );
        TInt err = lexConv.Val( convertedUnit, EDecimal );
        if( err != KErrNone )
        	{
        	error = err;
        	break;
        	}
        error = aPayloadTypes.Append( convertedUnit );
        index++;
        }

    MCEMM_DEBUG("CMceMediaSdpCodec::DecodeFormatList(), Exit ")
    
    return error;
    }
    
// -----------------------------------------------------------------------------
// CMceMediaSdpCodec::DecodeRemoteRtcpFieldL
//
// a=rtcp:53020 IN IP4 126.16.64.4
// -----------------------------------------------------------------------------
//
void CMceMediaSdpCodec::DecodeRemoteRtcpFieldL( 
    CSdpMediaField& aMediaLine, 
    CMceComMediaStream& aStream ) const
    {   
    MCEMM_DEBUG( "CMceMediaSdpCodec::ParseRemoteRtcpFieldL(), Entry" )

    RStringF rtcpAttributeName = iStringPool.OpenFStringL( KMceSdpAttrRtcp );
    CleanupClosePushL( rtcpAttributeName );
    _LIT8( KMatchIN,"*IN*" );       
    _LIT8( KMatchIp,"*IP*" );
    
    RPointerArray<CSdpAttributeField>& attrfields = 
        aMediaLine.AttributeFields();
    
    for ( TInt i = 0; i < attrfields.Count(); i++ )
        {
        CSdpAttributeField* attribute = attrfields[i];        
        if ( attribute->Attribute() == rtcpAttributeName ) 
            {
            const TDesC8& value = attribute->Value();
            TUint rtcpPort( 0 );
            
            if ( value.Match( KMatchIN ) != KErrNotFound )
                {
                // found remote IP4 RTCP address
                const TUint8 KAddrOffsetFromNetType = 4;
                TInt addr_offset = 
                    value.Match( KMatchIp ) + KAddrOffsetFromNetType;
                TPtrC8 remoteRtcpAddrTxt = value.Mid( addr_offset );
                
                const TUint8 KPortOffsetFromIP = 1;
                TInt port_offset = 
                    value.Match( KMatchIN ) - KPortOffsetFromIP;
                HBufC8* dataRemoteRtcpPort = value.Left( port_offset ).AllocLC();
                TPtr8 remoteRtcpPort( dataRemoteRtcpPort->Des() );
                remoteRtcpPort.TrimAll();
                TLex8 lexPT( remoteRtcpPort );
                User::LeaveIfError( lexPT.Val( rtcpPort, EDecimal ) );
                // copy the address into correct format
                TBuf16 <KMaxAddressLength> input;
                input.Copy( remoteRtcpAddrTxt );      
                input.TrimAll();
                MCEMM_DEBUG_SVALUE( "Found RTCP address", input )
                
                TInetAddr remoteRtcpAddr;
                User::LeaveIfError( remoteRtcpAddr.Input( input ) ); 
                TInetAddr localIpAddr( aStream.Session()->iLocalIpAddress );
                TBool validRemoteRtcpAddr( 
                    remoteRtcpAddr.IsLoopback() || !localIpAddr.Match( remoteRtcpAddr ) );
                MCEMM_DEBUG_DVALUE( "Remote RTCP addr valid:", validRemoteRtcpAddr )
                if ( validRemoteRtcpAddr )
                    {
                    aStream.SetRemoteRtcpMediaAddrL( input );
                    }
                CleanupStack::PopAndDestroy( dataRemoteRtcpPort );
                }
            else
                {
                // only port present
                HBufC8* dataRemoteRtcpPort = value.AllocLC();
                TPtr8 remoteRtcpPort( dataRemoteRtcpPort->Des() );
                remoteRtcpPort.TrimAll();
                TLex8 lexPT( remoteRtcpPort );
                User::LeaveIfError ( lexPT.Val( rtcpPort, EDecimal ) );
                CleanupStack::PopAndDestroy( dataRemoteRtcpPort );
                }

			aStream.SetRemoteRtcpMediaPort( rtcpPort );
            
            MCEMM_DEBUG_DVALUE( "RTCP Port", rtcpPort )
            }
        }
    
    CleanupStack::PopAndDestroy( &rtcpAttributeName );
      
    MCEMM_DEBUG( "CMceMediaSdpCodec::ParseRemoteRtcpFieldL(), Exit" )    
    }
    
// -----------------------------------------------------------------------------
// CMceMediaSdpCodec::EncodelocalRtcpAttrL
//
// a=rtcp:53020 IN IP4 126.16.64.4
// -----------------------------------------------------------------------------
//
void CMceMediaSdpCodec::EncodelocalRtcpAttrL( 
    CSdpMediaField& aMediaLine, 
    CMceComMediaStream& aStream ) const
    {   
    MCEMM_DEBUG("CMceMediaSdpCodec::EncodelocalRtcpAttrL, Entry ")
    if ( aStream.Session() && aStream.Session()->UseRtcp() )
		{
		MCEMM_DEBUG("RTCP attribute to be added")
    	TBuf8<KMceSdpMaxMediaLineLength> rtcpAttrValue;
		rtcpAttrValue.Num( aStream.iLocalMediaPort + 1, EDecimal );
		 if ( !aStream.Session()->iLocalIpAddress.IsUnspecified() )
			{
			CSdpConnectionField* connectionField = 
			    CSdpConnectionField::NewL( aStream.Session()->iLocalIpAddress );
			CleanupStack::PushL( connectionField );
			rtcpAttrValue.Append( KMceSdpSpace );   
		    rtcpAttrValue.Append( connectionField->NetType().DesC() );  
		    rtcpAttrValue.Append( KMceSdpSpace );        
		 	rtcpAttrValue.Append( connectionField->AddressType().DesC() );
		 	rtcpAttrValue.Append( KMceSdpSpace );      
		 	rtcpAttrValue.Append( connectionField->Address() );
		 	CleanupStack::PopAndDestroy( connectionField );
			}
		RStringF attributeRtcp = 
            SdpCodecStringPool::StringPoolL().OpenFStringL( KMceSdpAttrRtcp );
		CleanupClosePushL( attributeRtcp );

		CSdpAttributeField* rtcpLine = 
		            CSdpAttributeField::NewLC( attributeRtcp, rtcpAttrValue );
		        
		MCEMM_DEBUG_SVALUE("encoded rtcpPort", aStream.iLocalMediaPort + 1 )

		aMediaLine.AttributeFields().AppendL( rtcpLine );

		CleanupStack::Pop( rtcpLine ); 
		CleanupStack::PopAndDestroy( ); //attributeRtcp 
		}
    MCEMM_DEBUG("CMceMediaSdpCodec::EncodelocalRtcpAttrL, Exit ")    
    }    
    
// -----------------------------------------------------------------------------
// CMceMediaSdpCodec::CleanAttributes
// -----------------------------------------------------------------------------
//
void CMceMediaSdpCodec::CleanAttributes( CSdpMediaField& aMedia )
    {
    MCEMM_DEBUG("CMceMediaSdpCodec::CleanAttributes(), Entry ")
    
    //clean attributes
    aMedia.AttributeFields().ResetAndDestroy();
        
    //clean format attributes
    aMedia.FormatAttributeFields().ResetAndDestroy();
       
    aMedia.BandwidthFields().ResetAndDestroy();
    
    MCEMM_DEBUG("CMceMediaSdpCodec::CleanAttributes(), Exit ")
    }

// -----------------------------------------------------------------------------
// CMceMediaSdpCodec::DecodeSecureSessionL
// -----------------------------------------------------------------------------
//
void CMceMediaSdpCodec::DecodeSecureSessionL( 
    CSdpMediaField& aMediaLine,
    CMceComMediaStream& aStream, 
    TMceNegotiationRole aRole,
    TBool aUpdate )
    {
    
    CMceSecureMediaSession* secureSession = aStream.Session()->SecureSession();

    if ( aRole == EMceRoleAnswerer && !aUpdate )
        {
        aStream.Session()->SecureSessionL().DecodeSecureDesSdpOfferL( aStream, aMediaLine );
        if ( !aStream.Session()->iIsSecureSession )
            {
            aStream.Session()->DeleteSecureSession();
            }
        }
    else if ( secureSession && aRole == EMceRoleAnswerer && aUpdate )
        {
        // for long session
          if( secureSession->SdpCryptoAttributeCount( aMediaLine ) )
        	{
            secureSession->DecodeSecureDesSdpUpdateL( aStream, aMediaLine ) ;
        	}
        }
    else if ( secureSession && aRole == EMceRoleOfferer )
        {
       	// for Long Session
        	//sendonly receive only sendandreceive
     	secureSession->DecodeSecureDesSdpAnswerL( aStream, aMediaLine ) ;
       	}        
    }

// -----------------------------------------------------------------------------
// CMceMediaSdpCodec::EncodeSecureSessionL
// -----------------------------------------------------------------------------
//
void CMceMediaSdpCodec::EncodeSecureSessionL( 
    CMceComMediaStream& aStream, 
    CSdpMediaField& aMediaLine,
    TMceNegotiationRole aRole )
    {
    CMceSecureMediaSession* secureSession = aStream.Session()->SecureSession();

    if ( secureSession )
        {
        if ( aRole == EMceRoleOfferer )
            {
            secureSession->EncodeSecureDesSdpOfferL( aStream, aMediaLine );
            }
        else
            {
            secureSession->EncodeSecureDesSdpAnswerL( aStream, aMediaLine );
            }
        }
    }

// -----------------------------------------------------------------------------
// CMceMediaSdpCodec::DecodeDirectionL
// -----------------------------------------------------------------------------
//
void CMceMediaSdpCodec::DecodeDirectionL( CSdpMediaField& aMediaLine,
                                          CMceComMediaStream& aStream,
                                          CSdpDocument& aSdpDocument,
                                          TMceNegotiationRole aRole ) const
	{
    MCEMM_DEBUG("CMceMediaSdpCodec::DecodeDirectionL(), Entry ")

    TInt rmt_direction = 
        Direction( aMediaLine, aSdpDocument.AttributeFields() );
    TInt loc_direction = aStream.Direction();
    
    CSdpConnectionField* connectionField = aSdpDocument.ConnectionField();
    
    CMceSdpSession& sSession = aStream.Session()->SdpSession();
        
    CSdpConnectionField* nullAddress =
        CSdpConnectionField::DecodeL( KMceSipNullAddressA );
    				 
    CleanupStack::PushL( nullAddress );

    TUint oldSchool = sSession.iOOldSchool;
    TBool isNullAddress = EFalse;
    if ( connectionField )
        {
        isNullAddress = (connectionField->Address() == nullAddress->Address());
        }

    CleanupStack::PopAndDestroy( nullAddress );

    //trying to guess other party direction if have got no direction
    if ( rmt_direction == KErrNotFound || 
         ( rmt_direction == SdpCodecStringConstants::EAttributeSendonly && 
           loc_direction == SdpCodecStringConstants::EAttributeSendonly ) )
    
	    {
	    if ( aRole == EMceRoleOfferer )
	      	{
	   		// did we try old school hold?
	    	if ( oldSchool )           
	      	    {
          	    if ( loc_direction == SdpCodecStringConstants::EAttributeSendrecv )
          	        {
          	        if ( !isNullAddress )
          	            {
          	            rmt_direction = SdpCodecStringConstants::EAttributeSendrecv;
          	            }
          	        else
	      	            {
          	            rmt_direction = SdpCodecStringConstants::EAttributeRecvonly;
          	            }
          	        }
          	    else
         	        {
          	        if ( !isNullAddress )
    				    {
                        // ignore that other party didn't understand us
                        // and full cancel of old school process
                        sSession.iOOldSchool = 0;
                        sSession.iOOldSchoolProceeding = 0;
                        sSession.iOOldSchoolCompleted = 0;
                        }
	       		
                    rmt_direction = SdpCodecStringConstants::EAttributeRecvonly;
          	        }
                oldSchool = 0;
	      	    }    
            else  //  didn't try old school yet
                {
                if ( isNullAddress )
                    {
                    // assuming that other party wanna put us on hold
	                rmt_direction = SdpCodecStringConstants::EAttributeSendonly; 
                    }
	            else if ( loc_direction == 
	                          SdpCodecStringConstants::EAttributeSendonly &&
	               		  aStream.Session()->Modifier( KMceMediaDirection ) ==
	              			  KMceMediaDirectionWithAddress )
                    {
                    // failed to put other party on hold, trying old school
                    sSession.iOOldSchool = 1;    
                    User::Leave( KMceErrOldSchool );
                    }
                else
                    {
                    // NOP
                    }
                }
	      	}
        else // answerer
            {
            if ( isNullAddress )
                {
                // other party puts us on hold
                rmt_direction = SdpCodecStringConstants::EAttributeSendonly;
                sSession.iIOldSchool = 1;
                oldSchool = sSession.iIOldSchool;
                }
	        else
	            {
	            rmt_direction = SdpCodecStringConstants::EAttributeSendrecv;
	            }   	
            }
	    }

    sSession.iIOldSchool = oldSchool;

	// if unable to determine other party direction then just ignore that
    if ( rmt_direction != KErrNotFound )
        {
    	// now checking for permitted directions
    	// inverse direction
        TInt inv_dir = rmt_direction;
        switch (inv_dir)
        	{
          	case SdpCodecStringConstants::EAttributeRecvonly: 
          		{
          		inv_dir=SdpCodecStringConstants::EAttributeSendonly; 
          		break;		
          		}      
          	case SdpCodecStringConstants::EAttributeSendonly: 
          		{
          		inv_dir=SdpCodecStringConstants::EAttributeRecvonly; 
          		break;		
          		}
          	default:
          		{
          		break;		
          		}
        	}

		// all directions are permitted for answerer and 
		// inactive is always accepted
    	if ( aRole == EMceRoleOfferer && 
    	     rmt_direction != SdpCodecStringConstants::EAttributeInactive )
    		{
    		// other party has incompatible direction. terminate session.
           	if ( loc_direction != SdpCodecStringConstants::EAttributeSendrecv 
           	     && 
           	     loc_direction != inv_dir )
           	    {
           	    User::Leave( KErrArgument );
           	    }
    		}

    	if ( loc_direction != inv_dir )
    	    {
    	    aStream.SetDirection( rmt_direction, KMceNoEndpointAdjustment );
    	    }
    	}

    MCEMM_DEBUG_SVALUE("decoded direction", 
    					SDP_STRING( aStream.Direction() ).DesC() )
    MCEMM_DEBUG("CMceMediaSdpCodec::DecodeDirectionL(), Exit ")
 	}

// -----------------------------------------------------------------------------
// CMceMediaSdpCodec::UpdateDirectionL
// -----------------------------------------------------------------------------
//
    
void CMceMediaSdpCodec::UpdateDirectionL( 
    CSdpMediaField& aMediaLine, 
    CMceComMediaStream& aStream,
    CSdpDocument& aSdpDocument ) const
	{
    TInt direction = Direction( aMediaLine, aSdpDocument.AttributeFields() );
    CSdpConnectionField* connectionField = aSdpDocument.ConnectionField();
    if ( connectionField == NULL )
        {
    	connectionField = ( aMediaLine.ConnectionFields()[0] )	;
    	}

    if ( connectionField->Address() == KMceSipNullAddress )	
        {
        direction = SdpCodecStringConstants::EAttributeSendonly;    
        }
    UpdateStreamL( aStream, direction );
    aStream.SetDirection( direction, KMceNoEndpointAdjustment );
	}

// -----------------------------------------------------------------------------
// CMceMediaSdpCodec::EncodeDirectionL
// -----------------------------------------------------------------------------
//
void CMceMediaSdpCodec::EncodeDirectionL( 
    CMceComMediaStream& aStream, 
    CSdpMediaField& aMedia,
    CSdpDocument& aSdpDocument,
    TMceNegotiationRole aRole ) const
    {
    MCEMM_DEBUG("CMceMediaSdpCodec::EncodeDirectionL(), Entry ")
        
    CMceSdpSession& sSession = aStream.Session()->SdpSession();

    if ( aRole == EMceRoleOfferer && 
    	 sSession.iOOldSchool &&  
    	 aStream.Direction() != SdpCodecStringConstants::EAttributeSendrecv )
    	     	 
        {
        CSdpConnectionField* connField = 
            CSdpConnectionField::DecodeL( KMceSipNullAddressA );
        aSdpDocument.SetConnectionField( connField );
        MCEMM_DEBUG("using old school")
        }
    
    else
        {
        RStringF direction = SDP_STRING( aStream.Direction() );

        CSdpAttributeField* mediaAttr = 
            CSdpAttributeField::NewLC( direction, KNullDesC8 );
            
        MCEMM_DEBUG_SVALUE("encoded direction", mediaAttr->Attribute().DesC() )
            
        aMedia.AttributeFields().AppendL( mediaAttr );
        CleanupStack::Pop( mediaAttr );
        }
    
    MCEMM_DEBUG("CMceMediaSdpCodec::EncodeDirectionL(), Exit ")
    }
    
// -----------------------------------------------------------------------------
// CMceMediaSdpCodec::EncodePreconditionsL
// -----------------------------------------------------------------------------
//
void CMceMediaSdpCodec::EncodePreconditionsL( 
    CMceComMediaStream& aStream,
    CSdpMediaField& aMediaLine,
    TMceNegotiationRole aRole ) const
    {
    CMceSecureMediaSession* secureSession = aStream.Session()->SecureSession();

    if ( aStream.Session()->Modifier( KMcePreconditions ) == KMcePreconditionsSupported &&
            !secureSession)
        {
        if ( ( aRole == EMceRoleOfferer && 
               aStream.Session()->iUseLocalPreconditions ) ||
             ( aRole == EMceRoleAnswerer && 
               aStream.Session()->iUseRemotePreconditions ) )
            {
             TMcePreconditions* precondition = aStream.PreconditionsL( KMcePreconditions );
             if ( precondition )
	        	{
	        	precondition->EncodeL( aMediaLine );
	        	}
            }
        }
  	if ( aStream.Session()->Modifier( KMceSecPreconditions ) == KMcePreconditionsE2ESupported )
        {
        TMcePreconditions* precondition = aStream.PreconditionsL( KMceSecPreconditions );
        if ( precondition )
        	{
        	precondition->EncodeL( aMediaLine );
        	}
        }    
    }

// -----------------------------------------------------------------------------
// CMceMediaSdpCodec::DecodePreconditionsL
// -----------------------------------------------------------------------------
//
TMceSipWarningCode CMceMediaSdpCodec::DecodePreconditionsL( 
    CSdpMediaField& aMediaLine,
    CMceComMediaStream& aStream,
    TMceNegotiationRole aRole ) const
    {
	MCEMM_DEBUG("CMceMediaSdpCodec::DecodePreconditionsL EntryL")
	TMceSipWarningCode warning = KErrNone;
	
    if ( aStream.Session()->Modifier( KMcePreconditions ) == KMcePreconditionsSupported  )
    	{
    	if ( ( aRole == EMceRoleOfferer && 
               aStream.Session()->iUseLocalPreconditions ) ||
             ( aRole == EMceRoleAnswerer && 
               aStream.Session()->iUseRemotePreconditions )  )
    		{
    		aStream.PreconditionsL( KMcePreconditions )->DecodeL( aMediaLine );	
    		
	        }
        }   
	if ((aRole == EMceRoleOfferer && 
     	aStream.Session()->iClientCryptoSuites.Count()
     	&& aStream.Session()->Modifier( KMceSecPreconditions ) 
     	== KMcePreconditionsE2ESupported )
     	|| ( aRole ==EMceRoleAnswerer && (
     		aStream.Session()->iRemoteSecPreconditionsRequired ||
     		 aStream.Session()->iClientCryptoSuites.Count() )))
      
     	{
		TMcePreconditions* precondition = 
			aStream.PreconditionsL( KMceSecPreconditions, &aMediaLine );
	
    	if ( !precondition )
    		{
    		return KErrNone;
    		}
    	if ( warning && aStream.Preconditions().Count() )
    		{
    			//normal segmented precondition cases
    		aStream.Session()->iRemoteSecPreconditionsRequired = EFalse;
    		aStream.Session()->Modifier( KMceSecPreconditions ) = 
    			KMcePreconditionsNotUsed;
    		MCEMM_DEBUG("Secure preconds not used")	
    		MCEMM_DEBUG("CMceMediaSdpCodec::DecodePreconditionsL Exit")	
    		return KErrNone;
    		}
    
    	if ( !precondition->IsMet() )
    		{
    		MCEMM_DEBUG("Decode Secure Precondition")	
    		warning = precondition->DecodeL( aMediaLine );	
    		}
    	warning = warning == KMceSipPreconditionFailure ? 
     			warning : KErrNone;
     	MCEMM_DEBUG_DVALUE ("Decode Secure Precondition warning", warning ) 
     	}        
   	MCEMM_DEBUG("CMceMediaSdpCodec::DecodePreconditionsL Exit")	
	return warning;
    }

// -----------------------------------------------------------------------------
// CMceMediaSdpCodec::EncodeClientAttributesL
// -----------------------------------------------------------------------------
//
void CMceMediaSdpCodec::EncodeClientAttributesL( 
    CMceComMediaStream& aStream,
    CSdpMediaField& aMediaLine ) const
    {
    MCEMM_DEBUG("CMceMediaSdpCodec::EncodeClientAttributesL, Entry" )
    
    CDesC8Array* clientAttributes = aStream.iLocalMediaSDPLines;
    __ASSERT_ALWAYS( clientAttributes, User::Leave( KErrArgument ) );

    CMceSdpCodec::EncodeClientSdpFieldsL<CSdpMediaField>( *clientAttributes, aMediaLine );
    
        
    MCEMM_DEBUG("CMceMediaSdpCodec::EncodeClientAttributesL, Exit" )
    }    

// -----------------------------------------------------------------------------
// CMceMediaSdpCodec::DecodeClientAttributesL
// -----------------------------------------------------------------------------
//
void CMceMediaSdpCodec::DecodeClientAttributesL( 
    CSdpMediaField& aMediaLine, 
    CMceComMediaStream& aStream ) const
    {
    MCEMM_DEBUG("CMceMediaSdpCodec::DecodeClientAttributesL, Entry" )
    CDesC8Array* remoteAttributes = 
        new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
    CleanupStack::PushL( remoteAttributes );
    CBufFlat* encodeBuf = NULL;
    
    for ( TInt i = 0; i < aMediaLine.AttributeFields().Count(); i++ )
        {
    	encodeBuf = CBufFlat::NewL( KMceExternalizeBufferExpandSize );
    	CleanupStack::PushL( encodeBuf );
    	RBufWriteStream writeStream( *encodeBuf, 0 );
	    writeStream.PushL();
        
        aMediaLine.AttributeFields()[ i ]->EncodeL( writeStream );
        MCEMM_DEBUG_SVALUE( "found attribute", encodeBuf->Ptr( 0 ) )
        remoteAttributes->AppendL( encodeBuf->Ptr( 0 ) );
    	
    	CleanupStack::PopAndDestroy(); // writeStream
        CleanupStack::PopAndDestroy( encodeBuf ); // encodeBuf
        }
    
    for ( TInt i = 0; i < aMediaLine.BandwidthFields().Count(); i++ )
        {
        
    	encodeBuf = CBufFlat::NewL( KMceExternalizeBufferExpandSize );
    	CleanupStack::PushL( encodeBuf );
    	RBufWriteStream writeStream( *encodeBuf, 0 );
	    writeStream.PushL();
        
        aMediaLine.BandwidthFields()[ i ]->EncodeL( writeStream );
        MCEMM_DEBUG_SVALUE("found attribute", encodeBuf->Ptr( 0 ) )
        remoteAttributes->AppendL( encodeBuf->Ptr( 0 ) );
    	
    	CleanupStack::PopAndDestroy(); // writeStream
        CleanupStack::PopAndDestroy( encodeBuf ); // encodeBuf
        }
        
    if ( aStream.BoundStream() )
        {
        MCEMM_DEBUG("copy attributes to bound stream" )
        // copy attributes to bound stream
        CMceComMediaStream& boundStream = aStream.BoundStreamL();
        CDesC8Array* copyAttributes = 
            new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
        CleanupStack::PushL( copyAttributes );
        for ( TInt i = 0; i < remoteAttributes->MdcaCount(); i++ )
            {
            copyAttributes->AppendL( remoteAttributes->MdcaPoint( i ) );
            }
        CleanupStack::Pop( copyAttributes );
        MCE_DELETE( boundStream.iRemoteMediaSDPLines );
        boundStream.iRemoteMediaSDPLines = copyAttributes;
        }
        
    CleanupStack::Pop( remoteAttributes );
    MCE_DELETE( aStream.iRemoteMediaSDPLines );
    aStream.iRemoteMediaSDPLines = remoteAttributes;
    MCEMM_DEBUG("CMceMediaSdpCodec::DecodeClientAttributesL, Exit" )
    }

// -----------------------------------------------------------------------------
// CMceMediaSdpCodec::ValidateSdp
// -----------------------------------------------------------------------------
//
TMceSipWarningCode CMceMediaSdpCodec::ValidateSdpL( 
    CSdpMediaField& aMediaLine,
    CSdpDocument& /*aSdpDocument*/ )
    {
    TMceSipWarningCode warning = KErrNone;
    RArray<TUint> payloadTypes;
    TInt error = KErrNone;
    RPointerArray< CSdpFmtAttributeField >& formatLines =  
                                            aMediaLine.FormatAttributeFields();

    TInt dynamicPayloadsCount = 0;
    TInt rtpmapCount = 0;
    
    error = DecodeFormatList( aMediaLine, payloadTypes );
    
    User::LeaveIfError( error );
    
    if ( error == KErrNone )
        {
        TInt index = 0;
        while( index < payloadTypes.Count() )
            {
            dynamicPayloadsCount = payloadTypes[ index++ ] >= KMinDynamicPT ?
                                   dynamicPayloadsCount + 1 : dynamicPayloadsCount;
            }
        index = 0;
        while( index < formatLines.Count() )
            {
            rtpmapCount = formatLines[ index++ ]->Attribute() == 
                                SDP_STRING( SdpCodecStringConstants::EAttributeRtpmap ) ?
                          rtpmapCount + 1 : rtpmapCount;
            }
        
        if ( dynamicPayloadsCount > 0 && rtpmapCount == 0 )
            {
            warning = KMceSipBadRequest;
            }
        }
    else
        {
        warning = KMceSipServerInternalError;
        }
    
    payloadTypes.Close();
    
    return warning;
    }

// -----------------------------------------------------------------------------
// CMceMediaSdpCodec::DecodePayloadsL
// -----------------------------------------------------------------------------
//
TInt CMceMediaSdpCodec::DecodePayloadsL( 
    CSdpMediaField& aMediaLine,
    CMceComMediaStream& aStream,
    TMceNegotiationRole aRole ) const
    {
    TInt decoded( 0 );
    
    RArray<TUint> payloadTypes;
    CleanupClosePushL( payloadTypes );
    
    if ( aRole == EMceRoleAnswerer )
        {
        decoded = DecodeRtpmapLinesL( aMediaLine, aStream, EMceRoleAnswerer );
        decoded += DecodeStaticPayloadsL( aMediaLine, aStream, payloadTypes, EMceRoleAnswerer );
        }
    else
        { 
        // static payloads are decoded first if role is offerer
        decoded = DecodeStaticPayloadsL( aMediaLine, aStream, payloadTypes, EMceRoleOfferer );
        decoded += DecodeRtpmapLinesL( aMediaLine, aStream, EMceRoleOfferer );
        }
        
    UpdateSdpCodecIndexesL( aStream, payloadTypes );
    CleanupStack::PopAndDestroy( &payloadTypes );
    
    return decoded;
    }
    
// -----------------------------------------------------------------------------
// CMceMediaSdpCodec::UpdateSdpCodecIndexesL
// -----------------------------------------------------------------------------
//
void CMceMediaSdpCodec::UpdateSdpCodecIndexesL( 
    CMceComMediaStream& aStream,
    RArray<TUint>& aPayloadTypes ) const
    {
    TInt index( 0 );
    TInt codecSdpIndex( 0 );
    while( index < aPayloadTypes.Count() )
        {
        TUint payload = aPayloadTypes[ index++ ];
        CMceComCodec* codec = FindCodec( payload, aStream );
        if ( codec )
            {
            codec->iCodecSdpIndex = codecSdpIndex++;
            }  
        }
    }

// -----------------------------------------------------------------------------
// CMceMediaSdpCodec::GetCorrespondingFmtpLineL
// -----------------------------------------------------------------------------
//
TPtrC8 CMceMediaSdpCodec::GetCorrespondingFmtpLineL( 
    CSdpMediaField& aMediaLine, 
    CSdpFmtAttributeField& aRtpMapFormatLine ) const
    {
    const TDesC8& payloadTypeInRtpMap = aRtpMapFormatLine.Format();
    TUint rtpMapPT = ConvertDesToUintL( payloadTypeInRtpMap );
    
    RPointerArray< CSdpFmtAttributeField >& formatLines =  
        aMediaLine.FormatAttributeFields();
    
    TPtrC8 fmtp( KNullDesC8() );
    TBool found( EFalse );
    for ( TInt j = 0; j < formatLines.Count() && !found; j++ )
    	{
    	CSdpFmtAttributeField* formatLine = formatLines[ j ];
    	if ( formatLine->Attribute() == 
    	            SDP_STRING( SdpCodecStringConstants::EAttributeFmtp ) )
    		{
    		TUint fmtpPT = ConvertDesToUintL( formatLine->Format() );
    		if ( rtpMapPT == fmtpPT )
    		    {
    		    fmtp.Set( formatLine->Value() );
    		    found = ETrue;
    		    }
    		}
    	}
    return fmtp;
    }
    
// End of file
