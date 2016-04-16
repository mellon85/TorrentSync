
FOLDER=gcov
export BOOST_TEST_SHOW_PROGRESS="yes"

a=$(which lcov)
if [ -z $a ]
then
    echo "Couldn't find lcov"
    exit 1
fi

rm -rf gcov
cmake . -DCOVERAGE=1 && \
make -j3 && \
mkdir -p "$FOLDER" && \
./unittest && \
lcov --checksum -o "$FOLDER/gcov.info" --no-external --capture -d CMakeFiles -d torrentsync -d test &>/dev/null
lcov --remove "$FOLDER/gcov.info" "test/*" -o "$FOLDER/gcov.info.clean" &>/dev/null
genhtml -o $FOLDER $FOLDER/gcov.info.clean
