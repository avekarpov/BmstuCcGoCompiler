# !/bin/bash

# set -x

clear

go_ll_compiler=$(readlink -f ./build/go_compiler)

file_name=$1
name="${file_name%.*}"
file_path=$(readlink -f "$1")

echo "Build $name"

mkdir -p tmp
cd tmp

ll_file_path="${PWD}/${name}.ll"

$go_ll_compiler $file_path $ll_file_path

obj_file_path="${PWD}/${name}.o"

llc -filetype=obj $ll_file_path -o $obj_file_path

out_file_path="${PWD}/${name}"

clang++ $obj_file_path -o $out_file_path

$out_file_path