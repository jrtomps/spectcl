

#include "client.h"
#include <sstream>
#include <cstring>

TestXamine* TestXamine::m_pInstance = nullptr;

TestXamine* TestXamine::getInstance() {

    if (m_pInstance == nullptr) {
        m_pInstance = new TestXamine;
    }

    return m_pInstance;
}

void TestXamine::log(std::string message)
{
    m_record.push_back(message);
}



int Xamine_CreateSharedMemory(int specbytes, volatile Xamine_shared **ptr)
{
    std::ostringstream msg;
    msg << "Xamine_CreateSharedMemory(" << specbytes << ", Xamine_shared)";
    TestXamine::getInstance()->log(msg.str());
    return 0;
}


int Xamine_DetachSharedMemory()
{
    TestXamine::getInstance()->log("Xamine_DetachSharedMemory()");
    return 0;
}

int Xamine_Start() {
    TestXamine::getInstance()->log("Xamine_Start()");
    return 0;
}

int Xamine_Stop() {
    TestXamine::getInstance()->log("Xamine_Stop()");
    return 0;
}

int Xamine_Alive() {
    TestXamine::getInstance()->log("Xamine_Stop()");
    return 0;
}


void Xamine_GetMemoryName(char *namebuffer)
{
    std::strcpy(namebuffer, "TestXamine");
}

void Xamine_
