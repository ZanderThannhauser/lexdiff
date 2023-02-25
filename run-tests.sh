set -ev
unset on_error
make -j buildtype=release
/home/zander/bin/zest ./tests
