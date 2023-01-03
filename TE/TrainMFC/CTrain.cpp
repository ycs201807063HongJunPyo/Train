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
	trainY = 0;
}

CTrain::~CTrain()
{
}

void CTrain::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC1, testText1);
	DDX_Control(pDX, IDC_STATIC2, testText2);
}


BEGIN_MESSAGE_MAP(CTrain, CDialog)
	ON_BN_CLICKED(IDSTART, &CTrain::OnBnClickedStart)
	ON_BN_CLICKED(IDSTOP, &CTrain::OnBnClickedStop)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	//ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CTrain 메시지 처리기


void CTrain::OnBnClickedStart()
{
	trainFlag = TRUE;
	SetTimer(TRAIN_TIMER, 50, NULL);
}


void CTrain::OnBnClickedStop()
{
	KillTimer(TRAIN_TIMER);
	KillTimer(WAIT_TIMER);
	trainFlag = FALSE;
}

void CTrain::TimerFunction(int flag) {
	//열차 대기
	if (flag == 0) {
		KillTimer(TRAIN_TIMER);
		trainFlag = FALSE;
		SetTimer(WAIT_TIMER, 500, NULL);
	}
	//열차 출발
	else if (flag == 1) {
		KillTimer(WAIT_TIMER);
		trainFlag = TRUE;
		SetTimer(TRAIN_TIMER, 50, NULL);
	}

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
	int arraySize = (sizeof(railBottomLeft) / sizeof(*railBottomLeft));
	for (int i = 0; i < arraySize; i++) {
		//역 만들기
		dc.Rectangle(railTopLeft[i], railBottomLeft[i], railTopRight[i], railBottomRight[i]);
	}
	dc.SelectObject(oldPen);

	
	//출발 초록색 표시
	CBrush brush;
	CBrush* oldBrush = dc.SelectObject(&brush);
	brush.CreateSolidBrush(RGB(0, 255, 0));       // 초록
	/*
	if (trainFlag == TRUE) {
		brush.CreateSolidBrush(RGB(0, 255, 0));       // 초록
	}
	else {
		brush.CreateSolidBrush(RGB(255, 0, 0));       // 빨강
	}
	*/
	oldBrush = dc.SelectObject(&brush);

	dc.Rectangle(railTopLeft[trainCount], railBottomLeft[trainCount], railTopRight[trainCount], railBottomRight[trainCount]);
	dc.SelectObject(oldBrush);    // 시스템 브러시 객체를 돌려줌

	
	
	bmp.LoadBitmapW(IDB_TRAIN);

	// 로딩된 비트맵 정보 확인
	bmp.GetBitmap(&bmpInfo);

	// 메모리 DC에 선택
	pOldBmp = MemDC.SelectObject(&bmp);
	if (trainCount <= 4) {
		//오른
		dc.BitBlt(trainSpeed, 20, bmpInfo.bmWidth + trainSpeed, bmpInfo.bmHeight, &MemDC, 0, 0, SRCCOPY);
		//dc.Rectangle(trainSpeed, 20, trainSpeed+30, 50);
		trainX = bmpInfo.bmWidth + trainSpeed;
	}
	else if (trainCount <= 6) {
		//아래
		dc.BitBlt(trainX - bmpInfo.bmWidth, trainSpeed, trainX, bmpInfo.bmHeight+trainSpeed, &MemDC, 0, 0, SRCCOPY);
		//dc.Rectangle(trainX -30, trainSpeed, trainX, 30 + trainSpeed);
		trainY = bmpInfo.bmHeight + trainSpeed;
	}
	else if (trainCount <= 10) {
		//왼
		dc.BitBlt(trainSpeed, trainY-bmpInfo.bmHeight, bmpInfo.bmWidth + trainSpeed, trainY, &MemDC, 0, 0, SRCCOPY);
		//dc.Rectangle(trainSpeed, trainY - 20, trainSpeed + 10, trainY);
		trainX = bmpInfo.bmWidth + trainSpeed;
		
	}
	else if (trainCount <= 12) {
		//위
		dc.BitBlt(trainX - bmpInfo.bmWidth, trainSpeed, trainX, bmpInfo.bmHeight + trainSpeed, &MemDC, 0, 0, SRCCOPY);
	}

	MemDC.SelectObject(pOldBmp);

	
}


void CTrain::OnTimer(UINT_PTR nIDEvent)
{
	CString test;
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent)
	{
	case TRAIN_TIMER:
		if (trainCount <= 6) {
			if (trainCount == 0 && trainSpeed == 50) {
				trainSpeed += 0;
			}
			else {
				trainSpeed += 10;
			}
		}
		else {
			trainSpeed -= 10;
		}

		//역에 도착하면 멈추기
		if (trainCount <= 4 && bmpInfo.bmWidth + trainSpeed == railTopRight[trainCount]) {
			testText1.SetWindowTextW(_T("1"));
			TimerFunction(0);
		}
		else if (trainCount >= 5 && trainCount <= 6 && bmpInfo.bmHeight + trainSpeed == railBottomRight[trainCount]-10) {
			testText1.SetWindowTextW(_T("2"));
			TimerFunction(0);
		}
		
		else if (trainCount >= 5 && trainCount <= 10 && bmpInfo.bmWidth + trainSpeed == railTopRight[trainCount]) {
			testText1.SetWindowTextW(_T("3"));
			TimerFunction(0);
		}
		else if (trainCount >= 11 && trainCount <= 12 && bmpInfo.bmHeight + trainSpeed == railBottomRight[trainCount] - 10) {
			testText1.SetWindowTextW(_T("4"));
			TimerFunction(0);
		}
		Invalidate(TRUE);
		UpdateWindow();
		break;
	case WAIT_TIMER:
		//시간 지나면 다시 움직이기
		trainCount++;

		if (trainCount == 5) {
			trainSpeed = 20;
		}
		else if (trainCount == 7) {
			trainSpeed = trainX - bmpInfo.bmWidth;
		}
		else if (trainCount == 11) {
			trainSpeed = trainY - bmpInfo.bmHeight;
		}
		else if (trainCount == 13) {

			trainCount = 0;
			trainSpeed = 50;
			trainX = 0;
			trainY = 0;
		}
		TimerFunction(1);
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

/*
CString test;
			test.Format(L"%d", bmpInfo.bmHeight + trainSpeed);
			testText1.SetWindowTextW(test);
			test.Format(L"%d", trainCount);
			testText2.SetWindowTextW(test);
*/



/*

HBRUSH CTrain::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	
	HBRUSH hbr;


	if (nCtlColor == CTLCOLOR_STATIC)	//STATIC 컨트롤만 배경색 변경
		pDC->SetBkColor(RGB(0, 0, 0));	//배경색 변경

	hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
	return hbr;
	
}
*/
