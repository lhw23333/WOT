#!/bin/bash

git add MyProject/
echo "Add Finish"

git commit -m "$(date)"
echo "commit finish"

git push -u origin master
echo "push finish"
