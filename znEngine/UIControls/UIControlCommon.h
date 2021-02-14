#pragma once

class ZN_API CUIControlCommon
	: public CUIControl
	, public IUIControlCommon
{
public:
	CUIControlCommon(IScene& Scene);
	virtual ~CUIControlCommon();

	// CUIControl
	void Initialize() override;

	// IUIControlCommon
	void AddSubgeometry(const std::shared_ptr<IUIControlModel>& Model) override;
	const std::vector<std::shared_ptr<IUIControlModel>>& GetSubgeometries() const override;
	void ClearSubgeometries();

protected:
	std::vector<std::shared_ptr<IUIControlModel>>& GetSubgeometries();

private:
	std::vector<std::shared_ptr<IUIControlModel>> m_Subgeometries;
};