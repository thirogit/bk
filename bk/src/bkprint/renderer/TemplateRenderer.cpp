#include "stdafx.h"
#include "TemplateRenderer.h"
#include "../template/TextField.h"
#include "../template/RectField.h"
#include "../template/ImageField.h"
#include "../template/LogoField.h"
#include "SizeCalculator.h"
#include "../template/util/PaperTypeUtil.h"
#include "../template/ExpandableTemplate.h"
#include "../template/FixedTemplate.h"
#include <boost/foreach.hpp>
#include <boost/range/adaptors.hpp>

namespace bkprint
{

	void TemplateRenderer::PaperObtainingVisitor::Accept(ExpandableTemplate* aTemplate)
	{
		paper = Paper(aTemplate->GetPaperType(), aTemplate->GetOrientation());
	}
	void TemplateRenderer::PaperObtainingVisitor::Accept(FixedTemplate* aTemplate)
	{
		paper = Paper(aTemplate->GetPaperType(), aTemplate->GetOrientation());
	}

	///////////////////////////////////////////////////////////////////////////////////////////////

	TemplateRenderer::StyleFindingVisitor::StyleFindingVisitor(const std::wstring& styleId) : m_styleId(styleId)
		{
		}
		
		void TemplateRenderer::StyleFindingVisitor::Accept(ExpandableTemplate* aTemplate)
		{
			style = aTemplate->FindStyle(m_styleId);
		}

		void TemplateRenderer::StyleFindingVisitor::Accept(FixedTemplate* aTemplate)
		{
			style = aTemplate->FindStyle(m_styleId);
		}		

	///////////////////////////////////////////////////////////////////////////////////////////////

	void TemplateRenderer::EvaluatingVisitor::Accept(RectField* field)
	{
		m_object = m_parent->CreateRectObject(field);
	}

	void TemplateRenderer::EvaluatingVisitor::Accept(TextField* field)
	{
		m_object = m_parent->CreateTextObject(field, m_values);
	};

	void TemplateRenderer::EvaluatingVisitor::Accept(ImageField* field)
	{		
		m_object = m_parent->CreateImageObject(field);				
	}

	void TemplateRenderer::EvaluatingVisitor::Accept(LogoField* field)
	{
		m_object = m_parent->CreateImageObject(field);
	}

	TemplateRenderer::EvaluatingVisitor::EvaluatingVisitor(TemplateRenderer* parent, const Values &values) : m_object(NULL), m_values(values), m_parent(parent)
	{

	}

	///////////////////////////////////////////////////////////////////////////////////////////////

	TemplateRenderer::ValuesResolver::ValuesResolver(const Values& values) : m_values(values)
	{
	}

	std::wstring TemplateRenderer::ValuesResolver::Get(const std::wstring& sVariableName) const
	{
		return m_values.Get(sVariableName);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////


	CompositeObject* TemplateRenderer::EvaluateFields(FieldsContainer& fields, Values &values)
	{
		EvaluatingVisitor visitor(this, values);
		CompositeObject* composite = new CompositeObject();
		PtrEnumerator<Field> fieldsEnum = fields.EnumFields();
		while (fieldsEnum.hasNext())
		{
			Field* field = *fieldsEnum;
			field->Visit(&visitor);
			composite->m_objects.push_back(visitor.m_object);
			fieldsEnum.advance();
		}
		return composite;
	}

	TextObject* TemplateRenderer::CreateTextObject(const TextField* field, const Values& values)
	{
		TextObject* object = new TextObject();
		RealizedTextStyle* style = NULL;
		std::wstring refStyleId = field->GetRefStyle();
		if (refStyleId.empty())
		{
			TextStyle ownstyle = field->GetOwnStyle();
			style = RealizeStyle(ownstyle);
			m_ownStyles.push_back(style);
		}
		else
		{
			style = ObtainStyle(refStyleId);
		}

		ValuesResolver resolver(values);
		Mustache mustache = Mustache::Parse(field->GetText());
		object->m_text = mustache.Format(&resolver);

		object->m_font = style->GetFont();
		object->m_textColor = style->GetTextColor();
		object->m_textAlignment = style->GetTextAlignment();
		object->m_brush = ObtainBrush(field->GetFillColor());
		object->m_pen = ObtainPen(field->GetBorder());
		object->m_Position = CalculatePosition(field);

		return object;
	}

	RectObject* TemplateRenderer::CreateRectObject(const RectField* field)
	{
		RectObject* object = new RectObject();
		object->m_brush = ObtainBrush(field->GetFillColor());
		object->m_pen = ObtainPen(field->GetBorder());
		object->m_Position = CalculatePosition(field);
		return object;
	}

	ImgObject* TemplateRenderer::CreateImageObject(const ImageField* field)
	{
		ImgObject* object = new ImgObject();
		object->m_image = field->GetImage();
		object->m_brush = ObtainBrush(field->GetFillColor());
		object->m_pen = ObtainPen(field->GetBorder());
		object->m_Position = CalculatePosition(field);
		return object;
	}

	ImgObject* TemplateRenderer::CreateImageObject(const LogoField* field)
	{
		ImgObject* object = new ImgObject();
		object->m_image = field->GetImage();
		object->m_brush = NULL;
		object->m_pen = NULL;
		object->m_Position = CalculatePosition(field);
		return object;
	}

	CRect TemplateRenderer::CalculatePosition(const Field* field)
	{
		CPoint position = field->GetPosition();
		CSize size = field->GetSize();
		return CRect(position.x, position.y, position.x + size.cx, position.y + size.cy);
	}

	HPEN TemplateRenderer::ObtainPen(const Border& border)
	{
		if (!border.IsNull())
		{
			auto it = m_penCache.find(border);
			if (it == m_penCache.end())
			{
				HPEN pen = ::CreatePen(PS_SOLID, border.GetWidth(), border.GetColor());
				m_penCache[border] = pen;
				return pen;
			}
			return it->second;
		}
		return NULL;
	}

    HBRUSH TemplateRenderer::ObtainBrush(const Color& color)
	{
		if (!color.IsNull())
		{
			auto it = m_brushCache.find(color);
			if (it == m_brushCache.end())
			{				
				HBRUSH brush = ::CreateSolidBrush((COLORREF)color);
				m_brushCache[color] = brush;
				return brush;
			}

			return it->second;
		}
		return NULL;
	}

	RealizedTextStyle* TemplateRenderer::RealizeStyle(const TextStyle& ownStyle)
	{
		RealizedTextStyle* style = new RealizedTextStyle();
		Font font = ownStyle.GetFont();


		LOGFONT logfont;
		memset(&logfont, 0, sizeof(LOGFONT));
		wcscpy_s(logfont.lfFaceName, font.GetFaceName().c_str());
		logfont.lfHeight = font.GetHeight();
		logfont.lfItalic = font.GetIsItalic() ? 1 : 0;
		logfont.lfStrikeOut = font.GetIsStrikeout() ? 1 : 0;
		logfont.lfUnderline = font.GetIsUnderline() ? 1 : 0;
		logfont.lfWeight = font.GetIsBold() ? FW_BOLD : FW_NORMAL;

		style->SetFont(::CreateFontIndirect(&logfont));
		style->SetTextColor(ownStyle.GetTextColor());

		DWORD txtAlignment = 0;
		switch (ownStyle.GetHorAlign())
		{
		case HorizontalAlignment_Center:
			txtAlignment = DT_CENTER;
			break;
		case HorizontalAlignment_Left:
			txtAlignment = DT_LEFT;
			break;
		case HorizontalAlignment_Right:
			txtAlignment = DT_RIGHT;
			break;
		}

		switch (ownStyle.GetVerAlign())
		{
		case VerticalAlignment_Center:
			txtAlignment = txtAlignment | DT_VCENTER;
			break;
		case VerticalAlignment_Top:
			txtAlignment = txtAlignment | DT_TOP;
			break;
		case VerticalAlignment_Bottom:
			txtAlignment = txtAlignment | DT_BOTTOM;
			break;
		}


		style->SetTextAlignment(txtAlignment);

		return style;
	}

	RealizedTextStyle* TemplateRenderer::ObtainStyle(const std::wstring& styleId)
	{
		auto it = m_textStyleCache.find(styleId);
		if (it == m_textStyleCache.end())
		{
			StyleFindingVisitor visitor(styleId);
			m_template->Visit(&visitor);
			TextStyle style = visitor.style;
			if (style.IsNull())
				return NULL;

			RealizedTextStyle* realizedStyle = RealizeStyle(style);
			m_textStyleCache[styleId] = realizedStyle;
			return realizedStyle;
		}
		return it->second;
	}

	TemplateRenderer::TemplateRenderer(Template* aTemplate) : m_template(aTemplate)
	{

	}
	
	void TemplateRenderer::TransferCachedObjects(PrintOut& printOut)
	{
		BOOST_FOREACH(RealizedTextStyle* style, m_textStyleCache | boost::adaptors::map_values)
		{
			printOut.m_usedObjects.push_back(style->GetFont());
			style->SetFont(NULL);
			delete style;
		}
		m_textStyleCache.clear();

		BOOST_FOREACH(RealizedTextStyle* style, m_ownStyles)
		{
			printOut.m_usedObjects.push_back(style->GetFont());
			style->SetFont(NULL);
			delete style;
		}
		m_ownStyles.clear();

		BOOST_FOREACH(HPEN pen, m_penCache | boost::adaptors::map_values)
		{
			printOut.m_usedObjects.push_back(pen);					
		}
		m_penCache.clear();

		BOOST_FOREACH(HBRUSH brush, m_brushCache | boost::adaptors::map_values)
		{
			printOut.m_usedObjects.push_back(brush);			
		}
		m_brushCache.clear();

	}

///////////////////////////////////////////////////////////////////////////////////////////////////
	static const int _BOTTOM_MARGIN = 100;

	TemplateRenderer::ExpandableTemplateRenderingVisitor::ExpandableTemplateRenderingVisitor()
	{
		m_dataSrc = NULL;
		m_printOut = NULL;
		m_owner = NULL;
		m_template = NULL;
		m_state.currentPage = NULL;
		m_state.maxPages = -1;
		m_state.maxPaperY = -1;
		m_state.numPages = -1;
		m_state.printOut = NULL;
		m_state.y = -1;
	}

	void TemplateRenderer::ExpandableTemplateRenderingVisitor::Accept(ExpandableTemplate* aTemplate)
	{
		m_template = aTemplate;
		m_printOut->m_paper = Paper(m_template->GetPaperType(), m_template->GetOrientation());
		CSize paperSize = PaperTypeUtil::GetPaperSize(m_template->GetPaperType(), m_template->GetOrientation());

		m_state.printOut = m_printOut;
		m_state.maxPaperY = paperSize.cy - _BOTTOM_MARGIN;
		m_state.maxPages = CountPages();
		m_state.numPages = 0;

		StartNewPage();

		if (m_template->PageHeaderFields().GetShowOnFirstPage())
		{
			PutPageHeader();
		}

		FieldsContainer& headerFields = m_template->HeaderFields();
		SizeCalculator headerCalculator(headerFields);
		CompositeObject* header = m_owner->EvaluateFields(headerFields, m_dataSrc->GetHeaderValues());

		int headerHeight = headerCalculator.GetHeight();
		header->m_position = CRect(0, m_state.y, headerCalculator.GetWidth(), m_state.y + headerHeight);
		m_state.y += headerHeight;
		m_state.currentPage->m_objects.push_back(header);

		FieldsContainer& pageFooterFields = m_template->PageFooterFields();
		SizeCalculator pageFooterCalculator(pageFooterFields);
		int pageFooterHeight = pageFooterCalculator.GetHeight();

		int maxPageY = m_state.maxPaperY - pageFooterHeight;

		PtrEnumerator<Band> bandEnum = m_template->EnumBands();
		while (bandEnum.hasNext())
		{
			Band* band = *bandEnum;
			BandEvaluator* bandValues = m_dataSrc->GetBandValues(band->Name());
			if (bandValues != NULL)
			{

				FieldsContainer& bandHeaderFields = band->HeaderFields();
				SizeCalculator bandHeaderCalculator(bandHeaderFields);

				FieldsContainer& bandBodyFields = band->BodyFields();
				SizeCalculator bandBodyCalculator(bandBodyFields);
				int bandBodyH = bandBodyCalculator.GetHeight();

				if ((m_state.y + bandHeaderCalculator.GetHeight() + bandBodyH) >= maxPageY) //ensure that at least one row will be wisible on page
				{
					PutPageFooter();
					StartNewPage();
					PutPageHeader();
					PutBandHeader(band);
				}
				else
				{
					PutBandHeader(band);
				}							

				for (int r = 0, count = bandValues->Count(); r < count; r++)
				{
					if (m_state.y + bandBodyH >= maxPageY)
					{
						PutPageFooter();
						StartNewPage();
						PutPageHeader();
						PutBandHeader(band);
					}

					CompositeObject* bandRow = m_owner->EvaluateFields(bandBodyFields, bandValues->At(r));
					bandRow->m_position = CRect(0, m_state.y, bandBodyCalculator.GetWidth(), m_state.y + bandBodyH);
					m_state.y += bandBodyH;
					m_state.currentPage->m_objects.push_back(bandRow);
				}

				FieldsContainer& bandFooterFields = band->FooterFields();
				if (bandFooterFields.Count() > 0)
				{
					SizeCalculator bandFooterCalculator(bandFooterFields);
					int bandFootH = bandFooterCalculator.GetHeight();
					if (m_state.y + bandFootH > m_state.maxPaperY)
					{
						PutPageFooter();
						StartNewPage();
						PutPageHeader();
					}

					CompositeObject* bandFooter = m_owner->EvaluateFields(bandFooterFields, bandValues->Footer());

					bandFooter->m_position = CRect(0, m_state.y, bandFooterCalculator.GetWidth(), m_state.y+ bandFootH);
					m_state.y += bandFootH;
					m_state.currentPage->m_objects.push_back(bandFooter);
				}
			}
			bandEnum.advance();
		}

		DocumentFooter& footerFields = m_template->FooterFields();
		if (footerFields.Count() > 0)
		{
			SizeCalculator footerCalculator(footerFields);
			int footerH = footerCalculator.GetHeight();

			if (m_state.y + footerH > m_state.maxPaperY)
			{
				PutPageFooter();
				StartNewPage();
				PutPageHeader();
			}

			CompositeObject* footer = m_owner->EvaluateFields(footerFields, m_dataSrc->GetFooterValues());

			if (footerFields.GetStickToBottom())
			{
				m_state.y = m_state.maxPaperY - footerH - pageFooterHeight;
			}

			footer->m_position = CRect(0, m_state.y, footerCalculator.GetWidth(), m_state.y + footerH);
			m_state.y += footerH;

			m_state.currentPage->m_objects.push_back(footer);
		}

		PutPageFooter();
	}

	void TemplateRenderer::ExpandableTemplateRenderingVisitor::Accept(FixedTemplate* aTemplate) { /*do nothing*/ }


	void TemplateRenderer::ExpandableTemplateRenderingVisitor::StartNewPage()
	{
		m_state.currentPage = new Page();
		m_state.printOut->m_pages.push_back(m_state.currentPage);
		m_state.y = 0;
		m_state.numPages++;
	}

	void TemplateRenderer::ExpandableTemplateRenderingVisitor::PutBandHeader(Band* band)
	{
		FieldsContainer& bandHeaderFields = band->HeaderFields();
		if (bandHeaderFields.Count() > 0)
		{
			BandEvaluator* bandValues = m_dataSrc->GetBandValues(band->Name());
			SizeCalculator bandHeaderCalculator(bandHeaderFields);
			CompositeObject* bandHeader = m_owner->EvaluateFields(bandHeaderFields, bandValues->Header());
			int bandHdrH = bandHeaderCalculator.GetHeight();
			bandHeader->m_position = CRect(0, m_state.y, bandHeaderCalculator.GetWidth(), m_state.y + bandHdrH);
			m_state.y += bandHdrH;
			m_state.currentPage->m_objects.push_back(bandHeader);
		}
	}

	void TemplateRenderer::ExpandableTemplateRenderingVisitor::PutPageFooter()
	{
		FieldsContainer& pageFooterFields = m_template->PageFooterFields();
		SizeCalculator pageFooterCalculator(pageFooterFields);
		int pageFooterHeight = pageFooterCalculator.GetHeight();
		CompositeObject* pagefooter = m_owner->EvaluateFields(pageFooterFields, m_dataSrc->GetPageFooterValues(m_state.maxPages, m_state.numPages));
		pagefooter->m_position = CRect(0, m_state.maxPaperY - pageFooterHeight, pageFooterCalculator.GetWidth(), m_state.maxPaperY);
		m_state.currentPage->m_objects.push_back(pagefooter);
	}

	void TemplateRenderer::ExpandableTemplateRenderingVisitor::PutPageHeader()
	{
		PageHeader& pageHeaderFields = m_template->PageHeaderFields();
		int topMargin = pageHeaderFields.GetTopMargin();
		m_state.y += topMargin;
		if (pageHeaderFields.Count() > 0)
		{
			SizeCalculator calculator(pageHeaderFields);
			int height = calculator.GetHeight();
			CompositeObject* pageheader = m_owner->EvaluateFields(pageHeaderFields, m_dataSrc->GetPageHeaderValues(m_state.maxPages, m_state.numPages));
			pageheader->m_position = CRect(0, topMargin, calculator.GetWidth(), topMargin + height);
			m_state.currentPage->m_objects.push_back(pageheader);
			m_state.y += height;
		}
	}


	int TemplateRenderer::ExpandableTemplateRenderingVisitor::CountPages()
	{
		ExpandableTemplate* templ = (ExpandableTemplate*)m_template;

		CSize paperSize = PaperTypeUtil::GetPaperSize(templ->GetPaperType(), templ->GetOrientation());
		int maxPaperY = paperSize.cy - _BOTTOM_MARGIN;
		int y = 0;
		int pages = 1;

		FieldsContainer& headerFields = templ->HeaderFields();
		int headerH = SizeCalculator(headerFields).GetHeight();
		y += headerH;

		PageHeader& pageHeader = templ->PageHeaderFields();
		int pageHeaderH = SizeCalculator(pageHeader).GetHeight();

		if (pageHeader.GetShowOnFirstPage())
		{
			y += pageHeaderH;
		}

		FieldsContainer& pageFooterFields = templ->PageFooterFields();
		int pageFooterHeight = SizeCalculator(pageFooterFields).GetHeight();
		int maxPageY = maxPaperY - pageFooterHeight;

		
		PtrEnumerator<Band> bandEnum = templ->EnumBands();
		while (bandEnum.hasNext())
		{
			Band* band = *bandEnum;
			BandEvaluator* bandValues = m_dataSrc->GetBandValues(band->Name());

			if (bandValues != NULL && bandValues->Count() > 0)
			{
				FieldsContainer& bandHeaderFields = band->HeaderFields();
				int bandHeaderH = SizeCalculator(bandHeaderFields).GetHeight();
				y += bandHeaderH;

				FieldsContainer& bandBodyFields = band->BodyFields();
				SizeCalculator bandBodyCalculator(bandBodyFields);
				int bandBodyH = bandBodyCalculator.GetHeight();

				for (int r = 0, count = bandValues->Count(); r < count; r++)
				{
					if (y + bandBodyH >= maxPageY)
					{
						pages++;
						y = 0;
						y += pageHeaderH;
						y += bandHeaderH;
					}

					y += bandBodyH;
				}
				FieldsContainer& bandFooterFields = band->FooterFields();
				int bandFootH = SizeCalculator(bandFooterFields).GetHeight();
				if (y + bandFootH > maxPaperY)
				{
					pages++;
					y = 0;
					y += pageHeaderH;
				}
				y += bandFootH;
			}
			bandEnum.advance();
		}

		DocumentFooter& footerFields = m_template->FooterFields();
		int footerH = SizeCalculator(footerFields).GetHeight();

		if (y + footerH > maxPaperY)
		{
			pages++;
			y = 0;
			y += pageHeaderH;
		}
		return pages;
	}
///////////////////////////////////////////////////////////////////////////////////////////////////

	TemplateRenderer::FixedTemplateRenderingVisitor::FixedTemplateRenderingVisitor()
	{
		m_dataSrc = NULL;
		m_printOut = NULL;
		m_owner = NULL;
		m_template = NULL;
	}

	void TemplateRenderer::FixedTemplateRenderingVisitor::Accept(ExpandableTemplate* aTemplate) { /* do nothing */ }

	void TemplateRenderer::FixedTemplateRenderingVisitor::Accept(FixedTemplate* aTemplate)
	{
		m_template = aTemplate;

		m_printOut->m_paper = Paper(m_template->GetPaperType(), m_template->GetOrientation());
		CSize paperSize = PaperTypeUtil::GetPaperSize(m_template->GetPaperType(), m_template->GetOrientation());

		int sheetCount = m_dataSrc->GetSheetCount();

		for (int sheet = 0; sheet < sheetCount; sheet++)
		{
			Values values = m_dataSrc->GetValuesForSheet(sheet);
			for (int pageIndex = 0, pageCount = m_template->GetPageCount(); pageIndex < pageCount; pageIndex++)
			{
				FieldsContainer* pageFields = m_template->GetPage(pageIndex);
				SizeCalculator calculator(*pageFields);
				CompositeObject* pageObject = m_owner->EvaluateFields(*pageFields, values);
				pageObject->m_position = CRect(0, 0, calculator.GetWidth(), calculator.GetHeight());
				Page* page = new Page;
				m_printOut->m_pages.push_back(page);
				page->m_objects.push_back(pageObject);
			}
		}
	}
		

///////////////////////////////////////////////////////////////////////////////////////////////////

	PrintOut TemplateRenderer::Print(ExpandableTemplateDataSource* dataSrc)
	{
		PrintOut printOut;

		PaperObtainingVisitor paperVisitor;
		m_template->Visit(&paperVisitor);
		printOut.m_paper = paperVisitor.paper;
	
		ExpandableTemplateRenderingVisitor visitor;
		visitor.m_dataSrc = dataSrc;
		visitor.m_owner = this;
		visitor.m_printOut = &printOut;

		m_template->Visit(&visitor);

		TransferCachedObjects(printOut);
		return std::move(printOut);
	}


	
	PrintOut TemplateRenderer::Print(FixedTemplateDataSource* dataSrc)
	{
		PrintOut printOut;

		PaperObtainingVisitor paperVisitor;
		m_template->Visit(&paperVisitor);
		printOut.m_paper = paperVisitor.paper;


		FixedTemplateRenderingVisitor visitor;
		visitor.m_dataSrc = dataSrc;
		visitor.m_owner = this;
		visitor.m_printOut = &printOut;

		m_template->Visit(&visitor);

		TransferCachedObjects(printOut);

		return std::move(printOut);
	}

	PrintOut TemplateRenderer::Print(HyperExpandableTemplateDataSource* dataSrc)
	{
		PrintOut printOut;

		PaperObtainingVisitor paperVisitor;
		m_template->Visit(&paperVisitor);
		printOut.m_paper = paperVisitor.paper;


		for (int ds = 0, dsCount = dataSrc->GetDSCount(); ds < dsCount; ds++)
		{
			ExpandableTemplateRenderingVisitor visitor;
			visitor.m_dataSrc = dataSrc->GetDS(ds);
			visitor.m_owner = this;
			visitor.m_printOut = &printOut;

			m_template->Visit(&visitor);
		}

		TransferCachedObjects(printOut);
		return std::move(printOut);
	}


	PrintOut TemplateRenderer::Print(HyperFixedTemplateDataSource* dataSrc)
	{
		PrintOut printOut;

		PaperObtainingVisitor paperVisitor;
		m_template->Visit(&paperVisitor);
		printOut.m_paper = paperVisitor.paper;

		for (int ds = 0, dsCount = dataSrc->GetDSCount(); ds < dsCount; ds++)
		{
			FixedTemplateRenderingVisitor visitor;
			visitor.m_dataSrc = dataSrc->GetDS(ds);
			visitor.m_owner = this;
			visitor.m_printOut = &printOut;

			m_template->Visit(&visitor);
		}

		TransferCachedObjects(printOut);

		return std::move(printOut);
	}
}
