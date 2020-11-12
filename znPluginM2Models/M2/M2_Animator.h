#pragma once

#include "M2_Types.h"
#include "M2_Animation.h"

class ZN_API CM2_Animator
{
public:
	CM2_Animator(const IBaseManager& BaseManager, const CM2& M2Model);
	virtual ~CM2_Animator();

	void PlayAnimation(uint16 AnimationId, bool Loop);
	void PrintList();
	void Update(double _time, double _dTime);

	uint16 getSequenceIndex() const { return m_CurrentAnimation->getSequenceIndex(); }
	uint32 getCurrentTime() { return m_CurrentTime; }
	//uint32 getStart() const { return m_CurrentAnimation->getStart(); }
	//uint32 getEnd() const { return m_CurrentAnimation->getEnd(); }

	//void setOnEndFunction(Function* _onEnd);

private:
	std::unordered_map<uint16, std::shared_ptr<CM2_Animation>>	m_Animations;
	const CM2_Animation*		m_CurrentAnimation;
	bool						m_IsLoop;
	bool						m_IsStopped;

	//Function*					m_OnAnimationEnded;

	double						animtime;
	uint32						m_CurrentTime;

private:
	const CM2& m_M2Model;
};