#!/bin/bash

if test -d "${1}_rm"; then
    echo "Directory ${1}_rm/ already, won't continue, because that would overwrite"
    exit 1
fi
mkdir "${1}_rm"

IFS=$'\n' read -rd '' -a foundDirectories <<< \
    "$(ssh root@10.11.99.1 bash -s << EOF
for file in \$(ls /home/root/.local/share/remarkable/xochitl/*.metadata); do
    if grep -q "type.*CollectionType" "\$file" && grep -iq "visibleName.*${1}" "\$file"; then
        echo "\$file"
    fi
done
EOF
)"

for entry in "${foundDirectories[@]}"; do
    id="$(basename ${entry%.metadata})"
    echo "$id"
    IFS=$'\n' read -rd '' -a foundFiles <<< \
        "$(ssh root@10.11.99.1 bash -s << EOF
for file in \$(ls /home/root/.local/share/remarkable/xochitl/*.metadata); do
    if grep -q "type.*DocumentType" "\$file" && grep -iq "parent\": \"${id}\"" "\$file"; then
        echo "\$file"
    fi
done
EOF
)"
    for file in "${foundFiles[@]}"; do
        fileId="$(basename ${file%.metadata})"
        fileName=$(ssh root@10.11.99.1 "grep 'visibleName' ${file}" | sed 's/.*"\([^"]*\)"[, ]*$/\1/')
        echo "$fileName"
        curl --output "${1}_rm/${fileName}.pdf" "http://10.11.99.1/download/${fileId}/placeholder"
    done
done
