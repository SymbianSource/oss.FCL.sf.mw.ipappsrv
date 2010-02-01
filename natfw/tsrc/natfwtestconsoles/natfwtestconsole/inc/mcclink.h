/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef __MCCLINK_H__
#define __MCCLINK_H__

#include <e32base.h>
#include <in_sock.h>

class CMccSession;

class CMccLink : public CBase
    {
public:
    static CMccLink* NewL( TInt aLinkType, TUint aIapId, CMccSession* aSession );
    
    void CreateLinkL( TInt32& aLinkId, TInetAddr& aDestAddr, TRequestStatus& aStatus );
    
    void CreateLinkL( TInt32& aLinkId, TInetAddr& aDestAddr, TUint aRtcpPort,
        TRequestStatus& aStatus );
    
    void SetRemoteAddressL( TInetAddr& aDestAddr );
    
    void SetRemoteAddressL( TInetAddr& aDestAddr, TUint aRtcpPort );
    
    TUint32 LinkId();
    
    TInt Close();
    
    virtual ~CMccLink();
    
private:
    void ConstructL();
    
    CMccLink( TInt aLinkType, TUint aIapId, CMccSession* aSession );
    
private: //data
    TInt iLinkType;
    TUint iIapId;
    CMccSession* iSession; // not own
    TUint32 iLinkId;
    
    };
#endif
