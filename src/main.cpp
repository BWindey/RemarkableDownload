#include <iostream>
#include <nlohmann/json.hpp>

#include "program_options.h"

using json = nlohmann::json;

bool check_ssh_connection();
void parseMetadata(json j);


int main(int argc, char* argv[]) {
    try {
        program_options::parse(argc, argv);
    } catch (const std::exception &x) {
        std::cerr << x.what() << '\n';
        std::cerr << "Usage: " << argv[0] << "[-h] [-r] [-v] <FOLDER_NAME>\n";
        return 1;
    }

    if (program_options::help()) {
        std::cout <<
            "Download all files as pdf under the <FOLDER_NAME> folder. <FOLDER_NAME> is matched with regex (grep).\n"
            "To get the contents of the root-folder (My files), use 'root' as <FOLDER_NAME>.\n"
            "\n"
            "Syntax: " << argv[0] << "[-h] [-r] [-v] <FOLDER_NAME>\n"
            "\n"
            "options:\n"
            "  -v      verbose, display file-id's and wget output\n"
            "  -h      help, show this message\n";
    }

    if (!system(nullptr)) {
        std::cerr << "Cannot execute commands\n";
        return 2;
    }

    if (!check_ssh_connection()) {
        std::cerr << "No ssh-connection could be established.\n"
                     "Ensure you are connected via USB and the tablet is on.\n";
        return 3;
    }

    json j;
    parseMetadata(j);

    // De-allocate the memory used in program_options
    program_options::cleanup();

    return 0;
}


bool check_ssh_connection() {
    int returnCode = system("ssh -q -o ConnectTimeout=1 root@10.11.99.1 'exit'");
    if (returnCode == 0) {
        return true;
    }
    return false;
}

void parseMetadata(json j) {
    const char command[] = "ssh root@10.11.99.1 '"
                           "cat /home/root/.local/share/remarkable/xochitl/*.metadata"
                           " | sed \"s/}/},/\""
                           " | sed \"s/ *$//\"'";
    FILE *stream = popen(command, "r");
    if (stream == nullptr) {
        throw std::runtime_error("Couldn't read files from the tablet");
    }

    char buffer[128];
    std::string result = "[";

    while(fgets(buffer, sizeof(buffer), stream) != nullptr) {
        result += buffer;
    }

    unsigned long i = result.length() - 1;
    while (isspace(result[i]) != 0) {
        i--;
    }
    result = result.substr(0, i);
    result += "]";

    j = json::parse(result);

    for (auto &entry : j) {
        std::cout << "Visible name: " << entry["visibleName"] << '\n';
        std::cout << "Type:         " << entry["type"] << "\n\n";
    }

    pclose(stream);
}