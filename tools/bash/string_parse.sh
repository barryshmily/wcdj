#!/bin/bash

semverParse() {
	major="${1%%.*}"
	minor="${1#$major.}"
	minor="${minor%%.*}"
	patch="${1#$major.$minor.}"
	patch="${patch%%[-.]*}"
}

version=1.10.1
semverParse $version

echo $major $minor $patch

echo "done"

