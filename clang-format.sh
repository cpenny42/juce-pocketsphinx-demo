###############################################################################
# Jamboxx automation
#	common - clang-format.sh
#
# 	Applies clang-format to the codebase
###############################################################################

PLATFORM=mac
# PLATFORM=linux

# Put all the folders you want to use clang-format with here:
SOURCE_DIRECTORIES=("$(pwd)/app/voice-to-csv/Source" \
					"$(pwd)/app/lib/include")

# Helper function to find all the files matching these extensions recursively
find_source_files () {
	find "$1" -type f \( -name "*.hpp" \
					  -o -name "*.h"   \
					  -o -name "*.cpp" \
					  -o -name "*.c"   \
					  -o -name "*.m"   \
					  -o -name "*.mm" \)
}
export -f find_source_files

cd "$(pwd)/tools/clang-format/${PLATFORM}"
# For each source directory, find all the source code files recursively,
# then pass to clang-format (which formats the files in place with the -i argument).
for sourceDirectory in "${SOURCE_DIRECTORIES[@]}"; do
	directoryFiles=$(find_source_files "${sourceDirectory}")
	for files in "${directoryFiles[@]}"; do
		OLD_IFS=$IFS
		IFS=$'\n' fileList=($files)
		IFS=${OLD_IFS}
		for file in "${fileList[@]}"; do
			./clang-format -i "${file}"
		done
	done
done
