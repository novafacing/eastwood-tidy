set -e

usage() {
    echo "$0"
    echo "update.sh -l <llvm-project> [-u <user>] [-r <remote>] [-b <remote-bindir>]" 1>&2
    echo "    [-i <remote-include-dir>]" 1>&2
    echo "-l <llvm-project>: Full or relative path to llvm-project repository" 1>&2
    echo "[-u <user>]: Remote username to upload files with." 1>&2
    echo "[-r <remote>]: Remote address of server to upload files to." 1>&2
    echo "[-b <remote-bindir>]: Remote binary directory to upload clang-tidy binary to." 1>&2
    echo "[-i <remote-include-dir>]: Remote include directory to upload clang headers to." 1>&2
    exit 1
}

while getopts ":l:u:r:b:i:" o; do
    case "${o}" in
        l)
            LLVM_DIR=${OPTARG}
            [ ! -d "${LLVM_DIR}" -o -z "${LLVM_DIR}" ] && usage "LLVM Directory ${LLVM_DIR} does not exist."
            ;;
        u)
            USER=${OPTARG}
            ;;
        r)
            REMOTE=${OPTARG}
            ;;
        b)
            REMOTE_BINDIR=${OPTARG}
            ;;
        i)
            REMOTE_INCLUDEDIR=${OPTARG}
            ;;
        *)
            usage "Unrecognized argument ${o}"
            ;;
    esac
done
shift $((OPTIND-1))

echo "Running with:"
echo "LLVM_DIR=${LLVM_DIR}"
echo "REMOTE=${REMOTE}"
echo "REMOTE_BINDIR=${REMOTE_BINDIR}"
echo "REMOTE_INCLUDEDIR=${REMOTE_INCLUDEDIR}"

TIDY_BIN="${LLVM_DIR}/llvm/build/bin/clang-tidy"
TIDY_INC="${LLVM_DIR}/llvm/build/lib/clang/14.0.0/include/"

if [ ! -f "${TIDY_BIN}" ]; then
    echo "Clang tidy binary ${TIDY_BIN} does not exist."
    exit 1
fi

if [ ! -d "${TIDY_INC}" ]; then
    echo "Clang tidy include directory ${TIDY_INC} does not exist."
    exit 1
fi

# Used to get the `linter` shell script interface to clang-tidy
BASEDIR=$(dirname "$0")

# Args for data.cs.purdue.edu
# USER=cs240
# REMOTE=data.cs.purdue.edu
# REMOTE_BINDIR=/homes/cs240/bin/
# REMOTE_INCLUDEDIR=/homes/cs240/public/include/clang/include
# LLVM_DIR=/home/novafacing/hub/llvm-project


if [ ! -z "${USER}" -a ! -z "${REMOTE}" -a ! -z "${REMOTE_BINDIR}" -a ! -z "${REMOTE_INCLUDEDIR}" ]; then
    # Transfer binary to server
    echo "Uploading binary..."
    scp "${BASEDIR}/linter" "${USER}@${REMOTE}:${REMOTE_BINDIR}/linter"
    scp "${TIDY_BIN}" "${USER}@${REMOTE}:${REMOTE_BINDIR}/"
    scp -r "${TIDY_INC}" "${USER}@${REMOTE}:${REMOTE_INCLUDEDIR}"
    ssh "${USER}@${REMOTE}" "chmod -R 755 ${REMOTE_INCLUDEDIR}"
fi

# Copy binary to releases
echo "Copying binary to releases..."
cp "${TIDY_BIN}" "${BASEDIR}/../releases/clang-tidy-$(date +%y%m%d%H%M%S)"

echo "Done."


