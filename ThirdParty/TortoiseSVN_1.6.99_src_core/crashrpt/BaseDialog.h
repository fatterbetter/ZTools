#pragma once
#include <string>


/**
 * A base window class.
 * Provides separate window message handlers for every window object based on
 * this class.
 */
class CDialog
{
public:
	INT_PTR DoModal(HINSTANCE hInstance, int resID, HWND hWndParent);

	virtual LRESULT CALLBACK DlgFunc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

	operator HWND() {return m_hwnd;}
protected:
	HINSTANCE hResource;
	HWND m_hwnd;

	void InitDialog(HWND hwndDlg, UINT iconID);

	// the real message handler
	static INT_PTR CALLBACK stDlgFunc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

	// returns a pointer the dialog (stored as the WindowLong)
	inline static CDialog * GetObjectFromWindow(HWND hWnd)
	{
		return (CDialog *)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	}
};

