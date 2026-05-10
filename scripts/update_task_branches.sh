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
git checkout task/create-custom-editor && \
  git rebase task/add-slider && \
git checkout task/add-parameter && \
  git rebase task/create-custom-editor && \
git checkout task/flanger-plugin && \
  git rebase task/add-parameter && \
git checkout task/flanger-effect && \
  git rebase task/flanger-plugin && \
git checkout task/file-player && \
  git rebase task/flanger-effect && \
git checkout task/generate-sine && \
  git rebase task/file-player && \
git checkout main

