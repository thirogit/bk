#ifndef __DISPLACEMENT_H__
#define __DISPLACEMENT_H__

#include "../DocumentInterface.h"
#include "../../bkprint/template/FixedTemplate.h"
#include "../../bkprint/renderer/FixedTemplateDataSource.h"
#include <set>
#include <boost\range\any_range.hpp>
#include "../../runtime/FarmStructure.h"
#include "../../runtime/UniqueIds.h"
#include "../IdsInputInterface.h"
#include <boost/range/any_range.hpp>

typedef boost::any_range<CowNo,boost::forward_traversal_tag, CowNo, std::ptrdiff_t> CowNoRange;


class DisplacementOptions
{
public:
	DisplacementOptions(IPrintProfile* profile);
	uint32_t GetEvents();
	bool GetDontPrintFrom();
	bool GetDontPrintTo();
	bool GetDontPutPrintDate();
	const DateTime& GetPrintDate();
private:
	uint32_t m_events;
	bool m_dontPrintFrom;
	bool m_dontPrintTo;
	bool m_dontPutPrintDate;
	DateTime m_PrintDate;
};

class DisplacementDirection
{
public:
	DisplacementDirection();
	DisplacementDirection(const HentNo& from,const HentNo& to);
	DisplacementDirection(const DisplacementDirection& src);
	DisplacementDirection& operator=(const DisplacementDirection& right);

	const HentNo& GetFrom() const;
	const HentNo& GetTo() const;
private:
	HentNo m_from;
	HentNo m_to;
};

class DisplacementRecord
{
public:
	DisplacementRecord();

	void SetDirection(const DisplacementDirection& direction);	
	void SetEventDate(const DateTime& eventDt);

	void AddCow(const CowNo& cowNo);	

	const DisplacementDirection& GetDirection() const;
	const DateTime& GetEventDate() const;	
	size_t GetCowCount() const;
	CowNoRange GetCows() const;

private:
	std::set<CowNo> m_cows;
	DateTime m_EventDate;	
	DisplacementDirection m_Direction;
};

typedef boost::any_range<const DisplacementRecord*,boost::forward_traversal_tag, DisplacementRecord* const, std::ptrdiff_t> DisplacementRecordsRange;

class DisplacementInputInterface : public IdsInputInterface
{
public:
	~DisplacementInputInterface();
	DisplacementRecordsRange GetRecords();
	bool Query(const type_info& interfase_type);
	virtual void Feed(SeasonSession* pSession,const UniqueIds& range);
protected:	
	virtual void Consume(SeasonSession* pSession,const UniqueIds& range) = 0;
	void AddRecord(DisplacementRecord* record);
	void DeleteRecords();
private:
	std::vector<DisplacementRecord*> m_records;
};

class DisplacementNotification : public DocumentInterface
{


	class DisplacementNotificationDS : public bkprint::FixedTemplateDataSource
	{
	public:
		DisplacementNotificationDS(const DisplacementRecord* record, int cowsOnSheet, DisplacementOptions* options);
		virtual int GetSheetCount();
		virtual bkprint::Values GetValuesForSheet(int sheet);
	protected:
		uint32_t GetEvents();
		bkprint::Values GetValuesForCharacters(const std::wstring& s, int length, const std::wstring& valueNameFmt);
	protected:
		int m_cowsOnSheet;
		const DisplacementRecord* m_record;
		DisplacementOptions* m_options;
		
	};


	class HyperDisplacementNotificationDS : public bkprint::HyperFixedTemplateDataSource
	{
	public:
		HyperDisplacementNotificationDS(DisplacementNotification* parent, IPrintProfile* profile);
		~HyperDisplacementNotificationDS();
		virtual int GetDSCount();
		virtual bkprint::FixedTemplateDataSource* GetDS(int index);
	private:		
		std::vector<DisplacementNotificationDS*> m_dataSrcs;
		DisplacementOptions m_options;
	};

	friend class HyperDisplacementNotificationDS;
protected:
	DisplacementNotification(bkprint::Template* templ, DisplacementInputInterface* inputInterface);
	virtual bool QueryInputInterface(const type_info& interfase_type,void** interfase);	

	virtual bkprint::PrintOut Print(IPrintProfile* profile);
	virtual int GetCowsOnSheet() const = 0;
	
public:	
	
	virtual ~DisplacementNotification();	
protected:	
	DisplacementInputInterface* m_pInputInterface;
	bkprint::Template* m_pTemplate;
	DocumentContext* m_pContext;
};



class SimplexDisplacementNotification : public DisplacementNotification
{
public:
	SimplexDisplacementNotification(bkprint::Template* templ, DisplacementInputInterface* inputInterface);
};

template<int N>
class SimplexDisplacementNotificationN : public SimplexDisplacementNotification
{
public:
	SimplexDisplacementNotificationN(bkprint::Template* templ, DisplacementInputInterface* inputInterface) : SimplexDisplacementNotification(templ, inputInterface) {}
protected:
	virtual int GetCowsOnSheet() const
	{
		return N;
	}
};

class DuplexDisplacementNotification : public DisplacementNotification
{
public:
	DuplexDisplacementNotification(bkprint::Template* templ, DisplacementInputInterface* inputInterface);
	virtual int GetCowsOnSheet() const;
};

class BuyDocDisplacementInput : public DisplacementInputInterface
{
protected:
	virtual void Consume(SeasonSession* pSession,const UniqueIds& range);
	
};

class SellDocDisplacementInput : public DisplacementInputInterface
{
protected:
	virtual void Consume(SeasonSession* pSession,const UniqueIds& range);
};

class MoveDocDisplacementInput : public DisplacementInputInterface
{
protected:
	virtual void Consume(SeasonSession* pSession,const UniqueIds& range);
};

class BuyInvoiceDisplacementInput : public DisplacementInputInterface
{
protected:
	virtual void Consume(SeasonSession* pSession, const UniqueIds& range);
};

class SellInvoiceDisplacementInput : public DisplacementInputInterface
{
protected:
	virtual void Consume(SeasonSession* pSession, const UniqueIds& range);
};

typedef SimplexDisplacementNotificationN<4> DisplacementNotificationPostcard;
typedef SimplexDisplacementNotificationN<30> DisplacementNotification30;

template<class DISPLACEMENTCLASS,class INPUTINTERFACE>
class DiplacementNotificationDocument : public DISPLACEMENTCLASS
{
public:
	DiplacementNotificationDocument(bkprint::Template* templ) : DISPLACEMENTCLASS(templ, &m_input)
	{
	}
private:
	INPUTINTERFACE m_input;
};

typedef DiplacementNotificationDocument< SimplexDisplacementNotificationN<4>, BuyDocDisplacementInput> BuyDocDisplacementNotificationPostcard;
typedef DiplacementNotificationDocument< SimplexDisplacementNotificationN<30>, BuyDocDisplacementInput> BuyDocDisplacementNotification30;
typedef DiplacementNotificationDocument< SimplexDisplacementNotificationN<4>, SellDocDisplacementInput> SellDocDisplacementNotificationPostcard;
typedef DiplacementNotificationDocument< SimplexDisplacementNotificationN<30>, SellDocDisplacementInput> SellDocDisplacementNotification30;
typedef DiplacementNotificationDocument< SimplexDisplacementNotificationN<4>, MoveDocDisplacementInput> MoveDocDisplacementNotificationPostcard;
typedef DiplacementNotificationDocument< SimplexDisplacementNotificationN<30>, MoveDocDisplacementInput> MoveDocDisplacementNotification30;

typedef DiplacementNotificationDocument< DuplexDisplacementNotification, BuyDocDisplacementInput> BuyDocDisplacementNotification60;
typedef DiplacementNotificationDocument< DuplexDisplacementNotification, SellDocDisplacementInput> SellDocDisplacementNotification60;
typedef DiplacementNotificationDocument< DuplexDisplacementNotification, MoveDocDisplacementInput> MoveDocDisplacementNotification60;

typedef DiplacementNotificationDocument< SimplexDisplacementNotificationN<4>, BuyInvoiceDisplacementInput> BuyInvoiceDisplacementNotificationPostcard;
typedef DiplacementNotificationDocument< SimplexDisplacementNotificationN<4>, SellInvoiceDisplacementInput> SellInvoiceDisplacementNotificationPostcard;
typedef DiplacementNotificationDocument< SimplexDisplacementNotificationN<30>, BuyInvoiceDisplacementInput> BuyInvoiceDisplacementNotification30;
typedef DiplacementNotificationDocument< SimplexDisplacementNotificationN<30>, SellInvoiceDisplacementInput> SellInvoiceDisplacementNotification30;
typedef DiplacementNotificationDocument< DuplexDisplacementNotification, BuyInvoiceDisplacementInput> BuyInvoiceDisplacementNotification60;
typedef DiplacementNotificationDocument< DuplexDisplacementNotification, SellInvoiceDisplacementInput> SellInvoiceDisplacementNotification60;



#endif
