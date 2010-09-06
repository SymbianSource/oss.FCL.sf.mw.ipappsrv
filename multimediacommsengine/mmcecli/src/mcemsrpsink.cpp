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



#include <uricommon.h>
#include <uri8.h>
#include <uriutils.h>
#include<e32cmn.h>
#include<e32const.h>
#include <f32file.h>

#include "mcemsrpsink.h"
#include <mcertpsource.h>
#include <mcemsrpsource.h>
#include "mcecommsrpsink.h"
#include "mcesession.h"
#include "mcemediastream.h"
#include "mceclilogs.h"
#include "mcemanager.h"
#include "mcecommediastream.h"
#include <e32cmn.h>
#include <e32const.h>
#include <f32file.h>

#define _FLAT_DATA static_cast<CMceComMsrpSink*>( iFlatData )
#define FLAT_DATA( data ) (_FLAT_DATA->data)

#define _FLAT_ARRAY_DATA static_cast<CMceComMsrpSink*>(iFlatData)
#define FLAT_ARRAY_DATA(iArray,data) (_FLAT_ARRAY_DATA)->iArray.AppendL(data)

#define _FLAT_PTRARRAY_DATA static_cast<CMceComMsrpSink*>(iFlatData)
#define FLAT_PTRARRAY_DATA(iArray,data) (_FLAT_PTRARRAY_DATA)->iArray->AppendL(data)

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMceMsrpSink::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceMsrpSink* CMceMsrpSink::NewL()
    {    
    CMceMsrpSink* self = CMceMsrpSink::NewLC();    
	CleanupStack::Pop(self);
    return self;   
    }
	
	
// -----------------------------------------------------------------------------
// CMceMsrpSink::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CMceMsrpSink* CMceMsrpSink::NewLC()
    {    
    CMceMsrpSink* self = new (ELeave) CMceMsrpSink(); 
	CleanupStack::PushL(self);
	self->ConstructL();
    return self;   
    }   

	
// -----------------------------------------------------------------------------
// CMceMsrpSinke::ConstructL
// -----------------------------------------------------------------------------
//
void CMceMsrpSink::ConstructL()
    { 
    CMceComMsrpSink *sink = CMceComMsrpSink::NewLC();
    CMceMediaSink::ConstructL(sink);		
	CleanupStack::Pop(sink);
    }
       

// -----------------------------------------------------------------------------
// CMceMsrpSinke::~CMceMsrpSinke
// -----------------------------------------------------------------------------
//
EXPORT_C CMceMsrpSink::~CMceMsrpSink()
    {
    }	


// -----------------------------------------------------------------------------
// CMceMsrpSink::CMceMsrpSink
// -----------------------------------------------------------------------------
//
CMceMsrpSink::CMceMsrpSink()
    :CMceMediaSink()
    {
    }
	
// -----------------------------------------------------------------------------
// CMceMicSource::EnableL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceMsrpSink::EnableL()
    {
    MCECLI_DEBUG("CMceMsrpSink::EnableL, Entry");
    
    CMceMediaSink::DoEnableL(); 
    
    MCECLI_DEBUG("CMceMsrpSink::EnableL, Exit");
    
    }
    
// -----------------------------------------------------------------------------
// CMceMsrpSink::Disable
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceMsrpSink::DisableL()
    {
    MCECLI_DEBUG("CMceMsrpSink::DisableL, Entry");

    CMceMediaSink::DoDisableL();
    
    MCECLI_DEBUG("CMceMsrpSink::DisableL, Exit");
    
    }

// -----------------------------------------------------------------------------
// CMceMsrpSinke::Path
// -----------------------------------------------------------------------------
//

EXPORT_C RPointerArray<TUriC8>& CMceMsrpSink::Path()
    {
    return FLAT_DATA(iPath);
    }

 

// -----------------------------------------------------------------------------
// CMceMsrpSinke::SetPath
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceMsrpSink::SetPathL( const RPointerArray<TUriC8>& aMsrpPath )
    {
    __ASSERT_ALWAYS(iFlatData!=NULL, User::Leave(KErrArgument));
    // destroy the existing MSRP path entries
    static_cast<CMceComMsrpSink*>(iFlatData)->iPath.ResetAndDestroy();
    for(int i=0;i<aMsrpPath.Count();i++)
        {        
        TUriParser8* parser = new (ELeave) TUriParser8;
        CleanupStack::PushL(parser);
        User::LeaveIfError( parser->Parse(aMsrpPath[i]->UriDes()));
        FLAT_ARRAY_DATA(iPath, parser);
        CleanupStack::Pop(parser);
        }
    }


// -----------------------------------------------------------------------------
// CMceMsrpSink::AcceptTypesL
// -----------------------------------------------------------------------------
//

void CMceMsrpSink::AcceptTypesL(CDesC8ArrayFlat& aAcceptTypes)
    {
    __ASSERT_ALWAYS(iFlatData!=NULL, User::Leave(KErrArgument));
    
    for (TInt i=0; i< FLAT_DATA(iAcceptTypes).Count();i++)
        {
        aAcceptTypes.AppendL(FLAT_DATA(iAcceptTypes)[i]);
        }
    }
  
// -----------------------------------------------------------------------------
// CMceMsrpSink::SetAcceptTypesL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceMsrpSink::SetAcceptTypesL( const MDesC8Array& aAcceptTypes )
    {
    __ASSERT_ALWAYS(iFlatData!=NULL, User::Leave(KErrArgument));
    // reset the existing entries
    static_cast<CMceComMsrpSink*>(iFlatData)->iAcceptTypes.Reset();
    for(int i=0; i<aAcceptTypes.MdcaCount();i++)
        {       
        FLAT_ARRAY_DATA(iAcceptTypes, aAcceptTypes.MdcaPoint(i));        
        }
    }


// -----------------------------------------------------------------------------
// CMceMsrpSink::AcceptWrappedTypes
// -----------------------------------------------------------------------------
//

EXPORT_C TDes8& CMceMsrpSink::AcceptWrappedTypesL()
    {
    return FLAT_DATA(iAcceptWrappedTypes);
    }
    
// -----------------------------------------------------------------------------
// CMceMsrpSink::SetAcceptTypesL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceMsrpSink::SetAcceptWrappedTypesL( const TDes8& aAcceptWrappedTypes )
    {
    MCECLI_DEBUG(" \n CMceMsrpSink::SetAcceptWrappedTypesL : Entry");
    __ASSERT_ALWAYS(iFlatData!=NULL, User::Leave(KErrArgument));
    FLAT_DATA(iAcceptWrappedTypes) = aAcceptWrappedTypes;
    FLAT_DATA(iWrappedTypes) = ETrue;
    MCECLI_DEBUG(" \n CMceMsrpSink::SetAcceptWrappedTypesL : Entry");
    }


// -----------------------------------------------------------------------------
// CMceMsrpSink::SetFileSelector
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceMsrpSink::SetFileSelectorL(const TDesC& aName, const TDesC8& aFileType, TUint aFileSize )
    {
    __ASSERT_ALWAYS(iFlatData!=NULL, User::Leave(KErrArgument));
    FLAT_DATA(iFileShare) = ETrue;
    if (FLAT_DATA(iFileName) !=NULL )
        {
        delete FLAT_DATA(iFileName);
        FLAT_DATA(iFileName) = NULL;
        }
    FLAT_DATA(iFileName) = aName.Alloc();
    FLAT_DATA(iFileSize) = aFileSize;
    if (FLAT_DATA(iFileType) !=NULL )
        {
        delete FLAT_DATA(iFileType);
        FLAT_DATA(iFileType) = NULL;
        }
    FLAT_DATA(iFileType) = aFileType.Alloc();
    }

// -----------------------------------------------------------------------------
// CMceMsrpSink::SetFileDisposition
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceMsrpSink::SetFileDispositionL (const TDes8& aDispositionValue)
    {
    __ASSERT_ALWAYS(iFlatData!=NULL, User::Leave(KErrArgument));
    FLAT_DATA(iDispositionValue) = aDispositionValue;
    }


// -----------------------------------------------------------------------------
// CMceMsrpSink::SetFileTransferId
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceMsrpSink::SetFileTransferIdL(TDes8& aId)
    {
    __ASSERT_ALWAYS(iFlatData!=NULL, User::Leave(KErrArgument));
    //Set Current time as transferid (Which is unique)
    TTime transferId;
    transferId.HomeTime();
    aId.AppendNum(transferId.Int64());
    FLAT_DATA(iFileTransferid) = aId;
    }

// -----------------------------------------------------------------------------
// CMceMsrpSink::SetInformationAttr
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceMsrpSink::SetInformationAttrL(const TDes8& aInfo)
    {
    __ASSERT_ALWAYS(iFlatData!=NULL, User::Leave(KErrArgument));
    static_cast<CMceComMsrpSink*>(iFlatData)->iSessInfo = aInfo.AllocL();
    }

void CMceMsrpSink::InitializeL( CMceMediaStream& aParent )
    {
    MCECLI_DEBUG(" \n CMceMsrpSink::InitializeL : Entry");
    CMceMediaSink::InitializeL(aParent);
    
    RPointerArray<TUriC8>& msrpPath= Path();
    MCECLI_DEBUG_DVALUE("\n CMceMsrpSink::InitializeL: Msrp path count is: ", msrpPath.Count());
    if (msrpPath.Count() > 0 )
        {
        SetPortFromPathAttrL(* (msrpPath[msrpPath.Count()-1]) ,
                static_cast<CMceMessageStream&> (aParent) );
        }
    MCECLI_DEBUG("\n CMceMsrpSink::InitializeL : Exit");
    }


void CMceMsrpSink::SetPortFromPathAttrL(TUriC8& aMsrpUri, CMceMessageStream& aStream)
    {
    __ASSERT_ALWAYS(aStream.Type() == KMceMessage, User::Leave(KErrArgument));
    
    MCECLI_DEBUG("\n CMceMsrpSink::SetPortFromPathAttr : Entry");
    TUriParser8 parser;
    TInt parseValue = parser.Parse(aMsrpUri.UriDes()); 
    MCECLI_DEBUG_DVALUE("\n CMceMsrpSink::SetPortFromPathAttr: Parsed return value for MSRP URI: ",
            parseValue);
        
    TBuf8<50> host = parser.Extract(EUriHost);
    TBuf8<10> portBuf = parser.Extract(EUriPort);
    TInt Port;
        
    TBuf16<10> portBuf16;
    portBuf16.Copy(portBuf);
    TLex lex(portBuf16);
    lex.Val(Port);  // returns the port value 
    // Set media port
    aStream.SetLocalMediaPortL(Port);
	if (aStream.BoundStream())
		{
		aStream.BoundStreamL().SetLocalMediaPortL(Port);
		}
    MCECLI_DEBUG_DVALUE("\n CMceMsrpSink::SetPortFromPathAttr: Media port is : ", 
                Port);
    MCECLI_DEBUG("\n CMceMsrpSink::SetPortFromPathAttr : Exit");
    }


EXPORT_C void CMceMsrpSink::SetPathToSaveReceivedFile(const TDesC16& aFilePath)
    {
    if ( FLAT_DATA(iFilePath) != NULL)
        {
        delete FLAT_DATA(iFilePath);
        FLAT_DATA(iFilePath) = NULL;
        }
    FLAT_DATA(iFilePath) = aFilePath.Alloc();
    }

EXPORT_C void CMceMsrpSink::EnableFileTransferProgressNotifications(TBool enableFlag)
    {
    FLAT_DATA(iFTProgressNotification) = enableFlag;
    }
