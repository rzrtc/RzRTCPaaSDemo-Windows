#include "UserStateNotifySingleton.h"

UserStateNotifySingleton* UserStateNotifySingleton::m_pUserStateNotifySingleton = nullptr;
UserStateNotifySingleton* UserStateNotifySingleton::instance()
{
    if (!m_pUserStateNotifySingleton)
    {
        m_pUserStateNotifySingleton = new UserStateNotifySingleton;
    }
    return m_pUserStateNotifySingleton;
}