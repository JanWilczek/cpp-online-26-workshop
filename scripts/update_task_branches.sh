#!/bin/bash

git checkout main
git checkout homework/draw-background-noise && \
  git rebase main && \
git checkout task/add-custom-fonts && \
  git rebase homework/draw-background-noise && \
git checkout task/add-value-label && \
  git rebase task/add-custom-fonts && \
git checkout task/add-labels && \
  git rebase task/add-value-label && \
git checkout task/style-components && \
  git rebase task/add-labels && \
git checkout task/add-slider && \
  git rebase task/style-components && \
git checkout main

