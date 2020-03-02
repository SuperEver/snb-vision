cmake_minimum_required(VERSION 3.12)
aux_source_directory(. DIR_UTILS)
add_library(Vision_Utils OBJECT ${DIR_UTILS})