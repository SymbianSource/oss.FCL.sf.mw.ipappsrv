/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Class is used for creating media codecs.
*
*/




#ifndef MMCCCODECINFORMATIONFACTORY_H
#define MMCCCODECINFORMATIONFACTORY_H

// INCLUDES
#include <e32base.h>

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class CMccCodecInformation;
class TFourCC;

// CLASS DECLARATION
/**
*  Factory class returning instances of various codec information classes.
*
*  @lib mmccinterface.dll
*/
class CMccCodecInformationFactory : public CBase
    {
public:  // Constructors and destructor

    /**
    * Two-phased constructor.
    */
    IMPORT_C static CMccCodecInformationFactory* NewL( );
    
    /**
    * Destructor.
    */
    virtual ~CMccCodecInformationFactory();

public: 
    /**
    * Factory method to create instances of various CodecInformation classes.
    * @param    aSdpName    Codec information is created base on codec's SDP name.
    * @return   Codec information. Leaves with KErrNotSupported if codec is not supported.
    */
    IMPORT_C CMccCodecInformation* CreateCodecInformationL( const TDesC8& aSdpName );
    
    /**
    * Factory method to create instances of various CodecInformation classes.
    * @param    aFourCC    Codec information is created base on codec's FOURCC.
    * @return   Codec information. Leaves with KErrNotSupported if codec is not supported.
    */
    IMPORT_C CMccCodecInformation* CreateCodecInformationL( TFourCC aFourCC );

private:

    /**
    * C++ default constructor.
    */
    CMccCodecInformationFactory();

    /**
    * By default Symbian 2nd phase constructor is private.
    */
    void ConstructL( );

};


#endif      // MMCCCODECINFORMATIONFACTORY_H

// End of File