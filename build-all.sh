set -ev
export on_error=do_nothing
make buildtype=debug
make buildtype=test
make buildtype=release
