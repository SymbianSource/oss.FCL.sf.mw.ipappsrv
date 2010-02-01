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
* Description:    Inline methods implementations
*
*/





// ---------------------------------------------------------------------------
// TAmrFrame::TAmrFrame
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
inline TAmrFrame::TAmrFrame()    
    :
    iFrameType( EAmrFrame5_15 ), 
    iFrameQualityInd( 1 ),
    iCrc( 0 ), 
    iChannel( 0 ),
    iFrameData( NULL ),
    iBitIndex( 0 ) 
    {
    }
  

            
   
    
    
    

