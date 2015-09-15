#!/bin/bash

make &&
sudo env "PATH=$PATH" make install
