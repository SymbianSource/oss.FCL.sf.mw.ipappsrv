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
#include <e32base.h>
#include <e32std.h>
#include <e32def.h>
#include <stringpool.h>

#include <sdpdocument.h>
#include <sdpcodecstringconstants.h>
#include <sdpcodecstringpool.h>
#include <sdpmediafield.h>
#include <sdpattributefield.h>
#include <sdprtpmapvalue.h>
#include <sdpfmtattributefield.h>
#include <sdporiginfield.h>
#include <e32err.h>
#include <uricommon.h>
#include <uri8.h>
#include <uriutils.h>
#include <SdpConnectionField.h>

#include "mcemediadefs.h"
#include "mcemessagesdpcodec.h"
#include "mcecommessagestream.h"
#include "mcemessagestream.h"
#include "mcecommessagecodec.h"
#include "mcemediadefs.h"
#include "mcecomfactory.h"
#include "mcemmlogs.h"
#include "mcecommsrpsource.h"
#include "mcecomexternalsink.h"
#include "mcecomexternalsource.h"
#include "mcecommsrpsink.h"
#include "mcecommediasource.h"
#include "mcedefs.h"
#include "mcecomendpoint.h"
#include <mcemsrpsink.h>
#include <mcemsrpsource.h>
#include <mcemessagesink.h>
#include <mcemessagesource.h>
#include "mcecommessagesink.h"
#include "mcecommessagesource.h"
#include "mcemsrpsink.h"
#include "mceexternalsource.h"
#include "mceexternalsink.h"


// ================= MEMBER FUNCTIONS =======================


// -----------------------------------------------------------------------------
// CMceMessageSdpCodec::NewL
// -----------------------------------------------------------------------------
//
CMceMessageSdpCodec* CMceMessageSdpCodec::NewL()
    {
    MCEMM_DEBUG( "CMceMessageSdpCodec::CMceMessageSdpCodec::NewL, Entry" )
    RStringF message = MCE_SDP_STRING_MESSAGEL();
    /*
    RStringPool sPool;
    RStringF message =sPool.OpenFStringL(KMceMediaMessage);
    */    
        
    CMceMessageSdpCodec* self = new (ELeave) CMceMessageSdpCodec( message );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    MCEMM_DEBUG( "CMceMessageSdpCodec::CMceMessageSdpCodec::NewL, Exit" )
    return self;
    }

// -----------------------------------------------------------------------------
// CMceMessageSdpCodec::CMceMessageSdpCodec
// -----------------------------------------------------------------------------
//
CMceMessageSdpCodec::CMceMessageSdpCodec( RStringF aMedia )
    : CMceMediaSdpCodec( aMedia )
    {
    }


// -----------------------------------------------------------------------------
// CMceMessageSdpCodec::~CMceMessageSdpCodec
// -----------------------------------------------------------------------------
//
CMceMessageSdpCodec::~CMceMessageSdpCodec()
    { 
    MCEMM_DEBUG( "CMceMessageSdpCodec::~CMceMessageSdpCodec, Entry" )
    iNetType.Close();
    iAddressType.Close();
    iAcceptTypesStr.Close();
    iPathStr.Close();
    iAcceptWrappedTypesStr.Close();
    iFileSelectorStr.Close();
    iFileTransferidStr.Close();  
    iFileDispositionStr.Close();
    iConnectionStr.Close();
    MCEMM_DEBUG( "CMceMessageSdpCodec::~CMceMessageSdpCodec, Exit" )
    }

// -----------------------------------------------------------------------------
// CMceMessageSdpCodec::Encodes
// -----------------------------------------------------------------------------
//
TBool CMceMessageSdpCodec::Encodes( const CMceComMediaStream& aStream ) const
    {
    return aStream.iStreamType != CMceComMediaStream::ELocalStream &&
           aStream.iType == KMceMessage;
    }


// -----------------------------------------------------------------------------
// CMceMessageSdpCodec::EncodeRtpmapAttributeLC
// -----------------------------------------------------------------------------
//
CSdpFmtAttributeField* CMceMessageSdpCodec::EncodeRtpmapAttributeLC( CMceComCodec& /*aCodec*/ ) const
    {
    MCEMM_DEBUG( "CMceMessageSdpCodec::EncodeRtpmapAttributeLC(), Entry" )
    
    CSdpFmtAttributeField* rtpmapAttribute = NULL;
    
    CleanupStack::PushL( rtpmapAttribute );
    
    MCEMM_DEBUG( "CMceMessageSdpCodec::EncodeRtpmapAttributeLC(), Exit" )
    return rtpmapAttribute;
    }



// -----------------------------------------------------------------------------
// CMceMessageSdpCodec::EncodeMessageMediaAttributesL
// -----------------------------------------------------------------------------
//

void CMceMessageSdpCodec::EncodeMessageMediaAttributesL( CMceComMessageStream& aStream, 
                                                            CSdpMediaField& aMediaLine)                                             
     {
     // Message stream level attributes will be set through client SDP attributes.
    MCEMM_DEBUG( "CMceMessageSdpCodec::EncodeMessageMediaAttributesL, Entry" )
     CMceComMsrpSource* msrpSource = NULL;
     CMceComMsrpSink* msrpSink = NULL;
     if ( (aStream.iStreamType == CMceComMediaStream::ESendOnlyStream) ||
             (aStream.iStreamType == CMceComMediaStream::ESendStream))
         {
         for(TInt i=0; i<aStream.Sinks().Count(); i++)
             {
             if ( (aStream.Sinks())[i]->iType == KMceMSRPSink)
                 {
                 msrpSink = dynamic_cast<CMceComMsrpSink*>( aStream.Sinks()[i]);
                 break;
                 }
             }
         }
        else  //it meamsn it is sendrecv stream so the parameters are expected to be set to msrpSink
         {
         __ASSERT_ALWAYS(aStream.Source()->iType == KMceMSRPSource, User::Leave(KErrArgument));
         msrpSource = dynamic_cast<CMceComMsrpSource*>( aStream.Source());
         }
     
     __ASSERT_ALWAYS(msrpSource!=NULL || msrpSink!= NULL, User::Leave(KErrNotFound));
     
     RPointerArray<CMceComMediaSink> &sinks = aStream.Sinks(); //== KMceMessageSink
     
     if( (aStream.iSource->iType == KMceMSRPSource && sinks[0]->iType == KMceMessageSink) || aStream.iSource->iType == KMceMessageSource) // down link
         {
         if(aStream.iLocalMsrpPath.Length() && ( aStream.Session()->Streams().Count() != 2 || aStream.iStreamType != CMceComMediaStream::ESendOnlyStream) )
             {
             if (iPathStr.DesC().Length() == 0 )
                 {
                 iPathStr = SdpCodecStringPool::StringPoolL().OpenFStringL(KMceSDPNamePath);
                 CleanupClosePushL( iPathStr );
                 }
             
             HBufC16* localPath = HBufC16::NewLC(256);
             localPath->Des().Copy(aStream.iLocalMsrpPath);
             CUri8* uri = UriUtils::CreateUriL(*localPath);
             
             // if the realy is supported then the last entry will always indicates its own MSRP URI
             SetHostAddrFromPathAttrL(uri->Uri(), aMediaLine); 
             // Set local port number from the msrp path
             //SetPortFromPathAttrL((TUriC8&)uri->Uri(), aStream);            
             // Create the path SDP attr and append this to media line
             CSdpAttributeField* pathAttr = CSdpAttributeField::NewLC( iPathStr, aStream.iLocalMsrpPath);                     
             MCEMM_DEBUG_SVALUE( "path Attribute value from MSRP stack:", aStream.iLocalMsrpPath )         
             aMediaLine.AttributeFields().AppendL( pathAttr );
             CleanupStack::Pop(pathAttr);
             CleanupStack::PopAndDestroy(localPath);
             if (iPathStr.DesC().Length() != 0 )
                 CleanupStack::PopAndDestroy(&iPathStr);
             
             delete uri;
             }
         }
     else if(aStream.iSource->iType == KMceMessageSource && msrpSink->iType == KMceMSRPSink) // message uplink
         {
         //NOP
         }
     else
         {
         SetPathAttributeL(aStream, msrpSource, msrpSink, aMediaLine);
         }
    // EncodeFileAttributeL( aStream, aMediaLine );
     SetAcceptTypesAttributeL(msrpSource, msrpSink, aMediaLine);
     SetAcceptWrappedAttributeL(msrpSource, msrpSink, aMediaLine);
     SetFileShareAttributeL(msrpSource, msrpSink, aMediaLine);
     
     // Set Information attribute
     HBufC8* infoPtr = msrpSource!=NULL ? msrpSource->iSessInfo : msrpSink->iSessInfo;
     if (infoPtr != NULL)
         aMediaLine.SetInfoL(infoPtr->Des());

     // set msrp conneciton attribute
     SetConnectionAttributeL(aStream, aMediaLine);
     MCEMM_DEBUG( "CMceMessageSdpCodec::EncodeMessageMediaAttributesL, Exit" )
     }

// -----------------------------------------------------------------------------
// CMceMessageSdpCodec::EncodeMediaAttributesL
// -----------------------------------------------------------------------------
//
void CMceMessageSdpCodec::EncodeMediaAttributesL( CMceComCodec& /*aCodec*/, 
                                                CSdpMediaField& /*aMediaLine*/, 
                                                CSdpFmtAttributeField& /*aRtpmap*/ ) const
    {
    MCEMM_DEBUG( "CMceMessageSdpCodec::EncodeMediaAttributesL(), Entry" )
    MCEMM_DEBUG( "CMceMessageSdpCodec::EncodeMediaAttributesL(), Exit" )
    }
    
// -----------------------------------------------------------------------------
// CMceMessageSdpCodec::DecodeMediaAttributesL 
// -----------------------------------------------------------------------------
//
void CMceMessageSdpCodec::DecodeMediaAttributesL(
    CSdpMediaField& /*aMediaLine*/, 
    CMceComCodec& /*aCodec*/,
    CSdpFmtAttributeField& /*aRtpmap*/ ) const
    {
    MCEMM_DEBUG( "CMceMessageSdpCodec::DecodeMediaAttributesL(), Entry ")

    MCEMM_DEBUG("CMceMessageSdpCodec::DecodeMediaAttributesL(), Exit ")
    }


// -----------------------------------------------------------------------------
// CMceMessageSdpCodec::CreateStreamLC
// -----------------------------------------------------------------------------
//
CMceComMediaStream* CMceMessageSdpCodec::CreateStreamLC( TInt aType ) const
    {
    MCEMM_DEBUG( "CMceMessageSdpCodec::CreateStreamLC(), Entry" )
   CMceComMessageStream* messagestream = NULL;
            
    if( aType == SdpCodecStringConstants::EAttributeSendonly )
        {
        // downlink
        MCEMM_DEBUG( "create downlink" )
        messagestream = CreateDownlinkStreamL();
        }
    else if( aType == SdpCodecStringConstants::EAttributeRecvonly )        
        {
        // uplink 
        MCEMM_DEBUG( "create uplink" )
        messagestream = CreateUplinkStreamL();
        }
    else
        {
        // sendrcv 
        // create two way stream
        // create downlink
        MCEMM_DEBUG( "create two-way stream" )
        messagestream = CreateDownlinkStreamL();
        CleanupStack::PushL( messagestream );
        // create bounded uplink
       CMceComMessageStream *boundUplink = CreateUplinkStreamL();
        CleanupStack::PushL( boundUplink );
        messagestream->BindL( boundUplink );
        CleanupStack::Pop( boundUplink ); 
        CleanupStack::Pop( messagestream );
        }
    
    CleanupStack::PushL( messagestream );
    
    MCEMM_DEBUG( "CMceMessageSdpCodec::CreateStreamLC(), Exit" )
    return messagestream;
    
    }


// -----------------------------------------------------------------------------
// CMceMessageSdpCodec::UpdateStreamL
// -----------------------------------------------------------------------------
//
void CMceMessageSdpCodec::UpdateStreamL( CMceComMediaStream& aStream, TInt aDirection ) const
    {
    MCEMM_DEBUG( "CMceMessageSdpCodec::UpdateStreamL(), Entry ")
    MCE_DEFINE_AUDIO( stream, aStream );

   CMceComMessageStream* messagestream = NULL;
   CMceComMessageStream& amessagestream = (CMceComMessageStream&)( aStream );
    TInt streamType = stream.SdpStreamType();
    
    if ( aDirection != SdpCodecStringConstants::EAttributeInactive )
        {
        switch( streamType )
            {
            case SdpCodecStringConstants::EAttributeRecvonly:
                {
                //if direction is changed
                if ( aDirection != SdpCodecStringConstants::EAttributeSendonly )
                    {
                    MCEMM_DEBUG( "update from recvonly to send" )
                    MCEMM_DEBUG( "create uplink" )
                    messagestream = CreateUplinkStreamL();
                    }
                break;
                }
            case SdpCodecStringConstants::EAttributeSendonly:
                {
                //if direction is changed
                if ( aDirection != SdpCodecStringConstants::EAttributeRecvonly )
                    {
                    MCEMM_DEBUG( "update from sendonly to recv" )
                    MCEMM_DEBUG( "create downlink" )
                    messagestream = CreateDownlinkStreamL();
                    }
                break;
                }
            case SdpCodecStringConstants::EAttributeSendrecv:
            default:
                {
                break;
                }
            }
        }
        
    if ( messagestream )
        {
        CleanupStack::PushL( messagestream );

        MCEMM_DEBUG( "cloning codecs" )
        
        for( TInt index = 0; index < amessagestream.CodecCount(); ++index )
            {
            // No ceodecs for message stream presently
            CMceComMessageCodec* codec = static_cast<CMceComMessageCodec*>( amessagestream.CodecL( index )->CloneL() );
            CleanupStack::PushL( codec );
            
            MCEMM_DEBUG_SVALUE( "cloned codec", codec->iSdpName )
            
            messagestream->AddCodecL( codec );
            CleanupStack::Pop( codec );
            }

        amessagestream.BindL( messagestream );
        messagestream->InitializeL( *amessagestream.Session() );
        
        MCEMM_DEBUG( "binded stream created" )
        CleanupStack::Pop( messagestream ); 
        }
    
    MCEMM_DEBUG( "CMceMessageSdpCodec::UpdateStreamL(), Exit" )
    }

// -----------------------------------------------------------------------------
// CMceMessageSdpCodec::CreateCodecLC
// -----------------------------------------------------------------------------
//
CMceComCodec* CMceMessageSdpCodec::CreateCodecLC( CSdpFmtAttributeField& /*aRtpmap*/ ) const
    {
    MCEMM_DEBUG( "CMceMessageSdpCodec::CreateCodecLC(), Entry" )
    
    CMceComCodec* codec = NULL;

    TPtrC8 codecName(KMceSDPNameMsrp);
        
    TMceComMessageCodecFactory factory;
    codec =  factory.CreateLC( codecName );
    
    //Junk Value for dummy msrp codec
    codec->iPayloadType = 90;  // dummy payload type + using unassigned value
    MCEMM_DEBUG_SVALUE( "created codec", codecName )
        
    MCEMM_DEBUG( "CMceMessageSdpCodec::CreateCodecLC(), Exit" )
    return codec;
    }


// -----------------------------------------------------------------------------
// CMceMessageSdpCodec::CreateCodecLC
// -----------------------------------------------------------------------------
//
CMceComCodec* CMceMessageSdpCodec::CreateCodecLC( TUint /*aPayload*/, 
                                                CSdpMediaField& /*aMediaLine*/ ) const
    {
    MCEMM_DEBUG( "CMceMessageSdpCodec::CreateCodecLC( payload ), Entry" )

    TMceComMessageCodecFactory factory;  
    CMceComMessageCodec* codec = NULL;
    
    TUint aPayload = 90;  // dummy payload type + using unassigned value
    codec = factory.CreateCodecLC( aPayload );
    MCEMM_DEBUG( "CMceMessageSdpCodec::CreateCodecLC( payload ), Exit" )
   
    return codec;
    }

    
// -----------------------------------------------------------------------------
// CMceMessageSdpCodec::DecodeSessionMediaAttributesL
// -----------------------------------------------------------------------------
//
void CMceMessageSdpCodec::DecodeSessionMediaAttributesL( CMceComMediaStream& /*aStream*/, 
                                                       CSdpDocument& /*aSdpDocument*/ ) const
    {
    MCEMM_DEBUG( "CMceMessageSdpCodec::DecodeSessionMediaAttributesL(), Entry" )
    MCEMM_DEBUG( "CMceMessageSdpCodec::DecodeSessionMediaAttributesL(), Exit" )
    }
    
// -----------------------------------------------------------------------------
// CMceMessageSdpCodec::CreateUplinkStreamL
// -----------------------------------------------------------------------------
//
CMceComMessageStream* CMceMessageSdpCodec::CreateUplinkStreamL() const
    {
    MCEMM_DEBUG( "CMceMessageSdpCodec::CreateUplinkStreamL(), Entry" )
    
   CMceComMessageStream *messagestream =CMceComMessageStream::NewL();
    CleanupStack::PushL( messagestream );
    // uplink           
    CMceComMessageSource* messageSource = CMceComMessageSource::NewL();
    CleanupStack::PushL( messageSource );
    //messageSource->InitializeL( static_cast<CMceComMessageStream&> ( *messagestream ) );
    messageSource->InitializeL( *messagestream);
    messagestream->SetSourceL( messageSource );
    CleanupStack::Pop( messageSource );
    CMceComMsrpSink* msrpSink = CMceComMsrpSink::NewL();
    CleanupStack::PushL( msrpSink );
    msrpSink->InitializeL( *messagestream);
    messagestream->AddSinkL( msrpSink );
    CleanupStack::Pop( msrpSink ); 
    CleanupStack::Pop( messagestream );
    
    MCEMM_DEBUG( "CMceMessageSdpCodec::CreateUplinkStreamL(), Exit" )
    
    return messagestream;
    }


// -----------------------------------------------------------------------------
// CMceMessageSdpCodec::CodecsL
// -----------------------------------------------------------------------------
//
const RPointerArray<CMceComCodec>& CMceMessageSdpCodec::CodecsL( CMceComMediaStream& aStream ) const
    {
    __ASSERT_ALWAYS( aStream.iType == KMceMessage, User::Leave( KErrArgument ) );
    MCE_DEFINE_MESSAGE( stream, aStream );

    return reinterpret_cast< const RPointerArray< CMceComCodec >& >( stream.Codecs() );
    
    }


// -----------------------------------------------------------------------------
// CMceMessageSdpCodec::CreateDownlinkStreamL
// -----------------------------------------------------------------------------
//
CMceComMessageStream* CMceMessageSdpCodec::CreateDownlinkStreamL() const
    {
    MCEMM_DEBUG( "CMceMessageSdpCodec::CreateDownlinkStreamL(), Entry" )

   CMceComMessageStream *messagestream =CMceComMessageStream::NewL();
    CleanupStack::PushL( messagestream ); 
    // downlink         
    CMceComMsrpSource* msrpSource = CMceComMsrpSource::NewL();
    CleanupStack::PushL( msrpSource );
    //msrpSource->InitializeL( static_cast<CMceComMessageStream&> ( *messagestream ) );
    msrpSource->InitializeL( ( *messagestream ) );
    messagestream->SetSourceL( msrpSource );
    CleanupStack::Pop( msrpSource );
    CMceComMessageSink* sink = CMceComMessageSink::NewL();
    CleanupStack::PushL( sink );
    sink->InitializeL( ( *messagestream ) );
    messagestream->AddSinkL( sink );
    CleanupStack::Pop( sink ); 
    CleanupStack::Pop( messagestream );
    
    MCEMM_DEBUG( "CMceMessageSdpCodec::CreateDownlinkStreamL(), Exit" )
    
    return messagestream;
    }


// -----------------------------------------------------------------------------
// CMceMessageSdpCodec::SetHostAddrFromPathAttrL
// -----------------------------------------------------------------------------
//
void CMceMessageSdpCodec::SetHostAddrFromPathAttrL(const TUriC8& aMsrpUri, 
        CSdpMediaField& aMediaLine)
    {
    MCEMM_DEBUG("\n CMceMessageSdpCodec::SetHostAddrPortFromPathAttr : Entry");
    TUriParser8 parser;
    TInt parseValue = parser.Parse(aMsrpUri.UriDes()); 
    
    MCEMM_DEBUG_DVALUE("\n CMceMessageSdpCodec::SetHostAddrPortFromPathAttr: Parsed return value for MSRP URI: ", 
            parseValue);
    User::LeaveIfError(parseValue);
    TBuf8<50> host = parser.Extract(EUriHost);
    
    // check the type of the host and create the conneciton filed and insert it into the media field
    CSdpConnectionField* ConnFld = NULL;
    
    iNetType = SdpCodecStringPool::StringPoolL().StringF(SdpCodecStringConstants::ENetType,
                    SdpCodecStringPool::StringTableL());
    CleanupClosePushL(iNetType);
    
    if (UriUtils::HostType( host ) == UriUtils::ETextHost) // it is in www.example.com format
        {
        iAddressType = SdpCodecStringPool::StringPoolL().StringF(SdpCodecStringConstants::EAddressTypeIP4, 
                SdpCodecStringPool::StringTableL());
        CleanupClosePushL(iAddressType);
        ConnFld = CSdpConnectionField::NewL(iNetType, iAddressType, host);
        }
    else
        {
        if (UriUtils::HostType( host ) == UriUtils::EIPv6Host)
            {
            iAddressType = SdpCodecStringPool::StringPoolL().StringF(SdpCodecStringConstants::EAddressType, 
                    SdpCodecStringPool::StringTableL());
            CleanupClosePushL(iAddressType);
            ConnFld = CSdpConnectionField::NewL(iNetType, iAddressType, host);
            }
        else
            {
            iAddressType = SdpCodecStringPool::StringPoolL().StringF(SdpCodecStringConstants::EAddressTypeIP4, 
                            SdpCodecStringPool::StringTableL());
            CleanupClosePushL(iAddressType);
            ConnFld = CSdpConnectionField::NewL(iNetType, iAddressType, host);
            }
        }
    __ASSERT_ALWAYS(ConnFld!=NULL, User::Leave(KErrArgument));
    CleanupStack::PushL(ConnFld);
    // clear the connecitons fileds if exists
    if (aMediaLine.ConnectionFields().Count() > 0)
        {
        CSdpConnectionField* connField = aMediaLine.ConnectionFields()[0];
        __ASSERT_ALWAYS(connField!=NULL, User::Leave(KErrArgument));
        connField->SetAddressL(iNetType, iAddressType, host);
        CleanupStack::PopAndDestroy(ConnFld);    // delete the created conneciton filed as the parameters are set in above line
        }
    else
        {
        aMediaLine.ConnectionFields().AppendL(ConnFld);
        CleanupStack::Pop(ConnFld);
        }
    
    if (iAddressType.DesC().Length()!=0)
            CleanupStack::PopAndDestroy(&iAddressType);
    
    if (iNetType.DesC().Length() != 0)
        CleanupStack::PopAndDestroy(&iNetType);
    
    
    MCEMM_DEBUG("\n CMceMessageSdpCodec::SetHostAddrPortFromPathAttr : Exit");
    }
	
            
// -----------------------------------------------------------------------------
// CMceMessageSdpCodec::SetAcceptTypesAttributeL
// -----------------------------------------------------------------------------
//
void CMceMessageSdpCodec::SetAcceptTypesAttributeL(CMceComMsrpSource* msrpSource, 
        CMceComMsrpSink* msrpSink, 
        CSdpMediaField& aMediaLine)
    {
    MCEMM_DEBUG( "CMceMessageSdpCodec::SetAcceptTypesAttributeL, Entry" )
    if (iAcceptTypesStr.DesC().Length() == 0 )
        {
        iAcceptTypesStr = SdpCodecStringPool::StringPoolL().
                OpenFStringL(KMceSDPNameAcceptTypes);
        CleanupClosePushL(iAcceptTypesStr);
        }
    
    // Set accept-types attribute. 
    // If no value is provided then add * as a default value for this attribute 
    TBuf8<100> mediaTypes;
    TInt acceptTypescount = 
    msrpSource!=NULL ? msrpSource->iAcceptTypes.MdcaCount(): msrpSink->iAcceptTypes.MdcaCount();
    if (acceptTypescount == 0 )
        {
        // Default value is * . Means the applicaiton receives any type of media
        _LIT8(KStar,"*");
        mediaTypes.Append(KStar);
        }
    else
        {
        for (TInt i=0; i<acceptTypescount;i++)
            {
            mediaTypes.Append(
                    msrpSource!=NULL? (msrpSource->iAcceptTypes).MdcaPoint(i) : (msrpSink->iAcceptTypes).MdcaPoint(i));
            if (i+1 < acceptTypescount)
                {
                mediaTypes.Append(_L8(" "));           
                }
            } 
        }         
    // Create the accept-types SDP attr and append this to media line
    CSdpAttributeField* acceptTypesAttr = CSdpAttributeField::NewLC( iAcceptTypesStr, mediaTypes);
    MCEMM_DEBUG_SVALUE( "accept-types Attribute :",  mediaTypes)
    aMediaLine.AttributeFields().AppendL( acceptTypesAttr );
    CleanupStack::Pop(acceptTypesAttr);
    
    if (iAcceptTypesStr.DesC().Length() != 0)
        {
        CleanupStack::PopAndDestroy(&iAcceptTypesStr);
        }
    
    MCEMM_DEBUG( "CMceMessageSdpCodec::SetAcceptTypesAttributeL, Exit" )
    }


// -----------------------------------------------------------------------------
// CMceMessageSdpCodec::SetPathAttributeL
// -----------------------------------------------------------------------------
//     
void CMceMessageSdpCodec::SetPathAttributeL(CMceComMessageStream& aStream, 
        CMceComMsrpSource* msrpSource, 
        CMceComMsrpSink* msrpSink,
        CSdpMediaField& aMediaLine)
    {
    MCEMM_DEBUG( "CMceMessageSdpCodec::SetPathAttributeL, Entry" )
    // check if pathstr is initialized or not. if ot initialized then initialize it..
    if (iPathStr.DesC().Length() == 0 )
        {
        iPathStr = SdpCodecStringPool::StringPoolL().OpenFStringL(KMceSDPNamePath);
        CleanupClosePushL(iPathStr);
        }
    
    TInt MsrpUriCount =  msrpSource!=NULL? msrpSource->iPath.Count():msrpSink->iPath.Count(); 
         
    // Set path attribute
    HBufC8* pathValue1 = HBufC8::NewLC(200);
    TUriC8 *uri = NULL;
         
    for (TInt i=0; i<MsrpUriCount;i++)
        { 
        if (msrpSource!=NULL)
            {
            uri = msrpSource->iPath[i];
            }
        else
            {
            uri = msrpSink->iPath[i];
            }
        pathValue1->Des().Append( uri->UriDes());
                 
        if (i+1 < MsrpUriCount)
            {
            pathValue1->Des().Append(_L8(" "));
            }
        }
    if (uri != NULL)
        {
        // extract the host address only if the user configured to use the path attr for this purpose
        if (aStream.iUseMsrpPathAttrForHostAddrAndPort)
            {
            // if the realy is supported then the last entry will always indicates its own MSRP URI
            SetHostAddrFromPathAttrL(*uri, aMediaLine); 
            }
        // Create the path SDP attr and append this to media line
        CSdpAttributeField* pathAttr = CSdpAttributeField::NewLC( iPathStr, *pathValue1);                     
        MCEMM_DEBUG_SVALUE( "path Attribute value:", pathValue1->Des() )         
        aMediaLine.AttributeFields().AppendL( pathAttr );
        CleanupStack::Pop(pathAttr);
        }
    CleanupStack::PopAndDestroy(pathValue1);
    if (iPathStr.DesC().Length() != 0 )
                CleanupStack::PopAndDestroy(&iPathStr);
    MCEMM_DEBUG( "CMceMessageSdpCodec::SetPathAttributeL, Exit" )
    }


// -----------------------------------------------------------------------------
// CMceMessageSdpCodec::SetAcceptWrappedAttributeL
// -----------------------------------------------------------------------------
//
void CMceMessageSdpCodec::SetAcceptWrappedAttributeL(CMceComMsrpSource* msrpSource, 
        CMceComMsrpSink* msrpSink,
        CSdpMediaField& aMediaLine)
    {
    MCEMM_DEBUG( " CMceMessageSdpCodec::SetAcceptWrappedAttributeL, Entry" )
    if (iAcceptWrappedTypesStr.DesC().Length() == 0 )
        {
        iAcceptWrappedTypesStr = SdpCodecStringPool::StringPoolL().
                OpenFStringL(KMceSDPNameAcceptWrappedTypes);
        CleanupClosePushL(iAcceptWrappedTypesStr);
        }
    
        
    //Set Accept-Wrapped-Types attribute if its setted by client.
    TBool temp = msrpSource!=NULL? msrpSource->iWrappedTypes : msrpSink->iWrappedTypes;
    
    if (temp)
        {
        TBuf8<50> wrappedTypes;
        if (msrpSource!=NULL)
            {
            wrappedTypes.Copy(msrpSource->iAcceptWrappedTypes);
            }else
                {
                wrappedTypes.Copy(msrpSink->iAcceptWrappedTypes);
                }
        // Create the accept-wrapped-types SDP attr and append this to media line
        CSdpAttributeField* acceptTypesAttr = CSdpAttributeField::NewLC( iAcceptWrappedTypesStr, wrappedTypes);
        MCEMM_DEBUG_SVALUE( "accept-wrapped-types Attribute :",  wrappedTypes)
        aMediaLine.AttributeFields().AppendL( acceptTypesAttr );
        CleanupStack::Pop(acceptTypesAttr);
        }
    if (iAcceptWrappedTypesStr.DesC().Length() != 0 )
        {
        CleanupStack::PopAndDestroy(&iAcceptWrappedTypesStr);
        }
    
    MCEMM_DEBUG( " CMceMessageSdpCodec::SetAcceptWrappedAttributeL, Exit" )
    }


// -----------------------------------------------------------------------------
// CMceMessageSdpCodec::SetConnectionAttributeL
// -----------------------------------------------------------------------------
//
void CMceMessageSdpCodec::SetConnectionAttributeL(CMceComMessageStream& aStream,
        CSdpMediaField& aMediaLine)
    {
    MCEMM_DEBUG( " CMceMessageSdpCodec::SetConnectionAttributeL, Entry" )
    __ASSERT_ALWAYS(aStream.iType == KMceMessage, User::Leave(KErrArgument));
    if (iConnectionStr.DesC().Length() == 0 )
        {
        iConnectionStr = SdpCodecStringPool::StringPoolL().
            OpenFStringL(KMceSDPNameConnection);
        CleanupClosePushL(iConnectionStr);
            }
    TBuf8<30> connAttr;
    aStream.iMsrpConnUsage ? connAttr.Append(_L8("existing")) : connAttr.Append(_L8("new"));
    // Create the connection SDP attr and append this to media line
    CSdpAttributeField* connectionAttr = CSdpAttributeField::NewLC( iConnectionStr, connAttr);
    
    aMediaLine.AttributeFields().AppendL( connectionAttr );
    CleanupStack::Pop(connectionAttr);
    
    if (iConnectionStr.DesC().Length() != 0 )
        {
        CleanupStack::PopAndDestroy(&iConnectionStr);   
        }
    MCEMM_DEBUG( " CMceMessageSdpCodec::SetConnectionAttributeL, Exit" )
    }



// -----------------------------------------------------------------------------
// CMceMessageSdpCodec::SetFileShareAttributeL
// -----------------------------------------------------------------------------
//
void CMceMessageSdpCodec::SetFileShareAttributeL(CMceComMsrpSource* msrpSource, 
        CMceComMsrpSink* msrpSink,
        CSdpMediaField& aMediaLine)
    {
    MCEMM_DEBUG( " CMceMessageSdpCodec::SetFileShareAttributeL, Entry" )

    //Set File sharing Negotiation attributes if its called by Client.iFileShare
    if(msrpSource!=NULL ? msrpSource->iFileShare : msrpSink->iFileShare)
        {
        if ( iFileSelectorStr.DesC().Length() == 0)
            {
            iFileSelectorStr = SdpCodecStringPool::StringPoolL().OpenFStringL(KMCESDPNameFileselector);
            CleanupClosePushL(iFileSelectorStr);
            }
        // file-selector : 'name' attribute
        HBufC16* fileName = msrpSource!=NULL ? msrpSource->iFileName : msrpSink->iFileName;
        if (NULL != fileName && fileName->Des().Length())
            {
            HBufC8* fileSelector = HBufC8::NewLC(350);
            fileSelector->Des().Append(KMceSDPFileName);
            fileSelector->Des().Append('"');
            fileSelector->Des().Append(fileName->Des());
            fileSelector->Des().Append('"');
           
            TInt fileSize = 0;
            fileSize = msrpSource!=NULL ? msrpSource->iFileSize : msrpSink->iFileSize;
            if (fileSize == 0 )
                {
                // find file size
                RFs fs;
                User::LeaveIfError(fs.Connect()); 
                TEntry entry;
                //TBuf16<256> filenameDes;
                //filenameDes.Copy( fileName->Des());
                User::LeaveIfError(fs.Entry(fileName->Des(), entry));
                fileSize = entry.iSize; 
                fs.Close();
                }
            fileSelector->Des().Append(KMceSDPFileSize); 
            fileSelector->Des().AppendNum(fileSize);
            
            // default file disposition value is attachment.
            
            // file-selector : 'type' attribute
            HBufC8* fileType = NULL;
            fileType = msrpSource!=NULL ? msrpSource->iFileType : msrpSink->iFileType;
            if (fileType->Des().Length())
                {
                fileSelector->Des().Append(KMceSDPFileType);
                fileSelector->Des().Append(fileType->Des());
                }
            
            // Create the file-selector SDP attribute and append this to media line
            CSdpAttributeField* fileSelectorAttr = CSdpAttributeField::NewLC( iFileSelectorStr, *fileSelector);
            MCEMM_DEBUG_SVALUE( "file-selector-types Attribute :",  fileSelector->Des())
            aMediaLine.AttributeFields().AppendL( fileSelectorAttr );
            CleanupStack::Pop(fileSelectorAttr);
            CleanupStack::PopAndDestroy(fileSelector);
            }
            
        // Set file-disposition attribute
        TBuf8<25>  dispositionValue;
        if (msrpSource!=NULL)
            {
            dispositionValue.Copy(msrpSource->iDispositionValue);
            }
        else
            {
            dispositionValue.Copy(msrpSink->iDispositionValue);
            }
        
        if (dispositionValue != KNullDesC8 )
            {
            if ( iFileDispositionStr.DesC().Length() == 0)
                {
                iFileDispositionStr = SdpCodecStringPool::StringPoolL().OpenFStringL(KMCESDPNameFiledisposition);
                CleanupClosePushL(iFileDispositionStr);
                }
            // Create the file-disposition-types SDP attr and append this to media line
            CSdpAttributeField* filedispAttr = CSdpAttributeField::NewLC( iFileDispositionStr, dispositionValue);
            MCEMM_DEBUG_SVALUE( "file-disposition-types Attribute :",  dispositionValue)
            aMediaLine.AttributeFields().AppendL( filedispAttr );
            CleanupStack::Pop(filedispAttr);
            }
            
        // Set file-transfer-id attribute
        TBuf8<32> fileTransferid;
        fileTransferid = msrpSource!=NULL?msrpSource->iFileTransferid:msrpSink->iFileTransferid;
            
        if ( fileTransferid != KNullDesC8 )
            {
            if (iFileTransferidStr.DesC().Length() == 0 )
                {
                iFileTransferidStr = SdpCodecStringPool::StringPoolL().
                    OpenFStringL(KMCESDPNameFiletransferid);
                CleanupClosePushL(iFileTransferidStr);
                }
            // Create the file-transfer-id SDP attribute and append to media line
            CSdpAttributeField* filetransferAttr = CSdpAttributeField::NewLC( iFileTransferidStr, fileTransferid);
            MCEMM_DEBUG_SVALUE( "file-transfer-id Attribute :",  fileTransferid)
            aMediaLine.AttributeFields().AppendL( filetransferAttr );
            CleanupStack::Pop(filetransferAttr);
            }
    
        if (iFileTransferidStr.DesC().Length() != 0 )
            {
            CleanupStack::PopAndDestroy(&iFileTransferidStr);
            }
    
        if ( iFileDispositionStr.DesC().Length() != 0)
            {
            CleanupStack::PopAndDestroy(&iFileDispositionStr);
            }
    
        if ( iFileSelectorStr.DesC().Length() != 0)
            {
            CleanupStack::PopAndDestroy(&iFileSelectorStr);
            }
        }
    
    MCEMM_DEBUG( " CMceMessageSdpCodec::SetFileShareAttributeL, Exit" )
    }

// -----------------------------------------------------------------------------
// CMceMediaSdpCodec::DecodeClientAttributesL
// -----------------------------------------------------------------------------
//
void CMceMessageSdpCodec::DecodeClientAttributesL( 
    CSdpMediaField& aMediaLine, 
    CMceComMediaStream& aStream ) const
    {
    MCEMM_DEBUG("CMceMessageSdpCodec::DecodeClientAttributesL, Entry" )
    __ASSERT_ALWAYS(aStream.iType == KMceMessage, User::Leave(KErrArgument));
            
    CMceMediaSdpCodec::DecodeClientAttributesL(aMediaLine, aStream);
    
    CMceComMessageStream* msgStrm =(CMceComMessageStream*) (&aStream);
    
    CMceComMsrpSource* msrpSource = NULL;
    CMceComMsrpSink* msrpSink = NULL;
             
    //Decode client specific attributes and populate the source/sinks accordingly
    if (IS_RECEIVESTREAM(msgStrm))
        msrpSource = (CMceComMsrpSource*)(msgStrm->Source());
    else
        {
        for(TInt i=0; i<aStream.Sinks().Count(); i++)
            {
            if ( (aStream.Sinks())[i]->iType == KMceMSRPSink)
                {
                msrpSink = (CMceComMsrpSink*)( aStream.Sinks()[i]);
                break;
                }
            }
        }
        
    CBufFlat* encodeBuf = NULL;
        
    for ( TInt i = 0; i < aMediaLine.AttributeFields().Count(); i++ )
        {
        encodeBuf = CBufFlat::NewL( KMceExternalizeBufferExpandSize );
        CleanupStack::PushL( encodeBuf );
        RBufWriteStream writeStream( *encodeBuf, 0 );
        writeStream.PushL();
        
        // extract the MSRP path attribute if it exists 
        aMediaLine.AttributeFields()[ i ]->EncodeL( writeStream );
        MCEMM_DEBUG_SVALUE( "found attribute", encodeBuf->Ptr( 0 ) )
        TInt aPath = encodeBuf->Ptr(0).FindC(_L8("Path:"));
        if(aPath != KErrNotFound)
            {
            TInt length = encodeBuf->Ptr(0).Size();
            aStream.iRemoteMsrpPath = encodeBuf->Ptr(0).Mid(aPath+5, length-aPath-5-2);
            }
        
        // Extracts the connection setup accribute and set it if it exists
        TInt aSetup = encodeBuf->Ptr(0).FindC(_L8("setup:"));
        if(aSetup != KErrNotFound)
            {
            TInt length = encodeBuf->Ptr(0).Size();
            aStream.iConnStatus = encodeBuf->Ptr(0).Mid(aSetup+6, length-aSetup-6-2);
            }
        
        //extract the file sharing attributes and set them accordingly
        TInt fileSelectorId = encodeBuf->Ptr(0).FindC(_L8("file-selector:"));
        if ( fileSelectorId != KErrNotFound)
            {
            HBufC16* fileName = HBufC::NewL(256);
            TBuf8<50> fileSize;
            HBufC8* fileType = HBufC8::NewL(256);
            
            // Get file name, file size and file type.
            // look for name, type and size parameters
            TPtr8 fileSelectorData =  encodeBuf->Ptr(14);
            TInt nameIndex = encodeBuf->Ptr(0).FindC(_L8("name:"));
            if (nameIndex >= 0)
                {
                TPtr8 nameBuf = encodeBuf->Ptr(nameIndex);
                //nameBuf.Copy (encodeBuf->Ptr(nameIndex) );
                TInt spaceOffset = nameBuf.Locate(' ') ;
                if (spaceOffset != KErrNotFound )
                    {
                    fileName->Des().Copy(nameBuf.Mid(6, (spaceOffset-6-1 ))/*, (spaceOffset-6-1 )*/ );
                    }else
                        {
                        TInt crlfOffset = nameBuf.FindC(_L8("\r\n") ) ;
                        if (crlfOffset != KErrNotFound )
                            {
                            fileName->Des().Copy(nameBuf.Mid(6, (crlfOffset-6-1 ))/*, (crlfOffset-6-1 )*/);
                            }
                        }
                }
            // Find the file size if exists in the file selector
            TInt fileSizeIndex =  encodeBuf->Ptr(0).FindC(_L8("size:"));
            if (fileSizeIndex >= 0 && fileSizeIndex < encodeBuf->Size())
                {
                TPtr8 sizeBuf = encodeBuf->Ptr(fileSizeIndex);
                TInt spaceOffset = sizeBuf.Locate(' ') ;
                if (spaceOffset != KErrNotFound)
                    {
                    fileSize.Copy( sizeBuf.Mid(5, spaceOffset-5));
                    }else
                        {
                        TInt crlfOffset = sizeBuf.FindC(_L8("\r\n") ) ;
                        if (crlfOffset != KErrNotFound )
                            {
                            fileSize.Copy(sizeBuf.Mid(5, (crlfOffset-5 )));
                            }
                        }
                    }
            
            // Find the file type if exists in file selector
            TInt fileTypeIndex =  encodeBuf->Ptr(0).FindC(_L8("type:"));
            if (fileTypeIndex >=0 && fileTypeIndex < encodeBuf->Size())
                {
                TPtr8 typeBuf = encodeBuf->Ptr(fileTypeIndex);
                TInt spaceOffset = typeBuf.Locate(' ') ;
                if (spaceOffset != KErrNotFound)
                    {
                    fileType->Des().Copy(typeBuf.Mid(5, spaceOffset-5));
                    }else
                        {
                        TInt crlfOffset = typeBuf.FindC(_L8("\r\n") ) ;
                        if (crlfOffset != KErrNotFound )
                            {
                            fileType->Des().Copy( typeBuf.Mid(5, (crlfOffset-5 )) );
                            }
                        }
                }
                if (NULL != msrpSource )
                    {
                    msrpSource->iFileName = fileName;
                    msrpSource->iFileType = fileType;
                    TLex8 lex(fileSize);
                    lex.Val(msrpSource->iFileSize);
                    msrpSource->iFileShare = ETrue;
                    }
                else
                    {
                    msrpSink->iFileName = fileName;
                    msrpSink->iFileType =  fileType;
                    TLex8 lex(fileSize);
                    lex.Val(msrpSink->iFileSize);
                    msrpSink->iFileShare = ETrue;
                    }
                }
        
        // look for the file-transfer-id
        TInt fileTransferID= encodeBuf->Ptr(0).FindC(_L8("file-transfer-id:"));
        if ( fileTransferID != KErrNotFound)
            {
            TBuf8<32> fileTransferid;
            
            TInt length = encodeBuf->Ptr(0).Size();
            fileTransferid = encodeBuf->Ptr(0).Mid(fileTransferID+17, length-fileTransferID-17-2);
            // the code is pending
            if (NULL != msrpSource)
                msrpSource->iFileTransferid = fileTransferid;
            else
                msrpSink->iFileTransferid = fileTransferid;
            }
        
        CleanupStack::PopAndDestroy(); // writeStream
        CleanupStack::PopAndDestroy( encodeBuf ); // encodeBuf
        }
    MCEMM_DEBUG("CMceMessageSdpCodec::DecodeClientAttributesL, Exit" )
    }
	
	
	// -----------------------------------------------------------------------------
// CMceMessageSdpCodec::CreateFormatListL
// -----------------------------------------------------------------------------
//
HBufC8* CMceMessageSdpCodec::CreateFormatListL( CMceComCodec::TIterator& /*aCodecs*/ ) const
    {
    MCEMM_DEBUG("CMceMessageSdpCodec::CreateFmtListL(), Entry ")
        
    HBufC8* fmtList = HBufC8::NewL( KMceSdpFormatListLength );
    TPtr8 fmtListPtr( fmtList->Des() );
    
    // codecs can be NULL if the data path is being taken care by the applicaiton
    // irrespective of codecs the format list for message stream should be *
    _LIT8(KSdpFmtListValueStar,"*");
    fmtListPtr.Append(KSdpFmtListValueStar); 
    
    MCEMM_DEBUG("CMceMessageSdpCodec::CreateFmtListL(), Exit ")
        
    return fmtList;
    }
	
	// -----------------------------------------------------------------------------
// CMceMessageSdpCodec::DecodeStaticPayloadsL
// -----------------------------------------------------------------------------
//
TInt CMceMessageSdpCodec::DecodeStaticPayloadsL( CSdpMediaField& aMediaLine,
                CMceComMediaStream& aStream,
                RArray<TUint>& aPayloadTypes,
                TMceNegotiationRole aRole ) const
    {
    MCEMM_DEBUG("CMceMediaSdpCodec::DecodeStaticPayloadsL(), Entry ")
    TInt decoded = 0;

    DecodeFormatListL( aMediaLine, aPayloadTypes );
    TInt index = 0;
    
    if ( IS_SENDSTREAM(&aStream))
                {
                if ( (aStream.Source()->Type() == KMceExternalSource) ||
                        (aStream.Source()->Type() == KMceMessageSource)  )
                    {
                    decoded++;
                    return decoded;
                    }
                }else
                    {
                    for (TInt i=0; i<aStream.Sinks().Count();i++)
                        {
                        if ( (aStream.Sinks()[i]->Type() == KMceExternalSink) ||
                                (aStream.Sinks()[i]->Type() == KMceMSRPSink) )
                            {
                            decoded++;
                            return decoded;
                            }
                        }
                    }
    
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
// CMceMediaSdpCodec::DecodeFormatListL
// -----------------------------------------------------------------------------
//
void CMceMessageSdpCodec::DecodeFormatListL( 
    CSdpMediaField& aMedia, 
    RArray<TUint>& aPayloadTypes ) const
    {
    MCEMM_DEBUG("CMceMessageSdpCodec::DecodeFormatListL(), Entry ")
    
    User::LeaveIfError( DecodeFormatList( aMedia, aPayloadTypes ) );
        
    MCEMM_DEBUG("CMceMessageSdpCodec::DecodeFormatListL(), Exit ")   
    }


// -----------------------------------------------------------------------------
// CMceMessageSdpCodec::DecodeFormatList
// -----------------------------------------------------------------------------
//
TInt CMceMessageSdpCodec::DecodeFormatList( CSdpMediaField& aMedia, 
                               RArray<TUint>& aPayloadTypes ) const
    {
    MCEMM_DEBUG("CMceMessageSdpCodec::DecodeFormatList(), Entry ")
    
    const TDesC8& payloadlist = aMedia.FormatList();
    MCEMM_DEBUG_SVALUE( "decoded format list", payloadlist )
    TLex8 payloads( payloadlist );
    TInt error = KErrNone;
    //  Validate the formalist list if it is *
    if (payloadlist.Compare(_L8("*")) == 0 )
        {
        TUint msrpPayloadtype = 90; // dummy payload type + using unassigned value
        error = aPayloadTypes.Append( msrpPayloadtype );
        MCEMM_DEBUG("CMceMessageSdpCodec::DecodeFormatList(), Exit ") 
        return error;   
        } 
    else
        {
        return KErrNotSupported;
        }
    }
	
