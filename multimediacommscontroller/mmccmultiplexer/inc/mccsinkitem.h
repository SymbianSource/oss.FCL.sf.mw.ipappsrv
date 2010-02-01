/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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
*  Interface   : 
*
*/




#ifndef MCCSINKITEM_H
#define MCCSINKITEM_H

// INCLUDES
#include <e32base.h>
#include "mmccevents.h"
#include "mccmultiplexer.h"

// FORWARD DECLARATIONS
class CMMFBuffer;
class MDataSink;
class TMMFEvent;
class CPayloadFormatRead;
class TRtpRecvHeader;
class MAsyncEventHandler;
class CMMFDataBuffer;

// CLASS DECLARATION

/**
*
*/ 
class CMccSinkItem : public CBase
    {
    public: // Constructors and destructor
    
        static CMccSinkItem* NewLC( MDataSink* aSink, 
                                    TBool aSinkIsDecoder,
                                    TUid aMediaType,
                                    TBool aPassAllBuffersSink );
    
        virtual ~CMccSinkItem();
    
    public: // New functions
    
        void BufferFilledL( CMMFBuffer* aBuffer, 
                            const TRtpRecvHeader* aHeaderInfo,
                            TBool aPrimary,
                            TUid aMediaType );
                            
        void FillBufferRequestL( CMMFBuffer* aBuffer );
                            
        TBool Match( MDataSink* aSink );
        
        TInt SourceThreadLogon( MAsyncEventHandler& aEventHandler );
        
        TInt SendEventToClient( const TMMFEvent& aEvent );
        
        TUid MediaType() const;
        
        void SetSourceMode( TBool aPassAllRequestsSource );
        
        TBool PassAllRequestsToSource() const;
        
        TUid ResolveMediaType( CMMFBuffer* aBuffer ) const;

        void RegisterPayloadTypesL( const RArray<TUint>& aPayloadTypes );
        
        const RArray<TUint>& RegisteredPayloadTypes() const;
        
        CMMFBuffer* RequestedBuffer();
        
        MAsyncEventHandler* EventHandler();
        
        void SetMultiplexerState( CMccMultiplexer::TMccMultiplexerState aState );
        
        CMccMultiplexer::TMccMultiplexerState MultiplexerState() const;
   
    private: // Private construction
    
        CMccSinkItem( TUid aMediaType, TBool aPassAllBuffersSink );
        
        void ConstructL( MDataSink* aSink, TBool aSinkIsDecoder );
        
    private:
    
        MDataSink* Sink();
        
        void CopyBufferL( CMMFBuffer* aDesBuffer, CMMFBuffer* aOrigBuffer );
        
        TBool IsInternalDecoder( TUid aDecoderUid ) const;
        
        void StoreBufferL( CMMFBuffer* aBuffer );
        
        CMMFBuffer* GetStoredBuffer();
        
        TBool HandleBadDataL( CMMFDataBuffer* aBuffer );
        
        TBool HandleStreamStateChangeEvent( TMccEvent& aEvent );
        
    private:
    
        MDataSink* iSink;
        
        CPayloadFormatRead* iDecoder;
        
        CMMFBuffer* iRequestedBuffer;
        
        RPointerArray<CMMFBuffer> iStoredBuffers;
        
        MAsyncEventHandler* iEventHandler;
        
        TUid iMediaType;
        
        TBool iPassAllBuffersSink;
        
        TBool iPassAllRequestsSource;
        
        TMccEventType iReportedEvent;

        RArray<TUint> iPayloadsToAccept;

        TInt iBadDataCount;
        
        CMccMultiplexer::TMccMultiplexerState iState;
        
    private:
        #ifdef EUNIT_TEST
            friend class UT_CMccSinkItem;
            friend class UT_CMccMultiplexer;
        #endif
    };

#endif  // MCCSINKITEM_H

// End of File
