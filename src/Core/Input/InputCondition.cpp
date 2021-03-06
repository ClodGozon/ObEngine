#include <Input/InputCondition.hpp>
#include <Input/InputManager.hpp>
#include <Utils/StringUtils.hpp>

namespace obe::Input
{

    InputCombination InputCondition::getCombination() const
    {
        return m_triggerConditions;
    }

    void InputCondition::addCombinationElement(
        const InputCombinationElement combinationElement)
    {
        m_triggerConditions.push_back(combinationElement);
        m_enabled = true;
    }

    void InputCondition::setCombination(const InputCombination& combination)
    {
        m_triggerConditions = combination;
        m_enabled = true;
    }

    bool InputCondition::check() const
    {
        if (!m_enabled)
            return false;
        bool conditionOk = true;
        for (const InputCombinationElement& element : m_triggerConditions)
        {
            if (!(element.second & element.first->getState()))
            {
                conditionOk = false;
                break;
            }
        }
        return conditionOk;
    }

    void InputCondition::clear()
    {
        m_triggerConditions.clear();
        m_enabled = false;
    }
} // namespace obe::Input