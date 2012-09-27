#include "InputManager.h" 


InputManager* InputManager:: m_InputManager = 0;


InputManager::InputManager()
{
	m_Listener = NULL;
}

InputManager* InputManager::GetInstance()
{
	if(m_InputManager == 0)
		m_InputManager = new InputManager();
	return m_InputManager;
}

bool InputManager::registerInput(WPARAM input)
{

	for(unsigned int i = 0;i<m_RegisterInput.size();i++)
	{
		if (input == m_RegisterInput[i])
		{
			return false;
		}

	}

	m_RegisterInput.push_back(input);

}
void InputManager::unRegisterInput(WPARAM input)
{
	for(unsigned int i = 0;i<m_RegisterInput.size();i++)
	{
		if(input == m_RegisterInput[i])
		{
			m_RegisterInput.erase(m_RegisterInput.begin() + i);
		}
	}

}
void InputManager::handleInput(WPARAM input, bool Down)
{
	if(m_Listener != NULL)
	{

		for(unsigned int i = 0;i<m_RegisterInput.size();i++)
		{
			if(input == m_RegisterInput[i])
			{
				if(Down)
				{
					m_Listener->keyDownEvent(input);
					break;
				}
				else
				{
					m_Listener->keyUpEvent(input);
					break;
				}
			}

		}
	}
}
void InputManager::setListener(InputListener* listener)
{
	m_Listener = listener;
}