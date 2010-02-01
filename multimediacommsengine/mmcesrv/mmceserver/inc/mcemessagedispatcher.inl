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




// -----------------------------------------------------------------------------
// TMceMessageDispatcher::TMceMessageDispatcher
// -----------------------------------------------------------------------------
//
template <class T>
inline TMceMessageDispatcher<T>::TMceMessageDispatcher( T& aService, CMceCsServerITC& aItc )
   : iService( aService ),
     iItc( aItc )
    {
    }



// -----------------------------------------------------------------------------
// TMceMessageDispatcher::DoServiceL
// -----------------------------------------------------------------------------
//
template <class T>
inline void TMceMessageDispatcher<T>::DispatchL( TMceIds& aIds, 
                                                 TMceItcFunctions aFunction, 
								                 const RMessage2& aMessage )

    {

    MCESRV_DEBUG("TMceMessageDispatcher::DispatchL, Entry");
    MCESRV_DEBUG_IDS("IDS", aIds );
    
    HBufC8* encodedMessage = NULL;
    HBufC8* content = NULL;
    CMceMsgBase* message = NULL;
    HBufC8* returnMessage = NULL;
    
    aIds.iItcContext = &aMessage;
    
    if ( aIds.iMsgType == EMceItcMsgTypeWriteString ||
         aIds.iMsgType == EMceItcMsgTypeReadString )
        {
        MCESRV_DEBUG("reading message from client");
	    encodedMessage = ITC().ReadLC( aMessage, EMceItcArgContext );
	    encodedMessage = !encodedMessage ? 
	        KNullDesC8().AllocLC() : encodedMessage;
        
        if ( aIds.iMsgType == EMceItcMsgTypeReadString )
            {
            TPtr8 buffer = encodedMessage->Des();
            iService.DoServiceL( aIds, aFunction, buffer );
            ITC().WriteL( aMessage, 
                          buffer, 
                          EMceItcArgContext );
            }
        else
            {
            iService.DoServiceL( aIds, aFunction, *encodedMessage );
            }
        CleanupStack::PopAndDestroy( encodedMessage );
        }
    else if ( aIds.iMsgType != KMceNotAssigned )
	    {
        MCESRV_DEBUG("reading message from client");
	    encodedMessage = ITC().ReadLC( aMessage, EMceItcArgContext );
	    encodedMessage = !encodedMessage ? 
	        KNullDesC8().AllocLC() : encodedMessage;
	    content = ITC().ReadLC( aMessage, EMceItcArgMessageContent );
        MCESRV_DEBUG_DVALUE("content size", content ? content->Length() : 0 );
	    CleanupStack::Pop( content );
	    iService.StoreClientContent( content );
        message = DecodeL( static_cast<TMceItcDataType>( aIds.iMsgType ), 
                           *encodedMessage );
        MCESRV_DEBUG("calling ServiceL with message"); 
        CleanupStack::PushL( message  );
        message->PushL();
        iService.DoServiceL( aIds, aFunction, *message );
        message->Pop();
        CleanupStack::PopAndDestroy( message );
        CleanupStack::PopAndDestroy( encodedMessage );
	    }
    else
        {
        MCESRV_DEBUG("calling ServiceL without message"); 
        returnMessage = iService.DoServiceL( aIds, aFunction );
        if ( returnMessage )
            {
            CleanupStack::PushL( returnMessage );
            ITC().WriteL( aMessage, 
                          *returnMessage, 
                          EMceItcArgContext );
            CleanupStack::PopAndDestroy( returnMessage );
            }
        }
        
    MCESRV_DEBUG("TMceMessageDispatcher::DispatchL, Exit");
        
    }
    
// -----------------------------------------------------------------------------
// TMceMessageDispatcher::DecodeL
// -----------------------------------------------------------------------------
//
template <class T>
inline CMceMsgBase* TMceMessageDispatcher<T>::DecodeL( TMceItcDataType aType, 
                                                       const TDesC8& aMessage )
    {
    MCESRV_DEBUG("TMceMessageDispatcher::DecodeL, Entry");
    
	RDesReadStream readStream( aMessage );
    CleanupClosePushL( readStream );
    
    CMceMsgBase* message = TMceComFactory().CreateL( aType, readStream );
	CleanupStack::PopAndDestroy(); // readStream
	
    MCESRV_DEBUG_DVALUE("message type", aType );

    MCESRV_DEBUG("TMceMessageDispatcher::DecodeL, Exit");

    return message;
                
    }
    
