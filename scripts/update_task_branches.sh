#!/bin/bash

git checkout main
git checkout homework/draw-background-noise && \
  git rebase main && \
git checkout task/add-custom-fonts && \
  git rebase homework/draw-background-noise && \
git checkout main

