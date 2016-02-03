
#include <SpectraProcess.h>
#include <iostream>
#include <string>

using namespace std;

int main() {

    Spectra::CSpectraProcess process;

    process.exec();

    string line;
    while (cin) {
        cout << "% " << flush;
        getline(cin, line);
        if (line == "exit") break;
    }

    process.kill();

    return 0;
}

int gpTCLApplication = 0;
