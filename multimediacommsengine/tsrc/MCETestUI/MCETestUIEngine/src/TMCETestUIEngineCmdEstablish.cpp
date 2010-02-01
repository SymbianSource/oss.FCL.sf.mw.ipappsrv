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
* Description:    Implementation.
*
*/




// INCLUDES

#include "TMCETestUIEngineCmdEstablish.h"
#include "CMCETestUIQuestionSingleSelection.h"
#include "CMCETestUIQuestionDataQuery.h"
#include "CMCETestUIEngineOutSession.h"
#include <MCEOutSession.h>
#include "CMCETestUIEngineCodec.h"
#include "CMCETestUIEngineAudioStream.h"
#include "CMCETestUIEngineVideoCodec.h"
#include "CMCETestUIEngineVideoStream.h"
#include <MCESession.h>
#include <MceAudioStream.h>
#include <MCEAudioCodec.h>
#include <MceVideoStream.h>
#include <MCEVideoCodec.h>



// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdEstablish::TMCETestUIEngineCmdEstablish
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdEstablish::TMCETestUIEngineCmdEstablish(
            CMCETestUIEngine& aEngine,
		    CMCETestUIEngineOutSession& aOutSession )
    : TMCETestUIEngineCmdBase( aEngine ),
    iOutSession( aOutSession )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdEstablish::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdEstablish::ExecuteL()
	{
	if(iEngine.iMultiCodecSupport == EFalse)
		{
		DeleteCodecs();	
		}
	if(iOutSession.iSessionType == EPoCSession)
		{
		TInt num = 36000;
		HBufC8* content_type =  NULL; 
		CDesC8ArrayFlat* headers =  NULL;
		HBufC8* content =  NULL; 
		headers = new (ELeave) CDesC8ArrayFlat(1);
		CleanupStack::PushL( headers );
		headers->AppendL(_L8("User-Agent: PoC-client/OMA1.0 Nokia-S60/v1.0"));
		headers->AppendL(_L8("Accept-Contact: *;+mckoppa"));
	//	headers->AppendL(_L8("Accept-Contact: *;+g.poc.talkburst;require;explicit"));
		headers->AppendL(_L8("Privacy: id;user;header"));
		headers->AppendL(_L8("P-Preferred-Identity: \"Joku\" <sip:joku@jossain.com>"));
		if(iOutSession.iSessionParams.iPoCAdhocSessionType)
			{
			content_type = _L8("application/resource-lists+xml").AllocLC(); 
			_LIT8(KNewLine, "\r\n");
			_LIT8(KAhListXmlVersionLine,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
		    _LIT8(KAhListXmlResourceLists,"<resource-lists xmlns=\"urn:ietf:params:xml:ns:resource-lists\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">");	
		    _LIT8(KAhListXmlListTag,"<list>");
		    _LIT8(KAhListXmlListEndTag,"</list>");
		    _LIT8(KAhListXmlResourceListsEndTag ,"</resource-lists>");
		    _LIT8(KAhListXmlEntryUriStartTag ,"<entry uri=\"");
		    _LIT8(KAhListXmlEntryUriEndTag ,"\" />");
		    _LIT8(KAhUri,"sip:joku@jossain.com");
	/*		content = _L8("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n\
			<resource-lists xmlns=\"urn:ietf:params:xml:ns:resource-lists\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">\r\n\
			<list>\r\n\
			<entry uri=\"sip:g_heikki2@poc.wirelessfuture.com\" />\r\n\
			\r\n\
			</resource-lists>\r\n").AllocL();
			
	*/		
	
			

    		content = HBufC8::NewLC( KAhListXmlVersionLine().Length() 
    							 + KAhListXmlResourceLists().Length()
    							 + KAhListXmlListTag().Length()
    							 + KNewLine().Length() 
    							 + KNewLine().Length() 
    							 + KNewLine().Length() 
    							 + KAhListXmlEntryUriStartTag().Length()
    						 	 + KAhUri().Length()
    						     + KAhListXmlEntryUriEndTag().Length()
    						     + KNewLine().Length() 
    							 + KAhListXmlListEndTag().Length() 
								 + KAhListXmlResourceListsEndTag().Length() 
								 + KNewLine().Length()
								 + KNewLine().Length() );
								 
			content->Des().Append( KAhListXmlVersionLine );				// <?xml version="1.0" encoding="UTF-8"?>
		    content->Des().Append( KNewLine );
		    content->Des().Append( KAhListXmlResourceLists );			// <resource-lists xmlns="urn:ietf:params:xml....
		    content->Des().Append( KNewLine );
		    content->Des().Append( KAhListXmlListTag );					// <list>
		    content->Des().Append( KNewLine );
		    
		    content->Des().Append( KAhListXmlEntryUriStartTag );	// <entry uri="
    		content->Des().Append( KAhUri );					// sip:joku@jossain.com
    		content->Des().Append( KAhListXmlEntryUriEndTag );		// " />
    		content->Des().Append( KNewLine );
    		
    		content->Des().Append( KAhListXmlListEndTag );				// </list>
		    content->Des().Append( KNewLine );
		    content->Des().Append( KAhListXmlResourceListsEndTag );		// </resource-lists>
		    content->Des().Append( KNewLine );							// /r/n



			CleanupStack::Pop( content );
			CleanupStack::Pop( content_type );
			}
		
		CleanupStack::Pop( headers );
		iOutSession.OutSession().EstablishL(num,headers,content_type,content);
		}

	else
		{
		TInt num = 36000;
		HBufC8* content_type =  NULL; 
		CDesC8ArrayFlat* headers =  NULL;
		HBufC8* content =  NULL; 
		headers = new (ELeave) CDesC8ArrayFlat(1);
		CleanupStack::PushL( headers );
		if(iEngine.iKoppaFlag)
			{
			headers->AppendL(_L8("Accept-Contact: *;+mckoppa"));
			}
		CleanupStack::Pop( headers );
		iOutSession.OutSession().EstablishL(num,headers,content_type,content);
		}
	iEngine.EngineStateChangedL();	    
	}
	
// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdEstablish::DeleteCodecs
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdEstablish:: DeleteCodecs()
	{
	const RPointerArray<CMCETestUIEngineAudioStream>& audioStreams = 
            iOutSession.AudioStreamsL();
            
         for ( TInt i = 0; i < audioStreams.Count(); ++i )
        	{
        	 RPointerArray<CMCETestUIEngineCodec>& codecs = 
            	audioStreams[i]->CodecsL();
            	TInt count = codecs.Count();
            //delete all codecs except first one	
         	for ( TInt j = count-1; j > 0; --j )
				{
				 audioStreams[i]->AudioStream().RemoveCodecL(codecs[j]->Codec());
		         delete codecs[j];
		         codecs.Remove( j );
		         codecs.Compress();
		         iEngine.EngineStateChangedL();
		        }
        	}
    const RPointerArray<CMCETestUIEngineVideoStream>& videoStreams = 
            iOutSession.VideoStreamsL();
            
         for ( TInt i = 0; i < videoStreams.Count(); ++i )
        	{
        	 RPointerArray<CMCETestUIEngineVideoCodec>& codecs = 
            	videoStreams[i]->CodecsL();
            	TInt count = codecs.Count();
            //delete all codecs except first one	
         	for ( TInt j = count-1; j > 0; --j )
				{
				 videoStreams[i]->VideoStream().RemoveCodecL(codecs[j]->Codec());
		         delete codecs[j];
		         codecs.Remove( j );
		         codecs.Compress();
		         iEngine.EngineStateChangedL();
		        }
        	}
	}
// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdEstablish::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdEstablish::Caption() const
	{
	return KCommandCaptionEstablish;
	}


// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdEstablishWithValues::TMCETestUIEngineCmdEstablishWithValues
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdEstablishWithValues::TMCETestUIEngineCmdEstablishWithValues(
            CMCETestUIEngine& aEngine,
		    CMCETestUIEngineOutSession& aOutSession )
    : TMCETestUIEngineCmdBase( aEngine ),
    iOutSession( aOutSession )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdEstablishWithValues::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdEstablishWithValues::ExecuteL()
	{
	if(iEngine.iMultiCodecSupport == EFalse)
		{
		DeleteCodecs();	
		} 
    TSessionEstablishValues sessionValues;
    if(iEngine.SessionEstablishValues( sessionValues))
    	{
    	// change the values to the format needed by api 
    	CDesC8Array* headers;
    	headers = new (ELeave) CDesC8ArrayFlat( 1 );
    	CleanupStack::PushL( headers );
    //	if(sessionValues.headers.Compare(KNone))
    //		{
    		CMCETestUIQuestionDataQuery* question = 
	    			CMCETestUIQuestionDataQuery::NewLC();
    		question->SetCaptionL( KUserQuestionInputRecipient );
    		question->SetDefaultValueL(sessionValues.headers  );
			question->SetAnswerL( sessionValues.headers );
			TBufC8<100> headersbuffer( question->Answer8() );
			CleanupStack::PopAndDestroy( question );
			headers->AppendL( headersbuffer );
    //		}
    //	else
    //		{
    	//	headers = new (ELeave) CDesC8ArrayFlat( 1 );
		//	CleanupStack::PushL( headers );
			//headers->AppendL( KAcceptContactMCETestUI );	
    //		}
    /*	HBufC8* contentType;
    	HBufC8* content;
    	CDesC8Array* contentHeaders;
    	if(sessionValues.headers.Compare(KNone))
    		{
    		CMCETestUIQuestionDataQuery* question = 
	    			CMCETestUIQuestionDataQuery::NewLC();
    		question->SetCaptionL( KUserQuestionInputRecipient );
			question->SetDefaultValueL(sessionValues.headers  );
			question->SetAnswerL( sessionValues.headers );
			TBufC8<100> headersbuffer( question->Answer8() );
			CleanupStack::PopAndDestroy( question );
			headers = new (ELeave) CDesC8ArrayFlat( 1 );
			CleanupStack::PushL( headers );
			headers->AppendL( headersbuffer );
    		}
    	else
    		{
    		headers = new (ELeave) CDesC8ArrayFlat( 1 );
			CleanupStack::PushL( headers );
			headers->AppendL( KAcceptContactMCETestUI );	
    		}
    	if(sessionValues.contentType.Compare(KNone) )
    		{
    		contentType = HBufC8::NewLC(sessionValues.contentType.Length());
    		TPtr8 ptr8 = contentType->Des();
            ptr8.Copy(sessionValues.contentType);
    		}
    	else 
    		{
    		contentType = KNullDesC8().AllocLC();
    		}	
        if(sessionValues.content.Compare(KNone))
    		{
    		content = HBufC8::NewLC(sessionValues.content.Length());
    		TPtr8 ptr8 = content->Des();
            ptr8.Copy(sessionValues.content); 
    		}
    	else 
    		{
    		content = KNullDesC8().AllocLC();
    		}		
    	
    	if(sessionValues.contentHeaders.Compare(KNone))
    		{
    		CMCETestUIQuestionDataQuery* question = 
	    			CMCETestUIQuestionDataQuery::NewLC();
    		question->SetCaptionL( KUserQuestionInputRecipient );
			question->SetDefaultValueL(sessionValues.contentHeaders  );
			question->SetAnswerL( sessionValues.contentHeaders );
			TBufC8<100> headersbuffer( question->Answer8() );
			CleanupStack::PopAndDestroy( question );
			contentHeaders = new (ELeave) CDesC8ArrayFlat( 1 );
			CleanupStack::PushL( contentHeaders );
			contentHeaders->AppendL( headersbuffer );
    		}
    	else
    		{
    		contentHeaders = new (ELeave) CDesC8ArrayFlat( 1 );
			CleanupStack::PushL( contentHeaders );
			contentHeaders->AppendL( KAcceptContactMCETestUI );	
    		}	
    		
    	iOutSession.OutSession().EstablishL(sessionValues.timeOut, headers, contentType, content, contentHeaders );
		CleanupStack::Pop( contentHeaders );
		CleanupStack::Pop( content );
		CleanupStack::Pop( contentType );
		
		*/
		
		

	// ---------------------------------------
	// Next is to find a way around MMCE's bug
	
	HBufC8* contentType = KNullDesC8().AllocLC();
	HBufC8* content = KNullDesC8().AllocLC();
	
	/// --------------------------------------
	CleanupStack::Pop( content );
	CleanupStack::Pop( contentType );
	CleanupStack::Pop( headers );
	iOutSession.OutSession().EstablishL( sessionValues.timeOut, headers, contentType, content, NULL );

	iEngine.EngineStateChangedL();
   	}
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdEstablishWithValues::DeleteCodecs
// -----------------------------------------------------------------------------
//

EXPORT_C void TMCETestUIEngineCmdEstablishWithValues:: DeleteCodecs()
	{
	const RPointerArray<CMCETestUIEngineAudioStream>& audioStreams = 
            iOutSession.AudioStreamsL();
            
         for ( TInt i = 0; i < audioStreams.Count(); ++i )
        	{
        	 RPointerArray<CMCETestUIEngineCodec>& codecs = 
            	audioStreams[i]->CodecsL();
            	TInt count = codecs.Count();
            //delete all codecs except first one	
         	for ( TInt j = count-1; j > 0; --j )
				{
				 audioStreams[i]->AudioStream().RemoveCodecL(codecs[j]->Codec());
		         delete codecs[j];
		         codecs.Remove( j );
		         codecs.Compress();
		         iEngine.EngineStateChangedL();
		        }
        	}
    const RPointerArray<CMCETestUIEngineVideoStream>& videoStreams = 
            iOutSession.VideoStreamsL();
            
         for ( TInt i = 0; i < videoStreams.Count(); ++i )
        	{
        	 RPointerArray<CMCETestUIEngineVideoCodec>& codecs = 
            	videoStreams[i]->CodecsL();
            	TInt count = codecs.Count();
            //delete all codecs except first one	
         	for ( TInt j = count-1; j > 0; --j )
				{
				 videoStreams[i]->VideoStream().RemoveCodecL(codecs[j]->Codec());
		         delete codecs[j];
		         codecs.Remove( j );
		         codecs.Compress();
		         iEngine.EngineStateChangedL();
		        }
        	}
	}
// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdEstablishWithValues::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdEstablishWithValues::Caption() const
	{
	return KUserQuestionOptionEstalishWithValues;
	}


// End of File
