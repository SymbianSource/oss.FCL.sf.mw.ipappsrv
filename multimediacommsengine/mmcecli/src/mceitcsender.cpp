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




#include "mcesession.h"
#include "mceitcsender.h"
#include "mceitc.h"
#include "mcecomsession.h"
#include "mceserial.h"
#include "mceclilogs.h"


// -----------------------------------------------------------------------------
// CMceItcSender::NewL
// -----------------------------------------------------------------------------
//
CMceItcSender* CMceItcSender::NewL ( MMceItc& aITC )
	{
    CMceItcSender* self = CMceItcSender::NewLC( aITC );
    CleanupStack::Pop (self);
    return self;
	}

// -----------------------------------------------------------------------------
// CMceItcSender::NewLC
// -----------------------------------------------------------------------------
//
CMceItcSender* CMceItcSender::NewLC ( MMceItc& aITC )
	{
	CMceItcSender* self = new (ELeave) CMceItcSender( aITC );
    CleanupStack::PushL( self );
    return self;
	}


// -----------------------------------------------------------------------------
// CMceItcSender::CMceItcSender
// -----------------------------------------------------------------------------
//	
CMceItcSender::CMceItcSender ( MMceItc& aITC )
    : iITC ( aITC ),
      iEmptyBody ( KNullDesC8 )
    {
    }

// -----------------------------------------------------------------------------
// CMceItcSender::~CMceItcSender
// -----------------------------------------------------------------------------
//
CMceItcSender::~CMceItcSender ()
    {
    }


// -----------------------------------------------------------------------------
// CMceItcSender::Send
// -----------------------------------------------------------------------------
//
TInt CMceItcSender::Send(TMceIds& aIds, TMceItcFunctions aITCFunction)
	{
	
    TPckgBuf<TMceIds> mceIdsPckg( aIds );
    iITCMsgArgs.Set( EMceItcArgIds, &mceIdsPckg );

	TInt err = iITC.Send( aITCFunction, iITCMsgArgs );
	aIds = mceIdsPckg();
    return err;
	}

// -----------------------------------------------------------------------------
// CMceItcSender::SendL
// -----------------------------------------------------------------------------
//
void CMceItcSender::SendL( TMceIds& aIds, TMceItcFunctions aITCFunction )
	{
    MCECLI_DEBUG("CMceItcSender::SendL, Entry");
    MCECLI_DEBUG_ITC( "ITC", aITCFunction );
    MCECLI_DEBUG_IDS( "IDS", aIds );
    
    TInt err = Send( aIds, aITCFunction );
    if ( err != KErrNone )
        {
        MCECLI_DEBUG_DVALUE("CMceItcSender::SendL, Exit. error", err );
        User::Leave( err );
        }

    MCECLI_DEBUG("CMceItcSender::SendL, Exit");
	}

// -----------------------------------------------------------------------------
// CMceItcSender::SendL
// -----------------------------------------------------------------------------
//
void CMceItcSender::SendL( TMceIds& aIds,
             		       TMceItcFunctions aITCFunction,
                           CMceMsgBase& aMessage,
            			   HBufC8* aBody,
            			   TBool aTakeOwnershipOfBody )
    {
    MCECLI_DEBUG("CMceItcSender::SendL(message,body), Entry");
    
    aMessage.EncodeL();
    TPtr8 externalizedMessage = aMessage.EncodeBuffer().Ptr(0);

    Mem::FillZ( &iITCMsgArgs, sizeof( iITCMsgArgs ) );
    aIds.iMsgType = aMessage.Type();
        
    iITCMsgArgs.Set( EMceItcArgContext, &externalizedMessage );
    
    if ( aBody )
        {
        iITCMsgArgs.Set( EMceItcArgMessageContent, aBody );    
        }
    else
        {
        iITCMsgArgs.Set( EMceItcArgMessageContent, &iEmptyBody );
        }
    SendL( aIds, aITCFunction );
    if ( aTakeOwnershipOfBody )
    	{
    	delete aBody;	
    	}

    MCECLI_DEBUG("CMceItcSender::SendL(message,body), Exit");
    }
        
// -----------------------------------------------------------------------------
// CMceItcSender::SendL
// -----------------------------------------------------------------------------
//
void CMceItcSender::SendL( TMceIds& aIds,
		    TMceItcFunctions aITCFunction,
            TUint32& aData )
    {
    MCECLI_DEBUG("CMceItcSender::SendL(TUin32), Entry");

    Mem::FillZ( &iITCMsgArgs, sizeof( iITCMsgArgs ) );
    TPckgBuf<TUint32> mceDataPckg( aData );

    iITCMsgArgs.Set( EMceItcArgId, &mceDataPckg );
        
    SendL( aIds, aITCFunction );   
    
    MCECLI_DEBUG("CMceItcSender::SendL(TUin32), Exit");
    }

// -----------------------------------------------------------------------------
// CMceItcSender::WriteL
// -----------------------------------------------------------------------------
//
void CMceItcSender::WriteL( TMceIds& aIds,
		                    TMceItcFunctions aITCFunction,
                            const TDesC8& aData  )
    {
    MCECLI_DEBUG("CMceItcSender::WriteL, Entry");
    
    Mem::FillZ( &iITCMsgArgs, sizeof( iITCMsgArgs ) );
    aIds.iMsgType = EMceItcMsgTypeWriteString;
        
    iITCMsgArgs.Set( EMceItcArgContext, &aData );
    
    SendL( aIds, aITCFunction );
    
    MCECLI_DEBUG("CMceItcSender::WriteL, Exit");
    }
            

// -----------------------------------------------------------------------------
// CMceItcSender::ReadL
// -----------------------------------------------------------------------------
//
void CMceItcSender::ReadL( TMceIds& aIds,
		                   TMceItcFunctions aITCFunction,
                           TDes8& aData )
    {
    MCECLI_DEBUG("CMceItcSender::ReadL, Entry");
    Mem::FillZ( &iITCMsgArgs, sizeof( iITCMsgArgs ) );
    aIds.iMsgType = EMceItcMsgTypeReadString;

    iITCMsgArgs.Set( EMceItcArgContext, &aData );
        
    SendL( aIds, aITCFunction );
    
    MCECLI_DEBUG("CMceItcSender::ReadL, Exit");
    }
				

// -----------------------------------------------------------------------------
// CMceItcSender::ReadArrayL 
// -----------------------------------------------------------------------------
//
CDesC8Array* CMceItcSender::ReadArrayL(TMceIds& aIds,
				TMceItcFunctions aITCFunction )
	{
    MCECLI_DEBUG("CMceItcSender::ReadArrayL, Entry");
	
    Mem::FillZ(&iITCMsgArgs, sizeof(iITCMsgArgs));
    aIds.iMsgType = KMceNotAssigned;

    CMceMsgTextArray* textArray = new (ELeave) CMceMsgTextArray();
    CleanupStack::PushL( textArray );
    
	HBufC8* arrayBuf = HBufC8::NewLC( KMceMaxSizeInReadStream );
	TPtr8 arrayBufPtr = arrayBuf->Des();
    iITCMsgArgs.Set (EMceItcArgContext, &arrayBufPtr);
    SendL (aIds,aITCFunction);

    textArray->DecodeL( *arrayBuf );

    CDesC8Array* array = textArray->iArray;
    
    CleanupStack::PopAndDestroy(arrayBuf);//arrayBuf
    CleanupStack::PopAndDestroy( textArray );//textArray
    
    MCECLI_DEBUG("CMceItcSender::ReadArrayL, Exit");
    return array;
		
	}

// -----------------------------------------------------------------------------
// CMceItcSender::ReadArrayL 
// -----------------------------------------------------------------------------
//
CDesC8Array* CMceItcSender::ReadArrayL( TMceIds& aIds,
				                        TMceItcFunctions aITCFunction, 
				                        const TDesC8& aData )
	{
    MCECLI_DEBUG("CMceItcSender::ReadArrayL(inout), Entry");
	
    Mem::FillZ( &iITCMsgArgs, sizeof( iITCMsgArgs ) );
    aIds.iMsgType = KMceNotAssigned;

    CMceMsgTextArray* textArray = new (ELeave) CMceMsgTextArray();
    CleanupStack::PushL( textArray );
    
	HBufC8* arrayBuf = HBufC8::NewLC( KMceMaxSizeInReadStream );
	TPtr8 arrayBufPtr = arrayBuf->Des();
	arrayBufPtr.Copy( aData );
    iITCMsgArgs.Set ( EMceItcArgContext, &arrayBufPtr );
    SendL ( aIds, aITCFunction );

    textArray->DecodeL( *arrayBuf );

    CDesC8Array* array = textArray->iArray;
    
    CleanupStack::PopAndDestroy( arrayBuf );//arrayBuf
    CleanupStack::PopAndDestroy( textArray );//textArray
    
    MCECLI_DEBUG("CMceItcSender::ReadArrayL(inout), Exit");
    return array;
		
	}


// -----------------------------------------------------------------------------
// CMceItcSender::ReadStringL
// -----------------------------------------------------------------------------
//
HBufC8* CMceItcSender::ReadStringL(TMceIds& aIds,
				TMceItcFunctions aITCFunction )
	{

    MCECLI_DEBUG("CMceItcSender::ReadStringL, Entry");
	
    Mem::FillZ(&iITCMsgArgs, sizeof(iITCMsgArgs));
    aIds.iMsgType = KMceNotAssigned;

    
	HBufC8* string = HBufC8::NewLC( KMceMaxSizeInSessionAsDesc );
	TPtr8 stringPtr = string->Des();
    iITCMsgArgs.Set (EMceItcArgContext, &stringPtr);
    SendL (aIds,aITCFunction);
    
    CleanupStack::Pop( string );
    
    MCECLI_DEBUG("CMceItcSender::ReadStringL, Exit");
    return string;

	}



