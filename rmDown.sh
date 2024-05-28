#!/bin/bash

if test -d "${1}_rm"; then
    echo "Directory ${1}_rm/ already, won't continue, because that would overwrite"
    exit 1
fi

IFS=$'\n' read -rd '' -a foundDirectories <<< \
    "$(ssh root@10.11.99.1 bash -s << EOF
for file in \$(grep -li "visibleName.*${1}" /home/root/.local/share/remarkable/xochitl/*.metadata); do
    if grep -q "type.*CollectionType" "\$file"; then
        echo "\$file"
    fi
done
EOF
)"

mkdir "${1}_rm"

for entry in "${foundDirectories[@]}"; do
    id="$(basename ${entry%.metadata})"
    echo "$id"
    IFS=$'\n' read -rd '' -a foundFiles <<< \
        "$(ssh root@10.11.99.1 bash -s << EOF
for file in \$(grep -li "parent\": \"${id}\"" /home/root/.local/share/remarkable/xochitl/*.metadata); do
    if grep -q "type.*DocumentType" "\$file"; then
        echo "\$file"
    fi
done
EOF
)"
    for file in "${foundFiles[@]}"; do
        fileId="$(basename ${file%.metadata})"
        fileName="$(ssh root@10.11.99.1 \"grep 'visibleName' ${file}\" | sed 's/.*"\([^"]*\)"[, ]*$/\1/')"
        echo -e "\e[1m${fileName}:\e[0;3m ${fileId}\e[0m"
        wget -q -O "${1}_rm/${fileName}.pdf" "http://10.11.99.1/download/${fileId}/placeholder"
        test $? == 0 && echo "  downloaded"
    done
done
