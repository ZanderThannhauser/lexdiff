set -ev
unset on_error
make -j buildtype=release
sudo /home/zander/bin/zest ./tests
