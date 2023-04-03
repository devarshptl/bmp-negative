#include <cstring>
#include "lib/MImageClass.h"

using namespace std;

// driver code
int main(int argc, char *argv[]) {

    // args
    vector<string> args;
    for(int i=1; i<argc; i++) {
        args.emplace_back(argv[i]);
    }

    if (args.size() == 1) {
        if (std::strcmp(args[0].c_str(), "--help") == 0) {
            cout << "Command Help: $ blockapps <input_file> <output_filename>\n";
        } else {
            cerr << "Invalid argument passed. Expected --help.\n";
            ::exit(FAIL);
        }
    } else if (args.size() == 2) {
        string input = args[0], output = args[1];
        MImageClass image(input);
        MImageClass nImage = image.negative();
        nImage.save(output);
    } else {
        cerr << "Invalid number of argument passed.\n1) blockapps --help\n2) blockapps <input_file> <output_filename>\n";
        ::exit(FAIL);
    }
    return 0;
}
