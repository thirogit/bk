#include "stdafx.h"
#include "CollectorManagerDlg.h"
#include "../../utils/StringLoader.h"
#include "../../resource.h"
#include "../../ui/waitdlg\WaitDialog.h"
#include <utility>
#include "../../ui/listboxselectdlg/ListBoxSelectDlg.h"
#include "../CollectorException.h"
#include "../../ui/ExceptionMsgBox.h"
#include "../../image/Png.h"
#include <boost/foreach.hpp>
#include "../../ui\progressdlg\ComplexProgressDialog.h"
#include "CollectorDownloadDlg.h"
#include "../../ui/YesNoBox.h"

static const Color _red(255, 0, 0);
static const int _hide_tooltip_after_ms = 2000;
static const int _tooltip_fontheight = 45;

BEGIN_MESSAGE_MAP(CollectorManagerDlg, CLazyDlg<CSessionAwareDialog>)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_UPLOAD,OnUpload)
	ON_BN_CLICKED(IDC_DOWNLOAD, OnDownload)
END_MESSAGE_MAP()


CollectorManagerDlg::CollectorManagerDlg(CWnd* pParent) : CLazyDlg<CSessionAwareDialog>(CollectorManagerDlg::IDD, pParent)
{
	
}
void CollectorManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CLazyDlg<CSessionAwareDialog>::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COLLECTORLIST, m_CollectorList);
	DDX_Control(pDX, IDC_SNAPSHOTLIST, m_SnapshotList);	
	DDX_Control(pDX, IDC_MAINTOOLBAR, m_MainToolbar);
	DDX_Control(pDX, IDC_EXITTOOLBAR, m_ExitToolbar);
	DDX_Control(pDX, IDC_BUFFERTOOLBAR, m_BufferToolbar);

	DDX_Control(pDX, IDC_SECTIONHEADER, m_divider);
}

BOOL  CollectorManagerDlg::OnInitDialog()
{
	CLazyDlg<CSessionAwareDialog>::OnInitDialog();

	ListHeaderDescriptor collectorlistheader,snapshotlistheader;

	collectorlistheader
	(CollectorMember::DeviceName, StringLoader(IDS_HEADER_DEVICENAME))
		(CollectorMember::OsVersion, StringLoader(IDS_HEADER_OSVERSION))
		(CollectorMember::ClientVersion, StringLoader(IDS_HEADER_CLIENTVERSION))
		(CollectorMember::IpAddress, StringLoader(IDS_HEADER_COLLECTORIPADDRESS))
		(CollectorMember::Status, StringLoader(IDS_HEADER_COLLECTORDEVICESTATUS))
		;

	m_CollectorList.SetHeaderDescriptor(collectorlistheader);
	
	snapshotlistheader
	(DownloadSnapshotHeaderMember::HerdIndex, StringLoader(IDS_HEADER_HERD))
		(DownloadSnapshotHeaderMember::UploadDtTm, StringLoader(IDS_HEADER_UPLOADDTTM))
		(DownloadSnapshotHeaderMember::DownloadDtTm, StringLoader(IDS_HEADER_DOWNLOADDTTM))
		(DownloadSnapshotHeaderMember::User, StringLoader(IDS_HEADER_UPLOADUSER))		
		;
	m_SnapshotList.SetHeaderDescriptor(snapshotlistheader);
	
	boost::function<void()> onUploadFn = boost::bind(&CollectorManagerDlg::OnUpload, this);
	boost::function<void()> onDownloadFn = boost::bind(&CollectorManagerDlg::OnDownload, this);
	boost::function<void()> onSaveDownloadFn = boost::bind(&CollectorManagerDlg::OnSaveDownload, this);
	boost::function<void()> onExitFn = boost::bind(&CollectorManagerDlg::OnOK, this);
		
	m_MainToolbar.AddButton(onDownloadFn, CPng(IDPNG_DOWNLOAD));
	m_MainToolbar.AddButton(onUploadFn, CPng(IDPNG_UPLOAD));
	m_MainToolbar.AddButton(onSaveDownloadFn, CPng(IDPNG_SAVEDOWNLOAD));	
	m_ExitToolbar.AddButton(onExitFn, CPng(IDPNG_EXIT));

	boost::function<void()> onImportSavedSnapshotFn = boost::bind(&CollectorManagerDlg::OnImportSavedSnapshot, this);
	boost::function<void()> onDeleteShapshotFn = boost::bind(&CollectorManagerDlg::OnDeleteShapshot, this);

	m_BufferToolbar.AddButton(onImportSavedSnapshotFn, CPng(IDPNG_DOWNLOAD));
	m_BufferToolbar.AddButton(onDeleteShapshotFn, CPng(IDPNG_TRASHBIG));

	m_bridge.Create();
	m_server = GetServiceRegistry()->FindByType<CollectorServer>((CollectorServer*)NULL);
	m_server->AddObserver(this);

	std::vector<Collector> collectors = m_server->GetCollectors();

	auto it = collectors.begin();
	while (it != collectors.end())
	{
		m_CollectorListContent.AddCollector(it._Ptr);
		it++;
	}
	m_CollectorList.SetContent(&m_CollectorListContent);

	snapshot_iterator snapshotIt = m_storage.List(),endIt = snapshot_iterator();
	while (snapshotIt != endIt)
	{
		
		std::unique_ptr<DownloadSnapshotHeader> header = m_storage.GetHeader(*snapshotIt);

		m_SnapshotListContent.Add(header.get());
		snapshotIt++;
	}
	m_SnapshotList.SetContent(&m_SnapshotListContent);

	m_Msg.Create(this);
	m_Msg.SetShowCenteredInParent();
	return TRUE ;
}



void  CollectorManagerDlg::OnOK()
{
	EndDialog(IDOK); 
}

void CollectorManagerDlg::OnDestroy()
{
	m_server->RemoveObserver(this);
	m_bridge.Destroy();	
	CSessionAwareDialog::OnDestroy();
}

void CollectorManagerDlg::OnAfterDlgShow()
{

}

void CollectorManagerDlg::AddItem(const ICollector* collector)
{
	m_CollectorListContent.AddCollector(collector);
}

void CollectorManagerDlg::RemoveItem(uint32_t collectorId)
{
	m_CollectorListContent.RemoveCollector(collectorId);
}

void CollectorManagerDlg::OnCollectorConnected(const Collector& collector)
{
	AddItem(&collector);
}

void CollectorManagerDlg::OnCollectorDisconnected(uint32_t id)
{
	RemoveItem(id);
}

void CollectorManagerDlg::OnCollectorUpdated(const Collector& collector)
{
	m_CollectorListContent.UpdateCollector(&collector);
}

void CollectorManagerDlg::CollectorConnected(uint32_t id)
{
	boost::optional<Collector> collector = m_server->GetCollector(id);
	if (collector.is_initialized())
	{
		m_bridge.InvokeAny(std::bind(&CollectorManagerDlg::OnCollectorConnected, this, *collector.get_ptr()));		
	}	
}

void CollectorManagerDlg::CollectorDisconnected(uint32_t id)
{
	m_bridge.InvokeAny(std::bind(&CollectorManagerDlg::OnCollectorDisconnected, this, id));
}


void CollectorManagerDlg::CollectorUpdated(uint32_t id)
{
	boost::optional<Collector> collector = m_server->GetCollector(id);
	if (collector.is_initialized())
	{
		m_bridge.InvokeAny(std::bind(&CollectorManagerDlg::OnCollectorUpdated, this, *collector.get_ptr()));
	}
}

void CollectorManagerDlg::OnUpload()
{
	SeasonSession* session = GetSeasonSession();
	uint32_t collectorId = GetSelectedCollector();
	if (collectorId != NULL_ID)
	{
		uint32_t herdId = SelectHerd();
		if (herdId != NULL_ID)
		{			
			try
			{
				CComplexProgressDialog progressDialog(StringLoader(IDS_WAITUPLOADING), this);	

				progressDialog(L"packaging_hents", StringLoader(IDS_PACKAGING_HENTS))
					(L"packaging_classes", StringLoader(IDS_PACKAGING_CLASSES))
					(L"packaging_cows", StringLoader(IDS_PACKAGING_COWS))
					(L"starting_upload", StringLoader(IDS_STARTING_UPLOAD))
					(L"uploading_classes", StringLoader(IDS_UPLOADING_CLASSES))
					(L"uploading_hents", StringLoader(IDS_UPLOADING_HENTS))
					(L"uploading_cows", StringLoader(IDS_UPLOADING_COWS))
					(L"ending_upload", StringLoader(IDS_ENDING_UPLOAD));
					
				progressDialog.WaitForTask(boost::bind(&CollectorServer::Upload, m_server, collectorId, session, herdId, boost::placeholders::_1));					
					
			}
			catch (CollectorException& e)
			{
				ExceptionMsgBox box;
				box.Show(e);

			}
			catch (CollectorNotFoundException& e)
			{
				ShowCollectorLostMsg();
			}
		}
	}
}


void CollectorManagerDlg::Download(boost::function<void (const DownloadSnapshot*)>& handler)
{
	uint32_t collectorId = GetSelectedCollector();
	if (collectorId != NULL_ID)
	{
		try
		{
			CComplexProgressDialog progressDialog(StringLoader(IDS_WAITDOWNLOADING), this);

			progressDialog(L"starting_download", StringLoader(IDS_STARTING_DOWNLOAD))
				(L"downloading_indocs", StringLoader(IDS_DOWNLOADING_INDOCS))
				(L"downloading_outdocs", StringLoader(IDS_DOWNLOADING_OUTDOCS))
				(L"downloading_cows", StringLoader(IDS_DOWNLOADING_COWS))
				(L"ending_download", StringLoader(IDS_ENDING_DOWNLOAD))
				;

			std::unique_ptr<DownloadSnapshot> snapshot = progressDialog.WaitForTaskR<std::unique_ptr<DownloadSnapshot>>(boost::bind(&CollectorServer::Download, m_server, collectorId, boost::placeholders::_1));

			handler(snapshot.get());
		}
		catch (CollectorException& e)
		{
			ExceptionMsgBox box;
			box.Show(e);

		}
		catch (CollectorNotFoundException& e)
		{
			ShowCollectorLostMsg();
		}
	}
}

void CollectorManagerDlg::ShowCollectorLostMsg()
{
	m_Msg.SetText(StringLoader(IDS_COLLECTORLOST));
	m_Msg.SetStyle(TipWndStyle().inner(_red).outer(_red).textsize(_tooltip_fontheight));
	m_Msg.Show(_hide_tooltip_after_ms);
	MessageBeep(MB_ICONERROR);
}

uint32_t CollectorManagerDlg::GetSelectedCollector()
{
	int nSelectedRow = m_CollectorList.GetCurSel();
	if (nSelectedRow >= 0)
	{
		ICollector* collector = m_CollectorListContent.GetCollectorAt(nSelectedRow);
		return collector->GetId();
	}

	return NULL_ID;
}

uint32_t CollectorManagerDlg::GetSelectedSnapshot()
{
	int nSelectedRow = m_SnapshotList.GetCurSel();
	if (nSelectedRow >= 0)
	{
		const DownloadSnapshotHeader* snapshot = m_SnapshotListContent.GetSnapshotRowAt(nSelectedRow);
		return snapshot->GetId();
	}

	return NULL_ID;
}

uint32_t CollectorManagerDlg::SelectHerd()
{
	CListBoxSelectDlg herdSelect(this);
	herdSelect.SetCaption(StringLoader(IDS_SELECTHEARD));

	ComboContent herdContent;
	PtrEnumerator<IHerd> enumHerd = GetSeasonSession()->EnumHerds();
	while (enumHerd.hasNext())
	{
		IHerd* herd = *enumHerd;
		herdContent.AddItem(herd->GetId(), herd->GetHerdName());
		enumHerd.advance();
	}
	return herdSelect.Select(herdContent,NULL_ID);
}


void CollectorManagerDlg::OnDownload()
{
	boost::function<void(const DownloadSnapshot*)> handler = boost::bind(&CollectorManagerDlg::ImportSnapshot, this, boost::placeholders::_1);
	Download(handler);
}

void CollectorManagerDlg::OnSaveDownload()
{
	boost::function<void(const DownloadSnapshot*)> handler = boost::bind(&CollectorManagerDlg::SaveDownload, this, boost::placeholders::_1);
	Download(handler);
}

void CollectorManagerDlg::SaveDownload(const DownloadSnapshot* snapshot)
{
	uint32_t id = m_storage.Save(*snapshot);
	AddSavedSnapshot(id);
}

void CollectorManagerDlg::AddSavedSnapshot(uint32_t id)
{
	std::unique_ptr<DownloadSnapshotHeader> header = m_storage.GetHeader(id);

	m_SnapshotListContent.Add(header.get());
}

void CollectorManagerDlg::ImportSnapshot(const DownloadSnapshot* download)
{
	CollectorDownloadDlg downloadDlg(download, this);

	downloadDlg.DoModal();
}

void CollectorManagerDlg::OnImportSavedSnapshot()
{
	uint32_t id = GetSelectedSnapshot();
	if (id != NULL_ID)
	{
		std::unique_ptr<DownloadSnapshot> snapshot = m_storage.Load(id);

		CollectorDownloadDlg importDlg(snapshot.get(),this);
		importDlg.DoModal();

	}
}

void CollectorManagerDlg::OnDeleteShapshot()
{
	uint32_t id = GetSelectedSnapshot();
	if (id != NULL_ID)
	{
		if (YesNoBox().Ask(StringLoader(IDS_ASKDELETESNAPSHOT)))
		{
			m_storage.Delete(id);
			m_SnapshotListContent.Remove(id);
		}
	}
}
