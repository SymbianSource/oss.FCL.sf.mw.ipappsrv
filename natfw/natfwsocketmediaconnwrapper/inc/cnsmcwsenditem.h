/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Constructs a send buffer.
*
*/




#ifndef __CNSMCWSENDITEM_H
#define __CNSMCWSENDITEM_H

#include <e32base.h>

/**
 *  Send item
 *  Holds on a packet for buffering if socket is busy
 *
 *  @lib socketmediaconnwrapper.dll
 *  @since S60 v3.2
 */
NONSHARABLE_CLASS( CNsmcwSendItem ): public CBase
    {

public:

    /**
     * Two-phased constructor.
     * @param aPacket Data for send item.
     */
    static CNsmcwSendItem* NewL( const TDesC8& aPacket );
    
    /**
     * Two-phased constructor.
     * @param aPacket Data for send item.
     */
    static CNsmcwSendItem* NewLC( const TDesC8& aPacket );

    /**
    * Destructor.
    */
    virtual ~CNsmcwSendItem();

    /**
     * Returns data from send buffer.
     *
     * @since S60 v3.2
     * @return TDesC8& data from send buffer.
     */
    const TDesC8& GetData() const;

public:// Data

    static const TInt iOffset;

private:

    CNsmcwSendItem();

    void ConstructL( const TDesC8& aData );
    
private:

    /**
     * Data packet
     * Own.
     */
    HBufC8* iPacket;

    /**
     * Link
     */
    TSglQueLink iLink;
    };

#endif  // __CNSMCWSENDITEM_H
