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
*
*/




//  CLASS HEADER
#include "ut_cfcpluginengine.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <SDPDocument.h>
#include <SDPFmtAttributefield.h>
#include <SDPMediaField.h>


//  INTERNAL INCLUDES
#include "fcpluginengine.h"
#include "fcplugininfo.h"

const TUint KTBCPLocalPort = 2000;
const TUint KTBCPLocalPort2 = 2008;


_LIT8(KTBCP, "TBCPTest");
_LIT8(KSession,"Test" );
_LIT8(Kfmt, "queuing=1; tb_priority=2; timestamp=1");
_LIT8(KHello, "Hello");
_LIT8(KSDPMessage, "v=0\r\n\
o=mhandley 2890844526 2890842807 IN IP4 126.16.64.4\r\n\
s=SDP	Seminar\n\
i=A Seminar on the session description protocol\r\n\
u=http://www.cs.ucl.ac.uk/staff/M.Handley/sdp.03.ps\r\n\
e=mjh@isi.edu (Mark Handley)\r\n\
e=j.doe@example.com (Jane Doe)\r\n\
p=+44-171-380-7777\r\n\
p=+1 617 555 6011\r\n\
c=IN IP4 224.2.17.12/127\r\n\
b=X-YZ:128\r\n\
t=0 0\r\n\
r=604800 3600 0 90000\r\n\
r=604800 3600 0 90000\r\n\
t=2873397496 2873404696\r\n\
z=2882844526 -1h 2898848070 0\r\n\
k=uri:http://www.cs.ucl.ac.uk/staff/M.Handley/sdp.03.ps\r\n\
a=recvonly\r\n\
a=orient:landscape\r\n\
m=application 32416/234 udp wb\r\n\
a=orient:portrait\r\n\
m=audio 49170 RTP/AVP 0\r\n\
i=A Seminar on the session description protocol\r\n\
c=IN IP4 224.2.17.12/127\r\n\
c=IN IP4 224.2.17.12/127\r\n\
b=X-YZ:128\r\n\
b=X-YZ:128\r\n\
b=X-YZ:128\r\n\
k=uri:http://www.cs.ucl.ac.uk/staff/M.Handley/sdp.03.ps\r\n\
a=orient:portrait\r\n\
a=orient:portrait\r\n\
a=orient:portrait\r\n\
a=orient:portrait\r\n\
a=orient:portrait\r\n\
m=video 51372 RTP/AVP 31\r\n");



_LIT8(KFmtAttribute,"a=fmtp:TBCPTest format parameters\r\n");

_LIT8(KFmtAttribute2,"a=fmtp:TestPlugIn format parameters\r\n");



_LIT8(KSDPMessage2, "v=0\r\n\
o=mhandley 2890844526 2890842807 IN IP4 126.16.64.4\r\n\
s=SDP	Seminar\n\
i=A Seminar on the session description protocol\r\n\
u=http://www.cs.ucl.ac.uk/staff/M.Handley/sdp.03.ps\r\n\
e=mjh@isi.edu (Mark Handley)\r\n\
e=j.doe@example.com (Jane Doe)\r\n\
p=+44-171-380-7777\r\n\
p=+1 617 555 6011\r\n\
c=IN IP4 224.2.17.12/127\r\n\
b=X-YZ:128\r\n\
t=0 0\r\n\
r=604800 3600 0 90000\r\n\
r=604800 3600 0 90000\r\n\
t=2873397496 2873404696\r\n\
z=2882844526 -1h 2898848070 0\r\n\
k=uri:http://www.cs.ucl.ac.uk/staff/M.Handley/sdp.03.ps\r\n\
a=recvonly\r\n\
a=orient:landscape\r\n\
m=application 32416/234 udp TBCPTest\r\n\
a=fmtp:TBCPTest format parameters\r\n\
m=audio 49170 RTP/AVP 0\r\n\
i=A Seminar on the session description protocol\r\n\
c=IN IP4 224.2.17.12/127\r\n\
c=IN IP4 224.2.17.12/127\r\n\
b=X-YZ:128\r\n\
b=X-YZ:128\r\n\
b=X-YZ:128\r\n\
k=uri:http://www.cs.ucl.ac.uk/staff/M.Handley/sdp.03.ps\r\n\
a=orient:portrait\r\n\
a=orient:portrait\r\n\
a=orient:portrait\r\n\
a=orient:portrait\r\n\
a=orient:portrait\r\n\
m=video 51372 RTP/AVP 31\r\n");

// CONSTRUCTION
UT_CFCPlugInEngine* UT_CFCPlugInEngine::NewL()
    {
    UT_CFCPlugInEngine* self = UT_CFCPlugInEngine::NewLC(); 
    CleanupStack::Pop( self );

    return self;
    }

UT_CFCPlugInEngine* UT_CFCPlugInEngine::NewLC()
    {
    UT_CFCPlugInEngine* self = new( ELeave ) UT_CFCPlugInEngine();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CFCPlugInEngine::~UT_CFCPlugInEngine()
    {
    }

// Default constructor
UT_CFCPlugInEngine::UT_CFCPlugInEngine()
    {
    }

// Second phase construct
void UT_CFCPlugInEngine::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CFCPlugInEngine::SetupL(  )
    {
	SdpCodecStringPool::OpenL();
	iEngine = CFCPlugInEngine::NewL(*this);
	iErr=0;
    } 

void UT_CFCPlugInEngine::Teardown(  )
    {
    SdpCodecStringPool::Close(); 
    delete iEngine;
	}
	

void UT_CFCPlugInEngine::UT_CFCPlugInEngine_NewLL(  )
    {
    CFCPlugInEngine *temp=CFCPlugInEngine::NewL(*this);
    CleanupStack::PushL(temp);
    CDesC8Array* protocols = temp->SupportedProtocolL();
    CleanupStack::PushL(protocols);
    TInt pos=0;
    TInt count= protocols->Count();
    

    protocols->Find(KTBCP, pos);
    
    _LIT8(KOwnDesc, "pos Value = intvalue=%d" );
	EUNIT_PRINT(KOwnDesc, pos ); 
	EUNIT_ASSERT(count>0 );
	
    EUNIT_ASSERT(pos>=0);
    protocols->Reset();
    CleanupStack::PopAndDestroy(protocols); 
    CleanupStack::PopAndDestroy(temp);
    }

void UT_CFCPlugInEngine::UT_CFCPlugInEngine_NewLCL(  )
    {
    CFCPlugInEngine *temp=CFCPlugInEngine::NewLC(*this);
    
    CFCInterface* plugin  = ( CFCInterface* ) temp->PlugInByNameL(KTBCP) ;
    EUNIT_ASSERT(plugin->Name()==KTBCP);
    CleanupStack::PopAndDestroy(temp);
    delete plugin;
    }

void UT_CFCPlugInEngine::UT_CFCPlugInEngine_FCPlugInIterL(  )
    {
    TInt count = iEngine->FCPlugInIter().Count();
    EUNIT_ASSERT(count>0 );
    CFCPlugInInfo* plugin = NULL;
	plugin = iEngine->FCPlugInIter().First();
	while (plugin)
		{
		if (KTBCP().CompareF(plugin->Name()) == KErrNone)
			{
			plugin = iEngine->FCPlugInIter().Current();
			if (plugin->Name() == KTBCP)
				EUNIT_ASSERT(plugin->Name() == KTBCP);
			}
		plugin = iEngine->FCPlugInIter().Next();
		}
	plugin = iEngine->FCPlugInIter().First();	
	if(!plugin)
		{
		plugin=iEngine->FCPlugInIter().Previous();
		plugin = iEngine->FCPlugInIter().First();
		while (plugin)
			{
			if (KTBCP().CompareF(plugin->Name()) == KErrNone)
				{
				plugin = iEngine->FCPlugInIter().Current();
				if (plugin->Name() == KTBCP)
					EUNIT_ASSERT(plugin->Name() == KTBCP);
				}
			plugin = iEngine->FCPlugInIter().Next();
			}
		}
	plugin = iEngine->FCPlugInIter().First();
   	if(!plugin)
   		{
   		plugin=iEngine->FCPlugInIter().Previous();
   		plugin = iEngine->FCPlugInIter().First();
		while (plugin)
			{
			if (KTBCP().CompareF(plugin->Name()) == KErrNone)
				{
				if (plugin->Name() == KTBCP)
					EUNIT_ASSERT(plugin->Name() == KTBCP);
				}
			plugin = iEngine->FCPlugInIter().Last();
			}
   		}
   	RPointerArray<CFCPlugInInfo>* aMechanisms=
   	    new (ELeave) RPointerArray<CFCPlugInInfo>(1);
   	TFCPlugInIter tempiter= TFCPlugInIter(*aMechanisms);
   	EUNIT_ASSERT(!tempiter.Previous());
   	EUNIT_ASSERT(!tempiter.First());
   	EUNIT_ASSERT(!tempiter.Last());
   	EUNIT_ASSERT(!tempiter.Current());	
   	CleanupStack::PushL(aMechanisms);
   	CleanupStack::PopAndDestroy(aMechanisms);
   
    }

void UT_CFCPlugInEngine::UT_CFCPlugInEngine_ListMechanismImplementationsLL(  )
    {
    iEngine->ListMechanismImplementationsL();
    TInt count = iEngine->FCPlugInIter().Count();
    EUNIT_ASSERT(count>0 );
    }

void UT_CFCPlugInEngine::UT_CFCPlugInEngine_SupportedProtocolLL(  )
    {
    CDesC8Array* protocols = iEngine->SupportedProtocolL();
    CleanupStack::PushL(protocols);
    TInt pos=0;
    TInt count= protocols->Count();
    

    protocols->Find(KTBCP, pos);
    
   
	
	EUNIT_ASSERT(count>0 );
	
    EUNIT_ASSERT(pos>=0);
    protocols->Reset();
    CleanupStack::PopAndDestroy(protocols); 
    
    }

void UT_CFCPlugInEngine::UT_CFCPlugInEngine_PlugInByNameLL(  )
    {
    
    CFCInterface* test= (CFCInterface*)iEngine->PlugInByNameL(KTBCP) ;
    EUNIT_ASSERT(test->Name()==KTBCP);
    delete test;
    
    CFCInterface* fuu = ( CFCInterface* )iEngine->PlugInByNameL( KHello ) ;
    EUNIT_ASSERT( !fuu );
    }

void UT_CFCPlugInEngine::UT_CFCPlugInEngine_CreateNewSessionLL(  )
    {
    TInt KLoops = 10;
    for ( TInt i = 0; i< KLoops; i++ )
        {
        TUint port = KTBCPLocalPort + i;
        iSession= &(iEngine->CreateNewSessionL(KTBCP, 1, port ));
        EUNIT_ASSERT( iSession != NULL )
        EUNIT_ASSERT( iSession->Port() == port )
        EUNIT_ASSERT( iSession->NegotiatedFCPlugIn().Compare( KTBCP ) == 0 );
        }
    }

void UT_CFCPlugInEngine::UT_CFCPlugInEngine_ReleaseSessionL(  )
    {
    iSession= &(iEngine->CreateNewSessionL(KTBCP, 1, KTBCPLocalPort));
        
    CFCInterface* plugin  = ( CFCInterface* ) iEngine->PlugInByNameL(KTBCP) ;
    CleanupStack::PushL( plugin );
    
    CFCSession* fcsession = CFCSession::NewL(plugin, 1, KTBCPLocalPort); 
    CleanupStack::Pop( plugin );
    CleanupStack::PushL( fcsession );
    iEngine->ReleaseSession(*fcsession);
    iEngine->ReleaseSession(*iSession);
    
    // fcSession should still be there
    EUNIT_ASSERT( fcsession );
    TUint port =fcsession->Port();
    EUNIT_ASSERT( port==KTBCPLocalPort );
    CleanupStack::PopAndDestroy( fcsession );
    }

void UT_CFCPlugInEngine::UT_CFCPlugInEngine_FormatAttributeFieldsL(  )
    {
    iSession= &(iEngine->CreateNewSessionL(KTBCP, 1, KTBCPLocalPort));
	CFCInterface* plugin  = ( CFCInterface* ) iEngine->PlugInByNameL(KTBCP) ;
    CleanupStack::PushL( plugin );
    CFCSession* fcsession = CFCSession::NewL(plugin, 1, KTBCPLocalPort); 
    CleanupStack::Pop( plugin );
    CleanupStack::PushL( fcsession );
    RPointerArray<CSdpFmtAttributeField>* atts = new (ELeave) RPointerArray<CSdpFmtAttributeField>(1);
    CleanupStack::PushL(atts);
	CSdpFmtAttributeField* fmtatt = CSdpFmtAttributeField::DecodeLC(KFmtAttribute);
	atts->AppendL(fmtatt);
	EUNIT_ASSERT_NO_LEAVE( iEngine->SetFormatAttributeFieldsL(*iSession, atts) );
	EUNIT_ASSERT_SPECIFIC_LEAVE( iEngine->FormatAttributeFieldsL(*fcsession), 
								KErrNotFound );
	RPointerArray<CSdpFmtAttributeField>& temparray=iEngine->FormatAttributeFieldsL(*iSession);
	
	TInt temp;
	for (TInt aCount=0; aCount < temparray.Count(); aCount++)
    	{
    	if (fmtatt == temparray[ aCount ])
    	{
    		temp=aCount;
    	}
    	
    	}
	EUNIT_ASSERT(temparray[temp]==fmtatt);
	CleanupStack::PopAndDestroy(fmtatt);//fmtatt
	temparray.Close();
	CleanupStack::Pop(atts);//atts
	CleanupStack::PopAndDestroy( fcsession );
    }

void UT_CFCPlugInEngine::UT_CFCPlugInEngine_SetFormatAttributeFieldsLL(  )
    {
    iSession= &(iEngine->CreateNewSessionL(KTBCP, 1, KTBCPLocalPort));
    CFCInterface* plugin  = ( CFCInterface* ) iEngine->PlugInByNameL(KTBCP) ;
    CleanupStack::PushL( plugin );
        
    CFCSession* fcsession = CFCSession::NewL(plugin, 1, KTBCPLocalPort);
    CleanupStack::Pop( plugin );
    CleanupStack::PushL( fcsession );
    RPointerArray<CSdpFmtAttributeField>* atts = new (ELeave) RPointerArray<CSdpFmtAttributeField>(1);
    CleanupStack::PushL(atts);
	CSdpFmtAttributeField* fmtatt = CSdpFmtAttributeField::DecodeLC(KFmtAttribute);
	atts->AppendL(fmtatt);
	EUNIT_ASSERT_SPECIFIC_LEAVE( iEngine->SetFormatAttributeFieldsL(*fcsession, atts), 
								KErrNotFound );
	EUNIT_ASSERT_NO_LEAVE( iEngine->SetFormatAttributeFieldsL(*iSession, atts) );
	
	RPointerArray<CSdpFmtAttributeField>& temparray=iEngine->FormatAttributeFieldsL(*iSession);
	

	TInt temp;
	for (TInt aCount=0; aCount < temparray.Count(); aCount++)
    	{
    	if (fmtatt == temparray[ aCount ])
    	{
    		temp=aCount;
    	}
    	
    	}
	EUNIT_ASSERT(temparray[temp]==fmtatt);
	CleanupStack::PopAndDestroy(fmtatt);//fmtatt
	temparray.Close();
	CleanupStack::Pop(atts);//atts
	
    CleanupStack::PopAndDestroy( fcsession );
    }

void UT_CFCPlugInEngine::UT_CFCPlugInEngine_PeekDocumentLL(  )
    {
    iSession= &(iEngine->CreateNewSessionL(KTBCP, 1, KTBCPLocalPort));
    CFCInterface* plugin  = ( CFCInterface* ) iEngine->PlugInByNameL(KTBCP) ;
    CleanupStack::PushL( plugin );
    CFCSession* fcsession = CFCSession::NewL(plugin, 1, KTBCPLocalPort);
    CleanupStack::Pop( plugin );
    CleanupStack::PushL( fcsession );
    
	HBufC8* sdptest = KSDPMessage2().AllocLC();
	CSdpDocument* sdpDocument = CSdpDocument::DecodeL(*sdptest);
	CleanupStack::PushL(sdpDocument);
	EUNIT_ASSERT_SPECIFIC_LEAVE( iEngine->PeekDocumentL( *fcsession, *sdpDocument ), KErrNotFound );
    iEngine->PeekDocumentL( *iSession, *sdpDocument );
	
	RPointerArray<CSdpFmtAttributeField>* atts = new (ELeave) RPointerArray<CSdpFmtAttributeField>(1);
    CleanupStack::PushL(atts);
	CSdpFmtAttributeField* fmtatt = CSdpFmtAttributeField::DecodeLC(KFmtAttribute2);
	atts->AppendL(fmtatt);
	
	RPointerArray<CSdpFmtAttributeField>& temparray=iEngine->FormatAttributeFieldsL(*iSession);
	for (TInt aCount=0; aCount < temparray.Count(); aCount++)
    	{
    	CSdpFmtAttributeField* tempatt=temparray[ aCount ];
    
	    	if (fmtatt == tempatt)
		    	{
		    	EUNIT_ASSERT(tempatt==fmtatt);
		    	}
    	}
	CleanupStack::PopAndDestroy(fmtatt);//fmtatt
	atts->Close();
	CleanupStack::PopAndDestroy(atts);//atts
	CleanupStack::PopAndDestroy(sdpDocument);
	sdpDocument = NULL;
   	CleanupStack::PopAndDestroy(sdptest);//sdptest
    CleanupStack::PopAndDestroy( fcsession );
    }

void UT_CFCPlugInEngine::UT_CFCPlugInEngine_UpdateOfferLL(  )
    {
    iSession= &(iEngine->CreateNewSessionL(KTBCP, 1, KTBCPLocalPort));
    CFCInterface* plugin  = ( CFCInterface* ) iEngine->PlugInByNameL(KTBCP) ;
    CleanupStack::PushL( plugin );
       
    CFCSession* fcsession = CFCSession::NewL(plugin, 1, KTBCPLocalPort);
    CleanupStack::Pop( plugin );
    CleanupStack::PushL( fcsession );
    	
	HBufC8* sdptest = KSDPMessage().AllocLC();
	CSdpDocument* sdpDocument = CSdpDocument::DecodeL(*sdptest);
	CleanupStack::PushL(sdpDocument);

	
	RPointerArray<CSdpFmtAttributeField>* atts = new (ELeave) RPointerArray<CSdpFmtAttributeField>(1);
    CleanupStack::PushL(atts);
	CSdpFmtAttributeField* fmtatt = CSdpFmtAttributeField::DecodeLC(KFmtAttribute);
	atts->AppendL(fmtatt);
	CleanupStack::Pop(fmtatt);//fmtatt
	
	iEngine->SetFormatAttributeFieldsL(*iSession, atts);
	CleanupStack::Pop(atts);//atts
	EUNIT_ASSERT_SPECIFIC_LEAVE( iEngine->UpdateOfferL( *fcsession, *sdpDocument ), KErrNotFound );
	iEngine-> UpdateOfferL( *iSession, *sdpDocument );
	_LIT8(KOwnDesc, "count Value = intvalue=%d" );
	EUNIT_ASSERT(iSession->Port()==2000);
   	CleanupStack::PopAndDestroy(sdpDocument);
	sdpDocument = NULL;
	
   	CleanupStack::PopAndDestroy(sdptest);//sdptest
    CleanupStack::PopAndDestroy( fcsession );
       
    }

void UT_CFCPlugInEngine::UT_CFCPlugInEngine_UpdateAnswerLL(  )
    {
    iSession= &(iEngine->CreateNewSessionL(KTBCP, 1,KTBCPLocalPort));
    CFCInterface* plugin  = ( CFCInterface* ) iEngine->PlugInByNameL(KTBCP) ;
    CleanupStack::PushL( plugin );
    
    CFCSession* fcsession = CFCSession::NewL(plugin, 1, KTBCPLocalPort);
    CleanupStack::Pop( plugin );
    CleanupStack::PushL( fcsession );
    
    EUNIT_ASSERT_NO_LEAVE( iEngine->UpdateNeededL( *iSession ) );
    
	HBufC8* sdptest = KSDPMessage2().AllocLC();
	CSdpDocument* sdpDocument = CSdpDocument::DecodeL(*sdptest);
	CleanupStack::PushL(sdpDocument);
	//For test coverage
	EUNIT_ASSERT_SPECIFIC_LEAVE( iEngine->UpdateAnswerL( *fcsession,*sdpDocument ), KErrNotFound );
	//real test case
	iEngine->UpdateAnswerL( *iSession,*sdpDocument );
	EUNIT_ASSERT_NO_LEAVE( !iEngine->UpdateNeededL( *iSession ) );
	RPointerArray<CSdpMediaField>&  temparray =sdpDocument->MediaFields();
	for (TInt aCount=0; aCount < temparray.Count(); aCount++)
    	{
    	CSdpMediaField* mediaAnnouncement = temparray[ aCount ];
    	const TDesC8& aPlugInType= mediaAnnouncement->FormatList();
			
		if(KTBCP().Compare(aPlugInType)==0)
            {
    
    		EUNIT_ASSERT(aPlugInType==KTBCP);
    		}
    	}
	CleanupStack::PopAndDestroy(sdpDocument);
	sdpDocument = NULL;
   	CleanupStack::PopAndDestroy(sdptest);//sdptest
   	CleanupStack::PopAndDestroy( fcsession );
   		
    }

void UT_CFCPlugInEngine::UT_CFCPlugInEngine_StartConnectionLL(  )
    {
    
    iSession= &(iEngine->CreateNewSessionL(KTBCP, 1, KTBCPLocalPort));
    CFCInterface* plugin  = ( CFCInterface* ) iEngine->PlugInByNameL(KTBCP) ;
    CleanupStack::PushL( plugin );
    
    CFCSession* fcsession = CFCSession::NewL(plugin, 1, KTBCPLocalPort);
    CleanupStack::Pop( plugin );
    CleanupStack::PushL( fcsession );
    EUNIT_ASSERT_SPECIFIC_LEAVE( iEngine->StartConnectionL( *fcsession ), KErrNotFound );
    EUNIT_ASSERT_NO_LEAVE( iEngine->StartConnectionL( *iSession ) );
    EUNIT_ASSERT( iErr==4);
    CleanupStack::PopAndDestroy( fcsession );
    }

void UT_CFCPlugInEngine::UT_CFCPlugInEngine_IsReceiverLL()
    {
    iSession= &(iEngine->CreateNewSessionL(KTBCP, 1, KTBCPLocalPort));
    CFCInterface* plugin  = ( CFCInterface* ) iEngine->PlugInByNameL(KTBCP) ;
    CleanupStack::PushL( plugin );
    CFCSession* fcSession = CFCSession::NewL(plugin, 1, KTBCPLocalPort);
    CleanupStack::Pop( plugin );
    CleanupStack::PushL( fcSession );
    EUNIT_ASSERT( iEngine->IsReceiverL( *iSession ) );
    EUNIT_ASSERT_LEAVE( iEngine->IsReceiverL( *fcSession ) );
    CleanupStack::PopAndDestroy( fcSession );
    }

void UT_CFCPlugInEngine::UT_CFCPlugInEngine_SendToNetLL(  )
    {
    iSession= &(iEngine->CreateNewSessionL(KTBCP, 1, KTBCPLocalPort));
    CFCInterface* plugin  = ( CFCInterface* ) iEngine->PlugInByNameL(KTBCP) ;
    CleanupStack::PushL( plugin );
    
    CFCSession* fcsession = CFCSession::NewL(plugin, 1, KTBCPLocalPort);
    CleanupStack::Pop( plugin );
    CleanupStack::PushL( fcsession );
    HBufC8* data= KHello().AllocLC();
    EUNIT_ASSERT_SPECIFIC_LEAVE( iEngine->SendToNetL( *fcsession, data ), KErrNotFound );
    EUNIT_ASSERT_NO_LEAVE( iEngine->SendToNetL(*iSession, data) );
    EUNIT_ASSERT(iData->Des()==KHello);
    CleanupStack::PopAndDestroy(data);
    CleanupStack::PopAndDestroy( fcsession );
    }

void UT_CFCPlugInEngine::UT_CFCPlugInEngine_UpdateSessionLL(  )
    {
    // leaving case
    CFCInterface* plugin  = ( CFCInterface* ) iEngine->PlugInByNameL(KTBCP) ;
    CleanupStack::PushL( plugin );
    
    CFCSession* fcSession = CFCSession::NewL(plugin, 1, KTBCPLocalPort);
    CleanupStack::Pop( plugin );
    CleanupStack::PushL( fcSession );
    EUNIT_ASSERT_SPECIFIC_LEAVE( 
        iEngine->UpdateSessionL( *fcSession, KTBCPLocalPort ), KErrNotFound );
    CleanupStack::PopAndDestroy( fcSession );
    
    // successfull
    iSession= &(iEngine->CreateNewSessionL( KTBCP, 1, KTBCPLocalPort ) );
    EUNIT_ASSERT_NO_LEAVE( iEngine->UpdateSessionL( *iSession, KTBCPLocalPort2 ) );
    EUNIT_ASSERT( iSession->Port() == KTBCPLocalPort2 );
    }

void UT_CFCPlugInEngine::UT_CFCPlugInEngine_SetSessionPortLL(  )
    {
    // leaving case
    CFCInterface* plugin  = ( CFCInterface* ) iEngine->PlugInByNameL(KTBCP) ;
    CleanupStack::PushL( plugin );
    
    CFCSession* fcSession = CFCSession::NewL(plugin, 1, KTBCPLocalPort);
    CleanupStack::Pop( plugin );
    CleanupStack::PushL( fcSession );
    EUNIT_ASSERT_SPECIFIC_LEAVE( 
        iEngine->SetSessionPortL( *fcSession, KTBCPLocalPort ), KErrNotFound );
    CleanupStack::PopAndDestroy( fcSession );
    
    // successfull
    iSession= &(iEngine->CreateNewSessionL( KTBCP, 1, KTBCPLocalPort ) );
    EUNIT_ASSERT_NO_LEAVE( iEngine->SetSessionPortL( *iSession, KTBCPLocalPort2 ) );
    EUNIT_ASSERT( iSession->Port() == KTBCPLocalPort2 );
    }


void UT_CFCPlugInEngine::UT_CFCPlugInEngine_ReceivedDataL(  )
    {
    // First with no session. 
    HBufC8* data= KHello().AllocLC();
    iEngine->ReceivedData(data, iSession->PlugIn() );
    CleanupStack::PopAndDestroy( data );
    
    // create session
    iSession= &(iEngine->CreateNewSessionL(KTBCP, 1,KTBCPLocalPort));
    HBufC8* data2= KHello().AllocLC();
    iEngine->ReceivedData(data2, iSession->PlugIn() );
    EUNIT_ASSERT(iData->Des()==KHello);
    
    CleanupStack::PopAndDestroy( data2 ); 
    }

void UT_CFCPlugInEngine::UT_CFCPlugInEngine_ErrorNotifyL(  )
    {
    iSession= &(iEngine->CreateNewSessionL(KTBCP, 1,KTBCPLocalPort));
    iEngine->ErrorNotify(5, iSession->PlugIn() );
    
    EUNIT_ASSERT( iErr==5 );
    }

void UT_CFCPlugInEngine::UT_CFCPlugInEngine_ArrayCleanupL(  )
    {
    // First with NULL
    CleanupStack::PushL( TCleanupItem( CFCPlugInEngine::ArrayCleanup, NULL ) );
    CleanupStack::PopAndDestroy();
    	
	// Real test case
	RImplInfoPtrArray infoArray;		
	
	CleanupStack::PushL( TCleanupItem( CFCPlugInEngine::ArrayCleanup, &infoArray ) );	
	REComSession::ListImplementationsL( KCFCInterfaceUid,
										infoArray );
    CleanupStack::PopAndDestroy();
    }

        
void UT_CFCPlugInEngine::FCReceivedData(HBufC8* aData, const CFCSession* /*aFCSession*/)
    {
	iData=aData;
	}

void UT_CFCPlugInEngine::FCErrorNotify(TInt aErrCode, const CFCSession* /*aFCSession*/) 

    {
	iErr=aErrCode;
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CFCPlugInEngine,
    "CFCPlugInEngine Test",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CFCPlugInEngine",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CFCPlugInEngine_NewLL, Teardown)

EUNIT_TEST(
    "NewLC - test ",
    "CFCPlugInEngine",
    "NewLC",
    "FUNCTIONALITY",
    SetupL, UT_CFCPlugInEngine_NewLCL, Teardown)

EUNIT_TEST(
    "FCPlugInIter - test ",
    "CFCPlugInEngine",
    "FCPlugInIter",
    "FUNCTIONALITY",
    SetupL, UT_CFCPlugInEngine_FCPlugInIterL, Teardown)

EUNIT_TEST(
    "ListMechanismImplementationsL - test ",
    "CFCPlugInEngine",
    "ListMechanismImplementationsL",
    "FUNCTIONALITY",
    SetupL, UT_CFCPlugInEngine_ListMechanismImplementationsLL, Teardown)

EUNIT_TEST(
    "SupportedProtocolL - test ",
    "CFCPlugInEngine",
    "SupportedProtocolL",
    "FUNCTIONALITY",
    SetupL, UT_CFCPlugInEngine_SupportedProtocolLL, Teardown)

EUNIT_TEST(
    "PlugInByNameL - test ",
    "CFCPlugInEngine",
    "PlugInByNameL",
    "FUNCTIONALITY",
    SetupL, UT_CFCPlugInEngine_PlugInByNameLL, Teardown)

EUNIT_TEST(
    "CreateNewSessionL - test ",
    "CFCPlugInEngine",
    "CreateNewSessionL",
    "FUNCTIONALITY",
    SetupL, UT_CFCPlugInEngine_CreateNewSessionLL, Teardown)

EUNIT_TEST(
    "ReleaseSession - test ",
    "CFCPlugInEngine",
    "ReleaseSession",
    "FUNCTIONALITY",
    SetupL, UT_CFCPlugInEngine_ReleaseSessionL, Teardown)

EUNIT_TEST(
    "FormatAttributeFields - test ",
    "CFCPlugInEngine",
    "FormatAttributeFields",
    "FUNCTIONALITY",
    SetupL, UT_CFCPlugInEngine_FormatAttributeFieldsL, Teardown)

EUNIT_TEST(
    "SetFormatAttributeFieldsL - test ",
    "CFCPlugInEngine",
    "SetFormatAttributeFieldsL",
    "FUNCTIONALITY",
    SetupL, UT_CFCPlugInEngine_SetFormatAttributeFieldsLL, Teardown)

EUNIT_TEST(
    "PeekDocumentL - test ",
    "CFCPlugInEngine",
    "PeekDocumentL",
    "FUNCTIONALITY",
    SetupL, UT_CFCPlugInEngine_PeekDocumentLL, Teardown)

EUNIT_TEST(
    "UpdateOfferL - test ",
    "CFCPlugInEngine",
    "UpdateOfferL",
    "FUNCTIONALITY",
    SetupL, UT_CFCPlugInEngine_UpdateOfferLL, Teardown)

EUNIT_TEST(
    "UpdateAnswerL - test ",
    "CFCPlugInEngine",
    "UpdateAnswerL",
    "FUNCTIONALITY",
    SetupL, UT_CFCPlugInEngine_UpdateAnswerLL, Teardown)

EUNIT_TEST(
    "StartConnectionL - test ",
    "CFCPlugInEngine",
    "StartConnectionL",
    "FUNCTIONALITY",
    SetupL, UT_CFCPlugInEngine_StartConnectionLL, Teardown)

EUNIT_TEST(
    "IsReceiverL - test ",
    "CFCPlugInEngine",
    "IsReceiverL",
    "FUNCTIONALITY",
    SetupL, UT_CFCPlugInEngine_IsReceiverLL, Teardown)

EUNIT_TEST(
    "SendToNetL - test ",
    "CFCPlugInEngine",
    "SendToNetL",
    "FUNCTIONALITY",
    SetupL, UT_CFCPlugInEngine_SendToNetLL, Teardown)

EUNIT_TEST(
    "UpdateSessionL - test ",
    "CFCPlugInEngine",
    "UpdateSessionL",
    "FUNCTIONALITY",
    SetupL, UT_CFCPlugInEngine_UpdateSessionLL, Teardown)

EUNIT_TEST(
    "SetSessionPortL - test ",
    "CFCPlugInEngine",
    "SetSessionPortL",
    "FUNCTIONALITY",
    SetupL, UT_CFCPlugInEngine_SetSessionPortLL, Teardown)

EUNIT_TEST(
    "ReceivedData - test ",
    "CFCPlugInEngine",
    "ReceivedData",
    "FUNCTIONALITY",
    SetupL, UT_CFCPlugInEngine_ReceivedDataL, Teardown)

EUNIT_TEST(
    "ErrorNotify - test ",
    "CFCPlugInEngine",
    "ErrorNotify",
    "FUNCTIONALITY",
    SetupL, UT_CFCPlugInEngine_ErrorNotifyL, Teardown)

EUNIT_TEST(
    "ArrayCleanup - test ",
    "CFCPlugInEngine",
    "ArrayCleanup",
    "FUNCTIONALITY",
    SetupL, UT_CFCPlugInEngine_ArrayCleanupL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
