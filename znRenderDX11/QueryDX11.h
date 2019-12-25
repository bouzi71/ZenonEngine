#pragma once

#include "Query.h"

class ZN_API QueryDX11 : public IQuery
{
public:

	QueryDX11(ID3D11Device2* pDevice, QueryType queryType, uint8_t numBuffers);
	virtual ~QueryDX11();

	virtual void Begin(int64_t frame = 0L);
	virtual void End(int64_t frame = 0L);
	virtual bool QueryResultAvailable(int64_t frame = 0L);
	virtual QueryResult GetQueryResult(int64_t frame = 0L);
	virtual uint8_t GetBufferCount() const;

protected:

private:
	ATL::CComPtr<ID3D11Device2> m_pDevice;
	ATL::CComPtr<ID3D11DeviceContext2> m_pDeviceContext;

	typedef std::vector< ATL::CComPtr<ID3D11Query> > QueryBuffer;
	QueryBuffer m_DisjointQueries;
	// For timer queries, we need 2 sets of buffered queries.
	QueryBuffer m_Queries[2];

	QueryType m_QueryType;
	
	uint8_t m_NumBuffers; // How many queries will be used to prevent stalling the GPU.
};