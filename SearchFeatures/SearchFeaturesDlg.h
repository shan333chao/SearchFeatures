
// SearchFeaturesDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "FeatureCode.h"
#include "afxcmn.h"
#include <vector>

// CSearchFeaturesDlg �Ի���
class CSearchFeaturesDlg : public CDialogEx
{
// ����
public:
	CSearchFeaturesDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SEARCHFEATURES_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();

    virtual BOOL PreTranslateMessage(MSG* pMsg);

    //ѡ��������������������ı���Ӧ����
    afx_msg void OnCbnSelchangeComboProcesslist();
    
    //���԰�ť��Ӧ����
    afx_msg void OnBnClickedBtnTest();

    //������ť��Ӧ����
    afx_msg void OnBnClickedBtnSearch();

    //��ӵ��б�ť��Ӧ����
    afx_msg void OnBnClickedBtnAddlist();

    //���������뵽�ļ���ť��Ӧ����
    afx_msg void OnBnClickedBtnSave();

    //�����������ļ���ť��Ӧ����
    afx_msg void OnBnClickedBtnLoad();
    //�Ƴ��ַ����еĻ��з�
    void RemoveNewlines(CString& str);

    //���ɴ��밴ť��Ӧ����
    afx_msg void OnBnClickedBtnCreatecode();

private:
    /*
    GetProcessHandle
    ͨ������id��ȡ���̾��
    ����Ϊ����id
    ����ֵΪ���̾��
    */
    inline HANDLE GetProcessHandle(DWORD dwPid);
    /// <summary>
    /// ��ȡ�������ѡ�������
    /// </summary>
    /// <param name="str"></param>
    /// <returns></returns>
    CString GetSelectProcessName(CString str);

    //���ڻ�ȡ��ǰ���н���
    void GetAllProcess();

    /*
    SplitString �ַ����ָ��
    ��һ������ Ҫ�ָ���ַ���
    �ڶ������� ��ʲô�ַ�����Ϊ�ָ����� 
    ����ֵ���ָ�������ַ�������
    */
    std::vector<CString> SplitString(const CString& str, TCHAR delimiter);
    
    void CreateCppCode();
    void CreateECode();
    /// <summary>
    /// ���������� ��ѡ��
    /// </summary>
    /// <param name="pComboBox"></param>
    /// <param name="lpszText"></param>
    /// <returns></returns>
    int SelectComboItemByText(CComboBox* pComboBox, LPCTSTR lpszText);

    //��ʼ��UI
    void InitUI();

private:
    // ������ʾ�����б��ComboBox�ؼ�
    CComboBox m_comboProcessList;

    //�洢���̾��
    HANDLE m_hProcess;

    // ���������� ���Ҫ����C++����Ļ�����C++������
    CString m_strName;

    // ������ע�� ˵��
    CString m_strNotes;

    // ������
    CString m_strMarkCode;

    // Ҫ��ȡ��ƫ�� ��������Ͼ������� �����Ǹ���
    int m_nOffset;

    // Ҫ��ȡ�ĳ���
    UINT m_uLen;

    // ������������ʼ��ַ
    CString m_dwBeginAddr;

    //����������������ַ
    CString m_dwEndAddr;

    //�������б�
    CString m_strMarkCodeList;
    
    // �����û��������ͣ�0==��ַ 1==call 2==ƫ��
    int m_btnType;
    
    // ��ʾ�������������
    CListCtrl m_listResult;

    // ����Ҫ����ʲô���ԵĴ���
    CComboBox m_comboBoxLanguage;
    
    // �������б�edit�ؼ�����
    CEdit m_editMarkCodeList;
public:
    afx_msg void OnBnClickedButtonRefresh();
};
