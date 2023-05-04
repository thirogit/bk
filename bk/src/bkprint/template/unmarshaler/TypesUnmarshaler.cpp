#include "stdafx.h"
#include "TypesUnmarshaler.h"
#include "..\util\AlignUtil.h"
#include "..\util\ColorUtil.h"

namespace bkprint
{
	TextStyle TypesUnmarshaler::UnmarshalTextStyle(const pugi::xml_node & node)
	{		
		pugi::xml_node fontnode = node.child(L"font");
		pugi::xml_node colornode = node.child(L"textColor");

		pugi::xml_node halignnode = node.child(L"horAlign");
		pugi::xml_node valignnode = node.child(L"verAlign");


		Color txtclr = ColorUtil::FromString(colornode.text().as_string());
		HorizontalAlignment halign = AlignUtil::FromStringH(halignnode.text().as_string());
		VerticalAlignment valign = AlignUtil::FromStringV(valignnode.text().as_string());

		pugi::xml_node heightnode = fontnode.child(L"height");
		pugi::xml_node facenode = fontnode.child(L"face");
		pugi::xml_node boldnode = fontnode.child(L"bold");
		pugi::xml_node italicnode = fontnode.child(L"italic");
		pugi::xml_node underlinenode = fontnode.child(L"underline");
		pugi::xml_node strikeoutnode = fontnode.child(L"strikeout");

		Font font;

		font.SetFaceName(facenode.text().as_string());
		font.SetHeight(heightnode.text().as_int());
		font.SetIsItalic(italicnode.text().as_bool());
		font.SetIsUnderline(underlinenode.text().as_bool());
		font.SetIsStrikeout(strikeoutnode.text().as_bool());
		font.SetIsBold(boldnode.text().as_bool());

		TextStyle style;
		style.SetFont(font);
		style.SetHorAlign(halign);
		style.SetVerAlign(valign);
		style.SetTextColor(txtclr);

		return style;
	}

	Border TypesUnmarshaler::UnmarshalBorder(const pugi::xml_node & node)
	{
		Border border;
		if (node)
		{
			pugi::xml_node line_width_nnode = node.child(L"lineWidth");
			pugi::xml_node line_color_node = node.child(L"lineColor");

			border.SetColor(ColorUtil::FromString(line_color_node.text().as_string()));
			border.SetWidth(line_width_nnode.text().as_int());
		}

		return border;
	}

	Color TypesUnmarshaler::UnmarshalColor(const pugi::xml_node & node)
	{
		Color color;
		if (node)
		{
			color = ColorUtil::FromString(node.text().as_string());
		}
		return color;
	}
}