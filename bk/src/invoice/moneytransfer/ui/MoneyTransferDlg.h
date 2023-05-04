#ifndef __MONEYTRANSFERDLG_H__
#define __MONEYTRANSFERDLG_H__

#include "../../../runtime/UniqueIds.h"
#include "../../../ctrls/dateedit/dateedit.h"
#include <vector>
#include "../../../resource.h"
#include "../formatters/Formatter.h"
#include "../../../ui/lazydlg/LazyDlg.h"
#include "../../../SessionAwareDialog.h"
#include "../../../ctrls/contentlistctrl/ContentListCtrl.h"
#include "../content/MoneyTransfersContent.h"
#include "../../../ctrls/encodingcombo/EncodingCombo.h"
#include "../../../ctrls/BaseCombo.h"

class CMoneyTransferDlg : public CLazyDlg<CSessionAwareDialog>
{

	struct tag_InoiceId{};
	typedef boost::multi_index_container
		<
		MoneyTransferEntry*,
		boost::multi_index::indexed_by<
		boost::multi_index::random_access<>,
		boost::multi_index::ordered_unique < boost::multi_index::tag<tag_InoiceId>, boost::multi_index::const_mem_fun<MoneyTransferEntry, uint32_t, &MoneyTransferEntry::GetInvoiceId> > >
		>
		EntryIndexType;
	
public:
	enum { IDD = IDD_MONEYTRANSFER };

	CMoneyTransferDlg(UniqueIds& buyInvoiceIds,CWnd* pParent = NULL);   
	virtual ~CMoneyTransferDlg();	
	UniqueIds GetInvoiceIds() const;

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	virtual BOOL OnInitDialog();
	virtual void OnAfterDlgShow();

	virtual void OnOK();
	virtual void OnCancel();

	void CheckIfAllOK();
	void InitFormatsCombo();
	void Update();
	void SetTotal();

	CComboBox m_FormatsCb;
	UniqueIds m_buyInvoiceIds;
	MoneyTransfersContent m_Content;
	
	CContentListCtrl m_moneyTransferList;
	EntryIndexType m_Entries;
	std::vector<Formatter*> m_Formatters;
	CDateEdit m_TransferDate;

	CBaseCombo m_BankAccounts;
	CEncodingCombo m_Encodings;

	

	afx_msg void OnDestory();	
	afx_msg void OnDeleteMoneyTransferEntry();
	afx_msg void OnFormaterChange();
	
	DECLARE_MESSAGE_MAP()
};

#endif
