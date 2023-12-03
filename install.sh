#!/bin/sh

PROGRAM='whyfetch'
PROGRAMPATH="$HOME/.local/bin/whyfetch"

# If the file exists, remove it
if [ -f "${PROGRAMPATH}" ]; then
	printf "Uninstalling whyfetch\n"
	rm "${PROGRAMPATH}"
	if [ ! -f "${PROGRAMPATH}" ]; then
		printf "Script successfully uninstalled!\n"
	else
		printf "Error: Uninstallation failed\n"
	fi
# If the file doesn't exist, copy it over
elif [ ! -f "${PROGRAMPATH}" ]; then
	printf "Installing whyfetch\n"
	cp "${PROGRAM}" "${PROGRAMPATH}"
	if [ -f "${PROGRAMPATH}" ]; then
		printf "Script successfully installed!\n"
	else
		printf "Error: installation failed\n"
	fi
fi
