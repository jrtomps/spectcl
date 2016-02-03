#include "TestProcess.h"

CTestProcess::CTestProcess()
 : m_pid(0)
{
}

void CTestProcess::exec()
{
    m_pid = 1;
}

void CTestProcess::kill()
{
    m_pid = 0;
}

int CTestProcess::getPid() const {
    return m_pid;
}

bool CTestProcess::isRunning()
{
    return (m_pid == 1);
}
