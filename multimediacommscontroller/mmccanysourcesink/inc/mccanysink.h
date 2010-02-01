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



#ifndef MCCANYSINK_H
#define MCCANYSINK_H

//  INCLUDES
#include <e32std.h>
#include "mccdatasink.h"
#include "mmccevents.h"

// FORWARD DECLARATIONS

/**
*  
*/
class CMccAnySink : public CMccDataSink
    {
    public: // Methods called internally or by the controller

        /**
        * Two-phased constructor.
        */
        static MDataSink* NewSinkL( TUid aImplementationUid, 
                                        const TDesC8& aInitData );
                                        
		void ConstructSinkL ( const TDesC8& aInitData );

		virtual ~CMccAnySink();

    public: // From MDataSink

        TFourCC SinkDataTypeCode( TMediaId /*aMediaId*/ );

        TInt SetSinkDataTypeCode( TFourCC aCodec, 
                                    TMediaId aMedia );

        void BufferEmptiedL( CMMFBuffer* aBuffer );
        
        TBool CanCreateSinkBuffer();
        
        CMMFBuffer* CreateSinkBufferL( TMediaId, 
                                         TBool& aReference );
        
        TInt SinkThreadLogon( MAsyncEventHandler& aEventHandler );
        
        void SinkThreadLogoff();

        void SinkPrimeL();
        
        void SinkPlayL();
        
        void SinkPauseL();
        
        void SinkStopL();

        void EmptyBufferL( CMMFBuffer* aBuffer,
                          MDataSource* aProvider,
                          TMediaId aMediaId );
                          
        void BufferFilledL( CMMFBuffer* aBuffer );

    private:
    
        CMccAnySink();
    
		void SendStreamEventToClient( const TMccEventType& aEventType, 
		                              TInt aError = KErrNone );
        

    private:
		
	    MAsyncEventHandler* iAsyncEventHandler;
	    TBool iPaused;
	    TFourCC iFourCC;

	#ifdef EUNIT_TEST
        friend class UT_CMccAnySink;
    #endif	
        };


#endif 


// End of file

