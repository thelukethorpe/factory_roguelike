#!/bin/bash

cd build || exit 1

./tests --output-on-failure
