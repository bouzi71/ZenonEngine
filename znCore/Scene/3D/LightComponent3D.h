#pragma once

#include "../ComponentBase.h"
#include "../Light3D.h"

class 
    __declspec(uuid("572F705C-8824-4394-B031-DCC600AA9805"))
    ZN_API
    CLightComponent3D : public CComponentBase
{
public:
    typedef std::vector<std::shared_ptr<CLight3D>> LightList;

public:
    CLightComponent3D(std::shared_ptr<ISceneNode> OwnerNode);
    virtual ~CLightComponent3D();

    /**
     * Add a light to this scene node.
     */
    virtual void AddLight(std::shared_ptr<CLight3D> _light);
    virtual void RemoveLight(std::shared_ptr<CLight3D> _light);
    virtual const LightList& GetLights();
    
    // ISceneNodeComponent
    virtual bool Accept(IVisitor* visitor) override;

private:
    LightList         m_Lights;

};