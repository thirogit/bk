#ifndef __TEMPLATERENDERER_H__
#define __TEMPLATERENDERER_H__

#include "../template/TextAlignment.h"
#include "../template/FieldVisitor.h"
#include "TextObject.h"
#include "RectObject.h"
#include "ImgObject.h"
#include "../../uitypes/Color.h"
#include "CompositeObject.h"
#include "../template/Border.h"
#include "../template/TextStyle.h"
#include "../template/FieldsContainer.h"
#include "../template/Template.h"
#include "../template/Band.h"

#include "RealizedTextStyle.h"
#include <mustache/Mustache.h>
#include "Values.h"
#include "PrintOut.h"
#include "ExpandableTemplateDataSource.h"
#include "FixedTemplateDataSource.h"
#include "FreeImagePlus.h"


namespace bkprint
{
	class TemplateRenderer 
	{

		class PaperObtainingVisitor : public TemplateVisitor
		{
		public:
			virtual void Accept(ExpandableTemplate* aTemplate);
			virtual void Accept(FixedTemplate* aTemplate);
			Paper paper;
		};

		class ValuesResolver : public MustacheValueResolver
		{
		public:
			ValuesResolver(const Values& values);
			virtual std::wstring Get(const std::wstring& sVariableName) const;
		private:
			const Values& m_values;
		};

		class EvaluatingVisitor : public FieldVisitor
		{
		public:
			EvaluatingVisitor(TemplateRenderer* parent, const Values &values);
			virtual void Accept(RectField* field);
			virtual void Accept(TextField* field);	
			virtual void Accept(ImageField* field);
			virtual void Accept(LogoField* field);
			Object* m_object;
		private:
			TemplateRenderer* m_parent;
			const Values& m_values;
		};

		class RendererState
		{
		public:
			int maxPaperY;
			int maxPages;
			int numPages;
			PrintOut* printOut;
			Page* currentPage;
			int y;

		};

		class StyleFindingVisitor : public TemplateVisitor
		{
		public:
			StyleFindingVisitor(const std::wstring& styleId);
			virtual void Accept(ExpandableTemplate* aTemplate);
			virtual void Accept(FixedTemplate* aTemplate);

			TextStyle style;
		private:
			std::wstring m_styleId;
		};

		class ExpandableTemplateRenderingVisitor : public TemplateVisitor
		{
			
			void StartNewPage();
			void PutPageFooter();
			void PutPageHeader();
			void PutBandHeader(Band* band);
			int CountPages();

		public:
			ExpandableTemplateRenderingVisitor();
			virtual void Accept(ExpandableTemplate* aTemplate);
			virtual void Accept(FixedTemplate* aTemplate);
						
			ExpandableTemplateDataSource* m_dataSrc;
			PrintOut* m_printOut;
			TemplateRenderer* m_owner;
		private:
			ExpandableTemplate* m_template;
			RendererState m_state;
		};

		class FixedTemplateRenderingVisitor : public TemplateVisitor
		{
		public:
			FixedTemplateRenderingVisitor();
			virtual void Accept(ExpandableTemplate* aTemplate);
			virtual void Accept(FixedTemplate* aTemplate);

			FixedTemplateDataSource* m_dataSrc;
			PrintOut* m_printOut;
			TemplateRenderer* m_owner;
		private:
			FixedTemplate* m_template;
		};
			
	public:
		TemplateRenderer(Template* aTemplate);		

		PrintOut Print(ExpandableTemplateDataSource* dataSrc);
		PrintOut Print(FixedTemplateDataSource* dataSrc);
		PrintOut Print(HyperExpandableTemplateDataSource* dataSrc);
		PrintOut Print(HyperFixedTemplateDataSource* dataSrc);
		

	private:
		TextObject* CreateTextObject(const TextField* field,const Values& values);
		RectObject* CreateRectObject(const RectField* field);
		ImgObject* CreateImageObject(const ImageField* field);
		ImgObject* CreateImageObject(const LogoField* field);
		
		HPEN ObtainPen(const Border& border);
		HBRUSH ObtainBrush(const Color& color);
		RealizedTextStyle* RealizeStyle(const TextStyle& ownStyle);
		RealizedTextStyle* ObtainStyle(const std::wstring& styleId);
		CompositeObject* EvaluateFields(FieldsContainer& fields, Values &values);
		CRect CalculatePosition(const Field* field);
		//void ClearCaches();
		//void DeleteCaches();
		void TransferCachedObjects(PrintOut& printOut);

		std::map<std::wstring, RealizedTextStyle*> m_textStyleCache;
		std::vector<RealizedTextStyle*> m_ownStyles;
		std::map<Border, HPEN> m_penCache;
		std::map<Color, HBRUSH> m_brushCache;	

		Template* m_template;
	
		friend class ExpandableTemplateRenderingVisitor;

	};
}
#endif