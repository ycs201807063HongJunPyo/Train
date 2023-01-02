// CTrain.cpp: 구현 파일
//

#include "pch.h"
#include "TrainMFC.h"
#include "afxdialogex.h"
#include "CTrain.h"

#define TRAIN_TIMER 10
#define WAIT_TIMER 11
// CTrain 대화 상자

IMPLEMENT_DYNAMIC(CTrain, CDialog)

CTrain::CTrain(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_MAINTRAIN, pParent)
{
	startFlag = TRUE;
	trainFlag = FALSE;
	trainSpeed = 0;
	trainCount = 0;
	trainX = 0;
}

CTrain::~CTrain()
{
}

void CTrain::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTrain, CDialog)
	ON_BN_CLICKED(IDSTART, &CTrain::OnBnClickedStart)
	ON_BN_CLICKED(IDSTOP, &CTrain::OnBnClickedStop)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CTrain 메시지 처리기


void CTrain::OnBnClickedStart()
{
	trainFlag = TRUE;
	SetTimer(TRAIN_TIMER, 100, NULL);
}


void CTrain::OnBnClickedStop()
{
	KillTimer(TRAIN_TIMER);
	KillTimer(WAIT_TIMER);
	trainFlag = FALSE;
}





void CTrain::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CDC MemDC;

	// 화면 DC와 호환되는 메모리 DC를 생성
	MemDC.CreateCompatibleDC(&dc);
	// 비트맵 리소스 로딩
	CBitmap bmp;
	CBitmap* pOldBmp = NULL;

	//맵 만들기
	MemDC.SelectObject(pOldBmp);
	//역 테두리 설정
	CPen myPen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen* oldPen = dc.SelectObject(&myPen);
	//첫 사각형 구역(0)
	dc.Rectangle(10, 10, 110, 60);
	//두번째 사각형 구역(1)
	dc.Rectangle(110, 10, 210, 60);
	//세번째 사각형 구역(2)
	dc.Rectangle(210, 10, 310, 60);
	//네번째 사각형 구역(3)
	dc.Rectangle(310, 10, 410, 60);
	//다섯번째 사각형 구역(4)
	dc.Rectangle(410, 10, 510, 60);
	//6 사각형 구역(5)
	dc.Rectangle(410, 60, 510, 110);
	//7 사각형 구역(6)
	dc.Rectangle(410, 110, 510, 160);
	dc.SelectObject(oldPen);

	//도착하는 역 초록색 표시
	CBrush brush;
	CBrush* oldBrush = dc.SelectObject(&brush);
	brush.CreateSolidBrush(RGB(0, 255, 0));       // 녹색 채움색을 생성
	oldBrush = dc.SelectObject(&brush);

	dc.Rectangle(10 + (trainCount * 100), 10, 110 + (trainCount * 100), 60);
	dc.SelectObject(oldBrush);    // 시스템 브러시 객체를 돌려줌

	//이전 역 빨강색 표시
	if (trainCount >= 1) {
		CBrush brush;
		CBrush* oldBrush = dc.SelectObject(&brush);
		brush.CreateSolidBrush(RGB(255, 0, 0));       // 녹색 채움색을 생성
		oldBrush = dc.SelectObject(&brush);

		dc.Rectangle(10 + ((trainCount - 1) * 100), 10, 110 + ((trainCount - 1) * 100), 60);
		dc.SelectObject(oldBrush);    // 시스템 브러시 객체를 돌려줌
	}
	
	bmp.LoadBitmapW(IDB_TRAIN);

	// 로딩된 비트맵 정보 확인
	bmp.GetBitmap(&bmpInfo);
	pOldBmp = MemDC.SelectObject(&bmp);
	dc.BitBlt(trainSpeed, 20, bmpInfo.bmWidth + trainSpeed, bmpInfo.bmHeight, &MemDC, 0, 0, SRCCOPY);
	/* 내려가기 테스트
	// 메모리 DC에 선택
	pOldBmp = MemDC.SelectObject(&bmp);
	if (trainCount <= 4) {
		// 메모리 DC에 들어 있는 비트맵을 화면 DC로 복사하여 출력
		dc.BitBlt(trainSpeed, 20, bmpInfo.bmWidth + trainSpeed, bmpInfo.bmHeight, &MemDC, 0, 0, SRCCOPY);
		trainX = bmpInfo.bmWidth + trainSpeed;
	}
	else if (trainCount <= 6) {
		dc.BitBlt(trainX, 20+trainSpeed, trainX+110, bmpInfo.bmHeight+trainSpeed, &MemDC, 0, 0, SRCCOPY);
	}
	*/

	MemDC.SelectObject(pOldBmp);

	
}


void CTrain::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent)
	{
	case TRAIN_TIMER:
		trainSpeed += 10;
		//역에 도착하면 멈추기
		if (bmpInfo.bmWidth + trainSpeed == (110 + (trainCount * 100))) {
			KillTimer(TRAIN_TIMER);
			trainFlag = FALSE;
			SetTimer(WAIT_TIMER, 500, NULL);
		}
		Invalidate(TRUE);
		UpdateWindow();
		break;
	case WAIT_TIMER:
		//시간 지나면 다시 움직이기
		trainCount++;
		KillTimer(WAIT_TIMER);
		trainFlag = TRUE;
		SetTimer(TRAIN_TIMER, 100, NULL);
		Invalidate(TRUE);
		UpdateWindow();
		break;
	}
	



	CDialog::OnTimer(nIDEvent);
}


BOOL CTrain::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(0, 0, 0));
	return CDialog::OnEraseBkgnd(pDC);
}







HBRUSH CTrain::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr;


	if (nCtlColor == CTLCOLOR_STATIC)	//STATIC 컨트롤만 배경색 변경
		pDC->SetBkColor(RGB(0, 0, 0));	//배경색 변경

	hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
	return hbr;
}
