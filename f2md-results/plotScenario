#!/bin/bash

FOLDERS=$(find . -maxdepth 1 -type d)
REM_POINER='./'
PLOTS_DIR='gui-plots'
CUR_DIR='.'
FOLDERS=$(printf '%s\n' "${FOLDERS//$REM_POINER/}")
FOLDERS=$(printf '%s\n' "${FOLDERS//$PLOTS_DIR/}")
FOLDERS=$(printf '%s\n' "${FOLDERS//$CUR_DIR/}")

printf "Please select scenario:\n"
select d in $FOLDERS; do test -n "$d" && break; echo ">>> Invalid Selection"; done

cp -r ./gui-plots ./$d
cd ./$d
pwd
gnuplot './gui-plots/realTimePlot.p'
