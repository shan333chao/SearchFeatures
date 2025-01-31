
// SearchFeaturesDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SearchFeatures.h"
#include "SearchFeaturesDlg.h"
#include "afxdialogex.h"
#include <tlhelp32.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define CODEPATH "./����.txt"

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSearchFeaturesDlg �Ի���



CSearchFeaturesDlg::CSearchFeaturesDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSearchFeaturesDlg::IDD, pParent)
	, m_hProcess(INVALID_HANDLE_VALUE)
	, m_strName(_T(""))
	, m_strNotes(_T(""))
	, m_strMarkCode(_T(""))
	, m_nOffset(0)
	, m_uLen(4)
	, m_dwBeginAddr(_T("0x00001000"))
	, m_dwEndAddr(_T("0x07FFFFFF"))
	, m_strMarkCodeList(_T(""))
	, m_btnType(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSearchFeaturesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PROCESSLIST, m_comboProcessList);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_NOTES, m_strNotes);
	DDX_Text(pDX, IDC_EDIT_MARKCODE, m_strMarkCode);
	DDX_Text(pDX, IDC_EDIT_OFFSET, m_nOffset);
	DDX_Text(pDX, IDC_EDIT_LENGTH, m_uLen);
	DDX_Text(pDX, IDC_EDIT_BEGINADDR, m_dwBeginAddr);
	DDX_Text(pDX, IDC_EDIT_ENDADDR, m_dwEndAddr);
	DDX_Text(pDX, IDC_EDIT_MARKCODELIST, m_strMarkCodeList);
	DDX_Radio(pDX, IDC_RADIO_BASEADDR, m_btnType);
	DDX_Control(pDX, IDC_LIST_RESULT, m_listResult);
	DDX_Control(pDX, IDC_COMBO_LANGUAGE, m_comboBoxLanguage);
	DDX_Control(pDX, IDC_EDIT_MARKCODELIST, m_editMarkCodeList);
}

BEGIN_MESSAGE_MAP(CSearchFeaturesDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CSearchFeaturesDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_PROCESSLIST, &CSearchFeaturesDlg::OnCbnSelchangeComboProcesslist)
	ON_BN_CLICKED(IDC_BTN_SEARCH, &CSearchFeaturesDlg::OnBnClickedBtnSearch)
	ON_BN_CLICKED(IDC_BTN_TEST, &CSearchFeaturesDlg::OnBnClickedBtnTest)
	ON_BN_CLICKED(IDC_BTN_ADDLIST, &CSearchFeaturesDlg::OnBnClickedBtnAddlist)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CSearchFeaturesDlg::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_LOAD, &CSearchFeaturesDlg::OnBnClickedBtnLoad)
	ON_BN_CLICKED(IDC_BTN_CREATECODE, &CSearchFeaturesDlg::OnBnClickedBtnCreatecode)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, &CSearchFeaturesDlg::OnBnClickedButtonRefresh)
END_MESSAGE_MAP()


// CSearchFeaturesDlg ��Ϣ�������

BOOL CSearchFeaturesDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	GetAllProcess();
	InitUI();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CSearchFeaturesDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSearchFeaturesDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CSearchFeaturesDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSearchFeaturesDlg::OnBnClickedOk()
{
}


BOOL CSearchFeaturesDlg::PreTranslateMessage(MSG* pMsg)
{
	//����Esc��ť �����û���һ��Esc��ֱ���˳�����
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


CString  CSearchFeaturesDlg::GetSelectProcessName(CString str)
{

	CString result;

	// �ҵ���һ�� '[' ��λ��
	int pos = str.Find(_T('['));

	if (pos != -1)
	{
		// ��ȡ '[' ֮ǰ�����ַ���
		result = str.Left(pos-1);
	}
	else
	{
		// ���û���ҵ� '['����ֱ��ʹ��ԭ�ַ���
		result = str;
	}

	// ȥ����ȡ����ַ������˵Ŀհ��ַ�
	return  result.Trim();

 

}

void CSearchFeaturesDlg::GetAllProcess()
{

	HANDLE hProcess;                        //���̾��
	BOOL bProcess = FALSE;                  //��ȡ������Ϣ�ĺ�������ֵ

	PROCESSENTRY32 pe;                    //���������Ϣ
	pe.dwSize = sizeof(PROCESSENTRY32);

	hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);//��ȡ���̿���
	CString strTitle;
	bProcess = Process32First(hProcess, &pe);              //��ȡ��һ��������Ϣ
	while (bProcess)
	{
		strTitle.Format(_T("%s [%d]"), pe.szExeFile, pe.th32ProcessID);
		m_comboProcessList.InsertString(-1, strTitle);
		bProcess = Process32Next(hProcess, &pe);
	}

}

void CSearchFeaturesDlg::OnCbnSelchangeComboProcesslist()
{
	if (m_hProcess != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hProcess);
	}
	//ȡ����ID
	CString strPid;
	m_comboProcessList.GetWindowText(strPid);

	int nPosLeft = strPid.Find(_T("["));
	int nPosRight = strPid.Find(_T("]"));
	if (nPosLeft == -1 || nPosRight == -1)
	{
		return;
	}
	strPid = strPid.Right(nPosRight - nPosLeft);
	strPid.Replace(_T("]"), _T(""));
	m_hProcess = GetProcessHandle(_ttoi(strPid));
}

inline HANDLE CSearchFeaturesDlg::GetProcessHandle(DWORD dwPid)
{
	TOKEN_PRIVILEGES priv = { 0 };
	HANDLE hToken = NULL;
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{
		priv.PrivilegeCount = 1;
		priv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

		if (LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &priv.Privileges[0].Luid))
			AdjustTokenPrivileges(hToken, FALSE, &priv, 0, NULL, NULL);

		CloseHandle(hToken);
	}

	return OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
}

void CSearchFeaturesDlg::OnBnClickedBtnSearch()
{
	if (m_hProcess == INVALID_HANDLE_VALUE)
	{
		MessageBox(_T("û��ѡ����̣���ѡ�����"), _T("����"));
		return;
	}
	if (m_strMarkCodeList.IsEmpty())
	{
		MessageBox(_T("�������б�Ϊ�գ�����"), _T("����"));
		return;
	}
	//����ǰ�����list�����ظ���ʾ
	m_listResult.DeleteAllItems();

	//�ָ��ÿһ�е�������
	std::vector<CString> vecMarkCodeList = SplitString(m_strMarkCodeList, _T('\n'));
	CFeatureCode fc;

	//����ÿһ������������ 
	for each (auto var in vecMarkCodeList)
	{
		if (var.IsEmpty())
		{
			continue;
		}
		//��ȡ ע�� ������ ƫ�� ��ȡ���� ��ȡ��ʽ(��ַ ƫ�� CALL)
		std::vector<CString> vecMarkCodeLine = SplitString(var, _T(','));
		DWORD dwRetAddr[32] = { 0 };
		std::string strMarkCode = CStringA(vecMarkCodeLine[2]);

		DWORD dwBeginAddr = std::stoi(m_dwBeginAddr.GetBuffer(), nullptr, 16);
		DWORD dwEndAddr = std::stoi(m_dwEndAddr.GetBuffer(), nullptr, 16);
		DWORD dwCount = fc.FindMatchingCode(m_hProcess, strMarkCode, dwBeginAddr,
			dwEndAddr, dwRetAddr, _ttoi(vecMarkCodeLine[3]),
			vecMarkCodeLine[5].CompareNoCase(_T("CALL")) == 0, false);

		if (dwCount > 0)
		{
			//ȡֵ
			DWORD dwValue;
			ReadProcessMemory(m_hProcess, (LPVOID)dwRetAddr[0], &dwValue, _ttoi(vecMarkCodeLine[4]), NULL);

			//��������õ�UI��
			CString strResult;
			strResult.Format(_T("0x%08X"), dwValue);
			DWORD dwCount = m_listResult.GetItemCount();
			m_listResult.InsertItem(dwCount, _T(""));
			m_listResult.SetItemText(dwCount, 0, vecMarkCodeLine[0]);//��������
			m_listResult.SetItemText(dwCount, 1, strResult);//���ö�ȡ���
			m_listResult.SetItemText(dwCount, 2, vecMarkCodeLine[1]);//����ע��
		}
	}
}

void CSearchFeaturesDlg::OnBnClickedBtnTest()
{
	UpdateData();

	if (m_hProcess == INVALID_HANDLE_VALUE)
	{
		MessageBox(_T("û��ѡ����̣���ѡ�����"), _T("����"));
		return;
	}

	DWORD dwRetAddr[64] = { 0 };
	CFeatureCode fc;

	//����UI�ϵ���ֹ��ַ���ַ���������Ҫת������
	DWORD dwBeginAddr = std::stoi(m_dwBeginAddr.GetBuffer(), nullptr, 16);
	DWORD dwEndAddr = std::stoi(m_dwEndAddr.GetBuffer(), nullptr, 16);

	std::string strMarkCode = CStringA(m_strMarkCode);
	DWORD dwCount = fc.FindMatchingCode(m_hProcess, strMarkCode, dwBeginAddr, dwEndAddr, dwRetAddr, m_nOffset, m_btnType == 1, true);

	if (dwCount == 0)
	{
		MessageBox(_T("��������û��ƥ�䵽�κ����� �����������Ƿ���ȡ��ȷ"), _T("����"));
		return;
	}
	CString strMsg;

	if (dwCount > 1)
	{
		DWORD dwValue;
		ReadProcessMemory(m_hProcess, (LPVOID)dwRetAddr[0], &dwValue, m_uLen, NULL);
		strMsg.Format(_T("��������ƥ�䵽������ݣ�������������룬��һ�����ݵ�ֵΪ��%X"), dwValue);
		MessageBox(strMsg, _T("����"));
		return;
	}
	DWORD dwValue;
	ReadProcessMemory(m_hProcess, (LPVOID)dwRetAddr[0], &dwValue, m_uLen, NULL);

	strMsg.Format(_T("%X"), dwValue);
	MessageBox(strMsg, _T("���Խ��"));
}


void CSearchFeaturesDlg::OnBnClickedBtnAddlist()
{
	UpdateData();

	CString strTmp;
	CString strType = _T("��ַ");
	switch (m_btnType)
	{
	case 0:
		strType = _T("��ַ");
		break;
	case 1:
		strType = _T("CALL");
		break;
	case 2:
		strType = _T("ƫ��");
		break;
	default:
		break;
	}
	//��ʽ���������б� ��ʾ��ʽΪ ���� ע�� ������ ƫ�� ��ȡ���� ����
	strTmp.Format(_T("%s,%s,%s,%d,%d,%s\r\n"), m_strName, m_strNotes, m_strMarkCode, m_nOffset, m_uLen, strType);
	m_strMarkCodeList += strTmp;

	UpdateData(FALSE);
}


void CSearchFeaturesDlg::OnBnClickedBtnSave()
{
	UpdateData();

	CString strFileName = _T("test.bp");			//Ĭ�ϴ򿪵��ļ���
	CString strFilter = _T("�������ļ� (*.bp)|*.bp||");	//�ļ����ǵ�����
	CFileDialog openFileDlg(FALSE, NULL, strFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter, NULL);
	if (openFileDlg.DoModal() == IDOK)
	{
		//ѡ��·���� ���������б��浽�ļ���
		std::string strPath = CStringA(openFileDlg.GetPathName());
		FILE* pFile = nullptr;
		fopen_s(&pFile, strPath.c_str(), "wb+");
		if (pFile == nullptr)
		{
			MessageBox(_T("���ļ�ʧ��"), _T("����"));
			return;
		}
		std::string strTmp = CStringA(m_strMarkCodeList);
		fprintf(pFile, strTmp.c_str());
		fclose(pFile);
	}
}


void CSearchFeaturesDlg::OnBnClickedBtnLoad()
{
	CString fileName = _T("test.bp");			//Ĭ�ϴ򿪵��ļ���
	CString filter = _T("�������ļ� (*.bp)|*.bp||");	//�ļ����ǵ�����
	CFileDialog openFileDlg(TRUE, NULL, fileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter, NULL);
	if (openFileDlg.DoModal() == IDOK)
	{
		//ѡ���ļ��� ���ļ��ж�ȡ�������б���ʾ��UI��
		std::string strPath = CStringA(openFileDlg.GetPathName());
		FILE* pFile = nullptr;
		fopen_s(&pFile, strPath.c_str(), "rb");
		if (pFile == nullptr)
		{
			MessageBox(_T("���ļ�ʧ��"), _T("����"));
			return;
		}

		fseek(pFile, 0, SEEK_END);//���ļ�ָ���ƶ����ļ�β
		long fileSize = ftell(pFile);//��ȡ�ļ���С
		char* szMarkCode = new char[fileSize + 2];//�����ڴ�
		memset(szMarkCode, 0, fileSize + 2);
		rewind(pFile);

		//��ȡ�ļ�����ʾ
		fread(szMarkCode, 1, fileSize, pFile);
		szMarkCode[fileSize] = '\0';
		m_strMarkCodeList = szMarkCode;
		delete szMarkCode;
		fclose(pFile);
		UpdateData(FALSE);
	}
}

std::vector<CString> CSearchFeaturesDlg::SplitString(const CString& str, TCHAR delimiter)
{
	//���ݴ�������delimiter�ָ��ַ��� ���洢��vector���ٷ���
	std::vector<CString> tokens;
	int start = 0;
	for (int i = 0; i < str.GetLength(); i++)
	{
		if (str[i] == delimiter)
		{
			CString token = str.Mid(start, i - start);
			tokens.push_back(token);
			start = i + 1;
		}
	}
	CString token = str.Mid(start);
	tokens.push_back(token);
	return tokens;
}

void CSearchFeaturesDlg::OnBnClickedBtnCreatecode()
{
	if (m_listResult.GetItemCount() == 0)
	{
		MessageBox(_T("������������������ɴ���"), _T("����"));

		return;
	}

	//�����û�ѡ�������ɶ�Ӧ�Ĵ���
	switch (m_comboBoxLanguage.GetCurSel())
	{
	case 0://����C++����
		CreateCppCode();
		break;
	case 1://���������Դ���
		CreateECode();
		break;
	default:
		break;
	}
	ShellExecute(NULL, L"open", L"notepad", _T(CODEPATH), NULL, SW_SHOWNORMAL);
}

void CSearchFeaturesDlg::InitUI()
{
	CheckDlgButton(IDC_RADIO_BASEADDR, BST_CHECKED);

	DWORD dwStyle = m_listResult.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_INFOTIP | LVS_EX_DOUBLEBUFFER;
	m_listResult.SetExtendedStyle(dwStyle);
	m_listResult.InsertColumn(0, _T("��  ��"), LVCFMT_LEFT, 110);
	m_listResult.InsertColumn(1, _T("��  ��"), LVCFMT_LEFT, 100);
	m_listResult.InsertColumn(2, _T("ע  ��"), LVCFMT_LEFT, 110);

	m_comboBoxLanguage.InsertString(-1, _T("C++"));
	m_comboBoxLanguage.InsertString(-1, _T("������"));

	m_comboBoxLanguage.SetCurSel(0);
}

void CSearchFeaturesDlg::CreateCppCode()
{
	FILE* pFile = nullptr;
	fopen_s(&pFile, CODEPATH, "wb+");
	if (pFile == nullptr)
	{
		MessageBox(_T("���ļ�ʧ��"), _T("����"));
		return;
	}
	CString strCode = _T("");

	for (int i = 0; i < m_listResult.GetItemCount(); i++)
	{
		CString strLine;
		strLine.Format(_T("#define %s \t%s \t//%s\r\n"), m_listResult.GetItemText(i, 0), m_listResult.GetItemText(i, 1), m_listResult.GetItemText(i, 2));
		strCode += strLine;

	}
	std::string strTmp = CStringA(strCode);
	fprintf(pFile, strTmp.c_str());

	fclose(pFile);
}

void CSearchFeaturesDlg::CreateECode()
{
	//�����Գ�����ʽ .�汾 2\n    .���� ������, "������ֵ��", ����, ע��

	FILE* pFile = nullptr;
	fopen_s(&pFile, CODEPATH, "wb+");
	if (pFile == nullptr)
	{
		MessageBox(_T("���ļ�ʧ��"), _T("����"));
		return;
	}
	CString strCode = _T("");
	fprintf(pFile, ".�汾 2\n");
	for (int i = 0; i < m_listResult.GetItemCount(); i++)
	{
		CString strLine;
		strLine.Format(_T(".���� %s, \"��%s��\", ����, %s\n"),
			m_listResult.GetItemText(i, 0), m_listResult.GetItemText(i, 1), m_listResult.GetItemText(i, 2));
		strCode += strLine;
	}

	std::string strTmp = CStringA(strCode);
	fprintf(pFile, strTmp.c_str());

	fclose(pFile);
}

/// <summary>
/// ��������ѡ����
/// </summary>
/// <param name="pComboBox">�ؼ�</param>
/// <param name="lpszText">�������ַ���</param>
/// <returns></returns>
int CSearchFeaturesDlg::SelectComboItemByText(CComboBox* pComboBox, LPCTSTR lpszText)
{
	if (pComboBox == NULL || lpszText == NULL) return -1;

	int nCount = pComboBox->GetCount(); // ��ȡ��Ͽ����������
	CString partialText(lpszText);
	partialText.MakeLower(); // �������ı�תΪСд

	for (int i = 0; i < nCount; ++i)
	{
		CString strItem;
		pComboBox->GetLBText(i, strItem); // ��ȡÿ����ı�
		CString itemTextLower = strItem;
		itemTextLower.MakeLower(); // ����ǰ���ı�תΪСд

		// ����ҵ��˰���Ŀ���Ӵ������ѡ�и����������
		if (itemTextLower.Find(partialText) != -1)
		{
			pComboBox->SetCurSel(i);
			return i;
		}
	}

	// ���û���ҵ�ƥ�����ѡ���κ������-1
	//pComboBox->SetCurSel(-1);
	return -1;
}

/// <summary>
/// ˢ�½����б�
/// </summary>
void CSearchFeaturesDlg::OnBnClickedButtonRefresh()
{	//ȡ����ID
	CString strProcessName;
	m_comboProcessList.GetWindowText(strProcessName);
	strProcessName = GetSelectProcessName(strProcessName);
	m_comboProcessList.ResetContent();
	CSearchFeaturesDlg::GetAllProcess();
	m_comboProcessList.SetWindowTextW(strProcessName);
	SelectComboItemByText(&m_comboProcessList, strProcessName);

}
