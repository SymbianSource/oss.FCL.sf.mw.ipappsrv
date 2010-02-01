/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    ?Description
*
*/




#include <e32base.h>

#include "natfwunsafmessage.h"

#ifndef C_UNSAFMESSAGEENCODER_H
#define C_UNSAFMESSAGEENCODER_H

/**
 *  ?one_line_short_description
 *  ?more_complete_description
 *
 *  @code
 *   ?good_class_usage_example(s)
 *  @endcode
 *
 *  @lib ?library
 *  @since S60 ?S60_version *** for example, S60 v3.0
 */
class CUnsafMessageEncoder : public CBase
    {

public:

    static CUnsafMessageEncoder* NewL();
    static CUnsafMessageEncoder* NewLC();

    /**
     * Two-phased constructor.
     * @param ?arg1 ?description
     * @param ?arg2 ?description
     //*/
    //static CUnsafMessageEncoder* NewL(?type1 ?arg1, ?type2 ?arg2);
    

    /**
    * Destructor.
    */
    virtual ~CUnsafMessageEncoder();

    /**
     * ?description
     *
     * @since S60 ?S60_version
     * @param ?arg1 ?description
     * @param ?arg2 ?description
     * @return ?description
     */
     HBufC8* EncodeMessageL( const CNATFWUNSAFMessage& aRequest,
                             const TBool aUseSharedSecret = ETrue,
                             const TBool aAddFingerPrint = EFalse );

private:

    CUnsafMessageEncoder();

    void ConstructL();

private: // data

    };

#endif // C_UNSAFMESSAGEENCODER_H
