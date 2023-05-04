#ifndef __UIBRIDGE_H__
#define __UIBRIDGE_H__
	
#include <afxwin.h>
#include <functional>

class UIBridge 
{
	struct InvokeMessage
	{
		std::function<void ()> fn;
	};
public:
	UIBridge();
	~UIBridge();
	
	void Invoke(std::function<void ()>& fn);
	template<typename F> void InvokeAny(F& f)
	{
		std::function<void ()> fn = f;
		Invoke(fn);
	}
	
	void Create();
	void Destroy();
private:

	HWND m_hBridgeWnd;
	static LRESULT WINAPI BridgeWindowProc(HWND hWnd,UINT Msg,WPARAM wParam,LPARAM lParam);

	bool RegisterWindowClass();
	void UnregisterWindowClass();
	void Post(WPARAM wParam, LPARAM lParam);
		
	afx_msg LRESULT OnUIBridgeMsg(WPARAM wParam,LPARAM lParam);
};


#endif