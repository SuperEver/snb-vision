cmake_minimum_required(VERSION 3.5)

aux_source_directory(. DIR_KERNEL)
add_library(Vision_KERNEL OBJECT ${DIR_KERNEL})
