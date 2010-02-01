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




#ifndef TURNUDPSETTINGS_H__
#define TURNUDPSETTINGS_H__

#include <e32base.h>

#include "generalsettings.h" 


class CTurnUdpSettings : public CGeneralSettings
{
public:
    static CTurnUdpSettings* NewL( const TDesC8& aDomain, TUint aIapId );
    static CTurnUdpSettings* NewLC( const TDesC8& aDomain, TUint aIapId );

    ~CTurnUdpSettings( );
    
public: // from CSettings
    
    void CreateSettingsL();

private:
    
    void ConstructL( const TDesC8& aDomain );
    CTurnUdpSettings( TUint aIapId );
           
private: //DATA
    
    TUint32 iDomainKey;
};

#endif //TURNUDPSETTINGS_H__