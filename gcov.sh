
FOLDER=gcov
export BOOST_TEST_SHOW_PROGRESS="yes"

cmake . -DCOVERAGE=1 && \
make && \
mkdir -p $FOLDER && \
./unittest && \
lcov --path=$HOME/TorrentSync -o $FOLDER/gcov.info -c -d CMakeFiles && \
lcov -r $FOLDER/gcov.info /usr/include/\* $PWD/test/\* $PWD/turtle/\* -o $FOLDER/gcov.clean.info && \
genhtml -o $FOLDER $FOLDER/gcov.clean.info
