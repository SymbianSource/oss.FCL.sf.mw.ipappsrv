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




#ifndef MCCGENERICDATASOURCE_H
#define MCCGENERICDATASOURCE_H

//  INCLUDES
#include "mccdatasource.h"

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION

class CMccGenericDataSource : 
    public CMccDataSource 
    {
    public:
    
        CMccGenericDataSource ( TUid aType ) :
            CMccDataSource( aType ),
            iCanCreateSourceBuffer( ETrue ),
            iFourCC( KMccFourCCIdH263 ),
            iSourceBuffer( NULL )
            { };
    
        inline virtual ~CMccGenericDataSource() { }
        
    public: // From MDataSource

        /**
        * From MDataSource. Returns the data type as a fourCC code of the data source.
        *
        * @since    Series 60 3.0
        * @param    "aMediaId"
        *           This identifies the type of media eg audio or video and the stream id.
        *           Only KUidMediaTypeAudio -mediatype is supported.
        * @return    The fourCC code identifying the source datatype for the specified aMediaId.
        */
        TFourCC SourceDataTypeCode( TMediaId /*aMediaId*/ )
            { return iFourCC; }

        /**
        * From MDataSource. Method called by a MDataSink to request the data source to fill aBuffer with data.
        *
        * Datasource gets packet from jitterbuffer and forwards it to the payloadformatdecoder
        * for further processing. Datasource works synchronously; consumers BufferFilledL
        * is called when RTP packet is got succesfully from jitterbuffer.
        * 
        * @since    Series 60 3.0
        * @param    "aBuffer"
        *           The buffer that needs filling with data
        *
        * @param    "aConsumer" 
        *           The data sink that consumes the data. The data source needs this to make the BufferFilledL
        *           callback on aConsumer when the data source has completed filling the aBuffer.
        *
        * @param    "aMediaId"
        *           This identifies the type of media eg audio or video and the stream id.
        *           This parameter is required in cases where the source can supply data
        *           of more than one media type and/or multiple strams of data eg a multimedia file.
        * @return   None
        */
        void FillBufferL( CMMFBuffer* /*aBuffer*/, MDataSink* /*aConsumer*/, TMediaId /*aMediaId*/ ) { };
                          
        /**
        * From MDataSource. Not used. Leaves with KErrNotSupported.
        * 
        * @since    Series 60 3.0
        * @param    "aBuffer"
        *            The buffer that has been emptied by a data sink and is now available for reuse
        * @return   None
        */
        void BufferEmptiedL( CMMFBuffer* /*aBuffer*/ ) { };
        
        /**
        * From MDataSource. Method to indicate whether the data source can create a buffer.
        *
        * @since    Series 60 3.0
        * 
        * @return    EFalse
        */
        TBool CanCreateSourceBuffer() { return iCanCreateSourceBuffer; };
        
        /**
        * From MDataSource. Not used. Leaves with KErrNotSupported.
        *
        * @since    Series 60 3.0
        * @param    "aMediaId"
        *           This identifies the type of media eg audio or video and the stream id.
        *           This parameter is required in cases where the source can supply data
        *           of more than one media type and/or multiple strams of data eg a multimedia file.
        *
        * @param    "aReference"
        *           This must be written to by the method to indicate whether the created buffer is
        *           a 'reference' buffer.  A 'reference' buffer is a buffer that is owned by the source
        *           and should be used in preference to the sink buffer provided the sink buffer
        *           is also not a reference buffer.
        * .  
        * @return   The created buffer
        */
        CMMFBuffer* CreateSourceBufferL( TMediaId /*aMediaId*/, TBool& /*aReference*/ )
            { return iSourceBuffer; };
        
        /**
        * Method to perform any source construction dependant on the source construction
        * initialisation data aInitData
        *
        * This is a pure virtual function that a derrived data source must implement
        *
        * @param    "aInitData"
        *            source specific initialisation data required for source construction
        */
    		void ConstructSourceL ( const TDesC8& /*aInitData*/ ) { };
    
    public: // From MMccDataSource
    
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
        
        TBool iCanCreateSourceBuffer;
        
        TFourCC iFourCC;
    
        CMMFBuffer* iSourceBuffer;
        
    };
    
#endif      // MCCGENERICDATASOURCE_H   
            
// End of File
