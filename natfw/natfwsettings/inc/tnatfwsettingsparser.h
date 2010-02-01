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





#ifndef T_NATFWSETTINGSPARSER_H
#define T_NATFWSETTINGSPARSER_H


#include <e32std.h>

class CDesC8Array;


/**
 *  Class handles natsettings data parsing operations
 *
 *  @lib natsettings.lib
 *  @since S60 v3.2
 */
class TNATFWSettingsParser
    {

public:

    /** 
     * Parse address and port from given descriptor.
     * Port is set to 0 and aData is returned, if port was not found.
     * Parser accept form "address:port"
     *
     * @since S60 3.2
     * @param aData Target descriptor
     * @param aPort Parsed port
     * @return Address
     */ 
    static HBufC8* ParseAddressL( const TDesC8& aData, TInt& aPort );

    /** 
     * Parse address and port from given descriptor.
     * Port is set to 0 and aData is returned, if port was not found.
     * Parser accept form "address:port".
     *
     * @since S60 3.2
     * @param aData Target descriptor
     *        It's form should be [ address:port ]
     * @param aPort Parsed port
     * @return Address. The pointer is also put onto the cleanup stack
     */ 
    static HBufC8* ParseAddressLC( const TDesC8& aData, TInt& aPort );

    /** 
     * Parse data to descriptor array from the given descriptor.
     * Parser accept form "protocol1,protocol2,...".
     *
     * @since S60 3.2
     * @param aData Target descriptor    
     * @return Descriptor array, ownership is transferred
     */
    static CDesC8Array* ParseDataToArrayL( const TDesC8& aData ); 

    };


#endif // T_NATFWSETTINGSPARSER_H
