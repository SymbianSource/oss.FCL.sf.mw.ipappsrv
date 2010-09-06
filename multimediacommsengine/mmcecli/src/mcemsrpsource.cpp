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

#include "mcemsrpsource.h"
#include <mcertpsource.h>
#include "mcecommsrpsource.h"
#include "mcesession.h"
#include "mcemediastream.h"
#include "mceclilogs.h"
#include "mcemanager.h"
#include "mcemessagestream.h"
#include "mcecommediastream.h"

#define _FLAT_DATA static_cast<CMceComMsrpSource*>( iFlatData )
#define FLAT_DATA( data ) (_FLAT_DATA->data)

#define _FLAT_ARRAY_DATA static_cast<CMceComMsrpSource*>(iFlatData)
#define FLAT_ARRAY_DATA(iArray,data) (_FLAT_ARRAY_DATA)->iArray.AppendL(data)

#define _FLAT_PTRARRAY_DATA static_cast<CMceComMsrpSource*>(iFlatData)
#define FLAT_PTRARRAY_DATA(iArray,data) (_FLAT_PTRARRAY_DATA)->iArray->AppendL(data)

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMceMsrpSource::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceMsrpSource* CMceMsrpSource::NewL()
    {    
    CMceMsrpSource* self = NewLC();    
	CleanupStack::Pop(self);
    return self;   
    }
	
	
// -----------------------------------------------------------------------------
// CMceMsrpSource::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CMceMsrpSource* CMceMsrpSource::NewLC()
    {    
    CMceMsrpSource* self = new (ELeave) CMceMsrpSource(); 
	CleanupStack::PushL(self);
	self->ConstructL();
    return self;   
    }   

// -----------------------------------------------------------------------------
// CMceMsrpSourcee::ConstructL
// -----------------------------------------------------------------------------
//
void CMceMsrpSource::ConstructL()
    {    
	CMceComMsrpSource *source = CMceComMsrpSource::NewLC();
	CMceMediaSource::ConstructL(source);
	CleanupStack::Pop(source);
    }
       

// -----------------------------------------------------------------------------
// CMceMsrpSourcee::~CMceMsrpSourcee
// -----------------------------------------------------------------------------
//
EXPORT_C CMceMsrpSource::~CMceMsrpSource()
    {
    }

// -----------------------------------------------------------------------------
// CMceMsrpSink::CMceMsrpSink
// -----------------------------------------------------------------------------
//
CMceMsrpSource::CMceMsrpSource()
    :CMceMediaSource()
    {
    }
	
// -----------------------------------------------------------------------------
// CMceMicSource::EnableL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceMsrpSource::EnableL()
    {
    MCECLI_DEBUG("CMceMsrpSource::EnableL, Entry");
    
    CMceMediaSource::DoEnableL();
    
    MCECLI_DEBUG("CMceMsrpSource::EnableL, Exit");
    
    }
    
// -----------------------------------------------------------------------------
// CMceMsrpSource::Disable
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceMsrpSource::DisableL()
    {
    MCECLI_DEBUG("CMceMsrpSource::DisableL, Entry");

    CMceMediaSource::DoDisableL();
    
    MCECLI_DEBUG("CMceMsrpSource::DisableL, Exit");
    
    }

// -----------------------------------------------------------------------------
// CMceMsrpSinke::Path
// -----------------------------------------------------------------------------
//

EXPORT_C RPointerArray<TUriC8>& CMceMsrpSource::Path()
    {
    return FLAT_DATA(iPath);
    }

 

// -----------------------------------------------------------------------------
// CMceMsrpSource::SetPath
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceMsrpSource::SetPathL( const RPointerArray<TUriC8>& aMsrpPath )
    { 
    __ASSERT_ALWAYS(iFlatData!=NULL, User::Leave(KErrArgument));
    // destroy the existing entries
    static_cast<CMceComMsrpSource*>(iFlatData)->iPath.ResetAndDestroy(); 
    for(int i=0;i<aMsrpPath.Count();i++)
        {        
        TUriParser8* parser = new (ELeave) TUriParser8;
        CleanupStack::PushL(parser);
        User::LeaveIfError( parser->Parse(aMsrpPath[i]->UriDes()) );
        FLAT_ARRAY_DATA(iPath, parser);
        CleanupStack::Pop(parser);
        }
    }

// -----------------------------------------------------------------------------
// CMceMsrpSource::AcceptTypes
// -----------------------------------------------------------------------------
//

EXPORT_C void CMceMsrpSource::AcceptTypesL(CDesC8ArrayFlat& aAcceptTypes)
    {
    __ASSERT_ALWAYS(iFlatData!=NULL, User::Leave(KErrArgument));
    for (TInt i=0; i< FLAT_DATA(iAcceptTypes).Count();i++)
        {
        aAcceptTypes.AppendL(FLAT_DATA(iAcceptTypes)[i]);
        }
    }
    

EXPORT_C void CMceMsrpSource::SetAcceptTypesL( const MDesC8Array& aAcceptTypes )
    {
    __ASSERT_ALWAYS(iFlatData!=NULL, User::Leave(KErrArgument));
    // reset the existing entries
    static_cast<CMceComMsrpSource*>(iFlatData)->iAcceptTypes.Reset();
    for(int i=0; i<aAcceptTypes.MdcaCount();i++)
        {       
        FLAT_ARRAY_DATA(iAcceptTypes, aAcceptTypes.MdcaPoint(i));        
        }       
    }

// -----------------------------------------------------------------------------
// CMceMsrpSource::AcceptTypes
// -----------------------------------------------------------------------------
//

EXPORT_C TDes8& CMceMsrpSource::AcceptWrappedTypesL()
    {
    __ASSERT_ALWAYS(iFlatData!=NULL, User::Leave(KErrArgument));
    return FLAT_DATA(iAcceptWrappedTypes);
    }
    
// -----------------------------------------------------------------------------
// CMceMsrpSource::SetAcceptTypesL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceMsrpSource::SetAcceptWrappedTypesL( const TDes8& aAcceptWrappedTypes )
    {
    MCECLI_DEBUG(" \n CMceMsrpSource::SetAcceptWrappedTypesL : Entry");
    __ASSERT_ALWAYS(iFlatData!=NULL, User::Leave(KErrArgument));
    MCECLI_DEBUG_SVALUE("CMceMsrpSource::SetAcceptWrappedTypesL:", aAcceptWrappedTypes)
    FLAT_DATA(iAcceptWrappedTypes) = aAcceptWrappedTypes;
    FLAT_DATA(iWrappedTypes) = ETrue;
    MCECLI_DEBUG(" \n CMceMsrpSource::SetAcceptWrappedTypesL : Exit");
    }

// -----------------------------------------------------------------------------
// CMceMsrpSource::SetFileSelector
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceMsrpSource::SetFileSelectorL(const TDesC& aName, const TDesC8& aFileType, TUint aFileSize )
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
// CMceMsrpSource::SetFileDisposition
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceMsrpSource::SetFileDispositionL (const TDes8& aDispositionValue)
    {
    __ASSERT_ALWAYS(iFlatData!=NULL, User::Leave(KErrArgument));
    FLAT_DATA(iDispositionValue) = aDispositionValue;
    }

// -----------------------------------------------------------------------------
// CMceMsrpSource::SetFileTransferId
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceMsrpSource::SetFileTransferIdL(TDes8& aId)
    {
    __ASSERT_ALWAYS(iFlatData!=NULL, User::Leave(KErrArgument));
    //Set Current time as transferid (Which is unique)
    TTime transferId;
    transferId.HomeTime();
    aId.AppendNum(transferId.Int64());
    FLAT_DATA(iFileTransferid) = aId;
    }

// -----------------------------------------------------------------------------
// CMceMsrpSource::SetInformationAttr
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceMsrpSource::SetInformationAttrL(const TDes8& aInfo)
    {
    __ASSERT_ALWAYS(iFlatData!=NULL, User::Leave(KErrArgument));
    if (static_cast<CMceComMsrpSource*>(iFlatData)->iSessInfo != NULL)
        {
        delete static_cast<CMceComMsrpSource*>(iFlatData)->iSessInfo;
        static_cast<CMceComMsrpSource*>(iFlatData)->iSessInfo = NULL;
        }
    static_cast<CMceComMsrpSource*>(iFlatData)->iSessInfo = aInfo.Alloc();
    }


// -----------------------------------------------------------------------------
// CMceMsrpSource::InitializeL
// -----------------------------------------------------------------------------
//
void CMceMsrpSource::InitializeL( CMceMediaStream& aParent )
    {
    MCECLI_DEBUG(" \n CMceMsrpSource::InitializeL : Entry");
    CMceMediaSource::InitializeL(aParent);
    
    RPointerArray<TUriC8>& msrpPath= Path();
    MCECLI_DEBUG_DVALUE("\n CMceMsrpSource::InitializeL: Msrp path count is: ", msrpPath.Count());
    if (msrpPath.Count() > 0 )
        {
        SetPortFromPathAttrL(* (msrpPath[msrpPath.Count()-1]) ,
                static_cast<CMceMessageStream&> (aParent) );
        }
    MCECLI_DEBUG("\n CMceMsrpSource::InitializeL : Exit");
    }

// -----------------------------------------------------------------------------
// CMceMsrpSource::InitializeL
// -----------------------------------------------------------------------------
//
void CMceMsrpSource::SetPortFromPathAttrL(TUriC8& aMsrpUri, CMceMessageStream& aStream)
    {
    __ASSERT_ALWAYS(aStream.Type() == KMceMessage, User::Leave(KErrArgument));
    
    MCECLI_DEBUG("\n CMceMsrpSource::SetHostAddrPortFromPathAttr : Entry");
    TUriParser8 parser;
    TInt parseValue = parser.Parse(aMsrpUri.UriDes()); 
    MCECLI_DEBUG_DVALUE("\n CMceMsrpSource::SetHostAddrPortFromPathAttr: Parsed return value for MSRP URI: ",
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
    MCECLI_DEBUG_DVALUE("\n CMceMsrpSource::SetHostAddrPortFromPathAttr: Media port is : ", 
                Port);
    MCECLI_DEBUG("\n CMceMsrpSource::SetHostAddrPortFromPathAttr : Exit");
    }


// -----------------------------------------------------------------------------
// CMceMsrpSource::InitializeL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceMsrpSource::SetPathToSaveReceivedFile(const TDesC16& aFilePath)
    {
    if (FLAT_DATA(iFilePath) != NULL )
        {
        delete FLAT_DATA(iFilePath);
        FLAT_DATA(iFilePath) = NULL;
        }
    FLAT_DATA(iFilePath) = aFilePath.Alloc();
    }
    
// -----------------------------------------------------------------------------
// CMceMsrpSource::InitializeL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceMsrpSource::EnableFileTransferProgressNotifications(TBool enableFlag)
    {
    FLAT_DATA(iFTProgressNotification) = enableFlag;
    }
