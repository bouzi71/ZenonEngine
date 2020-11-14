#pragma once

#include "M2_Types.h"
class CM2;

class CM2_Animation
	: public IAnimation
{
public:
	CM2_Animation(const CM2& M2Model, const SM2_Sequence& Sequence, std::string AnimationName, uint16 IndexIntoSeq);
	virtual ~CM2_Animation();

	// IAnimation
	uint16 GetIndexInSequences() const override;
	const std::string& GetName() const override;
	uint32 GetFrameStart() const override;
	uint32 GetFrameEnd() const override;

	// Sequence
	//uint16					getSequenceIndex()	const { return m_SequenceIndex; }
	//bool                    hasNextVatianton()  const { return m_Next != nullptr; }
	//const CM2_Animation*	getNextVariation()	const { return m_Next.get(); }

	// Times
#if WOW_CLIENT_VERSION <= WOW_BC_2_4_3
	uint32					getStart()			const { return m_StartTimeStamp; }
	uint32					getEnd()			const { return m_EndTimeStamp; }
#else
	uint32					getStart()			const { return 0; }
	uint32					getEnd()			const { return m_Duration; }
#endif

private:
	CM2_Animation(const CM2_Animation&) = delete;
	const CM2_Animation& operator=(CM2_Animation&) = delete;

private:
	//const uint16			m_AnimID;
	const std::string		m_AnimationName;

	const uint16			m_SequenceIndex;
	std::unique_ptr<CM2_Animation> m_Next;

#if WOW_CLIENT_VERSION <= WOW_BC_2_4_3
	uint32					m_StartTimeStamp;
	uint32					m_EndTimeStamp;
#else
	uint32					m_Duration;
#endif
};