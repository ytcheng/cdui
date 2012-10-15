#ifndef COLORPICKER_HPP
#define COLORPICKER_HPP

class WindowImplBase;
class ChatDialog;
class CColorPicker : public WindowImplBase
{
public:
	CColorPicker(ChatDialog* chat_dialog, POINT ptMouse);

	LPCTSTR GetWindowClassName() const;

	virtual void OnFinalMessage(HWND hWnd);

	void Notify(TNotifyUI& msg);

	void Init();

	virtual tString GetSkinFile();

	virtual tString GetSkinFolder();

	virtual LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

private:
	POINT based_point_;
	ChatDialog* chat_dialog_;
};

#endif // COLORPICKER_HPP