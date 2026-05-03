#!/bin/bash

git checkout task4 && \
  git merge main && \
git checkout task3 && \
  git merge task4 && \
git checkout task2 && \
  git merge task3 && \
git checkout task1 && \
  git merge task2 && \
git checkout main

