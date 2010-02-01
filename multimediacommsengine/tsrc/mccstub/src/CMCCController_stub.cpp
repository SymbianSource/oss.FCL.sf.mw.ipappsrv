/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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




#include "CMccController_stub.h"


void CMccControllerStub::OpenL ()
    {
    if ( !Stub() )
        {
        CMccControllerStub* self = new (ELeave) CMccControllerStub();
        CleanupStack::PushL( self );
        User::LeaveIfError( Dll::SetTls( self ) );
        CleanupStack::Pop( self );
        }
    }

void CMccControllerStub::Close ()
    {
    CMccControllerStub* self = Stub();
    delete self;
    Dll::SetTls( NULL );
    }

    
CMccControllerStub* CMccControllerStub::Stub()
    {
    return static_cast<CMccControllerStub*>(Dll::Tls());
    }
    
CMccControllerStub::CMccControllerStub()
    {
    Reset( ETrue );
    }

CMccControllerStub::~CMccControllerStub()
    {
    }
    

void CMccControllerStub::Reset( TBool aAll )
    {
    iActionPtr = 0;
    
    for( TInt i = 0; i < KMce_MM_MaxArray;i++)
        {
        iActions[ i ] = TAction();
        }   
    
    if ( aAll )
        {
        iSessionID = 0;
        iLinkID = KMce_MM_LinkIdBase;
        iStreamID = KMce_MM_StreamIdBase;
        iEndpointID = KMce_MM_EndpointIdBase;
        iRequireSignalling = ETrue;
        iRefCount = 0;
        iStreamExists = -1;

        for( TInt i = 0; i < KMce_MM_MaxArray;i++)
            {
            iCodecs[ i ] = TCodecInfo();
            }   
        for( TInt j = 0; j < KMce_MM_MaxArray;j++)
            {
            iLinks[ j ] = TLink();
            }   
        }
    
    iSetFmtpFailure = EFalse;    
    }
    
void CMccControllerStub::SetCodec( TCodecInfo aCodecInfo )
    {
    TInt i = 0;
    
    while( iCodecs[ i ].iStreamID != KMceNotAssigned &&
           iCodecs[ i ].iStreamID != aCodecInfo.iStreamID )
        {
        if ( i == KMce_MM_MaxArray - 1 )
            {
            User::Panic( _L(""), KErrNotFound );
            }
        else
            {
            i++;
            }
        }
    
    iCodecs[ i ] = aCodecInfo;       
    }

void CMccControllerStub::GetCodec( TCodecInfo& aCodecInfo )
    {
    TInt i = 0;
    
    while( iCodecs[ i ].iStreamID != aCodecInfo.iStreamID )
        {
        if ( i == KMce_MM_MaxArray - 1 )
            {
            User::Panic( _L(""), KErrNotFound );
            }
        else
            {
            i++;
            }
        }
    
    aCodecInfo = iCodecs[ i ];
    
    }

void CMccControllerStub::CreateStream( TLink aLink )
    {
    TInt i = 0;

    
    while( iLinks[ i ].iSessionID != KMceNotAssigned &&
           iLinks[ i ].iSessionID != aLink.iSessionID && 
           iLinks[ i ].iLinkID != aLink.iLinkID )
        {
        if ( i == KMce_MM_MaxArray - 1 )
            {
            User::Panic( _L(""), KErrNotFound );
            }
        else
            {
            i++;
            }
        }
    
    iLinks[ i ].iSessionID = aLink.iSessionID;
    iLinks[ i ].iLinkID = aLink.iLinkID;
    iLinks[ i ].iStreams++;
    
    }

CMccControllerStub::TLink& CMccControllerStub::GetLink( const TLink& aLink )
    {
    TInt i = 0;
    
    while( iLinks[ i ].iSessionID != aLink.iSessionID &&
           iLinks[ i ].iLinkID != aLink.iLinkID )
        {
        if ( i == KMce_MM_MaxArray - 1 )
            {
            User::Panic( _L(""), KErrNotFound );
            }
        else
            {
            i++;
            }
        }
    
    return iLinks[ i ];
    
    }
    
    
TUint32 CMccControllerStub::NextSessionId()
    {
    return iSessionID++;
    }

TUint32 CMccControllerStub::NextLinkId()
    {
    return iLinkID++;
    }

TUint32 CMccControllerStub::NextStreamId()
    {
    return iStreamID++;
    }

TUint32 CMccControllerStub::NextEndpointId()
    {
    if ( !iEndpointID )
        {
        iEndpointID++;
        }
    return iEndpointID++;
    }
    
TBool CMccControllerStub::StreamsExists( TUint32 aSessionId, 
                                         TUint32 aLinkId )
    {
    TLink link;
    link.iSessionID = aSessionId;
    link.iLinkID = aLinkId;
        
    return iStreamExists == -1 ? GetLink( link ).iStreams > 0 : iStreamExists == 1;
    }
    

    
    
TInt CMccControllerStub::ControllerCalled( TMMCActionType aAction,
                                           TUint32 aSessionID,
                                           TUint32 aLinkID,
                                           TUint32 aStreamID,
                                           TUint32 aEndpointID,
                                           TUint32 aLinkType,
                                           TUint32 aStreamType )
                                           
    {
    TAction action;
    action.iAction = aAction;
    action.iSessionID = aSessionID;
    action.iLinkID = aLinkID;
    action.iStreamID = aStreamID;
    action.iEndpointID = aEndpointID;
    action.iLinkType = aLinkType;
    action.iStreamType = aStreamType;
    action.iPaused = KMceNotAssigned;
    action.iEnableRTCP = KMceNotAssigned;
    
    iActions[ iActionPtr++ ] = action;
    iActionPtr = iActionPtr < KMce_MM_MaxArray-1 ? iActionPtr : 0;
    
    if ( aAction == EMccCreateStream )
        {
        TLink link;
        link.iSessionID = aSessionID;
        link.iLinkID = aLinkID;
        CreateStream( link );
        
        }
    else if ( aAction == EMccDeleteStream )
        {
        TLink link;
        link.iSessionID = aSessionID;
        link.iLinkID = aLinkID;
        TLink& l = GetLink( link );
        l.iStreams--;
        }
        
    return iLeaveError;
    
    }

TInt CMccControllerStub::ControllerStreamCtrCalled( TMMCActionType aAction,
                                                    TUint32 aSessionID,
                                                    TUint32 aLinkID,
                                                    TUint32 aStreamID,
                                                    TUint32 aEndpointID,
                                                    TUint32 aPaused,
                                                    TUint32 aEnableRTCP )
                                           
    {
    TAction action;
    action.iAction = aAction;
    action.iSessionID = aSessionID;
    action.iLinkID = aLinkID;
    action.iStreamID = aStreamID;
    action.iEndpointID = aEndpointID;
    action.iLinkType = KMceNotAssigned;
    action.iStreamType = KMceNotAssigned;
    action.iPaused = aPaused;
    action.iEnableRTCP = aEnableRTCP;
    action.iKey = KMceNotAssigned;
    action.iParamSet = KMceNotAssigned;
    
    iActions[ iActionPtr++ ] = action;

    iActionPtr = iActionPtr < KMce_MM_MaxArray-1 ? iActionPtr : 0;

    if ( aAction == EMccCreateStream )
        {
        TLink link;
        link.iSessionID = aSessionID;
        link.iLinkID = aLinkID;
        CreateStream( link );
        
        }
    else if ( aAction == EMccDeleteStream )
        {
        TLink link;
        link.iSessionID = aSessionID;
        link.iLinkID = aLinkID;
        TLink& l = GetLink( link );
        l.iStreams--;
        }
        
    
    return iLeaveError;
    
    }


TInt CMccControllerStub::ControllerStreamSettings(  TUint32 aSessionID,
                                                    TUint32 aLinkID,
                                                    TUint32 aStreamID,
                                                    TUint32 aEndpointID,
                                                    TUint32 aKey,
                                                    TUint32 aSet )
    {
    TAction action;
    action.iAction = EMccStreamSettings;
    action.iSessionID = aSessionID;
    action.iLinkID = aLinkID;
    action.iStreamID = aStreamID;
    action.iEndpointID = aEndpointID;
    action.iLinkType = KMceNotAssigned;
    action.iStreamType = KMceNotAssigned;
    action.iPaused = KMceNotAssigned;
    action.iEnableRTCP = KMceNotAssigned;
    action.iKey = aKey;
    action.iParamSet = aSet;
    
    iActions[ iActionPtr++ ] = action;

    iActionPtr = iActionPtr < KMce_MM_MaxArray-1 ? iActionPtr : 0;

    return iLeaveError;
    
    
    }

TInt CMccControllerStub::ControllerSignal(  TUint32 aSessionID,
                                            TUint32 aLinkID,
                                            TUint32 aStreamID,
                                            TUint32 aEndpointID,
                                            TUint32 aEventType )
    {
    TAction action;
    action.iAction = EMccSendMediaSignal;
    action.iSessionID = aSessionID;
    action.iLinkID = aLinkID;
    action.iStreamID = aStreamID;
    action.iEndpointID = aEndpointID;
    action.iLinkType = KMceNotAssigned;
    action.iStreamType = KMceNotAssigned;
    action.iPaused = KMceNotAssigned;
    action.iEnableRTCP = KMceNotAssigned;
    action.iKey = KMceNotAssigned;
    action.iParamSet = KMceNotAssigned;
    action.iEventType = aEventType;
    
    iActions[ iActionPtr++ ] = action;

    iActionPtr = iActionPtr < KMce_MM_MaxArray-1 ? iActionPtr : 0;

    return iLeaveError;
    
    
    }




    
//end of file
