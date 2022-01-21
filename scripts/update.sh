USER=cs240
REMOTE=data.cs.purdue.edu
REMOTE_BINDIR=/homes/cs240/bin/
REMOTE_INCLUDEDIR=/homes/cs240/public/include/clang/include
LLVM_DIR=/home/novafacing/hub/llvm-project
TIDY_BIN="${LLVM_DIR}/llvm/build/bin/clang-tidy"
TIDY_INC="${LLVM_DIR}/llvm/build/lib/clang/14.0.0/include/"
BASEDIR=$(dirname "$0")

if [ ! -f "${TIDY_BIN}" ]; then
    echo "Clang tidy binary ${TIDY_BIN} does not exist."
    exit 1
fi
if [ ! -d "${TIDY_INC}" ]; then
    echo "Clang tidy include directory ${TIDY_INC} does not exist."
    exit 1
fi

# Transfer binary to server
scp "${BASEDIR}/linter" "${USER}@${REMOTE}:${REMOTE_BINDIR}/linter"
scp "${TIDY_BIN}" "${USER}@${REMOTE}:${REMOTE_BINDIR}/"
scp -r "${TIDY_INC}" "${USER}@${REMOTE}:${REMOTE_INCLUDEDIR}"
ssh "${USER}@${REMOTE}" "chmod -R 755 ${REMOTE_INCLUDEDIR}"

# Copy binary to releases
cp "${TIDY_BIN}" "${BASEDIR}/../releases/clang-tidy-$(date +%y%m%d%H%M%S)"

echo "Done."


