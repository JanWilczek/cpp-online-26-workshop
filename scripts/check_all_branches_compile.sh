#!/bin/bash

git checkout main && \
  cmake --build --preset default && \
git checkout homework/draw-background-noise && \
  cmake --build --preset default && \
git checkout task/add-custom-fonts && \
  cmake --build --preset default && \
git checkout task/add-value-label && \
  cmake --build --preset default && \
git checkout task/add-labels && \
  cmake --build --preset default && \
git checkout task/style-components && \
  cmake --build --preset default && \
git checkout task/add-slider && \
  cmake --build --preset default && \
git checkout task/create-custom-editor && \
  cmake --build --preset default && \
git checkout task/add-parameter && \
  cmake --build --preset default && \
git checkout task/flanger-plugin && \
  cmake --build --preset default && \
git checkout task/flanger-effect && \
  cmake --build --preset default && \
git checkout task/file-player && \
  cmake --build --preset default && \
git checkout task/generate-sine && \
  cmake --build --preset default && \
git checkout main

