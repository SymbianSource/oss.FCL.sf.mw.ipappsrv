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




#ifndef CMceSipCONNECTION_H
#define CMceSipCONNECTION_H

#include <e32base.h>

//  FORWARD DECLARATION
class CSIPProfile;

// CLASS DECLARATION

//CMceSipConnection stub
class CMceSipConnection: public CBase
    {
public:
    
    static CMceSipConnection* NewL( CSIPProfile& aProfile );
    
    CSIPProfile& Profile() const;
    
    ~CMceSipConnection();
    
private: 
    
    void ConstructL();
    
    CMceSipConnection( CSIPProfile& aProfile );
    
private: // data
    
    CSIPProfile& iProfile;
    
    };

#endif

// End of File
