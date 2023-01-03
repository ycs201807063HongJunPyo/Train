#pragma once
#include "afxdialogex.h"


// CTrain 대화 상자

class CTrain : public CDialog
{
	DECLARE_DYNAMIC(CTrain)

public:
	CTrain(CWnd* pParent = nullptr);   // 표준 생성자입니다.

	virtual ~CTrain();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAINTRAIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedStop();
	afx_msg	void TimerFunction(int flag);
	BOOL startFlag;
	BOOL trainFlag;
	int trainSpeed;  //열차 도착 위치(속도)
	int trainCount;  //역 도착횟수
	int trainX;
	int trainY;
	int railTopLeft[13] = { 10,110,210,310,410,410,410,310,210,110,10,10,10 };
	int railTopRight[13] = { 110,210,310,410,510,510,510,410,310,210,110,110,110 };
	int railBottomLeft[13] = { 10,10,10,10,10,60,110,110,110,110,110,60,10 };
	int railBottomRight[13] = { 60,60,60,60,60,110,160,160,160,160,160,110,60 };
	BITMAP bmpInfo;  //비트맵

	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CStatic testText1;
	CStatic testText2;
};
