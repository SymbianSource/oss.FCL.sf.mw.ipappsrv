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




#ifndef __WRONGSERVERSETTINGS_H__
#define __WRONGSERVERSETTINGS_H__

#include <e32base.h>
#include <mnatfwserversettings.h>

#include "settings.h" 

class CWrongServerSettings : public CSettings
{
public:
    static CWrongServerSettings* NewL( const TDesC8& aDomain );
    static CWrongServerSettings* NewLC( const TDesC8& aDomain );

    ~CWrongServerSettings( );
    
protected: // from CSettings
    void CreateSettingsL();

private:
    void ConstructL( const TDesC8& aDomain );
    CWrongServerSettings();
           
private: //DATA
    HBufC8* iDomain;
    TUint32 iDomainKey;
    TUint32 iStunServerAddressKey;
    TUint32 iTurnServerAddressKey;
    
    HBufC8* iStoredStunAddress;
    HBufC8* iStoredTurnAddress;
};

#endif //__WRONGSERVERSETTINGS_H__