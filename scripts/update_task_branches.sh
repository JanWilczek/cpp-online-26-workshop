#!/bin/bash

git checkout main
git checkout homework/draw-background-noise && \
  git merge main && \
git checkout task/add-custom-font && \
  git merge homework/draw-background-noise && \
git checkout main

