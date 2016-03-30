// Copyright 2014 Joseph Riedel. All Rights Reserved.

#pragma once

// Provides hooks into CEF without needing to link against it.

class ICefRuntimeAPI;
class ICefRuntimeCallbacks;
class ICefWebView;
class ICefWebViewCallbacks;

#if defined(WIN32)
#if defined(_WINDLL)
#define CEFRUNTIME_API  __declspec(dllexport)
#else
#define CEFRUNTIME_API __declspec(dllimport)
#endif
#else
#define CEFRUNTIME_API
#endif

#include "CEFJavaScriptAPI.hpp"

struct CefRuntimeRect
{
	int X;
	int Y;
	int Width;
	int Height;
};

enum ECefRuntimeMouseButton
{
	CEFRT_MouseLeft = 0,
	CEFRT_MouseMiddle,
	CEFRT_MouseRight
};

enum ECefRuntimeEventFlags
{
	CEFRT_None = 0,
	CEFRT_CapsLockOn = 1 << 0,
	CEFRT_ShiftDown = 1 << 1,
	CEFRT_ControlDown = 1 << 2,
	CEFRT_AltDown = 1 << 3,
	CEFRT_LeftMouseButton = 1 << 4,
	CEFRT_MiddleMouseButton = 1 << 5,
	CEFRT_RightMouseButton = 1 << 6,
	// Mac OS-X command key.
	CEFRT_CommandDown = 1 << 7,
	CEFRT_NumLockOn = 1 << 8,
	CEFRT_IsKeyPad = 1 << 9,
	CEFRT_IsLeft = 1 << 10,
	CEFRT_IsRight = 1 << 11
};

enum ECefRuntimeKeyEvent
{
	CEFRT_RawKeyDown = 0,
	CEFRT_KeyDown,
	CEFRT_KeyUp,
	CEFRT_Char
};

struct CefRuntimeMouseEvent
{
	int X;
	int Y;
	unsigned int Modifiers;
};

struct CefRuntimeKeyEvent {
	///
	// The type of keyboard event.
	///
	ECefRuntimeKeyEvent Type;

	///
	// Bit flags describing any pressed modifier keys. See
	// cef_event_flags_t for values.
	///
	unsigned int Modifiers;

	///
	// The Windows key code for the key event. This value is used by the DOM
	// specification. Sometimes it comes directly from the event (i.e. on
	// Windows) and sometimes it's determined using a mapping function. See
	// WebCore/platform/chromium/KeyboardCodes.h for the list of values.
	///
	int WindowsKeyCode;

	///
	// The actual key code genenerated by the platform.
	///
	int NativeKeyCode;

	///
	// Indicates whether the event is considered a "system key" event (see
	// http://msdn.microsoft.com/en-us/library/ms646286(VS.85).aspx for details).
	// This value will always be false on non-Windows platforms.
	///
	bool IsSystemKey;

	///
	// The character generated by the keystroke.
	///
	unsigned short Character;

	///
	// Same as |character| but unmodified by any concurrently-held modifiers
	// (except shift). This is useful for working out shortcut keys.
	///
	unsigned short UnmodifiedCharacter;

	///
	// True if the focus is currently on an editable field on the page. This is
	// useful for determining if standard key events should be intercepted.
	///
	bool FocusOnEditableField;
};

class ICefRuntimeAPI
{
public:
	virtual ~ICefRuntimeAPI() {};

	//! Asynchronous call to create a webview.
	virtual void CreateWebView(const char* InStartupURL, int InSizeX, int InSizeY, bool InTransparentPainting, ICefWebViewCallbacks *InCallbacks) = 0;

	//! Get the variant factory for creating variants.
	virtual ICefRuntimeVariantFactory* GetVariantFactory() = 0;

	//! Call this when you are done with the runtime API.
	virtual void Release() = 0;
};

class ICefRuntimeCallbacks
{
public:
	virtual ~ICefRuntimeCallbacks() {};

	//! When CefRuntime is finished with your client object this will be called.
	virtual void Release() = 0;

};

class ICefStream
{
public:
	virtual ~ICefStream() {}

	enum SeekType {
		Stream_Set,
		Stream_Current,
		Stream_End
	};

	//! When the CefRuntime is done with your stream it will release it with this function.
	virtual void Release() = 0;

	virtual int Read(void* Ptr, int Count) = 0;
	virtual bool Seek(int Offset, SeekType Whence) = 0;
	virtual int GetPos() = 0;
	virtual bool IsEOF() = 0;

};

class ICefWebView
{
public:
	virtual ~ICefWebView() {}

	//! Get the variant factory for creating variants.
	virtual ICefRuntimeVariantFactory* GetVariantFactory() = 0;

	virtual void Resize(int InSizeX, int InSizeY) = 0;
	virtual void LoadURL(const char *InURL) = 0;

	virtual void ExecuteJSHook(const char* InHookName, ICefRuntimeVariantList* InArguments) = 0;
		
	///
	// Set whether mouse cursor change is disabled.
	///
	/*--cef()--*/
	virtual void SetMouseCursorChangeDisabled(bool InDisabled) = 0;

	///
	// Returns true if mouse cursor change is disabled.
	///
	/*--cef()--*/
	virtual bool IsMouseCursorChangeDisabled() = 0;

	///
	// Notify the browser that it has been hidden or shown. Layouting and
	// CefRenderHandler::OnPaint notification will stop when the browser is
	// hidden. This method is only used when window rendering is disabled.
	///
	/*--cef()--*/
	virtual void WasHidden(bool InHidden) = 0;

	///
	// Send a key event to the browser.
	///
	/*--cef()--*/
	virtual void SendKeyEvent(const CefRuntimeKeyEvent& InEvent) = 0;

	///
	// Send a mouse click event to the browser. The |x| and |y| coordinates are
	// relative to the upper-left corner of the view.
	///
	/*--cef()--*/
	virtual void SendMouseClickEvent(const CefRuntimeMouseEvent& InEvent,
		ECefRuntimeMouseButton InMouseButton,
		bool bInMouseUp, int InClickCount) = 0;

	///
	// Send a mouse move event to the browser. The |x| and |y| coordinates are
	// relative to the upper-left corner of the view.
	///
	/*--cef()--*/
	virtual void SendMouseMoveEvent(const CefRuntimeMouseEvent& InEvent,
		bool bInMouseLeave) = 0;

	///
	// Send a mouse wheel event to the browser. The |x| and |y| coordinates are
	// relative to the upper-left corner of the view. The |deltaX| and |deltaY|
	// values represent the movement delta in the X and Y directions respectively.
	// In order to scroll inside select popups with window rendering disabled
	// CefRenderHandler::GetScreenPoint should be implemented properly.
	///
	/*--cef()--*/
	virtual void SendMouseWheelEvent(const CefRuntimeMouseEvent& InEvent,
		int InDeltaX, int InDeltaY) = 0;

	///
	// Send a focus event to the browser.
	///
	/*--cef()--*/
	virtual void SendFocusEvent(bool InSetFocus) = 0;

	///
	// Send a capture lost event to the browser.
	///
	/*--cef()--*/
	virtual void SendCaptureLostEvent() = 0;

	///
	// Get the NSTextInputContext implementation for enabling IME on Mac when
	// window rendering is disabled.
	///
	/*--cef(default_retval=NULL)--*/
	virtual void* GetNSTextInputContext() = 0;

	///
	// Handles a keyDown event prior to passing it through the NSTextInputClient
	// machinery.
	///
	/*--cef()--*/
	virtual void HandleKeyEventBeforeTextInputClient(void* InKeyEvent) = 0;

	///
	// Performs any additional actions after NSTextInputClient handles the event.
	///
	/*--cef()--*/
	virtual void HandleKeyEventAfterTextInputClient(void* InKeyEvent) = 0;

	///
	// Returns true if the browser can navigate backwards.
	///
	/*--cef()--*/
	virtual bool CanGoBack() = 0;

	///
	// Navigate backwards.
	///
	/*--cef()--*/
	virtual void GoBack() = 0;

	///
	// Returns true if the browser can navigate forwards.
	///
	/*--cef()--*/
	virtual bool CanGoForward() = 0;

	///
	// Navigate forwards.
	///
	/*--cef()--*/
	virtual void GoForward() = 0;

	///
	// Returns true if the browser is currently loading.
	///
	/*--cef()--*/
	virtual bool IsLoading() = 0;

	///
	// Reload the current page.
	///
	/*--cef()--*/
	virtual void Reload() = 0;

	///
	// Reload the current page ignoring any cached data.
	///
	/*--cef()--*/
	virtual void ReloadIgnoreCache() = 0;

	///
	// Stop loading the page.
	///
	/*--cef()--*/
	virtual void StopLoad() = 0;

	//! Call to destroy the WebView and release any resources
	virtual void Release() = 0;

};

/*! Provides hooks into the application to handle events
	related to a WebView.

	NOTE: Methods on this interface will be called from different
	threads but the same function will not be called more than
	once at the same time.
*/
class ICefWebViewCallbacks
{
public:
	virtual ~ICefWebViewCallbacks() {}

	// Called when the WebView is finished being created.
	virtual void WebViewCreated(ICefWebView* InWebView) = 0;

	// Called when regions in the webview are rendered.
	virtual void Repaint(int InNumRegions, const CefRuntimeRect* InRegions, const void* InBuffer, int InWidth, int InHeight) = 0;

	virtual void OnCursorChange(void* InPlatformCursorHandle) = 0;

	// Called when the focused item changes
	virtual void FocusedNodeChanged(bool InIsEditableField) = 0;

	// Called by JavaScript to execute a hook function in the game
	virtual void ExecuteJSHook(const char* InHookName, ICefRuntimeVariantList* InArguments) = 0;

	virtual ICefStream* GetFileStream(const char* FilePath) = 0;

	// Allows the CEF API to make protected calls into game code in an order
	// that does not cause dead-locks. Game code always does:
	// LockGameCodeCriticalSection()
	// WebView->CallThatLocksCriticalSection()
	//
	// We must preserve that call order to avoid flipping the lock acquisition
	// and dead-locking.

	virtual bool EnterCriticalSection() = 0;
	virtual void LeaveCriticalSection() = 0;

	// When the associated webview is being released.
	// If there are no more references to the ICefWebViewCallbacks 
	// object it should be destroyed by the owner.
	virtual void Release(ICefWebView *InWebView) = 0;
};