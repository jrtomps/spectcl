
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/Asserter.h>

#include <config.h>

#include <Asserts.h>

#define private public
#define protected public
#include <FilterBufferDecoder.h>
#undef protected
#undef private

#include <CXdrMemInputStream.h>
#include <Analyzer.h>

#include <vector>

using namespace std;

class CBufferDecoder;


class CTestAnalyzer : public CAnalyzer
{
public:

    void OnPhysics(CBufferDecoder& ) {}

};

class CFilterBufferDecoderTests : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CFilterBufferDecoderTests);
    CPPUNIT_TEST_SUITE_END();

private:
        CFilterBufferDecoder m_decoder;
        CAnalyzer            m_analyzer;

public:
    void setUp() {
        m_decoder = CFilterBufferDecoder();
        m_analyzer = CTestAnalyzer();
    }

    void tearDown() {

    }

    void translateBuffer_0 () {
        vector<char> buffer;
        CXdrMemInputStream stream(buffer.size(), buffer.data());

        m_decoder.TranslateBuffer(stream, buffer.size(),m_analyzer);

    }

    void translateBuffer_1 () {

    }

    void processEvents_0 () {

    }


};

CPPUNIT_TEST_SUITE_REGISTRATION(CFilterBufferDecoderTests);

