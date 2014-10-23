#!/bin/sh

chmod +x _site/*
rsync -avz ./_site/ poulson@cardinal.stanford.edu:~/WWW/
