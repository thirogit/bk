#if !defined(_PASSPORTRECOGNITIONPROFILE_H_)
#define _PASSPORTRECOGNITIONPROFILE_H_

#include <arrays/NewPtrFlushArray.h>
#include "PassportField.h"
#include <map>
#include "IPassportRecognitionProfile.h"
#include "PassportFieldDescriptor.h"
#include "PassportFieldMetrics.h"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/mem_fun.hpp>

class SerializablePassportRecognitionProfile;

class PassportRecognitionProfile : public IPassportRecognitionProfile
{

	struct PassportFieldKey
	{
		typedef std::wstring result_type;
		result_type operator()(PassportField* pField) const
		{
			return pField->GetDescriptor()->GetFieldName();
		}
	};

	struct index_tag_fieldname{};
	
	typedef boost::multi_index_container
		<
		PassportField*,
		boost::multi_index::indexed_by<
		boost::multi_index::ordered_unique < boost::multi_index::tag<index_tag_fieldname>, PassportFieldKey >
									  >
		> FieldsIndex;




public:
    PassportRecognitionProfile(uint32_t profileId);
	PassportRecognitionProfile(const IPassportRecognitionProfile* src);
    PassportRecognitionProfile(const PassportRecognitionProfile& copy);
	
    virtual ~PassportRecognitionProfile();
    void operator=(const PassportRecognitionProfile& copy);
	void CopyFrom(const IPassportRecognitionProfile* copy);

	uint32_t GetId() const;
    int GetXDimention() const;
    int GetYDimention() const;

	void SetXDimention(int dimx);
	void SetYDimention(int dimy);
    const std::wstring& GetProfileName() const;
	void SetProfileName(const std::wstring& profileName);
	IPassportField* GetFieldByName(const std::wstring& sFieldName) const;
	PtrEnumerator<IPassportField> EnumFields() const;
	void SetFieldMetrics(const std::wstring& sFieldName,const PassportFieldMetrics& metrics);
	void SetFieldMetric(const std::wstring& sFieldName, FIELD_METRIC metric, int metricValue);
	void SetFieldMetrics(IPassportField* field);
protected:
	void CopyFieldsFrom(const IPassportRecognitionProfile* src);
	PassportField* FindFieldByName(const std::wstring& sFieldName) const;
		
	
	int m_iDimX;
    int m_iDimY;
    std::wstring m_sProfileName;
	FieldsIndex m_FieldsIndex;
	uint32_t m_Id;

	friend class SerializablePassportRecognitionProfile;
};

#endif //!defined(_PASSPROFILEPASSPROFILE_H_)

