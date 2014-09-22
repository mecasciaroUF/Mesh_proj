/************************************************/
/* TWAIN.C - handle the Twain interface */
/* Release to the public domain, as adapted from
/* the source files found in the EZTWAIN package on
/* http://www.twain.org. I absolve myself of all
/* responsibilities and you're free to use it as you will.
/*
/*   1/10/96 - Jonathan Arnold jdarnold@buddydog.org
/************************************************/

#include <windows.h>

#include "twaini.h"
#include "twain.h"

HINSTANCE TwainInst = NULL;
static DSMENTRYPROC	TwainEntry;			// entry point of Data Source Manager (TWAIN.DLL)
static TW_IDENTITY	SourceId;			// source identity structure
static TW_IDENTITY	AppId = {			// application identity structure
	0,									// Id, filled in by DSM
	{ 1, 5, TWLG_USA, TWCY_USA, "<?>"},	// Version
    TWON_PROTOCOLMAJOR,
    TWON_PROTOCOLMINOR,
    DG_IMAGE | DG_CONTROL,
    "<?>",								// Mfg
    "<?>",								// Family
    "Your Product Name Here"								// Product
    };
static TW_USERINTERFACE twUI;
static HANDLE		hDib;				// bitmap returned by native transfer  
static BOOL TwainDone;					// True while we're still waiting for capture
static TW_UINT16		rc;					// result code       
int TwainXferMech(void);

int TwainExists( void )
{
  UINT oldErrorMode;
  
	if ( TwainInst != NULL )
	{ // Already loaded it and it worked
		return TRUE;
	}
	
	// Just try to load the TWAIN DLL
	// make it so Windows doesn't complain if it isn't found
	oldErrorMode = SetErrorMode( SEM_NOOPENFILEERRORBOX );
	TwainInst = LoadLibrary( "TWAIN_32.DLL" );
	SetErrorMode( oldErrorMode );	
	
	if ( TwainInst != NULL )
	{ // Load the main entry point DSM_ENTRY
		TwainEntry = (DSMENTRYPROC) GetProcAddress(TwainInst,
																							 MAKEINTRESOURCE (1));
	}

	return TwainInst != NULL;
}

// Call the Source Manager with a triplet
int TwainMgr(unsigned long dg, unsigned dat, unsigned msg, void FAR *pd)
{
	rc = TWRC_FAILURE;
	if (TwainEntry) {
		rc = (*TwainEntry)(&AppId, NULL, dg, (TW_UINT16)dat, (TW_UINT16)msg, (TW_MEMREF)pd);
	}
	return (rc == TWRC_SUCCESS);
} // TWAIN_Mgr

int FAR PASCAL __export TWAIN_DS(unsigned long dg, unsigned dat, unsigned msg, void FAR *pd)
// Call the current source with a triplet
{
	rc = TWRC_FAILURE;
	if (TwainEntry) {
		rc = (*TwainEntry)(&AppId, &SourceId, dg,(TW_UINT16)dat,(TW_UINT16)msg, (TW_MEMREF)pd);
	}
	return (rc == TWRC_SUCCESS);
} // TWAIN_DS

BOOL TwainOpenSourceManager(HWND hwnd)
{
	TW_INT32 hwnd32 = (TW_INT32)hwnd;
	BOOL rv = FALSE;

	if (TwainMgr(DG_CONTROL, DAT_PARENT, MSG_OPENDSM, &hwnd32)) {
		rv = TRUE;
	}

	return (rv);
} // TWAIN_OpenSourceManager

BOOL TwainOpenDefaultSource(void)
{
	// open the system default source
	SourceId.ProductName[0] = '\0';
	SourceId.Id = 0;
	if ( TwainMgr(DG_CONTROL, DAT_IDENTITY, MSG_OPENDS, &SourceId) )
		return TRUE;
	else
		return FALSE;

} // TWAIN_OpenDefaultSource

BOOL TwainEnableSource(HWND hwnd)
{
	twUI.ShowUI = TRUE;
	twUI.hParent = (TW_HANDLE)hwnd;
	TWAIN_DS(DG_CONTROL, DAT_USERINTERFACE, MSG_ENABLEDS, &twUI);
	if (rc == TWRC_FAILURE) {
		return FALSE;
	} else {
		return TRUE;
	}
} // TWAIN_EnableSource

void TwainCloseSourceManager(HWND hwnd)
{
	TW_INT32 hwnd32 = (TW_INT32)hwnd;

	TwainMgr(DG_CONTROL, DAT_PARENT, MSG_CLOSEDSM, &hwnd32);

} // TWAIN_CloseSourceManager

int TwainXferMech(void)
{
	// Set up native transfer
	TW_CAPABILITY   cap;
	pTW_ONEVALUE    pval;
	int status = TWRC_FAILURE;

	cap.Cap = CAP_XFERCOUNT;
	cap.ConType = TWON_ONEVALUE;

	// alloc the container
	if ((cap.hContainer = GlobalAlloc(GHND, sizeof(TW_ONEVALUE))) != NULL)
	{
		pval = (pTW_ONEVALUE)GlobalLock(cap.hContainer);
		pval->ItemType = TWTY_UINT16;
		pval->Item = 1;							// Only allow 1 image

		GlobalUnlock(cap.hContainer);

		status = TwainMgr( DG_CONTROL, DAT_CAPABILITY, MSG_SET, &cap);

		GlobalFree((HANDLE)cap.hContainer);
	}

	return status;
}	
void TwainNativeXferReady(LPMSG pmsg)
{
	TW_UINT32		hNative;
	TW_PENDINGXFERS pendingXfers;

    pmsg = pmsg;		// suppress warning

	TWAIN_DS(DG_IMAGE, DAT_IMAGENATIVEXFER, MSG_GET, &hNative);

	if (rc != TWRC_XFERDONE)
		hDib = NULL;

	switch (rc) {
		case TWRC_XFERDONE:
			// hNative contains a valid native image (handle)
			// application is responsible for de-allocating.

			// Need to acknowledge the end of the transfer
			hDib = (HANDLE)hNative;
			break;
						
		case TWRC_CANCEL:
			// user cancelled the transfer
			// hNative is invalid

			// acknowledge the end of the transfer
			break;

		case TWRC_FAILURE:
		default:
			// the transfer failed (e.g. insufficient memory)
			// hNative is invalid
			// check condition code for more info

			// state transition failed
			// image data is still pending
			break;
	} // switch

	TWAIN_DS(DG_CONTROL, DAT_PENDINGXFERS, MSG_ENDXFER, &pendingXfers);
	
	if (pendingXfers.Count > 0)
		TWAIN_DS(DG_CONTROL, DAT_PENDINGXFERS, MSG_RESET, &pendingXfers);

} // TWAIN_NativeXferReady

int TwainMessageHook(LPMSG lpmsg)
// returns TRUE if msg processed by TWAIN (source)
{
	int   bProcessed = FALSE;

	// source enabled
	TW_EVENT	twEvent;
	twEvent.pEvent = (TW_MEMREF)lpmsg;
	twEvent.TWMessage = MSG_NULL;
	// see if source wants to process (eat) the message
	TWAIN_DS(DG_CONTROL, DAT_EVENT, MSG_PROCESSEVENT, &twEvent);
	bProcessed = (rc == TWRC_DSEVENT);
	switch (twEvent.TWMessage) {
		case MSG_XFERREADY:
			TwainNativeXferReady(lpmsg);
			TwainDone = TRUE;
			break;
		case MSG_CLOSEDSREQ:
			TwainDone = TRUE;
			break;
		case MSG_NULL:
			// no message returned from DS
			break;
	} // switch
	return bProcessed;
} // TWAIN_MessageHook

void TwainModalEventLoop(void)
{
	MSG msg;          
	
	TwainDone = FALSE;
	hDib = NULL;

	while (!TwainDone && !hDib && GetMessage((LPMSG)&msg, NULL, 0, 0)) {
		if (!TwainMessageHook ((LPMSG)&msg)) {
			TranslateMessage ((LPMSG)&msg);
			DispatchMessage ((LPMSG)&msg);
		}
	} // while
} // TwainModalEventLoop

/*******/
/* TwainGetImage - High level call which returns an hDIB (or NULL */
/*  if something went awry)                                        */
/*******/
HANDLE TwainGetImage( HWND hwnd )
{
	static int _SelectedSource = FALSE;

	hDib = NULL;

	if ( !_SelectedSource )
	{ // first time thru, select source
		TW_IDENTITY		NewSourceId={0};
		
		_SelectedSource = TRUE;			// Just do it once

		if ( !TwainOpenSourceManager( hwnd ) )
			 return NULL;
	
		// Make sure there is more than one, so we don't just confuse matters
		TwainMgr(DG_CONTROL, DAT_IDENTITY, MSG_GETFIRST, &NewSourceId );
		if ( (*TwainEntry)(&AppId, NULL, DG_CONTROL, DAT_IDENTITY,
											 (TW_UINT16)MSG_GETNEXT, (TW_MEMREF)&NewSourceId )
				 != TWRC_ENDOFLIST )
		{ // There's more than one, so let user select it the first time thru
			TwainMgr(DG_CONTROL, DAT_IDENTITY, MSG_GETDEFAULT, &NewSourceId);
			if (  !TwainMgr(DG_CONTROL, DAT_IDENTITY, MSG_USERSELECT, &NewSourceId) )
				 goto error_exit;
		}

		TwainCloseSourceManager( hwnd );
	}
	
	if ( !TwainOpenSourceManager( hwnd ) )
		return NULL;
	
	if ( !TwainOpenDefaultSource() )
		goto error_exit;
	
	if ( !TwainEnableSource( hwnd ) )
		goto error_exit;

	if ( TwainXferMech() != TWRC_SUCCESS )
		 goto error_exit;
	
	// source is enabled, wait for transfer or source closed
	TwainModalEventLoop();

error_exit:
	
	// Now. turn things off
	TWAIN_DS(DG_CONTROL, DAT_USERINTERFACE, MSG_DISABLEDS, &twUI);
	TwainMgr(DG_CONTROL, DAT_IDENTITY, MSG_CLOSEDS, &SourceId);
	TwainCloseSourceManager( hwnd );		
	
	return hDib;
}

