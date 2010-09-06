/*
* Copyright (c) 2004-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Provides a comfort noise generator class
*
*/


#ifndef MCCMSRPSINK_H
#define MCCMSRPSINK_H

//  INCLUDES
#include <e32std.h>
#include "mccdatasink.h"
#include "mmccevents.h"
#include "mccmsrpdatasender.h"
#include "mccmsrpinterface.h"

class CMccMsrpDataSender;

// FORWARD DECLARATIONS

/**
*  
*/
class CMccMsrpSink : public CMccDataSink, public TMccMsrpSessionParams
    {
    public: // Methods called internally or by the controller

        /**
        * Two-phased constructor.
        */
        static MDataSink* NewSinkL( TUid aImplementationUid, 
                                        const TDesC8& aInitData );
                                        
		void ConstructSinkL ( const TDesC8& aInitData );

		virtual ~CMccMsrpSink();

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
        
        void SetSessionParamsL( const TMccMsrpSessionParams& aParams );

    private:
    
        CMccMsrpSink();
    
		void SendStreamEventToClient( const TMccEventType& aEventType, 
		                              TInt aError = KErrNone );
        

    private:
		
	    MAsyncEventHandler* iAsyncEventHandler;
	    TBool iPaused;
	    TFourCC iFourCC;
	    
	    CMccMsrpDataSender* iDataSender;

	#ifdef EUNIT_TEST
        friend class UT_CMccMsrpSink;
    #endif	
        };


#endif 


// End of file

