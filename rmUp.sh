#!/bin/bash

scriptname="$(basename "$0")"

# Default options
declare -i verbose=0

# Error function
error() {
	echo "Usage: ${scriptname} [-h] [-v] <FILE_NAME>" 1>&2 && exit "$1"
}

help() {
    echo "Upload the <FILE_NAME> to the remarkable."
    echo "The new file will be placed somewhere, I don't seem to be able to control where..."
    echo ""
    echo "Syntax: ${scriptname} [-h] [-v] <FILE_NAME>"
    echo ""
    echo "options:"
    echo "  -v      verbose, only turns on verbose for curl, as there isn't more to this"
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

# Make sure that there is only 1 argument remaining
if (( $# != 1)); then
    error 2
fi

# Make sure this argument is a real file
if [[ ! -e "$1" ]]; then
    error 3
fi

flags=""
if (( verbose == 1)); then
    flags="--verbose"
fi

curl --connect-timeout 1 "${flags}" \
    -H "Content-Type: multipart/form-data" \
    -F "file=@$(realpath "$1")" \
    "http://10.11.99.1/upload"
