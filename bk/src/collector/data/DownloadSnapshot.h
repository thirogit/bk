#ifndef __DOWNLOADSNAPSHOT_H__
#define __DOWNLOADSNAPSHOT_H__

#include <casajson/json.h>
#include <vector>
#include "DownloadDoc.h"
#include "DownloadCow.h"
#include <arrays/Enumerator.h>
#include <boost/range/any_range.hpp>

typedef boost::any_range<DownloadDoc*,boost::bidirectional_traversal_tag, DownloadDoc*, std::ptrdiff_t> DownloadDocRange;
typedef boost::any_range<DownloadCow*, boost::bidirectional_traversal_tag, DownloadCow*, std::ptrdiff_t> DownloadCowRange;
typedef boost::any_range<DownloadHent*, boost::bidirectional_traversal_tag, DownloadHent*, std::ptrdiff_t> DownloadHentRange;


class DownloadSnapshot
{
public:
	DownloadSnapshot();
	DownloadSnapshot(const DownloadSnapshot& src);
	DownloadSnapshot(DownloadSnapshot&& src);
	~DownloadSnapshot();

	DownloadSnapshot& operator=(const DownloadSnapshot &right);	
	void CopyFrom(const DownloadSnapshot &right);

	void AddInDoc(const DownloadDoc& indoc);
	void AddOutDoc(const DownloadDoc& outdoc);
	void AddCow(const DownloadCow& cow);

	void SetHerdIndex(uint16_t herdIndex);
	void SetUploadUser(const std::wstring& user);
	void SetDownloadDtTm(const DateTime& downloadDtTm);
	void SetUploadDtTm(const DateTime& uploadDtTm);

	uint16_t GetHerdIndex() const;
	const std::wstring& GetUploadUser() const;
	const DateTime& GetDownloadDtTm() const;
	const DateTime& GetUploadDtTm() const;

	PtrEnumerator<DownloadDoc> EnumInDocs();
	DownloadDocRange InDocs() const;

	PtrEnumerator<DownloadDoc> EnumOutDocs();
	DownloadDocRange OutDocs() const;

	PtrEnumerator<DownloadCow> EnumCows();
	DownloadCowRange Cows() const;


	PtrEnumerator<DownloadHent> EnumHents();
	DownloadHentRange Hents() const;

	void Empty();

	json::value ToJson() const;
	static DownloadSnapshot FromJson(const json::value& jsonObj);
private:

	std::vector<DownloadDoc*> m_InDocs;
	std::vector<DownloadDoc*> m_OutDocs;
	std::vector<DownloadCow*> m_Cows;
	std::vector<DownloadHent*> m_Hents;

	uint16_t m_herdIndex;
	std::wstring m_uploadUser;
	DateTime m_downloadDtTm;
	DateTime m_uploadDtTm;
};

#endif
