#!/bin/bash

scriptname="$(basename "$0")"

# Default options
declare -i verbose=0

# Error function
error() {
	echo "Usage: ${scriptname} [-h] [-v] <FOLDER_NAME>" 1>&2 && exit "$1"
}

help() {
    echo "Download all files as pdf under the <FOLDER_NAME> folder. <FOLDER_NAME> is matched with regex (grep)."
    echo ""
    echo "Syntax: ${scriptname} [-h] [-v] <FOLDER_NAME>"
    echo ""
    echo "options:"
    echo "  -v      verbose, display file-id's and wget output"
    echo "  -h      help, show this message"
}

# Option handling
while getopts ":vh" opt
do
	case "${opt}" in
		v)  verbose=1
			;;
        h)  help
            exit 0
            ;;
		\?) error 1
			;;
	esac
done

shift $((OPTIND - 1))


nameMatch="$1"

# Get all directories that match the name
IFS=$'\n' read -rd '' -a foundDirectories <<< \
    "$(ssh root@10.11.99.1 bash -s <<- EOF
        for file in \$(grep -li "visibleName.*${nameMatch}" /home/root/.local/share/remarkable/xochitl/*.metadata); do
            if grep -q "type.*CollectionType" "\$file"; then
                echo "\$file"
            fi
        done
EOF
)"


# For every found directory on tablet, make directory matching locally
for entry in "${foundDirectories[@]}"; do
    directoryId="$(basename ${entry%.metadata})"
    directoryName=$(ssh root@10.11.99.1 "grep 'visibleName' ${entry}" | sed 's/.*"\([^"]*\)"[, ]*$/\1/')

    echo -en "\e[1;4m${directoryName}\e[0m"
    if (( verbose == 1 )); then
        echo ":\e[3m ${directoryId}\e[0m"
    else
        echo ''
    fi

    # Make directory if didn't exist yet
    if ! test -d "${directoryName}_rm"; then
        mkdir "${directoryName}_rm"
    else
        echo -e "\e[1;33mWarning:\e[0m directory ${directoryName}_rm/ already exists, contents may be overwritten!"
        read -p "Do you want to continue? [y/n] " -n 1 -r
        echo ''     # new-line
        if [[ ! "$REPLY" =~ ^[Yy]$ ]]; then
            echo ''
            continue
        fi
    fi

    echo ''

    IFS=$'\n' read -rd '' -a foundFiles <<< \
        "$(ssh root@10.11.99.1 bash -s <<- EOF
            for file in \$(grep -li "parent\": \"${directoryId}\"" /home/root/.local/share/remarkable/xochitl/*.metadata); do
                if grep -q "type.*DocumentType" "\$file"; then
                    echo "\$file"
                fi
            done
EOF
)"
    for file in "${foundFiles[@]}"; do
        fileId="$(basename ${file%.metadata})"
        fileName=$(ssh root@10.11.99.1 "grep 'visibleName' ${file}" | sed 's/.*"\([^"]*\)"[, ]*$/\1/')
        echo -en "\e[1m${fileName}: \e[0m"
        if (( verbose == 1 )); then
            echo -e "\e[3m ${fileId}\e[0m"
            flags="-nv --show-progress"
        else
            flags="-q"
        fi
        wget ${flags} -O "${directoryName}_rm/${fileName}.pdf" "http://10.11.99.1/download/${fileId}/placeholder"
        test $? == 0 && echo -e "  \t\e[32mdownloaded\e[0m"
    done

    # New-line between directories
    echo ''
done
