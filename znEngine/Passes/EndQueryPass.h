#pragma once

#include "AbstractPass.h"

class EndQueryPass : public AbstractPass
{
public:
    EndQueryPass( std::shared_ptr<Query> query );
    virtual ~EndQueryPass();

    virtual void Render( Render3DEventArgs& e );

private:
    std::shared_ptr<Query> m_pQuery;
};