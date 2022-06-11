#!/bin/bash
# Copyright Resolved
# ============================================================================

CLANG_FORMAT=$(which clang-format) || (echo "Please install 'clang-format' tool first"; exit 1)

version=$("${CLANG_FORMAT}" --version | sed -n "s/.*\ \([0-9]*\)\.[0-9]*\.[0-9]*.*/\1/p")
if [[ "${version}" -lt "10" ]]; then
  echo "clang-format's version must be at least 10.0.0"
  exit 1
fi

CURRENT_PATH=$(pwd)
SCRIPTS_PATH=$(dirname "$0")

echo "CURRENT_PATH=${CURRENT_PATH}"
echo "SCRIPTS_PATH=${SCRIPTS_PATH}"

# print usage message
function usage()
{
  echo "Format the specified source files to conform the code style."
  echo "Usage:"
  echo "bash $0 [-a] [-c] [-l] [-h]"
  echo "e.g. $0 -c"
  echo ""
  echo "Options:"
  echo "    -a format of all files, default case"
  echo "    -c format of the files changed compared to last commit"
  echo "    -l format of the files changed in last commit"
  echo "    -h Print usage"
}

# check and set options
function checkopts()
{
  # init variable
  mode="all"    # default format all files

  # Process the options
  while getopts 'aclh' opt
  do
    case "${opt}" in
      a)
        mode="all"
        ;;
      c)
        mode="changed"
        ;;
      l)
        mode="lastcommit"
        ;;
      h)
        usage
        exit 0
        ;;
      *)
        echo "Unknown option ${opt}!"
        usage
        exit 1
    esac
  done
}

# init variable
# check options
checkopts "$@"

# switch to project root path, which contains clang-format config file '.clang-format'
cd "${SCRIPTS_PATH}/.." || exit 1

LIST_FILE='__format_files_list__'
FILE_SUFFIX='\.cc$\|\.h$\|\.cpp$'
CHECK_DIRS="src\|tests"
if [[ "X${mode}" == "Xall" ]]; then
  find src tests -type f -name "*" | grep ${FILE_SUFFIX} > "${LIST_FILE}" || true
elif [[ "X${mode}" == "Xchanged" ]]; then
  git diff --name-only HEAD | grep ${CHECK_DIRS} | grep ${FILE_SUFFIX} > "${LIST_FILE}" || true
else  # "X${mode}" == "Xlastcommit"
  git diff --name-only HEAD~ HEAD | grep ${CHECK_DIRS} | grep ${FILE_SUFFIX} > "${LIST_FILE}" || true
fi

while read line; do
  if [ -f "${line}" ]; then
    ${CLANG_FORMAT} -i "${line}"
  fi
done < "${LIST_FILE}"

rm "${LIST_FILE}"
cd "${CURRENT_PATH}" || exit 1

echo "Specified cpp source files have been format successfully."
