/*
* Copyright (c) 2006-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Provides MSRP support
*
*/




// INCLUDE FILES
#include "mccmsrpmanager.h"
#include "mccinternalcodecs.h"
#include "mmccevents.h"
#include "mccsubcontrollerlogs.h"
#include "mccresources.h"
#include "mccqoshandler.h"
#include "mccinternaldef.h"
#include <CommDbConnPref.h> 
#include "mccmsrpinterface.h"
#include "mccmsrpsink.h"
#include "mccmsrpsource.h"
#include <CMSRP.h>
#include <CMSRPSession.h>
#include <MsrpCommon.h>
#include <MMSRPSessionObserver.h>
#include <uri8.h>

// LOCAL CONSTANTS AND MACROS

// ============================= LOCAL FUNCTIONS ===============================

    
void CMccMsrpManager::ConnectStatus( TInt aStatus )
    {
    __SUBCONTROLLER_INT1("CMccMsrpManager::ConnectStatus : status is", aStatus )
    iConnectStatus = aStatus;
    __ASSERT_ALWAYS(iMsrpSession, User::Leave(KErrNotReady));
    if ( aStatus == KErrNone)
        {
        // Send file is requested
        if (iFileSendRequested )
            {
            __SUBCONTROLLER( "CMccMsrpManager::ConnectStatus: SendFileL is issued" )
                    
            iMsrpSession->NotifyProgress(iFTProgressNotification);
            TFileName fileName;
            fileName.Copy(iFileName->Des());
            TInt error = iMsrpSession->SendFile(fileName, iFileType->Des());
            if (error != KErrNone && iMsrpSessionOb !=NULL )
                iMsrpSessionOb->SendFileNotification(error);
                }
        else if (iFileReceiveRequested)
            { // receive file is requested
            __SUBCONTROLLER( "CMccMsrpManager::ConnectStatus: ReceiveFile is issued" )
            
            iMsrpSession->NotifyProgress(iFTProgressNotification);
            TFileName fileName;
            fileName.Copy(iFileName->Des());
            TInt error = iMsrpSession->ReceiveFile(fileName, iFileSize, iFileType->Des());
            if (error != KErrNone && iMsrpSessionOb !=NULL)
                {
                iMsrpSessionOb->ReceiveFileNotification(error);
                }
            }
        }
    else
        {
        if (iFileSendRequested )
            {
            if (iMsrpSessionOb !=NULL )
                iMsrpSessionOb->SendFileNotification(aStatus);
            }
        else
            {
            if (iMsrpSessionOb !=NULL )
                iMsrpSessionOb->ReceiveFileNotification(aStatus);
            }
        }
        //CActiveScheduler::Stop();
        __SUBCONTROLLER( "CMccMsrpManager::ConnectStatus:exit" )
        }



// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccMsrpManager::CMccMsrpManager
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMccMsrpManager::CMccMsrpManager(
        MAsyncEventHandler& aHandler, 
        MMccResources& aMccResources) : 
    iEventHandler( aHandler ),
    iMccResources( aMccResources )
    {
    ifirstTime = ETrue;
    iFileSendRequested = EFalse;
    iFileReceiveRequested = EFalse;
    iIsRegisteredForConnOb = EFalse;
    iRefCountForMSRPSession = 0;
    }

// -----------------------------------------------------------------------------
// CMccMsrpManager::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMccMsrpManager::ConstructL(TUid /*aUID*/)
    {
    __SUBCONTROLLER( "CMccMsrpManager::ConstructL" )
    
    //iMsrp = CMSRP::NewL(aUID);
    
    iMsrpObserver = CMsrpCallbackMgrObserver::NewL(iEventHandler); 
    
    iFileShare = NULL;
    iFileName = NULL;
    iFileType = NULL;
    iMsrpSessionOb = NULL;
    
    __SUBCONTROLLER( "CMccMsrpManager::ConstructL exit" )
    }

// -----------------------------------------------------------------------------
// CMccMsrpManager::NewL
// Static constructor.
// -----------------------------------------------------------------------------
//
CMccMsrpManager* CMccMsrpManager::NewL(MAsyncEventHandler& aHandler, MMccResources& aMccResources, TUid aUID)
    {
    __SUBCONTROLLER( "CMccMsrpManager::NewL" )
    CMccMsrpManager* self = new (ELeave) CMccMsrpManager( aHandler, aMccResources );     
    CleanupStack::PushL( self );      
    self->ConstructL(aUID);   
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// CMccMsrpManager::SetLinkId
// -----------------------------------------------------------------------------
//
void CMccMsrpManager::SetLinkId( TUint32 aLinkId )
    {
    iLinkId = aLinkId;
    }

// -----------------------------------------------------------------------------
// CMccMsrpManager::~CMccMsrpManager
// Destructor
// -----------------------------------------------------------------------------
//
CMccMsrpManager::~CMccMsrpManager()
    {
    __SUBCONTROLLER( "CMccMsrpManager::~CMccMsrpManager" )
 
    if (  NULL != iMsrpSession )
        {
        iMsrp->CloseMSRPSession(iMsrpSession);
        }
    if ( NULL != iMsrpObserver )
        {
        delete iMsrpObserver;
        }
    delete iFileName;
    delete iFileType;
    __SUBCONTROLLER( "CMccMsrpManager::~CMccMsrpManager, exit" )
    }

// -----------------------------------------------------------------------------
// CMccMsrpManager::GetLocalIpAddressesL
// -----------------------------------------------------------------------------
//
TDes8& CMccMsrpManager::GetLocalMSRPPath( )
    {
    return iLocalMsrpPath;
    }


// -----------------------------------------------------------------------------
// CMccMsrpManager::SetLocalMSRPPath
// -----------------------------------------------------------------------------
//
void CMccMsrpManager::SetLocalMSRPPath(TDesC8& aMsrpPath )
    {
    iLocalMsrpPath.Zero();
    iLocalMsrpPath = aMsrpPath;
    }
    

CMSRPSession* CMccMsrpManager::GetMsrpSession()
    {
    iRefCountForMSRPSession++;
    return iMsrpSession;
    }

// -----------------------------------------------------------------------------
// CMccMsrpManager::GetLocalIpAddressesL
// -----------------------------------------------------------------------------
//
void CMccMsrpManager::GetLocalIpAddressesL( TMccCreateLink& aClientData )
    {
    __SUBCONTROLLER( "CMccMsrpManager::GetLocalIpAddressesL, entry" )
  
    // Extract the local address and port from msrp path    
    if (iLocalMsrpPath.Length() > 0)
        {
        TUriParser8 parser;
        TInt parseValue = parser.Parse(iLocalMsrpPath); 
            
        TBuf8<50> host = parser.Extract(EUriHost);
        TBuf16<60> hostBuf16;
        hostBuf16.Copy(host);
        TLex hostLex(hostBuf16);
        TInt32 addr;
        hostLex.Val(addr);
        
        TBuf8<10> portBuf = parser.Extract(EUriPort);
        TBuf16<10> portBuf16;
        portBuf16.Copy(portBuf);
        TLex lex(portBuf16);
        TInt Port;
        lex.Val(Port);
        
        TInetAddr addr1(addr, Port );
        aClientData.iLocalAddress = addr1; 
        
        __SUBCONTROLLER_INT1("CMccMsrpManager::GetLocalIpAddressesL: Host no",addr)
        __SUBCONTROLLER_INT1("CMccMsrpManager::GetLocalIpAddressesL: Port no", Port)
        __SUBCONTROLLER( "CMccMsrpManager::GetLocalIpAddressesL, exit" )
        }
    __SUBCONTROLLER( "CMccMsrpManager::GetLocalIpAddressesL, exit" )
    }




// -----------------------------------------------------------------------------
// CMccMsrpManager::CreateSessionL
// Creates RTP session and adds session to SessionArray
// -----------------------------------------------------------------------------
//
CMSRPSession* CMccMsrpManager::CreateMSRPSessionL(MMSRPSessionObserver &aObserver, TUint aIapId)
    {
    __SUBCONTROLLER( "CMccMsrpManager::CreateMSRPSessionL: Entry" )
     iMsrpSession = iMsrp->CreateMSRPSessionL(aObserver, aIapId) ;
    
    __ASSERT_ALWAYS(iMsrpSession != NULL, User::Leave(KErrArgument));
    
    iLocalMsrpPath.Zero();
    iLocalMsrpPath = iMsrpSession->LocalMSRPPath();
    iMsrpSessionOb = &aObserver;
    
    __SUBCONTROLLER( "CMccMsrpManager::CreateMSRPSessionL : Exit" )
    return iMsrpSession;
    }


// -----------------------------------------------------------------------------
// CMccMsrpManager::CreateTransmitStreamL
// Creates a new transmit stream based on given parameters
// -----------------------------------------------------------------------------
//
TUint32 CMccMsrpManager::CreateTransmitStreamL( MDataSink& aSink, 
    const TMccCodecInfo& /*aCodecInfo*/ )
    {
    __SUBCONTROLLER( "CMccMsrpManager::CreateTransmitStreamL" )
    TUint32 streamId( 0 );
    if (!iIsRegisteredForConnOb)
        {
        iMsrpObserver->RegisterConnectionObserver(*this);
        iIsRegisteredForConnOb = ETrue;
        }
    
    CMccMsrpSink& dSink = static_cast<CMccMsrpSink&>( aSink );
        
    TMccMsrpSessionParams sessMsrpParams (iMsrp, iMsrpSession);
    
    dSink.SetSessionParamsL( sessMsrpParams );
    
    return streamId;
    }


// -----------------------------------------------------------------------------
// CMccMsrpManager::CreateReceiveStreamL
// Creates a new receive stream based on given parameters
// -----------------------------------------------------------------------------
//
TUint32 CMccMsrpManager::CreateReceiveStreamL( MDataSource& aSource, 
    const TMccCodecInfo& /*aCodecInfo*/ )
    {
    __SUBCONTROLLER( "CMccMsrpManager::CreateReceiveStreamL" )
    TUint32 streamId( 0 );
    
    CMccMsrpSource& dSource = static_cast<CMccMsrpSource&>( aSource );
    TMccMsrpSessionParams sessParams( iMsrp, iMsrpSession, iMsrpObserver);
    if (!iIsRegisteredForConnOb )
        {
        // register connection observer if not registered yet
        iMsrpObserver->RegisterConnectionObserver(*this);
        }
    if (! iFileShare )
        {
        iMsrpObserver->RegisterDataObserver(dSource);
        }

    return streamId;
    }


// -----------------------------------------------------------------------------
// CMccMsrpManager::SetFileShareAttrbs()
// stores the File Sharing attributes
// -----------------------------------------------------------------------------
//
void CMccMsrpManager::SetFileShareAttrbs(HBufC16* aFileName, 
        TInt aFileSize, 
        HBufC8* aFileType, 
        TBool aFTProgressNotification) 
    {
    __SUBCONTROLLER( "CMccMsrpManager::SetFileShareAttrbs, entry" )
    iFileShare = ETrue; 
    if (NULL != aFileName )
        iFileName = aFileName->Des().Alloc();
    iFileSize = aFileSize;
    if (NULL != aFileType )
        iFileType = aFileType->Des().Alloc();
    iFTProgressNotification = aFTProgressNotification;
    __SUBCONTROLLER( "CMccMsrpManager::SetFileShareAttrbs: file share attrbs set. exit" )
    }


// -----------------------------------------------------------------------------
// CMccMsrpManager::SetRemoteMsrpPathL
// Sets remote MSRP path for MSRP stack to either connect or listen
// -----------------------------------------------------------------------------
//
TInt CMccMsrpManager::SetRemoteMsrpPathL( 
        const TDes8& aRemoteMsrpPath, 
        const TDes8& aConnStatus )
    {
    __SUBCONTROLLER( "CMccMsrpManager::SetRemoteMsrpPathL, entry" )
    //Connection Setup. active , actpass, passive
    // check the remote partys preference for the connection negotiaiton
    TInt conn1 = aConnStatus.Find(_L8("passive"));
    TInt conn2 = aConnStatus.Find(_L8("active"));
    
    if(conn1 != KErrNotFound)
        {
        __SUBCONTROLLER( "CMccMsrpManager::SetRemoteMsrpPathL::Connect, entry" )
        
        // Wait for Synchronization
        User::After(1500000);
        User::LeaveIfError( iMsrpSession->Connect( aRemoteMsrpPath ));
        
        if (iFileShare || NULL!= iFileName)
            {
            __SUBCONTROLLER( "CMccMsrpManager::SetRemoteMsrpPathL File Send is requested" )
            iFileSendRequested = ETrue;
            }
        }
    else
        {
        __SUBCONTROLLER( "CMccMsrpManager::SetRemoteMsrpPathL::Listen, entry" )
        User::LeaveIfError( iMsrpSession->Listen( aRemoteMsrpPath ));
        if (iFileShare || NULL!=iFileName)
            {
            __SUBCONTROLLER( "CMccMsrpManager::SetRemoteMsrpPathL File receive is requested" )
            iFileReceiveRequested = ETrue;
            }
        }
    ifirstTime = EFalse;
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// CMccMsrpManager::SendFileNotification
//  Notifies that the entire file has been sent successfully
// -----------------------------------------------------------------------------
//
void CMccMsrpManager::SendFileNotification(TBool /*aStatus*/) 
    {
    __SUBCONTROLLER( "CMccMsrpManager::SendFileNotification, entry" )
    // reset the value as the request for the file transfer is completed
    iFileSendRequested = EFalse; 
    }
                
 

// -----------------------------------------------------------------------------
// CMccMsrpManager::SendFileNotification
//  Notifies that the entire file has been received successfully
// -----------------------------------------------------------------------------
//
void CMccMsrpManager::ReceiveFileNotification(TBool /*status*/)
    {
    __SUBCONTROLLER( "CMccMsrpManager::ReceiveFileNotification, entry" )
    // reset the value as the request for the file transfer is completed
    iFileReceiveRequested = EFalse; 
    }



// -----------------------------------------------------------------------------
// CMccMsrpManager::SetMsrpObject
//  Sets CMsrp object. not owned
// -----------------------------------------------------------------------------
//
void CMccMsrpManager::SetMsrpObject(CMSRP* aMsrpObject)
    {
    iMsrp = aMsrpObject;
    }
