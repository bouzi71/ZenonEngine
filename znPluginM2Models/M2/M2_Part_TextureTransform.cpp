#include "stdafx.h"

// Include
#include "M2.h"
#include "M2_Base_Instance.h"

// General
#include "M2_Part_TextureTransform.h"

CM2_Part_TextureTransform::CM2_Part_TextureTransform(const CM2& M2Object, const std::shared_ptr<IFile>& File, const SM2_TextureTransform& M2TextureTransform)
	: m_M2Object(M2Object)
{
	m_TranslateAnimated.Initialize(M2TextureTransform.translation, File, M2Object.getSkeleton().GetAnimFiles());
	m_RotateAnimated.Initialize(M2TextureTransform.rotation, File, M2Object.getSkeleton().GetAnimFiles());
	m_ScaleAnimated.Initialize(M2TextureTransform.scaling, File, M2Object.getSkeleton().GetAnimFiles());
}

CM2_Part_TextureTransform::~CM2_Part_TextureTransform()
{
}

glm::mat4 CM2_Part_TextureTransform::GetTransform(const CM2_Base_Instance* M2Instance, uint32 GlobalTime) const
{
	glm::mat4 matrix(1.0f);
	if (const auto& animator = M2Instance->GetModelsComponent())
	{
		if (m_TranslateAnimated.IsUsesBySequence(animator->GetCurrentAnimationIndex()))
		{
			matrix = glm::translate(matrix, m_TranslateAnimated.GetValue(animator->GetCurrentAnimationIndex(), animator->GetCurrentTime_(), m_M2Object.getSkeleton().getGlobalLoops(), GlobalTime));
		}

		if (m_RotateAnimated.IsUsesBySequence(animator->GetCurrentAnimationIndex()))
		{
			matrix = glm::translate(matrix, glm::vec3(0.5f)); // TODO: Maybe glm::vec2(0.5f, 0.5f, 0.0f) ?
			matrix *= glm::toMat4(m_RotateAnimated.GetValue(animator->GetCurrentAnimationIndex(), animator->GetCurrentTime_(), m_M2Object.getSkeleton().getGlobalLoops(), GlobalTime));
			matrix = glm::translate(matrix, glm::vec3(-0.5f));
		}

		if (m_ScaleAnimated.IsUsesBySequence(animator->GetCurrentAnimationIndex()))
		{
			matrix = glm::scale(matrix, m_ScaleAnimated.GetValue(animator->GetCurrentAnimationIndex(), animator->GetCurrentTime_(), m_M2Object.getSkeleton().getGlobalLoops(), GlobalTime));
		}
	}

	return matrix;
}
