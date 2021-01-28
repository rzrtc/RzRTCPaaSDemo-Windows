#include "OpenGLSignalMngr.h"
OpenGLSignalMngr* OpenGLSignalMngr::m_pOpenGLSignalMngr = nullptr;

OpenGLSignalMngr* OpenGLSignalMngr::instance()
{
	if (!m_pOpenGLSignalMngr)
	{
		m_pOpenGLSignalMngr = new OpenGLSignalMngr;
	}
	return m_pOpenGLSignalMngr;
}
