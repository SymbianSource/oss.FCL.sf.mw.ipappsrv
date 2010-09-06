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




#include "mcecommsrpsource.h"
#include "mcecommediastream.h"
#include "mcecomsession.h"
#include "mcemsrpsource.h"
#include "e32const.h"
#include <e32std.h>
#include "mceserial.h"
#include "mceclilogs.h"

#ifdef MCE_COMMON_SERVER_SIDE

#include <mmccInterface.h>
#include "mcesdpsession.h"
#include "mcemediamanager.h"
#include "mcesrvlogs.h"


#endif//MCE_COMMON_SERVER_SIDE

// ============================ MEMBER FUNCTIONS ===============================



// -----------------------------------------------------------------------------
// CMceComAvSink::NewL
// -----------------------------------------------------------------------------
//
CMceComMsrpSource* CMceComMsrpSource::NewL()
    {
    CMceComMsrpSource* self = NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMceComMsrpSource::NewLC
// -----------------------------------------------------------------------------
//
CMceComMsrpSource* CMceComMsrpSource::NewLC()
    {
    CMceComMsrpSource* self = new (ELeave) CMceComMsrpSource();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CMceComMsrpSource::~CMceComMsrpSource
// -----------------------------------------------------------------------------
//
CMceComMsrpSource::~CMceComMsrpSource()
    { 
    iPath.ResetAndDestroy();
    delete iFileName;
    delete iSessInfo;
    delete iFileType;
    }

// -----------------------------------------------------------------------------
// CMceComMsrpSource::CMceComMsrpSource
// -----------------------------------------------------------------------------
//
CMceComMsrpSource::CMceComMsrpSource()
  :CMceComMediaSource( KMceMSRPSource ),
  iPath(KMceArrayGranularity),
  iSessInfo(NULL),
  iAcceptTypes(KMceArrayGranularity),
  iAcceptWrappedTypes (KMceAcceptWrappedTypes),
  iWrappedTypes(EFalse),
  iFileShare(EFalse),
  iFileName(NULL)
    {
    }

void CMceComMsrpSource::ConstructL(CMceComMsrpSource& aSource )
    {    
    iPath.ResetAndDestroy();
    for(int i=0;i<aSource.iPath.Count();i++)
        {       
        TUriParser8* parser = new (ELeave) TUriParser8;
        parser->Parse(aSource.iPath[i]->UriDes());
         CleanupStack::PushL(parser);
         iPath.AppendL(parser);
         CleanupStack::Pop(parser);
          }
   
    if (iSessInfo !=NULL)
        {
        delete iSessInfo;
        iSessInfo = NULL;
        }
    if (aSource.iSessInfo!=NULL)
        {
        iSessInfo = aSource.iSessInfo->Des().AllocL();
        }
        
    iAcceptTypes.Reset();  
    for(int i=0; i<aSource.iAcceptTypes.MdcaCount();i++)
        {
        iAcceptTypes.AppendL(aSource.iAcceptTypes.MdcaPoint(i) ); 
        }
     iAcceptWrappedTypes = aSource.iAcceptWrappedTypes;
     iWrappedTypes = aSource.iWrappedTypes;
     iFileShare = aSource.iFileShare;
     
     if (iFileName!=NULL)
         {
         delete iFileName;
         iFileName = NULL;
         }
     if (aSource.iFileName!=NULL)
         {
         iFileName = aSource.iFileName->Des().AllocL();
         }
     iFileSize = aSource.iFileSize;
     iFileTransferid = aSource.iFileTransferid;
     iDispositionValue = aSource.iDispositionValue;
     iSecureConnection = aSource.iSecureConnection;
     if (iFileType!=NULL)
         {
         delete iFileType;
         iFileType = NULL;
         }
     if (aSource.iFileType!=NULL)
         {
         iFileType = aSource.iFileType->Des().AllocL();
         }
     iFTProgressNotification = aSource.iFTProgressNotification;
    }
        
    
void CMceComMsrpSource::ConstructL()
    {
    iFTProgressNotification = ETrue;
    }

// -----------------------------------------------------------------------------
// CMceComMsrpSource::CloneL
// -----------------------------------------------------------------------------
//
CMceComMediaSource* CMceComMsrpSource::CloneL()
    {
    CMceComMsrpSource* copy = CMceComMsrpSource::NewLC();
    copy->ConstructL(*this);
    CleanupStack::Pop(copy);
    return copy;        
    }


// -----------------------------------------------------------------------------
// CMceComMsrpSource::InternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceComMsrpSource::InternalizeFlatL( RReadStream& aReadStream )
    {
    CMceComMediaSource::InternalizeFlatL( aReadStream );
    MceSerial::DecodeL(this->iPath, aReadStream);
    MceSerial::DecodeL(this->iSessInfo, aReadStream);
    CDesC8Array* array = NULL;
    MceSerial::DecodeL(array, aReadStream);
    if (array)
        {
        this->iAcceptTypes.Reset();
        for (TInt i=0;i<array->MdcaCount();i++)
            {
            this->iAcceptTypes.AppendL(array->MdcaPoint(i)); 
            }
            delete array; // free up the memory
            }    
    MceSerial::DecodeL(this->iAcceptWrappedTypes, aReadStream);
    this->iWrappedTypes = aReadStream.ReadInt8L();
    
#ifdef MCE_COMMON_SERVER_SIDE
    MCESRV_DEBUG_SVALUE( "CMceComMsrpSource::InternalizeFlatL: accept-wrapped-types:",  this->iAcceptWrappedTypes)
    MCESRV_DEBUG_DVALUE("CMceComMsrpSource::InternalizeFlatL: wrappedTypes", this->iWrappedTypes)
    
#else
    MCECLI_DEBUG_DVALUE("CMceComMsrpSource::InternalizeFlatL: wrappedTypes value", this->iWrappedTypes)
    MCECLI_DEBUG_SVALUE("CMceComMsrpSource::InternalizeFlatL: AcceptWrappedTypes", this->iAcceptWrappedTypes) 
    
#endif
    this->iFileShare = aReadStream.ReadInt8L();
    MceSerial::DecodeL(this->iFileName, aReadStream);
    this->iFileSize = aReadStream.ReadUint32L();
    MceSerial::DecodeL(this->iFileType, aReadStream);
    MceSerial::DecodeL(this->iFileTransferid, aReadStream);
    MceSerial::DecodeL(this->iDispositionValue, aReadStream);
    this->iSecureConnection = aReadStream.ReadInt8L();  
    MceSerial::DecodeL(this->iFilePath, aReadStream);
    this->iFTProgressNotification = aReadStream.ReadInt8L();
    }

// -----------------------------------------------------------------------------
// CMceComMsrpSource::ExternalizeFlatL
// -----------------------------------------------------------------------------
//
void CMceComMsrpSource::ExternalizeFlatL( RWriteStream& aWriteStream )
    {
    CMceComMediaSource::ExternalizeFlatL( aWriteStream );
    MceSerial::EncodeL(this->iPath, aWriteStream);
    MceSerial::EncodeL(this->iSessInfo, aWriteStream);
    MceSerial::EncodeL(&(this->iAcceptTypes), aWriteStream);
    MceSerial::EncodeL(this->iAcceptWrappedTypes, aWriteStream);
    aWriteStream.WriteInt8L(this->iWrappedTypes);
    aWriteStream.WriteInt8L(this->iFileShare);
    MceSerial::EncodeL(this->iFileName, aWriteStream);
    aWriteStream.WriteUint32L(this->iFileSize);
    MceSerial::EncodeL(this->iFileType, aWriteStream);
    MceSerial::EncodeL(this->iFileTransferid, aWriteStream);
    MceSerial::EncodeL(this->iDispositionValue, aWriteStream);
    aWriteStream.WriteInt8L(this->iSecureConnection);
    MceSerial::EncodeL(this->iFilePath, aWriteStream);
    aWriteStream.WriteInt8L(this->iFTProgressNotification);
    }

// -----------------------------------------------------------------------------
// CMceComMsrpSink::UpdateL
// -----------------------------------------------------------------------------
//
void CMceComMsrpSource::UpdateL( CMceComMediaSource& aUpdate )
    {
    CMceComMediaSource::UpdateL( aUpdate );
    CMceComMsrpSource& update = static_cast<CMceComMsrpSource&>( aUpdate );
    
    //Set MSRP path member
    
    if (update.iPath.Count() > 0)
        {
        iPath.ResetAndDestroy();
        for(int i=0;i<update.iPath.Count();i++)
            {        
            TUriParser8* parser = new (ELeave) TUriParser8;
            CleanupStack::PushL(parser);
            User::LeaveIfError(parser->Parse(update.iPath[i]->UriDes()) );
            iPath.AppendL(parser);        
            CleanupStack::Pop(parser);
            }
        }
        
    //Set Accept-types member
    if (update.iAcceptTypes.MdcaCount() > 0)
        {
        iAcceptTypes.Reset();
        for(int i=0; i<update.iAcceptTypes.MdcaCount();i++)
            {       
            iAcceptTypes.AppendL(update.iAcceptTypes.MdcaPoint(i));        
            }    
        }
        
    //Set secureconneciton member
    if (iSecureConnection != update.iSecureConnection)
        {
        iSecureConnection = update.iSecureConnection;
        }
    
    if (update.iAcceptWrappedTypes.Size())
        {
        iAcceptWrappedTypes = update.iAcceptWrappedTypes;
        iWrappedTypes = update.iWrappedTypes;
        }
    // look for the updaitons for file share attributes
    UpdateFileShareAttrL(update);
    if (update.iSessInfo != NULL )
        {
        if (iSessInfo!=NULL)
            {
            delete iSessInfo;
            iSessInfo = NULL;
            }
        iSessInfo =  update.iSessInfo->Des().AllocL() ;
        }
    
    iFTProgressNotification = update.iFTProgressNotification;
    }

void CMceComMsrpSource::UpdateFileShareAttrL(CMceComMsrpSource& aUpdate )
    {
    //Set FileSharing Attributes
    if(aUpdate.iFileShare || aUpdate.iFileTransferid.Length() || aUpdate.iDispositionValue.Size() )
        {
        iFileShare = aUpdate.iFileShare;
        if (iFileName!=NULL )
            {
            delete iFileName;
            iFileName = NULL;
            }
    
        iFileName = aUpdate.iFileName->Des().AllocL();
        iFileSize = aUpdate.iFileSize;
        if (iFileType!=NULL )
            {
            delete iFileType;
            iFileType = NULL;
            }
            
        iFileType = aUpdate.iFileType->Des().AllocL();
        iFileTransferid = aUpdate.iFileTransferid;
        iDispositionValue = aUpdate.iDispositionValue;
        }
    }

// -----------------------------------------------------------------------------
// CMceComMsrpSource::UseRTCP
// -----------------------------------------------------------------------------
//
TBool CMceComMsrpSource::UseRTCP()
    {
    return EFalse;
    }

#ifdef MCE_COMMON_SERVER_SIDE

// -----------------------------------------------------------------------------
// CMceComMsrpSource::MccType
// -----------------------------------------------------------------------------
//
const TUid CMceComMsrpSource::MccType()
    {
    return KUidMccMsrpSource;
    }


// -----------------------------------------------------------------------------
// CMceComMsrpSource::PrepareL
// -----------------------------------------------------------------------------
//
void CMceComMsrpSource::PrepareL()
    {
    //NOP  // Not required as the data path for message stream will be handled by applications
    }
   
#endif//MCE_COMMON_SERVER_SIDE

