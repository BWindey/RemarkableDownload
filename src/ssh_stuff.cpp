#include "ssh_stuff.h"

bool check_ssh_connection() {
    int returnCode = system("ssh -q -o ConnectTimeout=1 root@10.11.99.1 'exit'");
    if (returnCode == 0) {
        return true;
    }
    return false;
}

json parseMetadata() {
    // Get all contents of metadata-files and insert the file-name inside each content
    const char command[] = R"(ssh root@10.11.99.1 'tail -vn +1 /home/root/.local/share/remarkable/xochitl/*.metadata \
        | sed "s/==> \/home.*xochitl\/\(.*\)\.metadata <==.*/{\n\"UUID\": \"\1\",/" \
        | tr -d "\n" \
        | tr -s " " \
        | sed "s/,{/,/g" \
        | sed "s/}{/}, {/g" \
        | sed "s/^.*$/[&]/"')";

    FILE *stream = popen(command, "r");
    if (stream == nullptr) {
        throw std::runtime_error("Couldn't read files from the tablet");
    }

    char buffer[128];
    std::string result;

    while(fgets(buffer, sizeof(buffer), stream) != nullptr) {
        result += buffer;
    }

    unsigned long i = result.length() - 1;
    while (isspace(result[i]) != 0) {
        i--;
    }
    result = result.substr(0, i + 1);

    pclose(stream);

    return json::parse(result);
}