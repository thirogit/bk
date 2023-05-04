#ifndef __PAPER_H__
#define __PAPER_H__

#include "../types/PaperOrientation.h"
#include "../types/PaperType.h"

namespace bkprint
{
	class Paper
	{
	public:
		Paper();
		Paper(PaperType type, PaperOrientation orientation);
		Paper(const Paper& src);
		Paper& operator=(const Paper& right);

		PaperType GetType() const;
		PaperOrientation GetOrientation() const;

		void SetType(PaperType type);
		void SetOrientation(PaperOrientation orientation);

	private:
		PaperType m_type;
		PaperOrientation m_orientation;
	};
}
#endif