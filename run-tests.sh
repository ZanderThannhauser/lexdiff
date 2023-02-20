set -ev
make -j buildtype=release on_error=do_nothing
zest ./tests
