
FOLDER=gcov
export BOOST_TEST_SHOW_PROGRESS="yes"

rm -rf gcov
cmake . -DCOVERAGE=1 && \
make && \
mkdir -p "$FOLDER" && \
./unittest && \
lcov --checksum -o "$FOLDER/gcov.info" --no-external --capture -d CMakeFiles -d torrentsync -d test &>/dev/null
lcov --remove "$FOLDER/gcov.info" "test/*" -o "$FOLDER/gcov.info.clean" &>/dev/null
genhtml -o $FOLDER $FOLDER/gcov.info.clean
