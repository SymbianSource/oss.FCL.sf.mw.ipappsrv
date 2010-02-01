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
* Description:    
*
*/




#ifndef MCCGENERICDATASINK_H
#define MCCGENERICDATASINK_H

//  INCLUDES
#include "mccdatasink.h"

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*
*/
class CMccGenericDataSink : 
    public CMccDataSink 
    {
    public:
    
        CMccGenericDataSink ( TUid aType ) :
            CMccDataSink( aType ),
            iCanCreateSinkBuffer( ETrue ),
            iFourCC( KMccFourCCIdH263 ),
            iSinkBuffer( NULL )
            { };
    
        virtual ~CMccGenericDataSink() { }
        
    public: // From MDataSink
    
        /**
        * From MDataSink pure virtual function that must be implemented.
        *                 Returns the data type as a fourCC code of the data sink.
        * @since Series 60 3.0
        * @param "aMediaId" This identifies the type of media eg audio and the stream id.
        *                    This parameter is required in cases where the sink can accept data
        *                    of more than one media type.
        * @return the datatype code ( codec ).
        */
        TFourCC SinkDataTypeCode( TMediaId /*aMediaId*/ ) { return iFourCC; };

        /**
        * From MDataSink. Pure virtual function that must be implemented.
        *                  Out of usage. Leave KErrNotSupported.
        * @since Series 60 3.0
        * @param "aBuffer" Out of usage
        * @param "aSupplier" Out of usage
        * @param "aMediaId" Out of usage
        * @return void
        */
        void EmptyBufferL( CMMFBuffer* /*aBuffer*/, 
                           MDataSource* /*aSupplier*/, 
                           TMediaId /*aMediaId*/ ) { };

        /**
        * From MDataSink pure virtual function that must be implemented. 
        *                 Out of usage. Function leaves with KErrNotSupported.  
        * @since    Series 60 3.0
        * @param    "aCodec" Out of usage
        * @param    aMedia    Out of usage
        * @return    void
        */
        void BufferFilledL( CMMFBuffer* /*aBuffer*/ ) { };

        /**
        * From MDataSink pure virtual function that must be implemented.
        * Method to indicate whether the data sink can create a buffer.
        * @deprecated
        *
        * @since Series 60 3.0
        * @param "aBuffer" Out of usage
        * @return False
        */
        TBool CanCreateSinkBuffer()
          { return iCanCreateSinkBuffer; };

        /**
        * From MDataSink pure virtual function that must be implemented.
        * Out of usage. Function leaves with KErrNotSupported.
        * @deprecated
        *
        * @since Series 60 3.0
        * @param "aMediaId" Out of usage.
        * @param "aReference" Out of useage.
        * @return 
        */
        CMMFBuffer* CreateSinkBufferL( TMediaId /*aMediaId*/, 
                                       TBool& /*aReference*/ )
            { return iSinkBuffer; };
                                  
        /**
        * Method to perform any sink construction dependant on the sink construction
        * initialisation data aInitData
        *
        * This is a pure virtual function that a derrived data sink must implement
        *
        * @param    "aInitData"
        *            Sink specific initialisation data required for sink construction
        */
        void ConstructSinkL( const TDesC8& /*aInitData*/ ) { };
        
    public: // From MMccDataSink
    
        virtual void GetParameterL( TUint32 aParam, TDes8& aVal )
            {
            if ( aParam == iParam )
                {
                aVal = iVal;
                }
            }
        
        virtual void SetParameterL( TUint32 aParam, const TDesC8& aVal )
            {
            iParam = aParam;
            iVal = aVal;
            }
        
    protected:
    
        TUint32 iParam;
        
        TBuf8<100> iVal;
    
        TBool iCanCreateSinkBuffer;
        
        TFourCC iFourCC;

        CMMFBuffer* iSinkBuffer;
    };
    

#endif      // MCCGENERICDATASINK_H   
            
// End of File
