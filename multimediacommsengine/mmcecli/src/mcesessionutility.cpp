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



#include <badesca.h>
#include <mcesession.h>
#include <uri8.h>
#include <sdpcodecstringpool.h>
#include <SdpAttributeField.h>
#include <sdpdocument.h>
#include "mceclilogs.h"
#include "mcecomsession.h"
#include "mcecommediastream.h"
#include "mcesessionutility.h"
#include <SdpCodecStringPool.h>
#include <stringpool.h>
#include "mcecommediastream.h"
#include <mcertpsource.h>
#include <mcemsrpsource.h>
#include <sdpconnectionfield.h>
#include <sdpbandwidthfield.h>
#include <mcertpsource.h>

//# define _OPEN_SDPCODEC_STRINGPOOL  SdpCodecStringPool::OpenL()

#define _CLOSE_SDPCODEC_STRINGPOOL   SdpCodecStringPool::Close();


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// TMceSessionUtility::getSessionSdPLineL
// -----------------------------------------------------------------------------
//
EXPORT_C HBufC8* TMceSessionUtility::getSessionSdPLineL(CMceSession* aSession, 
        TSDPLineName aSdpLineName)
    {
    __ASSERT_ALWAYS(aSession, User::Leave(KErrArgument));    
    
    // Ownership is tranferred to us for sessionLines
    CDesC8Array* sessionLines = static_cast<CDesC8Array*> (aSession->SessionSDPLinesL());
    
    if (sessionLines->MdcaCount() == 0)
        {
        MCECLI_DEBUG("TMceSessionUtility::getSessionSdPLineL: No session attributes set in the passed session \n");
        sessionLines->Reset();
        delete sessionLines;
        return NULL;
        }
    
    // Open SIP coec string pool
    TRAPD(error, SdpCodecStringPool::OpenL());
    if (error != KErrAlreadyExists && error != KErrNone)
        {
        delete sessionLines;
        User::Leave(error);
        }
    CSdpDocument* sdpDoc = NULL;
    CleanupStack::PushL(sessionLines);
    ConstructSdpDocumentL(sessionLines, sdpDoc); 
    CleanupStack::Pop(sessionLines);
    
    HBufC8* retValue = NULL;
    switch(aSdpLineName)
        {
        case EVersion:
            {
            TInt value = sdpDoc->SdpVersion();
            TBuf8<10> tmp;
            tmp.Num(value);
            retValue = tmp.AllocL();
            }
            break;
         
        case ESession:
            retValue = sdpDoc->SessionName().AllocL();
            break;
            
        case EInfo:
            retValue = sdpDoc->Info().AllocL();
            break;
            
        case EUri:
            {
            // Ownership will not be transferred to us so shouldn't delete the pointer
            CUri8* uri = sdpDoc->Uri();
            if (uri != NULL)
                {
                retValue = uri->Uri().UriDes().AllocL();
                }
            }
            break;
            
        case EZone:
            retValue = sdpDoc->ZoneAdjustments().AllocL();
            break;
            
        default:
            sessionLines->Reset();
            delete sessionLines;
            delete sdpDoc;
            _CLOSE_SDPCODEC_STRINGPOOL;
            User::Leave(KErrNotSupported);
        }
    sessionLines->Reset();
    delete sessionLines;
    delete sdpDoc;
    
    if (error == KErrNone)
        _CLOSE_SDPCODEC_STRINGPOOL;
    __ASSERT_ALWAYS(retValue!=NULL, User::Leave(KErrArgument));
    return retValue;
    }
        


// -----------------------------------------------------------------------------
// TMceSessionUtility::getRemoteMediaSDPAttrL
// -----------------------------------------------------------------------------
//
EXPORT_C HBufC8* TMceSessionUtility::getRemoteMediaSDPAttrL(CMceMediaStream* aStream,
                const TDesC8& aAttrName)
    {
    __ASSERT_ALWAYS(aStream, User::Leave(KErrArgument));
    // Ownership is not transferred to us
    CMceSession* session = aStream->Session();
    __ASSERT_ALWAYS(session, User::Leave(KErrArgument));
    
	if ( (session->State() >= CMceSession::ECancelling) || 
		(session->State() <= CMceSession::EIdle) )
        {
        MCECLI_DEBUG(" Session state shouldn't be idle Or shouldn't be either in cancelling, terminating , terminated  \n");
        User::Leave(KErrArgument);
        }
    
    CDesC8Array* mediaAttrs = NULL; 
    mediaAttrs = aStream->FlatData()->iRemoteMediaSDPLines;
    
    if (mediaAttrs == NULL || mediaAttrs->MdcaCount() == 0)
        {
        if (aStream->BoundStream())
			{
			mediaAttrs = aStream->BoundStreamL().FlatData()->iRemoteMediaSDPLines;		
			}
		}
		
	if (mediaAttrs == NULL || mediaAttrs->MdcaCount() == 0)
		{
		// Look for the another staream of same type to extract the remote media sdp attributes
        CMceMediaStream* tempStrm;
        for(TInt i=0; i<session->Streams().Count(); i++)
            {
            tempStrm = session->Streams()[i];
            if (tempStrm != aStream && (tempStrm->Type() == aStream->Type()) )
                {
                mediaAttrs = tempStrm->FlatData()->iRemoteMediaSDPLines;
                }
            }
        if (mediaAttrs == NULL || mediaAttrs->MdcaCount() == 0)
            {
            MCECLI_DEBUG("TMceSessionUtility::getRemoteMediaSDPAttrL: No Remote Media SDP attributes set to the streams \n");
            return NULL;
            }
        }
    //_OPEN_SDPCODEC_STRINGPOOL;
    TRAPD(error, SdpCodecStringPool::OpenL());
    if (error != KErrAlreadyExists && error != KErrNone)
        {
        User::Leave(error);
        }
    // If the attribute is found then the value will be in aValue.
    HBufC8* value = NULL;
    value = FindAttributeL(mediaAttrs, aAttrName);
    if (error == KErrNone)
        _CLOSE_SDPCODEC_STRINGPOOL;
    return value;
    }
        
 

// -----------------------------------------------------------------------------
// TMceSessionUtility::GetLocalConnectionAddrL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMceSessionUtility::GetLocalConnectionAddrL(CMceSession* aSess, TInetAddr& aAddr)
    {
    __ASSERT_ALWAYS(aSess, User::Leave(KErrArgument));
    aAddr = aSess->FlatData()->iLocalIpAddress;
    }

 

// -----------------------------------------------------------------------------
// TMceSessionUtility::GetRemoteConnectionAddrL
// -----------------------------------------------------------------------------
//
EXPORT_C HBufC8* TMceSessionUtility::GetRemoteConnectionAddrL(CMceMediaStream* aStream/*, TDesC8* hostAddr*/)
    {
    CMceSession* session = aStream->Session();
    __ASSERT_ALWAYS(session, User::Leave(KErrArgument));
    if ( (session->State() >= CMceSession::ECancelling) || 
            (session->State() <= CMceSession::EIdle) )
        {
        MCECLI_DEBUG("TMceSessionUtility::GetRemoteConnectionAddrL: Session state shouldn't be idle Or shouldn't be either in cancelling, terminating , terminated");
        User::Leave(KErrArgument);
        }
    TBuf8<10> attrName;
    
    if (aStream->Type() == KMceMessage)
        {
        // Get the remote end's path attribute and extracts the host number from that
        //ownership willbe transferred to us so have to free up later
        HBufC8* pathValue = NULL;
        attrName.Zero();
        attrName.Copy(_L8("path"));
        pathValue = TMceSessionUtility::getRemoteMediaSDPAttrL(aStream, attrName);
        if (NULL!=pathValue)
            {
            TUriParser8 parser;
            TInt parseValue = parser.Parse(*pathValue); 
                    
            MCECLI_DEBUG_DVALUE("\n CMceMessageSdpCodec::SetHostAddrPortFromPathAttr: Parsed return value for MSRP URI: ", 
                    parseValue);
                
            User::LeaveIfError(parseValue);
            //TBuf8<50> host = parser.Extract(EUriHost);
            HBufC8* hostAddr = parser.Extract(EUriHost).AllocL();
            delete pathValue;
            return hostAddr;
            }
        }
    
    // check if c line exists for the m-line correspands to this stream
    attrName.Zero();
    attrName.Copy(_L8("c"));
    HBufC8* hostAddr = NULL;
    hostAddr = TMceSessionUtility::getRemoteMediaSDPAttrL(aStream, attrName);
    if (hostAddr!=NULL)
        return hostAddr;
    else
        {
        TBuf<32> addr;
        session->FlatData()->iRemoteIpAddress.Output(addr);
        TBuf8<32> value;
        value.Copy(addr);
        return value.AllocL();
        }
    }
    
// -----------------------------------------------------------------------------
// TMceSessionUtility::GetLocalMediaPortL   
// -----------------------------------------------------------------------------
//
EXPORT_C void TMceSessionUtility::GetLocalMediaPortL(CMceMediaStream* aStream, TUint& aMediaPort )
    {
    MCECLI_DEBUG("TMceSessionUtility::GetLocalMediaPortL: Entry ");
    __ASSERT_ALWAYS(aStream!=NULL, User::Leave(KErrArgument));

	if ( (aStream->Session()->State() >= CMceSession::ECancelling) || 
			(aStream->Session()->State() <= CMceSession::EIdle) )
        {
        MCECLI_DEBUG("TMceSessionUtility::GetRemoteConnectionAddrL :  Session state shouldn't be idle Or shouldn't be either in cancelling, terminating , terminated");
        User::Leave(KErrArgument);
        }
    
	aMediaPort = aStream->FlatData()->LocalMediaPort();
	MCECLI_DEBUG_DVALUE("TMceSessionUtility::GetLocalMediaPortL : ", aMediaPort)
	MCECLI_DEBUG("TMceSessionUtility::GetLocalMediaPortL: Exit ");
    }
         


// -----------------------------------------------------------------------------
// TMceSessionUtility::GetRemoteMediaPortL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMceSessionUtility::GetRemoteMediaPortL(CMceMediaStream* aStream, 
        TUint& aMediaPort )
    {
    MCECLI_DEBUG("TMceSessionUtility::GetRemoteMediaPortL: Entry ");
    __ASSERT_ALWAYS(aStream!=NULL, User::Leave(KErrArgument));
    CMceSession* session = aStream->Session();
    __ASSERT_ALWAYS(session, User::Leave(KErrArgument));
    if ( (session->State() >= CMceSession::ECancelling) || 
            (session->State() <= CMceSession::EIdle) )
        {
        MCECLI_DEBUG("TMceSessionUtility::GetRemoteConnectionAddrL :  Session state shouldn't be idle Or shouldn't be either in cancelling, terminating , terminated");
        User::Leave(KErrArgument);
        }
    if (aStream->Type() == KMceMessage)
        {
        TBuf8<10> aAttrName(_L8("path"));
    
        // Get the remote end's path attribute and extracts the port number from that
        //ownership willbe transferred to us so have to free up later
        HBufC8* pathValue = NULL;
        pathValue = TMceSessionUtility::getRemoteMediaSDPAttrL(aStream, aAttrName);
    
        if (pathValue == NULL)
            {
            aMediaPort = aStream->FlatData()->RemoteMediaPort();
            return;
            }
        // 
        TUriParser8 parser;
        TInt parseValue = parser.Parse(pathValue->Des()); 
        MCECLI_DEBUG_DVALUE("\n CMceMsrpSource::SetHostAddrPortFromPathAttr: Parsed return value for MSRP URI: ",
                parseValue);
            
        TBuf8<10> portBuf = parser.Extract(EUriPort);
            
        TBuf16<10> portBuf16;
        portBuf16.Copy(portBuf);
        TLex iLex(portBuf16);
        iLex.Val(aMediaPort);  // returns the port value
        delete pathValue;
        return;
        }
    else
        {
        // Get send streams  and use remotemediaport
        if ( aStream->Source()->Type() == KMceRTPSource)
            {
            // find the send stream and get the remote media port
            if (aStream->BoundStream() && 
                    aStream->BoundStreamL().Source() != NULL &&
                    aStream->BoundStreamL().Source()->Type() != KMceRTPSource)
                {
                aMediaPort = aStream->BoundStreamL().FlatData()->RemoteMediaPort();
                return;
                }
            // if the streams are not binded then find the send stream aong the session streams
            for (TInt i=0; i<session->Streams().Count(); i++)
                {
                if (session->Streams()[i]!=NULL && (session->Streams()[i]!=aStream) &&
                        session->Streams()[i]->Type() == aStream->Type())
                    {
                    if (session->Streams()[i]->Source()->Type()!=KMceRTPSource)
                        {
                        aMediaPort = session->Streams()[i]->FlatData()->RemoteMediaPort();
                        return;
                        }
                    } // inside if
                } // For loop
            } // 
        }
    aMediaPort = aStream->FlatData()->RemoteMediaPort();
    MCECLI_DEBUG_DVALUE("TMceSessionUtility::GetRemoteMediaPortL : ", aMediaPort)
    MCECLI_DEBUG("TMceSessionUtility::GetRemoteMediaPortL: Exit ");
    }



// -----------------------------------------------------------------------------
// TMceSessionUtility::FindAttributeL
// -----------------------------------------------------------------------------
//
HBufC8* TMceSessionUtility::FindAttributeL(CDesC8Array* asdpLines , const TDesC8& aAttrName)
    {
    // Put all those session level names in single DesC8 object and pass that to CSdpDocument
    TBool attrFound = EFalse; 
    CSdpAttributeField* attr=NULL;
    RStringF searchAttr = SdpCodecStringPool::StringPoolL().OpenFStringL(aAttrName);
    HBufC8* retValue = NULL;
    for (TInt i=0; i< asdpLines->MdcaCount()&& !attrFound; i++)
        {
        TBuf8<200> attrText(asdpLines->MdcaPoint(i));
        
        // media attrivutes can also contain 'b' &'c' lines so skip them 
        _LIT8( KMceSdpConnectionLineMarker, "c=" );
        _LIT8( KMceSdpBandwidthLineMarker, "b=" );
        
        // Look for media specific conneciton(c= ) line
        if ( (attrText.Find(KMceSdpConnectionLineMarker) >= 0) )
            {
            if (attrText.Find(aAttrName) >= 0) 
                {
                // get the conneciton filed value 
                CSdpConnectionField* conn = NULL;
                TRAPD(err, conn = CSdpConnectionField::DecodeL(attrText));
                if (err != KErrNone)
                    {
                    searchAttr.Close();
                    _CLOSE_SDPCODEC_STRINGPOOL;
                    User::Leave(err);
                    }
                __ASSERT_ALWAYS(conn!=NULL, User::Leave(KErrArgument));
                retValue = conn->Address().Alloc();
                attrFound = ETrue;
                delete conn; // Free the memory for conn pointer
                }
            }
        // Look for media specific bandwidth(b= ) line
        else if ( (attrText.Find(KMceSdpBandwidthLineMarker) >= 0) )
            {
            if ((attrText.Find(aAttrName) >= 0))
                {
                // get the bandwidth filed and return the value
                CSdpBandwidthField* bandWidth = NULL;
                TRAPD(err, bandWidth = CSdpBandwidthField::DecodeL(attrText));
                if (err != KErrNone)
                    {
                    searchAttr.Close();
                    _CLOSE_SDPCODEC_STRINGPOOL;
                    User::Leave(err);
                    }
                __ASSERT_ALWAYS(bandWidth!=NULL, User::Leave(KErrArgument));
                TBuf8<20> value;
                value.Num(bandWidth->Value());
                retValue = value.Alloc();
                attrFound = ETrue;
                delete bandWidth;
                }
            }
        else
            // Look for media specific attribute(a= ) lines
            {
            // Have to free up later point of time
            TRAPD(err, attr = CSdpAttributeField::DecodeL(attrText));
            if (err != KErrNone)
                {
                searchAttr.Close();
                _CLOSE_SDPCODEC_STRINGPOOL;
                User::Leave(err);
                }
            if (attr == NULL)
                {
                MCECLI_DEBUG("TMceSessionUtility::FindAttributeL: Error in decoding the media attribute : ");
                searchAttr.Close();
                _CLOSE_SDPCODEC_STRINGPOOL;
                User::Leave(KErrArgument);
                }
            if (attr->Attribute() ==  searchAttr && !attrFound)
                {
                retValue = attr->Value().Alloc();
                attrFound = ETrue;
                }
            delete attr;
            }
        }
    if (attrFound == EFalse)
        {
        retValue = NULL;
        }
    searchAttr.Close();
    return retValue;
    }


// -----------------------------------------------------------------------------
// TMceSessionUtility::ConstructSdpDocumentL
// -----------------------------------------------------------------------------
//
void TMceSessionUtility::ConstructSdpDocumentL(CDesC8Array* asdpLines, CSdpDocument*& asdpDoc)
    {
    if (asdpDoc)
        {
        delete asdpDoc;
        asdpDoc = NULL;
        }
    TBuf8<1000> text; 
    for(TInt i=0; i<asdpLines->MdcaCount(); i++)
        {
        TBuf8<200> temp(asdpLines->MdcaPoint(i));
        text.Append( temp );
        }
    TInt leaveValue = KErrNone;
    // Ownership for sdpDoc is tranferred to us.. 
    TRAP(leaveValue, asdpDoc = CSdpDocument::DecodeL(text));
    
    if (leaveValue != KErrNone)
        {
        MCECLI_DEBUG_DVALUE("TMceSessionUtility::ConstructSdpDocument: Error in decoding the SdpDocument text : %d", leaveValue);
        SdpCodecStringPool::Close();
        User::Leave(leaveValue);
        }    
    }
