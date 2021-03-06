#include "stdafx.h"

// General
#include "UIControlRTSResourcesPanel.h"

// Additional
#include "UIControls/Common/UICommonModel.h"
#include "UIControls/Common/UICommonMaterial.h"

namespace
{
	struct SUISpreadsheetCell
	{
		SUISpreadsheetCell(glm::vec2 CellStart, glm::vec2 CellSize, glm::vec2 TextureSize)
			: CellSize(CellSize)
		{
			UVCellStart = CellStart / TextureSize;
			UVCellSize = CellSize / TextureSize;
			UVCellEnd = UVCellStart + UVCellSize;
		}

		glm::vec2 UVCellStart;
		glm::vec2 UVCellSize;
		glm::vec2 UVCellEnd;

		glm::vec2 CellSize;
	};

	const SUISpreadsheetCell cUIWindowCell(glm::vec2(0, 0), glm::vec2(536.0f, 118.0f), glm::vec2(1193.0f, 1193.0f));
}



CUIControlRTSResourcesPanel::CUIControlRTSResourcesPanel(IScene& Scene)
	: CUIControlCommon(Scene)
	, m_ResourceName("Name")
	, m_ResourceValue(888)
{
}

CUIControlRTSResourcesPanel::~CUIControlRTSResourcesPanel()
{
}

void CUIControlRTSResourcesPanel::SetResourceName(std::string Name)
{
}

void CUIControlRTSResourcesPanel::SetResourceValue(int32 ResourceValue)
{
}



//
// CUIControl
//
void CUIControlRTSResourcesPanel::Initialize()
{
	__super::Initialize();

	//<SubTexture name="button_rectangleWood.png" x="320" y="1152" width="80" height="40"/>

	m_GoldText = GetScene().CreateUIControlTCast<IUIControlText>(shared_from_this());
	m_GoldText->SetLocalPosition(glm::vec2(200.0f, 35.0f));
	m_GoldText->SetFont(GetBaseManager().GetManager<IznFontsManager>()->Add(GetRenderDevice(), "Fonts/consola.ttf", 56));
	m_GoldText->SetText("888 $");
	m_GoldText->SetColor(ColorRGBA(0.8f, 0.8f, 0.1f, 1.0f));

	auto material = MakeShared(CUICommonMaterial, GetBaseManager().GetApplication().GetRenderDevice());
	material->SetTexture(GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("Interface Pack/Spritesheet/interfacePack_sheet@2.png"));

	{
		auto subGeom = MakeShared(CUICommonModel, GetRenderDevice());
		subGeom->SetTranslate(glm::vec2(0.0f, 0.0f));
		subGeom->SetSize(cUIWindowCell.CellSize);
		subGeom->SetMaterial(material);
		subGeom->SetGeom(GetBaseManager().GetApplication().GetRenderDevice().GetPrimitivesFactory().CreateUIQuad(glm::vec2(1.0f), cUIWindowCell.UVCellStart, cUIWindowCell.UVCellEnd));
		AddSubgeometry(subGeom);
	}

	{
		auto subGeom = MakeShared(CUICommonModel, GetRenderDevice());
		subGeom->SetTranslate(glm::vec2(0, cUIWindowCell.CellSize.y - 18));
		subGeom->SetSize(cUIWindowCell.CellSize);
		subGeom->SetMaterial(material);
		subGeom->SetGeom(GetBaseManager().GetApplication().GetRenderDevice().GetPrimitivesFactory().CreateUIQuad(glm::vec2(1.0f), cUIWindowCell.UVCellStart, cUIWindowCell.UVCellEnd));
		AddSubgeometry(subGeom);
	}
	SetSize(cUIWindowCell.CellSize);
}
