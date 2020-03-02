cmake_minimum_required(VERSION 3.12)

aux_source_directory(./libjpeg DIR_JPEG)
#aux_source_directory(./libpng DIR_PNG)

#add_library(Vision_External OBJECT ${DIR_JPEG} ${DIR_PNG})
add_library(Vision_External OBJECT ${DIR_JPEG})
