#!/bin/bash

set -ex

cargo install deqp-runner \
  -j ${FDO_CI_CONCURRENT:-4} \
  --version 0.5.1 \
  --root /usr/local \
  $EXTRA_CARGO_ARGS
