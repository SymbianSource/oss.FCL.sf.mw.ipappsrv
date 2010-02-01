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


#include <sdpdocument.h>

#include <sdpcodecstringconstants.h>
#include <sdpcodecstringpool.h>
#include <sdpmediafield.h>
#include <sdpattributefield.h>
#include <sdprtpmapvalue.h>
#include <sdpfmtattributefield.h>
#include <mmcccodecinformation.h>

#include "mcesdpcodec.h"
#include "mcecomcodec.h"
#include "mcemmlogs.h"


const TChar KMceRealPoint = '.';


// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CMceSdpCodec::CMceSdpCodec
// -----------------------------------------------------------------------------
CMceSdpCodec::CMceSdpCodec( RStringF aMedia )
    : iMedia( aMedia )
    {
    }


// -----------------------------------------------------------------------------
// CMceSdpCodec::~CMceSdpCodec
// -----------------------------------------------------------------------------
CMceSdpCodec::~CMceSdpCodec()
    {
    iMedia.Close();
    delete iCapabilities;
    iStringTable = NULL;
    iCapabilitiesPT.Close();
    }

// ---------------------------------------------------------
// CMceSdpCodec::ConstructL
// ---------------------------------------------------------
void CMceSdpCodec::ConstructL()
    {

    iStringPool = SdpCodecStringPool::StringPoolL();
    iStringTable = &SdpCodecStringPool::StringTableL();
    
    iCapabilities = new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
    iCapabilitiesPT.Reset();
    }

// -----------------------------------------------------------------------------
// CMceSdpCodec::Media
// -----------------------------------------------------------------------------
RStringF CMceSdpCodec::Media()
    {
    return iMedia;
    }


// -----------------------------------------------------------------------------
// CMceSdpCodec::Decodes
// -----------------------------------------------------------------------------
TBool CMceSdpCodec::Decodes( const CSdpMediaField& aMedia ) const
    {
    return iMedia == aMedia.Media();
    }

// -----------------------------------------------------------------------------
// CMceSdpCodec::AddCapabilityL
// -----------------------------------------------------------------------------
void CMceSdpCodec::AddCapabilityL( const TDesC8& aCapability )
    {
    iCapabilities->AppendL( aCapability );
    }

// -----------------------------------------------------------------------------
// CMceSdpCodec::AddCapabilityPayloadTypeL
// -----------------------------------------------------------------------------
void CMceSdpCodec::AddCapabilityPayloadTypeL( const TUint aPayloadType )
    {
    iCapabilitiesPT.AppendL(aPayloadType);
    }

// -----------------------------------------------------------------------------
// CMceSdpCodec::IsSupported
// -----------------------------------------------------------------------------
TBool CMceSdpCodec::IsSupported( const TDesC8& aCapability ) const
    {
    TBool isSupported = EFalse;
    TInt index = 0;
    
    while( !isSupported && index < iCapabilities->Count() )
        {
        isSupported = !aCapability.CompareF( iCapabilities->MdcaPoint( index ++ ) );
        }
    
    return isSupported;
    
    }

// -----------------------------------------------------------------------------
// CMceSdpCodec::IsPayloadTypeSupported
// -----------------------------------------------------------------------------
TBool CMceSdpCodec::IsPayloadTypeSupported( const TDesC8& aCapability, const TUint aPayloadType ) const
    {
    TBool isSupported = EFalse;
    TInt index = 0;
    
    while( !isSupported && index < iCapabilities->Count() )
        {
        if ( !aCapability.CompareF( iCapabilities->MdcaPoint( index ) ) &&  index < iCapabilitiesPT.Count())
        	{
        	if (iCapabilitiesPT [ index ] >= KMinDynamicPT || iCapabilitiesPT [ index ] == aPayloadType)
        		{
        		isSupported = ETrue;
        		}
        	}
        index ++;
        }
    
    return isSupported;
    
}

// -----------------------------------------------------------------------------
// CMceSdpCodec::Direction
// -----------------------------------------------------------------------------
//
TInt CMceSdpCodec::Direction( 
    CSdpMediaField& aMedia, 
    RPointerArray<CSdpAttributeField>& aSessionAttributes ) const
    {
    
    TInt direction = KErrNotFound;
    TInt index = 0;
    
    // look for the direction attribute, go through all attribute fields.
    while( direction == KErrNotFound && index < aSessionAttributes.Count() )
        {
        RStringF attribute = aSessionAttributes[index++]->Attribute();
        
        if ( attribute == SDP_STRING( SdpCodecStringConstants::EAttributeSendonly ) ||
             attribute == SDP_STRING( SdpCodecStringConstants::EAttributeRecvonly ) ||
             attribute == SDP_STRING( SdpCodecStringConstants::EAttributeInactive ) ||
             attribute == SDP_STRING( SdpCodecStringConstants::EAttributeSendrecv ) )
            {
            // downlink
            direction = attribute.Index( *iStringTable );
            }
        }
        
    if ( direction == KErrNotFound )
        {
        index = 0;
        
        RPointerArray<CSdpAttributeField>& attrfields = aMedia.AttributeFields();
        // look for the direction attribute, go through all attribute fields. if not
        // found then use the default sendrcv
        while( direction == KErrNotFound && index < attrfields.Count() )
            {
    
            RStringF attribute = attrfields[index++]->Attribute();
            if ( attribute == SDP_STRING( SdpCodecStringConstants::EAttributeSendonly ) ||
                 attribute == SDP_STRING( SdpCodecStringConstants::EAttributeRecvonly ) ||
                 attribute == SDP_STRING( SdpCodecStringConstants::EAttributeInactive ) ||
                 attribute == SDP_STRING( SdpCodecStringConstants::EAttributeSendrecv ) )
                {
                // downlink
                direction = attribute.Index( *iStringTable );
                }
            
            }
        }
         
    return direction;
    
    }




// -----------------------------------------------------------------------------
// CMceSdpCodec::PrepareForEncodeL
// -----------------------------------------------------------------------------
//
void CMceSdpCodec::PrepareForEncodeL( CMceComMediaStream& /*aStream*/,
                                           CSdpMediaField& aMediaLine )
    {
    __ASSERT_ALWAYS( aMediaLine.IsValid(),
                     User::Leave( KErrArgument ) );
    CleanAttributes( aMediaLine );
    if( aMediaLine.Key() )
    	{
    	aMediaLine.SetKey(NULL);
    	}
    }


    
// -----------------------------------------------------------------------------
// CMceSdpCodec::EncodeRtpMapFieldsL
// -----------------------------------------------------------------------------
//
CSdpFmtAttributeField* CMceSdpCodec::EncodeRtpMapFieldsL( 
    const TUint8 aPayloadType,
    const TDesC8& aEncodingName,
    const TUint aClockRate,
    const TDesC8& aEncodingParam ) const
    {
    MCEMM_DEBUG("CMceSdpCodec::EncodeRtpMapFieldsL(), Entry ");
    
    MCE_DEFINE_DECSTR( clockrate, aClockRate );
    MCE_DEFINE_DECSTR( payload, aPayloadType );
    RStringF rtpmapStr = SDP_STRING( SdpCodecStringConstants::EAttributeRtpmap );
                
    TSdpRtpmapValue rtpmapValue( aEncodingName, 
                                 clockrate, 
                                 aEncodingParam );                              
    
    HBufC8* rtpmapBuf = rtpmapValue.EncodeL();
    CleanupStack::PushL( rtpmapBuf );

    
    CSdpFmtAttributeField* rtpmapAttribute = 
        CSdpFmtAttributeField::NewL( rtpmapStr, payload, *rtpmapBuf );
    CleanupStack::PopAndDestroy( rtpmapBuf ); 
    
    MCEMM_DEBUG("CMceSdpCodec::EncodeRtpMapFieldsL(), Exit ");
    
    return rtpmapAttribute;
    }


// -----------------------------------------------------------------------------
// CMceSdpCodec::FindAttribute
// -----------------------------------------------------------------------------
//
CSdpAttributeField* CMceSdpCodec::FindAttribute( TInt aAttribute, 
                                                 CSdpMediaField& aMediaLine ) const
    {
    CSdpAttributeField* attributeField = NULL;
    RPointerArray<CSdpAttributeField>& attributeFields = 
                                                aMediaLine.AttributeFields();
    TInt index = 0;
    while( !attributeField && index < attributeFields.Count() )
        {
        attributeField = attributeFields[ index++ ];
        RStringF attribute = attributeField->Attribute();
        attributeField = 
            attribute == SDP_STRING( aAttribute ) ? attributeField : NULL;
        }

    return attributeField;
        
    }


// -----------------------------------------------------------------------------
// CMceSdpCodec::ConvertDesToUintL
// -----------------------------------------------------------------------------
//
TUint CMceSdpCodec::ConvertDesToUintL( const TDesC8& aConv )
    {
    TUint covertedUint = 0;
    TLex8 lexConv( aConv );
    User::LeaveIfError ( lexConv.Val( covertedUint, EDecimal ) );
    return covertedUint;
    }

// -----------------------------------------------------------------------------
// CMceSdpCodec::ConvertDesToIntL
// -----------------------------------------------------------------------------
//
TInt CMceSdpCodec::ConvertDesToIntL( const TDesC8& aConv )
    {
    TInt covertedInt = 0;
    TLex8 lexConv( aConv );
    User::LeaveIfError ( lexConv.Val( covertedInt ) );
    return covertedInt;
    }


// -----------------------------------------------------------------------------
// CMceSdpCodec::ConvertTRealToDesLC
// -----------------------------------------------------------------------------
//
HBufC8* CMceSdpCodec::ConvertTRealToDesLC( const TReal& aConv )
    {
    const TInt KMceMaxTRealLen = 20;
    HBufC8* buf  = HBufC8::NewLC( KMceMaxTRealLen );
    TPtr8 ptr = buf->Des();
    TRealFormat format;
    format.iType = KRealFormatGeneral;
    format.iWidth = KMceMaxTRealLen;
    format.iPoint = KMceRealPoint;
    ptr.Num( aConv, format );
    return buf;
    }

// -----------------------------------------------------------------------------
// CMceSdpCodec::ConvertDesToReal
// -----------------------------------------------------------------------------
//
TReal CMceSdpCodec::ConvertDesToRealL( const TDesC8& aConv )
    {
    TReal val;
    TLex8 lexer( aConv );
    User::LeaveIfError( lexer.Val( val, KMceRealPoint ) );
    return val;
    }


