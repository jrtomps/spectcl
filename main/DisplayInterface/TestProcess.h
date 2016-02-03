#ifndef TESTPROCESS_H
#define TESTPROCESS_H

#include "Subprocess.h"



/*!
 * \brief The CTestProcess class
 *
 *  A simple implementation of the subprocess interface that
 *  just manipulates its pid. Not running is pid=0 and running is pid=1.
 */
class CTestProcess : public Subprocess
{
private:
    int m_pid;

public:
    CTestProcess();

    void exec();
    void kill();

    int getPid() const;
    bool isRunning() const;
};

#endif // TESTPROCESS_H
