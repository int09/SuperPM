// MSortListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "MSortListCtrl.h"

#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")
#include <strsafe.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMSortListCtrl
#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0) 
#define KEYUP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1) 
#define HDF_SORTDOWN		0x0200
#define HDF_SORTUP			0x0400

enum 
{
		SORT_NON,	 //当前尚未排序
		SORT_ASCEND, //升序
		SORT_DESCEND //降序
};

//2012-11-21 17:18:23
typedef struct DATA_TIME_FORMAT
{
	TCHAR szYear[5];	//2012-   -最终会被替换成结束标志
	TCHAR szMonth[3];	//11-   -最终会被替换成结束标志
	TCHAR szDay[3];		//21    空格最终会被替换成结束标志
	TCHAR szHour[3];	//17:	:最终会被替换成结束标志
	TCHAR szMinute[3]; //18:	:最终会被替换成结束标志
	TCHAR szSecond[3]; //23		最后一个字符会被替换成结束标志
}DATA_TIME_FORMAT,*LPDATA_TIME_FORMAT;

#define DATATIMEFORMAT(T) 		T.szYear[4]=(TCHAR)'\0';	\
								T.szMonth[2]=(TCHAR)'\0';	\
								T.szDay[2]=(TCHAR)'\0';		\
								T.szHour[2]=(TCHAR)'\0';	\
								T.szMinute[2]=(TCHAR)'\0';	\
								T.szSecond[2]=(TCHAR)'\0';	


CMSortListCtrl::CMSortListCtrl() : m_bNcHitTest(FALSE),m_nCurCol(-1),m_bNcHitTested(FALSE)
{

}

CMSortListCtrl::~CMSortListCtrl()
{

}


BEGIN_MESSAGE_MAP(CMSortListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CMSortListCtrl)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
	//}}AFX_MSG_MAP
	ON_WM_NCHITTEST()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMSortListCtrl message handlers


UINT CMSortListCtrl::OnNcHitTest(CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(!m_bNcHitTest) return CListCtrl::OnNcHitTest(point);

	RECT rc;
    GetWindowRect(&rc);
	if (abs(point.y-rc.bottom+1) < 4)
	{
		if(IsWindowVisible()) m_bNcHitTested = TRUE;
		return HTBOTTOM;
	}
	return CListCtrl::OnNcHitTest(point);
}


void CMSortListCtrl::OnSize(UINT nType, int cx, int cy) 
{

	
	if(!m_bNcHitTest || !m_bNcHitTested) { CListCtrl::OnSize(nType, cx, cy); return;}
	
	CListCtrl::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here

	CRect rc;
	GetWindowRect(rc);
	ScreenToClient(rc);
	CPoint point;
	GetCursorPos(&point);
	ScreenToClient(&point);

	if(point.y >=rc.bottom-5 && point.y <= rc.bottom+5 && IsWindowVisible())
	{
		DWORD dwStyle = GetStyle();
		if(dwStyle & WS_HSCROLL)
			cy += GetSystemMetrics(SM_CYHSCROLL); 
		::PostMessage(m_hMsgWnd,m_nMsg,NULL,MAKELPARAM(cx,cy));
	}
}


void CMSortListCtrl::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default
	CListCtrl::OnGetMinMaxInfo(lpMMI);

	lpMMI->ptMinTrackSize.y = m_nMinY?m_nMinY:0;
	lpMMI->ptMaxTrackSize.y = m_nMaxY?m_nMaxY:-1;
}

void CMSortListCtrl::SetResizingInfo(INT nMinY,INT nMaxY,HWND hMsgWnd,INT nMsg)
{
	m_bNcHitTest = TRUE;
	m_nMinY = nMinY;
	m_nMaxY = 	nMaxY;
	m_hMsgWnd = hMsgWnd;
	m_nMsg = nMsg;
}

static int CALLBACK CompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CMSortListCtrl * lpThis = (CMSortListCtrl *)lParamSort;
	if(lpThis->m_nCurCol == -1)
		return 0;

	CMMAP::iterator itMark,itType;
	itMark = lpThis->m_ColSortMark.find(lpThis->m_nCurCol);
	itType = lpThis->m_ColType.find(lpThis->m_nCurCol);
	if(itMark == lpThis->m_ColSortMark.end() || itType == lpThis->m_ColType.end())
		return 0;

	INT nItem1=-1,nItem2=-1;
	CString szItem1,szItem2;

	LVFINDINFO stFindInfo;
	stFindInfo.flags = LVFI_PARAM;
    stFindInfo.lParam = lParam1;
	nItem1 = lpThis->FindItem(&stFindInfo);

	stFindInfo.flags = LVFI_PARAM;
    stFindInfo.lParam = lParam2;
	nItem2 = lpThis->FindItem(&stFindInfo);

	szItem1 = lpThis->GetItemText(nItem1, lpThis->m_nCurCol);
	szItem2 = lpThis->GetItemText(nItem2, lpThis->m_nCurCol);

	INT nResult = 0;
	switch(itType->second)
	{
	case TYPE_FLOAT:
		nResult = (STOF(szItem1) == STOF(szItem2) )?0:( STOF(szItem1) > STOF(szItem2) )?1:-1;	
		break;
	case TYPE_INTGER:
		nResult = (_ttoi(szItem1) == _ttoi(szItem2) )?0:( _ttoi(szItem1) > _ttoi(szItem2) )?1:-1;
		break;
	case TYPE_STRING:
		nResult = StrCmpI(szItem1,szItem2);
		break;
	case TYPE_DATE:
		{
			DATA_TIME_FORMAT dt1,dt2;
			CopyMemory(&dt1,(LPCTSTR)szItem1,szItem1.GetLength() * sizeof(TCHAR));
			CopyMemory(&dt2,(LPCTSTR)szItem2,szItem2.GetLength() * sizeof(TCHAR));
			DATATIMEFORMAT(dt1);
			DATATIMEFORMAT(dt2);
			CTime t1(_ttoi(dt1.szYear),_ttoi(dt1.szMonth),_ttoi(dt1.szDay),
				_ttoi(dt1.szHour),_ttoi(dt1.szMinute),_ttoi(dt1.szSecond));
			
			CTime t2(_ttoi(dt2.szYear),_ttoi(dt2.szMonth),_ttoi(dt2.szDay),
				_ttoi(dt2.szHour),_ttoi(dt2.szMinute),_ttoi(dt2.szSecond));
			nResult = ( t1 == t2 )?0:(t1 > t2 )?1:-1;
		}
		break;
	}
	
	if(itMark->second == SORT_DESCEND && nResult)
		return (nResult==1)?-1:1;
	return nResult;
}


void CMSortListCtrl::SetHeaderSortImage(HWND listView, int columnIndex, BOOL isAscending)
{
    HWND header = ListView_GetHeader(listView);
	
    int columnCount = Header_GetItemCount(header);
    for (int i = 0; i<columnCount; i++)
    {
        HDITEM hi = {0};
		
        //I only need to retrieve the format if i'm on
        //windows xp. If not, then i need to retrieve
        //the bitmap also.
        hi.mask = HDI_FORMAT;
		
        Header_GetItem(header, i, &hi);
		
        //Set sort image to this column
        if(i == columnIndex)
        {
            //Windows xp has a easier way to show the sort order
            //in the header: i just have to set a flag and windows
            //will do the drawing. No windows xp, no easy way.
            hi.fmt &= ~(HDF_SORTDOWN|HDF_SORTUP);
            hi.fmt |= isAscending ? HDF_SORTUP : HDF_SORTDOWN;
        }
        //Remove sort image (if exists)
        //from other columns.
        else
        {
            hi.fmt &= ~(HDF_SORTDOWN|HDF_SORTUP);
        }
		
        Header_SetItem(header, i, &hi);
    }
}

void CMSortListCtrl::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	m_nCurCol = pNMListView->iSubItem;
	
	CMMAP::iterator it;
	it = m_ColSortMark.find(m_nCurCol);
	if(it  == m_ColSortMark.end())
		m_ColSortMark.insert(CMPAIR(m_nCurCol,SORT_NON));
	it = m_ColSortMark.find(m_nCurCol);
	
	//没有赋值
	switch(it->second)
	{
	case SORT_NON:
		it->second = SORT_ASCEND;	
		break;
	case SORT_ASCEND:
		it->second = SORT_DESCEND;
		break;
	case SORT_DESCEND:
		it->second = SORT_ASCEND;
		break;
	}
	SortItems(CompareProc,(DWORD)this);
	SetHeaderSortImage(GetSafeHwnd(), m_nCurCol,(it->second==SORT_ASCEND)?TRUE:FALSE);
	*pResult = 0;
}

BOOL CMSortListCtrl::SetItemText( int nItem, int nSubItem, LPCTSTR lpszText )
{
	if(lpszText == NULL) return FALSE;
	return CListCtrl::SetItemText(nItem,nSubItem,lpszText);
}

BOOL CMSortListCtrl::SetItemTextA(int nItem, int nSubItem, LPCSTR lpszText)
{
	ASSERT(::IsWindow(m_hWnd));
	ASSERT((GetStyle() & LVS_OWNERDATA)==0);
	LVITEMA lvi;
	lvi.iSubItem = nSubItem;
	lvi.pszText = (LPSTR)lpszText;
	return (BOOL) ::SendMessageA(m_hWnd, LVM_SETITEMTEXTA, nItem, (LPARAM)&lvi);
}



BOOL CMSortListCtrl::SetItemInt(int nItem, int nSubItem,INT nValue,INT radix)
{
	_itot(nValue,m_szFormatBuffer,radix);
	return CListCtrl::SetItemText(nItem,nSubItem,m_szFormatBuffer);
}

BOOL CMSortListCtrl::SetItemFormatText(int nItem, int nSubItem, LPCTSTR szFormat,...)
{
	va_list vagrc;
	va_start(vagrc,szFormat);
	StringCbVPrintf(m_szFormatBuffer,sizeof(m_szFormatBuffer),szFormat,vagrc);
	va_end(vagrc);
	return CListCtrl::SetItemText(nItem,nSubItem,m_szFormatBuffer);
}


BOOL CMSortListCtrl::SetItemFormatTextA(int nItem, int nSubItem, LPCSTR szFormat,...)
{
	va_list vagrc;
	va_start(vagrc,szFormat);
	StringCbVPrintfA(m_szFormatBufferA,sizeof(m_szFormatBufferA),szFormat,vagrc);
	va_end(vagrc);
	return SetItemTextA(nItem,nSubItem,m_szFormatBufferA);
}


int CMSortListCtrl::InsertColumn( int nCol,
								 const LVCOLUMN* pColumn,
								 COL_TYPE ColType/*该列的数据类型*/,
								 BOOL bAutoSize/*是否自动调整列的大小*/)
{
	INT nResult = CListCtrl::InsertColumn(nCol,pColumn);
	m_ColType.insert(CMPAIR(nCol,ColType));
	if(bAutoSize) 
	{
		CClientDC dc(this);
		CSize sz = dc.GetTextExtent(pColumn->pszText);
		SetColumnWidth(nCol,sz.cx+20);
	}
	return nResult;
}



int CMSortListCtrl::InsertColumn(int nCol, 
								 LPCTSTR lpszColumnHeading, 
								 COL_TYPE ColType/*该列的数据类型 默认为字符串*/,
								 int nFormat,/*对齐方式默认是 左对齐*/
								 int nWidth,
								 int nSubItem ,
								 BOOL bAutoSize/*是否自动调整列的大小 默认为自动*/)
{
	INT nResult = CListCtrl::InsertColumn(nCol,lpszColumnHeading,nFormat,nWidth,nSubItem);
	m_ColType.insert(CMPAIR(nCol,ColType));
	if(bAutoSize) 
	{
		CClientDC dc(this);
		CSize sz = dc.GetTextExtent(lpszColumnHeading);
		SetColumnWidth(nCol,sz.cx+20);
	}
	return nResult;
}

int CMSortListCtrl::InsertColumn(LPCOLUMN_DATA lpColumnData,DWORD dwSize)
{
	for (DWORD i = 0; i < dwSize;i++)
	{
		InsertColumn(lpColumnData[i].nCol,lpColumnData[i].szText,lpColumnData[i].ColType,
			lpColumnData[i].nFormat,lpColumnData[i].nLenght,-1,lpColumnData[i].bAutoSize);
	}
	return 1;
}
